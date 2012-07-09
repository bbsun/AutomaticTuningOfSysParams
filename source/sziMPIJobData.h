#ifndef _sziMPIJobData_h_
#define _sziMPIJobData_h_

#include "sziStreamable.h"
#include "sziSmartPointer.h"
#include "sziMPIContext.h"

namespace szi
{

    class MPIJobData : public Streamable
    {
    public:
        typedef MPIJobData Self;
        typedef SmartPointer< Self > Pointer;
        typedef SmartPointer< const Self > ConstPointer;

        typedef MPIContext::RankType RankType;

        RankType m_Rank;
        mutable int m_OpId;

        // write self to a StreamBuffer
        virtual void streamOut( StreamBuffer& sb ) const
        {
            sb << this->m_Rank;
            sb << this->m_OpId;
        }

        // read and update self from a StreamBuffer
        virtual void streamIn( StreamBuffer& sb )
        {
            sb >> this->m_Rank;
            sb >> this->m_OpId;
        }

        MPIJobData() : m_Rank(-1), m_OpId(-1) {}
        virtual ~MPIJobData() {}
    };

} // namespace szi

#endif // _sziMPIJobData_h_
