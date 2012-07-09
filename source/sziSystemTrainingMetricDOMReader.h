#ifndef _sziSystemTrainingMetricDOMReader_h_
#define _sziSystemTrainingMetricDOMReader_h_

#include <itkDOMReader.h>
#include "sziSystemTrainingMetric.h"

namespace szi
{

    class SystemTrainingMetricDOMReader : public itk::DOMReader<SystemTrainingMetric>
    {
    public:
        /** Standard class typedefs. */
        typedef SystemTrainingMetricDOMReader Self;
        typedef itk::DOMReader<SystemTrainingMetric> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SystemTrainingMetricDOMReader, DOMReader );

    protected:
        SystemTrainingMetricDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "SystemTrainingMetric" )
            {
                this->GetLogger()->Fatal( "Input DOM object is invalid!\n" );
                throw 0;
            }

            OutputType* output = this->GetOutput();
            if ( output == 0 )
            {
                OutputType::Pointer object = OutputType::New();
                output = object;
                this->SetOutput( output );
            }

            // read data fields
        }

    private:
        SystemTrainingMetricDOMReader( const Self & ); // purposely not implemented
        SystemTrainingMetricDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziSystemTrainingMetricDOMReader_h_
