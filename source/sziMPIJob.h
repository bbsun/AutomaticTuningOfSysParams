#ifndef _sziMPIJob_h_
#define _sziMPIJob_h_

#include <itkSimpleFastMutexLock.h>

#include "sziExecutable.h"
#include "sziSmartPointer.h"
#include "sziMPIContext.h"
#include "sziMPIJobData.h"

#include "sziLogService.h"

namespace szi
{

    /**
    Abstract class to represent any job to be processed by a worker using MPI.
    */
    class MPIJob : public Executable
    {
    public:
        typedef MPIJob Self;
        typedef SmartPointer< Self > Pointer;
        typedef SmartPointer< const Self > ConstPointer;

        enum State { JOB_UNKNOWN=0, JOB_WAITING, JOB_PROCESSING, JOB_DONE };

        typedef MPIContext::RankType RankType;

        typedef MPIJobData DataType;

        virtual void setData( DataType* data ) { this->m_Data = data; }
        DataType* getData() { return this->m_Data; }
        const DataType* getData() const { return this->m_Data; }

        /** Set/get the unique ID of this job. */
        virtual void setRank( RankType rank ) { this->getData()->m_Rank = rank; }
        RankType getRank() const { return this->getData()->m_Rank; }

        /** Set/get the process, represented using an unique ID, that will do this job. */
        virtual void setWorkerRank( RankType rank ) { this->m_WorkerRank = rank; }
        RankType getWorkerRank() const { return this->m_WorkerRank; }

        /** Set/get the current state of this job. */
        virtual void setState( State state )
        {
            this->m_StateLocker.Lock();
            this->m_State = state;
            this->m_StateLocker.Unlock();
        }
        State getState() const
        {
            State result = JOB_UNKNOWN;
            this->m_StateLocker.Lock();
            result = this->m_State;
            this->m_StateLocker.Unlock();
            return result;
        }

        /**
        Abstract method to start job execution using the specified worker.
        It is called from the worker, which is usally a separate thread.
        Application-specific implementations must be provided in the subclasses.
        */
        virtual void execute() {}

    protected:
        MPIJob() : m_WorkerRank(-1), m_State(JOB_UNKNOWN) {}

    private:
        RankType m_WorkerRank;

        State m_State;
        itk::SimpleFastMutexLock m_StateLocker;

        DataType::Pointer m_Data;
    };

} // namespace szi

#endif // _sziMPIJob_h_
