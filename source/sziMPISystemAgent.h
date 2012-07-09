#ifndef _sziMPISystemAgent_h_
#define _sziMPISystemAgent_h_

#include "sziSystem.h"
#include "sziMPISystemParametersTunerContext.h"
#include "sziMPIJobScheduler.h"

namespace szi
{

    /**
    Master-side implementation of System. It receives requests from the master and
    forwards them to an available slave to complete the requested jobs, then gets the results
    from the slave and forwards them to the master.
    */
    class MPISystemAgent : public System
    {
    public:
        /** Standard class typedefs. */
        typedef MPISystemAgent Self;
        typedef System Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPISystemAgent, szi::System );

        typedef MPIJobScheduler SchedulerType;

        typedef MPIJob::RankType RankType;

        virtual void setJobScheduler( SchedulerType* scheduler ) { this->m_JobScheduler = scheduler; }
        virtual SchedulerType* getJobScheduler() const { return this->m_JobScheduler; }

        virtual MeasureType getPerformanceScore() const
        {
            const DataType* data = this->getData();

            data->m_OpId = MPISystemParametersTunerContext::TAG_SPT_GET_SCORE;

            // will block until job execution is finished
            this->getJobScheduler()->runJob( data->m_Rank, true );
            this->getJobScheduler()->endJob( data->m_Rank );

            const_cast<Self*>(this)->setPerformanceScore( data->m_Score );

            return Superclass::getPerformanceScore();
        }

        virtual void updatePerformanceScore()
        {
            DataType* data = this->getData();

            data->m_Parameters = this->getTunableParameters();
            data->m_OpId = MPISystemParametersTunerContext::TAG_SPT_UPDATE_SCORE;

            // just send the request to run this job, will return right away
            this->getJobScheduler()->startJob( data->m_Rank );
            this->getJobScheduler()->runJob( data->m_Rank );
        }

        /**
        MPIJob method to process the data using the assigned worker.
        This method is called by a slave worker agent, and runs in a thread.
        */
        virtual void execute()
        {
            DataType* data = this->getData();
            RankType rank = this->getWorkerRank();

            if ( data->m_OpId == MPISystemParametersTunerContext::TAG_SPT_UPDATE_SCORE )
            {
                // send the command type to the slave worker
                MPIContext::send( rank, MPISystemParametersTunerContext::TAG_SPT_UPDATE_SCORE );

                // send the system data to the slave worker
                MPIContext::send( (Streamable&)(*data), rank, MPISystemParametersTunerContext::TAG_SPT_UPDATE_SCORE );

                int tag = MPIContext::receive( rank );
                if ( tag != MPIContext::TAG_OK )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "execute(): communication failed!" << End;
                }
            }

            else if ( data->m_OpId == MPISystemParametersTunerContext::TAG_SPT_GET_SCORE )
            {
                // send the command type to the slave worker
                MPIContext::send( rank, MPISystemParametersTunerContext::TAG_SPT_GET_SCORE );

                int tag = MPIContext::receive( rank );
                if ( tag != MPIContext::TAG_OK )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "execute(): communication failed!" << End;
                }

                // wait for the result from the slave worker
                double score = 0;
                MPIContext::receive( score, rank, MPISystemParametersTunerContext::TAG_SPT_GET_SCORE );

                // update the score in the system data
                data->m_Score = score;
            }
        }

    protected:
        MPISystemAgent() {}

    private:
        MPISystemAgent( const Self & ); // purposely not implemented
        MPISystemAgent& operator=( const Self & ); // purposely not implemented

        mutable SchedulerType::Pointer m_JobScheduler;
    };

} // namespace szi

#endif // _sziMPISystemAgent_h_
