#ifndef _sziMetricDOMReader_h_
#define _sziMetricDOMReader_h_

#include <itkDOMReader.h>
#include <itkCostFunction.h>

#include "sziSystemTrainingMetricDOMReader.h"

namespace szi
{

    class MetricDOMReader : public itk::DOMReader<itk::CostFunction>
    {
    public:
        /** Standard class typedefs. */
        typedef MetricDOMReader Self;
        typedef itk::DOMReader<itk::CostFunction> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MetricDOMReader, DOMReader );

    protected:
        MetricDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;
            
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname == "SystemTrainingMetric" )
            {
                typedef SystemTrainingMetricDOMReader ReaderType;
                typedef ReaderType::OutputType RealOutputType;
                //
                OutputType* o = this->GetOutput();
                RealOutputType* output = dynamic_cast<RealOutputType*>( o );
                if ( o && output == 0 )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output is invalid and will be ignored!" << End;
                }
                //
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( output );
                reader->Update( inputdom );
                output = reader->GetOutput();
                //
                this->SetOutput( output );
            }

            // the metric type is not supported
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this metric type is not supported!" << End;
            }
            
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        MetricDOMReader( const Self & ); // purposely not implemented
        MetricDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziMetricDOMReader_h_
