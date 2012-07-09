#ifndef _sziMPIJobScheduler_h_
#define _sziMPIJobScheduler_h_

#include <itkObject.h>
#include "sziExecutable.h"
#include "sziSmartPointer.h"

#include "sziMPIContext.h"
#include "sziMPIJob.h"
#include "sziMPIWorker.h"

namespace szi
{

    class MPIWorkerAgent : public itk::Object, public MPIWorker
    {
    public:
        /** Standard class typedefs. */
        typedef MPIWorkerAgent Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPIWorkerAgent, Object );

    protected:
        MPIWorkerAgent() {}

    private:
        MPIWorkerAgent( const Self & ); // purposely not implemented
        MPIWorkerAgent& operator=( const Self & ); // purposely not implemented
    };

    /**
    Abstract class to manage MPI workers, and to schedule jobs to be executed on those workers.
    It is also the simpliest job scheduler.
    */
    class MPIJobScheduler : public itk::Object, public Executable
    {
    public:
        /** Standard class typedefs. */
        typedef MPIJobScheduler Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPIJobScheduler, Object );

        typedef MPIWorker WorkerType;
        typedef WorkerType::Pointer WorkerPointer;

        typedef MPIJob JobType;
        typedef JobType::Pointer JobPointer;

        typedef MPIContext::RankType RankType;

        /**
        Return the number of jobs that has been added.
        */
        virtual unsigned int getNumberOfJobs() const
        {
            return this->m_JobList.size();
        }

        /**
        Check whether a job has been added or not.
        */
        virtual bool hasJob( RankType rank ) const
        {
            for ( JobList::const_iterator i = this->m_JobList.begin(); i != this->m_JobList.end(); i++ )
            {
                if ( (*i)->getRank() == rank ) { return true; }
            }
            return false;
        }

        virtual JobType* getJob( RankType rank )
        {
            for ( JobList::iterator i = this->m_JobList.begin(); i != this->m_JobList.end(); i++ )
            {
                if ( (*i)->getRank() == rank ) { return (*i); }
            }
            return 0;
        }

        /**
        Add a new job into the job list.
        */
        virtual void addJob( JobType* job )
        {
            if ( job == 0 || this->hasJob(job->getRank()) )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "addJob(): not able to add this job" << End;
            	return;
            }

            job->setState( JobType::JOB_UNKNOWN );

            this->m_JobList.push_back( JobPointer(job) );
        }

        /**
         * Request to allocate the resource (i.e. a slave worker) for a job, for subsequent execution.
         */
        virtual void startJob( RankType rank )
        {
        	JobType* job = this->getJob( rank );
        	RankType wrank = rank + 1;
        	WorkerType* worker = this->getWorker( wrank );

        	worker->setState( WorkerType::WORKER_IDLE );
        	job->setWorkerRank( wrank );
        }

        /**
         * Request to release the resource (i.e. a slave worker) that was previously assigned to a job.
         */
        virtual void endJob( RankType rank )
        {
        	JobType* job = this->getJob( rank );
        	RankType wrank = rank + 1;
        	WorkerType* worker = this->getWorker( wrank );

        	worker->setState( WorkerType::WORKER_UNKNOWN );
        	job->setWorkerRank( -1 );
        }

        /**
         * Put a job into waiting list for subsequent execution.
         */
        virtual void runJob( RankType rank, bool wait = false )
        {
            JobType* job = this->getJob( rank );
            RankType wrank = rank + 1;
            WorkerType* worker = this->getWorker( wrank );

            worker->setState( WorkerType::WORKER_BUSY );
            job->execute();
            worker->setState( WorkerType::WORKER_IDLE );
        }

        /**
        Abstract function to be implemented in subclasses.
        It performs the actual job scheduling operation.
        */
        virtual void execute() {}

        /**
        Abstract function to be implemented in subclasses.
        It sends a request to the job scheduling thread to stop the service.
        */
        virtual void terminate() {}

        /**
        Return the number of workers that has been added.
        */
        virtual unsigned int getNumberOfWorkers() const
        {
            return this->m_WorkerList.size();
        }

        /**
        Check whether a worker has been added or not.
        */
        virtual bool hasWorker( RankType rank ) const
        {
            for ( WorkerList::const_iterator i = this->m_WorkerList.begin(); i != this->m_WorkerList.end(); i++ )
            {
                if ( (*i)->getRank() == rank ) { return true; }
            }
            return false;
        }

        virtual WorkerType* getWorker( RankType rank )
        {
            for ( WorkerList::iterator i = this->m_WorkerList.begin(); i != this->m_WorkerList.end(); i++ )
            {
                if ( (*i)->getRank() == rank ) { return (*i); }
            }
            return 0;
        }

        /**
        Add a new worker into the worker list.
        */
        virtual void addWorker( WorkerType* worker )
        {
            if ( worker == 0 || this->hasWorker(worker->getRank()) )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "addWorker(): not able to add worker" << End;
            	return;
            }

            worker->setState( WorkerType::WORKER_IDLE );

            this->m_WorkerList.push_back( WorkerPointer(worker) );
        }

        virtual void initialize()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): =====start=====" << End;

            typedef MPIWorkerAgent RealWorkerType;
            unsigned int nworkers = MPIContext::getNumberOfWorkers();
            for ( unsigned int i = 1; i < nworkers; i++ )
            {
                RealWorkerType::Pointer worker = RealWorkerType::New();
                worker->setRank( (RankType)i );
                this->addWorker( (RealWorkerType*)worker );
            }

            unsigned int njobs = this->getNumberOfJobs();

            if ( njobs >= nworkers )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): not able to proceed (" << (nworkers-1) << " slave workers for " << njobs << " jobs)!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): -----e-n-d-----" << End;
        }

    protected:
        typedef std::list<JobPointer> JobList;
        JobList m_JobList;

        typedef std::list<WorkerPointer> WorkerList;
        WorkerList m_WorkerList;

        MPIJobScheduler() {}

    private:
        MPIJobScheduler( const Self & ); // purposely not implemented
        MPIJobScheduler& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziMPIJobScheduler_h_
