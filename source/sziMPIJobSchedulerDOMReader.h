#ifndef _sziMPIJobSchedulerDOMReader_h_
#define _sziMPIJobSchedulerDOMReader_h_

#include <itkDOMReader.h>
#include "sziMPIJobScheduler.h"

#include "sziSimpleMPIJobScheduler.h"

#include "sziLogService.h"

namespace szi
{

    class MPIJobSchedulerDOMReader : public itk::DOMReader<MPIJobScheduler>
    {
    public:
        /** Standard class typedefs. */
        typedef MPIJobSchedulerDOMReader Self;
        typedef itk::DOMReader<MPIJobScheduler> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPIJobSchedulerDOMReader, DOMReader );

    protected:
        MPIJobSchedulerDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();

            if ( tagname == "MPIJobScheduler" )
            {
				OutputType::Pointer output = OutputType::New();
				this->SetOutput( output );
            }

            else if ( tagname == "SimpleMPIJobScheduler" )
			{
				typedef SimpleMPIJobScheduler RealOutputType;
				RealOutputType::Pointer object = RealOutputType::New();
				OutputType* output = (RealOutputType*)object;
				this->SetOutput( output );
			}

            // the scheduler type is not supported
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this scheduler type is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        MPIJobSchedulerDOMReader( const Self & ); // purposely not implemented
        MPIJobSchedulerDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziMPIJobSchedulerDOMReader_h_
