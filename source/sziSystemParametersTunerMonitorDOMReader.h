#ifndef _sziSystemParametersTunerMonitorDOMReader_h_
#define _sziSystemParametersTunerMonitorDOMReader_h_

#include <itkDOMReader.h>
#include <itkCommand.h>

#include "sziSimpleSystemParametersTunerMonitor.h"

namespace szi
{

    class SystemParametersTunerMonitorDOMReader : public itk::DOMReader<itk::Command>
    {
    public:
        /** Standard class typedefs. */
        typedef SystemParametersTunerMonitorDOMReader Self;
        typedef itk::DOMReader<itk::Command> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SystemParametersTunerMonitorDOMReader, DOMReader );

    protected:
        SystemParametersTunerMonitorDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname == "SimpleSystemParametersTunerMonitor" )
            {
                typedef SimpleSystemParametersTunerMonitor RealOutputType;
                //
                OutputType* o = this->GetOutput();
                RealOutputType* output = dynamic_cast<RealOutputType*>( o );
                if ( o && output == 0 )
                {
                    syslog->Warning( "The user-specified output is invalid and will be ignored!" );
                }
                //
                if ( output == 0 )
                {
                    RealOutputType::Pointer object = RealOutputType::New();
                    output = (RealOutputType*)object;
                    this->SetOutput( output );
                }
            }

            // the monitor type is not supported
            else
            {
                syslog->Fatal( "Reading of this monitor type is not supported!" );
            }
        }

    private:
        SystemParametersTunerMonitorDOMReader( const Self & ); // purposely not implemented
        SystemParametersTunerMonitorDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziSystemParametersTunerMonitorDOMReader_h_
