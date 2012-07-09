#ifndef _sziMPIWorkerDOMReader_h_
#define _sziMPIWorkerDOMReader_h_

#include <itkDOMReader.h>
#include "sziMPIWorker.h"

namespace szi
{

    class MPIWorkerDOMReader : public itk::DOMReader<MPIWorker>
    {
    public:
        /** Standard class typedefs. */
        typedef MPIWorkerDOMReader Self;
        typedef itk::DOMReader<MPIWorker> Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using an object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPIWorkerDOMReader, DOMReader );

        typedef MPIWorker OutputType;

        void SetOutputToMaster( bool master = true ) { this->m_OutputToMaster = master; }
        bool GetOutputToMaster() const { return this->m_OutputToMaster; }

        void SetOutputToSlave( bool slave = true ) { this->m_OutputToMaster = !slave; }
        bool GetOutputToSlave() const { return !this->m_OutputToMaster; }

    protected:
        MPIWorkerDOMReader() : m_OutputToMaster(true) {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* );

    private:
        MPIWorkerDOMReader( const Self & ); // Purposely not implemented.
        MPIWorkerDOMReader& operator=( const Self & ); // Purposely not implemented.

        bool m_OutputToMaster;
    };

} // namespace szi

#endif // _sziMPIWorkerDOMReader_h_
