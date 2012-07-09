#ifndef _sziSystemData_h_
#define _sziSystemData_h_

#include <itkSingleValuedCostFunction.h>

#include <itkDataObject.h>
#include "sziMPIJobData.h"

namespace szi
{

    /**
    Abstract class to represent the associated data of a system.
    */
    class SystemData : public itk::DataObject, public MPIJobData
    {
    public:
        /** Standard class typedefs. */
        typedef SystemData Self;
        typedef itk::DataObject Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SystemData, DataObject );

        typedef itk::SingleValuedCostFunction::ParametersType ParametersType;
        typedef itk::SingleValuedCostFunction::MeasureType MeasureType;

        ParametersType m_Parameters;
        MeasureType m_Score;

        // write self to a StreamBuffer
        virtual void streamOut( StreamBuffer& sb ) const
        {
            MPIJobData::streamOut( sb );
            //
            sb << (const itk::Array<double>&)this->m_Parameters;
            sb << this->m_Score;
        }

        // read and update self from a StreamBuffer
        virtual void streamIn( StreamBuffer& sb )
        {
            MPIJobData::streamIn( sb );
            //
            sb >> (itk::Array<double>&)this->m_Parameters;
            sb >> this->m_Score;
        }

    protected:
        SystemData() : m_Score( 0 ) {}

    private:
        SystemData( const Self & ); // purposely not implemented
        SystemData& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziSystemData_h_
