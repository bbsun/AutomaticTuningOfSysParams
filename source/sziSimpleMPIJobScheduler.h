#ifndef _sziSimpleMPIJobScheduler_h_
#define _sziSimpleMPIJobScheduler_h_

#include <itkObject.h>
#include "sziMPIJobScheduler.h"
#include "sziThreadExecuter.h"

namespace szi
{

	class ThreadedMPIWorkerAgent : public MPIWorkerAgent, public Thread
	{
    public:
        /** Standard class typedefs. */
        typedef ThreadedMPIWorkerAgent Self;
		typedef MPIWorkerAgent Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

		/** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ThreadedMPIWorkerAgent, szi::MPIWorkerAgent );

		virtual void execute()
		{
			this->start();
		}

	protected:
		virtual void run()
		{
            this->setState( WORKER_BUSY );

            JobType* job = this->getJob();
            if ( job )
            {
                job->setWorkerRank( this->getRank() );
                job->setState( JobType::JOB_PROCESSING );
                job->execute();
                job->setState( JobType::JOB_DONE );
            }

            this->setState( WORKER_IDLE );
		}

		ThreadedMPIWorkerAgent() {}

    private:
        ThreadedMPIWorkerAgent( const Self & ); // purposely not implemented
        ThreadedMPIWorkerAgent& operator=( const Self & ); // purposely not implemented
	};

    /**
    A simple implementation of MPIJobScheduler.
    */
    class SimpleMPIJobScheduler : public MPIJobScheduler, public Thread
    {
    public:
        /** Standard class typedefs. */
        typedef SimpleMPIJobScheduler Self;
        typedef MPIJobScheduler Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SimpleMPIJobScheduler, szi::MPIJobScheduler );

        /**
         * Request to allocate the resource (i.e. a worker) for a job, for subsequent execution.
         */
        virtual void startJob( RankType rank )
        {
        	JobType* job = this->getJob( rank );
        	RankType wrank = job->getWorkerRank();
        	WorkerType* worker = this->getWorker( wrank );

        	if ( wrank < 0 )
        	{
        	}
        }

        /**
         * Request to release the resource (i.e. a worker) that was previously assigned to a job.
         */
        virtual void endJob( RankType rank )
        {
        	JobType* job = this->getJob( rank );
        	RankType wrank = job->getWorkerRank();
        	WorkerType* worker = this->getWorker( wrank );

        	if ( wrank > 0 )
        	{
        	}
        }

        /**
         * Put a job into waiting list for subsequent execution.
         */
        virtual void runJob( RankType rank, bool wait = false )
        {
            JobType* job = this->getJob( rank );
            RankType wrank = job->getWorkerRank();

        	if ( wrank < 1 || job->getState() == JobType::JOB_PROCESSING ) return;

			job->setState( JobType::JOB_WAITING );
			if ( wait )
			{
				while ( job->getState() != JobType::JOB_DONE );
			}
        }

		virtual void initialize()
		{
			getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): =====start=====" << End;

			typedef ThreadedMPIWorkerAgent RealWorkerType;
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
				getSystemLogger() << StartWarning(this->GetNameOfClass()) << "initialize(): performance won't be optimal (" << (nworkers-1) << " slave workers for " << njobs << " jobs)!" << End;
            }

			getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): -----e-n-d-----" << End;
		}

		/** Thread function to perform job scheduling. */
        virtual void execute()
        {
			this->start();
        }

		/** Terminate the job scheduling service. */
        virtual void terminate()
        {
            this->m_TerminateScheduling = true;
        }

	protected:
		/** Thread function to perform job scheduling. */
        virtual void run()
        {
            this->m_TerminateScheduling = false;

            while ( true )
            {
                // terminate job scheduling if such a request was made
                if ( this->m_TerminateScheduling )
                {
                    break;
                }

                // get the next job
                JobType* job = 0;
                for ( JobList::iterator i = this->m_JobList.begin(); i != this->m_JobList.end(); i++ )
                {
                    JobType* job_i = *i;
                    if ( job_i->getState() == JobType::JOB_WAITING )
                    {
                        job = job_i;
                        break;
                    }
                }
                if ( job == 0 ) continue;

                // find the next available communication channel
                for ( WorkerList::iterator i = this->m_WorkerList.begin(); i != this->m_WorkerList.end(); i++ )
                {
                    WorkerType* worker = *i;
                    // if one available channel was found, bind it to a job
                    if ( worker->getState() == WorkerType::WORKER_IDLE )
                    {
                        worker->setJob( job );
                        worker->setState( WorkerType::WORKER_UNKNOWN );
						worker->execute();
                    }
                }
            }
        }

        SimpleMPIJobScheduler() : m_TerminateScheduling(false) {}

    private:
        SimpleMPIJobScheduler( const Self & ); // purposely not implemented
        SimpleMPIJobScheduler& operator=( const Self & ); // purposely not implemented

        bool m_TerminateScheduling;
    };

} // namespace szi

#endif // _sziSimpleMPIJobScheduler_h_
