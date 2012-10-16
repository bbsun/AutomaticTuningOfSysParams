#ifndef _sziSystemDOMReader_h_
#define _sziSystemDOMReader_h_

#include <itkDOMReader.h>
#include "sziSystem.h"

#include "sziTestSystemDOMReader.h"
#include "sziRegistrationSystemDOMReader.h"
#include "sziLiverSegmentationSystemDOMReader.h"

#include "sziLogService.h"

namespace szi
{

    class SystemDOMReader : public itk::DOMReader<System>
    {
    public:
        /** Standard class typedefs. */
        typedef SystemDOMReader Self;
        typedef itk::DOMReader<System> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SystemDOMReader, DOMReader );

    protected:
        SystemDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            OutputType* output = this->GetOutput();
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname == "TestSystem" )
            {
                typedef TestSystemDOMReader ReaderType;
                typedef ReaderType::OutputType RealOutputType;
                //
                RealOutputType* o = dynamic_cast<RealOutputType*>( output );
                if ( output && o == 0 )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output is invalid and will be ignored!" << End;
                }
                //
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( o );
                reader->Update( inputdom );
                output = reader->GetOutput();
                //
                this->SetOutput( output );
            }

            else if ( tagname == "RegistrationSystem" )
            {
                typedef RegistrationSystemDOMReader ReaderType;
                typedef ReaderType::OutputType RealOutputType;
                //
                RealOutputType* o = dynamic_cast<RealOutputType*>( output );
                if ( output && o == 0 )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output is invalid and will be ignored!" << End;
                }
                //
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( o );
                reader->Update( inputdom );
                output = reader->GetOutput();
                //
                this->SetOutput( output );
            }
            
            else if ( tagname == "LiverSegmentationSystem" )
            {
                typedef LiverSegmentationSystemDOMReader ReaderType;
                typedef ReaderType::OutputType RealOutputType;
                //
                RealOutputType* o = dynamic_cast<RealOutputType*>( output );
                if ( output && o == 0 )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output is invalid and will be ignored!" << End;
                }
                //
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( o );
                reader->Update( inputdom );
                output = reader->GetOutput();
                //
                this->SetOutput( output );
            }

            // the metric type is not supported
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this system type is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        SystemDOMReader( const Self & ); // purposely not implemented
        SystemDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziSystemDOMReader_h_
