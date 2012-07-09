#ifndef _sziMPIWorker_h_
#define _sziMPIWorker_h_

#include <itkSimpleFastMutexLock.h>

#include "sziExecutable.h"
#include "sziSmartPointer.h"
#include "sziMPIJob.h"
#include "sziMPIContext.h"
#include "sziLogService.h"

namespace szi
{

    class MPIWorker : public Executable
    {
    public:
        typedef MPIWorker Self;
        typedef SmartPointer< Self > Pointer;
        typedef SmartPointer< const Self > ConstPointer;

        enum State { WORKER_UNKNOWN=0, WORKER_IDLE, WORKER_BUSY };

        typedef MPIContext::RankType RankType;

        typedef MPIJob JobType;

        virtual void setRank( RankType rank )
        {
            this->m_Rank = rank;
        }
        RankType getRank() const { return this->m_Rank; }

        /** Set/get the current state of this worker. */
        virtual void setState( State state )
        {
            this->m_StateLocker.Lock();
            this->m_State = state;
            this->m_StateLocker.Unlock();
        }
        State getState() const
        {
            State result = WORKER_UNKNOWN;
            this->m_StateLocker.Lock();
            result = this->m_State;
            this->m_StateLocker.Unlock();
            return result;
        }

        /** Associate a job with this worker for execution. */
        virtual void setJob( JobType* job ) { this->m_Job = job; }
        JobType* getJob() { return this->m_Job; }
        const JobType* getJob() const { return this->m_Job; }

        /**
        Execute the associated job, and set the corresponding states
        for this worker as well as the associated job.
        */
        virtual void execute()
        {
            JobType* job = this->getJob();
            if ( job )
            {
                job->setWorkerRank( this->getRank() );
                job->setState( JobType::JOB_PROCESSING );
                job->execute();
                job->setState( JobType::JOB_DONE );
            }
        }

    protected:
        MPIWorker() : m_Rank(-1), m_State(WORKER_UNKNOWN) {}

    private:
        /** Variable to hold the unique ID of this worker. */
        RankType m_Rank;

        /** Variable to hold the current state of this worker. */
        State m_State;
        itk::SimpleFastMutexLock m_StateLocker;

        /** Variable to hold the job that is serviced by this worker. */
        JobType::Pointer m_Job;
    };

} // namespace szi

#endif // _sziMPIWorker_h_
