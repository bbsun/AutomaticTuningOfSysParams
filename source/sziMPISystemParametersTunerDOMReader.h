#ifndef _sziMPISystemParametersTunerDOMReader_h_
#define _sziMPISystemParametersTunerDOMReader_h_

#include "sziMPIWorkerDOMReader.h"

#include "sziMPISystemParametersTunerMaster.h"
#include "sziSystemParametersTunerDOMReader.h"
#include "sziMPIJobSchedulerDOMReader.h"

#include "sziMPISystemParametersTunerSlave.h"
#include "sziSystemDOMReader.h"

#include "sziLogService.h"

namespace szi
{

    class MPISystemParametersTunerDOMReader : public MPIWorkerDOMReader
    {
    public:
        /** Standard class typedefs. */
        typedef MPISystemParametersTunerDOMReader Self;
        typedef MPIWorkerDOMReader Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using an object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPISystemParametersTunerDOMReader, szi::MPIWorkerDOMReader );

        typedef Superclass::OutputType OutputType;

    protected:
        void GenerateMaster( const DOMNodeType* inputdom )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateMaster(): =====start=====" << End;

            OutputType* o = this->GetOutput();
            typedef MPISystemParametersTunerMaster RealOutputType;
            RealOutputType* output = dynamic_cast<RealOutputType*>( o );
            //
            if ( o && output == 0 )
            {
            	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateMaster(): User-supplied master output is invalid and will be ignored!" << End;
            }
            //
            if ( output == 0 )
            {
                RealOutputType::Pointer object = RealOutputType::New();
                output = object;
                this->SetOutput( output );
            }

            const DOMNodeType* node = 0;

            // read the tuner
            node = inputdom;
            if ( node )
            {
                typedef SystemParametersTunerDOMReader ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( output->getTuner() );
                reader->Update( node );
                output->setTuner( reader->GetOutput() );
            }

            // read the job scheduler
            node = inputdom->GetChildByID( "scheduler" );
            if ( node )
            {
                typedef MPIJobSchedulerDOMReader ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( output->getJobScheduler() );
                reader->Update( node );
                output->setJobScheduler( reader->GetOutput() );
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateMaster(): -----e-n-d-----" << End;
        }

        void GenerateSlave( const DOMNodeType* inputdom )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateSlave(): =====start=====" << End;

            OutputType* o = this->GetOutput();
            typedef MPISystemParametersTunerSlave RealOutputType;
            RealOutputType* output = dynamic_cast<RealOutputType*>( o );
            //
            if ( o && output == 0 )
            {
            	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateSlave(): User-supplied slave output is invalid and will be ignored!" << End;
            }
            //
            if ( output == 0 )
            {
                RealOutputType::Pointer object = RealOutputType::New();
                output = object;
                this->SetOutput( output );
            }

            const DOMNodeType* node = 0;

            // read the system
            node = inputdom->GetChildByID( "system" );
            if ( node )
            {
                typedef SystemDOMReader ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( output->getSystem() );
                reader->Update( node );
                output->setSystem( reader->GetOutput() );
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateSlave(): -----e-n-d-----" << End;
        }

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "SystemParametersTuner" )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): The input DOM object is invalid!" << End;
            }

            if ( this->GetOutputToMaster() )
            {
                this->GenerateMaster( inputdom );
            }
            else
            {
                this->GenerateSlave( inputdom );
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

        MPISystemParametersTunerDOMReader() {}

    private:
        MPISystemParametersTunerDOMReader( const Self & ); // Purposely not implemented.
        MPISystemParametersTunerDOMReader& operator=( const Self & ); // Purposely not implemented.
    };

} // namespace szi

#endif // _sziMPISystemParametersTunerDOMReader_h_
