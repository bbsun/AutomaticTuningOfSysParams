#ifndef _sziMPISystemParametersTunerMaster_h_
#define _sziMPISystemParametersTunerMaster_h_

#include <itkObject.h>
#include "sziMPIWorker.h"
#include "sziSystemParametersTuner.h"
#include "sziMPISystemAgent.h"
#include "sziMPIJobScheduler.h"

namespace szi
{

    class MPISystemParametersTunerMaster : public itk::Object, public MPIWorker
    {
    public:
        /** Standard class typedefs. */
        typedef MPISystemParametersTunerMaster Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPISystemParametersTunerMaster, Object );

        typedef MPIContext::RankType RankType;

        typedef SystemParametersTuner TunerType;
        typedef TunerType::SystemType SystemType;

        typedef MPIJobScheduler SchedulerType;

        virtual void setTuner( TunerType* tuner ) { this->setJob( tuner ); }
        TunerType* getTuner() { return static_cast<TunerType*>( this->getJob() ); }
        const TunerType* getTuner() const { return static_cast<const TunerType*>( this->getJob() ); }

        virtual void setJobScheduler( SchedulerType* scheduler ) { this->m_JobScheduler = scheduler; }
        SchedulerType* getJobScheduler() { return this->m_JobScheduler; }
        const SchedulerType* getJobScheduler() const { return this->m_JobScheduler; }

        virtual void initialize()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): =====start=====" << End;

        	if ( this->isInitialized() )
        	{
        		getSystemLogger() << StartWarning(this->GetNameOfClass()) << "initialize(): initialization has been done!" << End;
        		return;
        	}

            TunerType* tuner = this->getTuner();
            if ( tuner == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): tuner is null" << End;
            }

            // Create a default job scheduler if the user hasn't provided one.
            SchedulerType* scheduler = this->getJobScheduler();
            if ( scheduler == 0 )
            {
                MPIJobScheduler::Pointer object = MPIJobScheduler::New();
                scheduler = (MPIJobScheduler*)object;
                this->setJobScheduler( scheduler );
            }

            // Get the data (training examples) from the tuner for subsequent job agents creation.
            typedef TunerType::DataType DataType;
            DataType* data = tuner->getData();

            // Create and add all job agents on the master side that will run on slaves.
            typedef MPISystemAgent JobType;
            unsigned int njobs = data->size();
            for ( unsigned int i = 0; i < njobs; i++ )
            {
                JobType::Pointer job = JobType::New();
                job->setData( data->at(i) );
                job->setRank( (RankType)i );
                scheduler->addJob( (JobType*)job );
            }

            scheduler->initialize();

            SystemType* system = tuner->getSystem();
            if ( system == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): system is null" << End;
            }

            // As performance score computations within the tuner (i.e. master-side jobs) are actually
            // performed on slave sides, the system component of the tuner needs to be replaced with an agent
            // that communicates with remote slaves.
            JobType::Pointer agent = JobType::New();
            // The agent will use a job scheduler to choose which slave to do the job.
            agent->setJobScheduler( scheduler );
            // The agent will have the same tunable parameters as the one it represents.
            agent->setTunableParameters( system->getTunableParameters() );
            // Replace the original system with this agent.
            tuner->setSystem( agent );

            tuner->initialize();

            MPIWorker::initialize();

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): -----e-n-d-----" << End;
        }

        virtual void execute()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): =====start=====" << End;

        	if ( !this->isInitialized() )
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "execute(): worker not initialized" << End;
			}

            this->getJobScheduler()->execute();
            //
            try
            {
            	MPIWorker::execute();
            }
            catch (...)
            {
            }
            //
            this->getJobScheduler()->terminate();

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): -----e-n-d-----" << End;
        }

        virtual void finalize()
        {
        	MPIContext::terminateAllSlaves();
        	MPIWorker::finalize();
        }

    protected:
        MPISystemParametersTunerMaster() {}

    private:
        MPISystemParametersTunerMaster( const Self & ); // Purposely not implemented.
        MPISystemParametersTunerMaster& operator=( const Self & ); // Purposely not implemented.

        SchedulerType::Pointer m_JobScheduler;
    };

} // namespace szi

#endif // _sziMPISystemParametersTunerMaster_h_
