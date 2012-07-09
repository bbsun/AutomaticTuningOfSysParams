#ifndef _sziSystemParametersTunerDOMReader_h_
#define _sziSystemParametersTunerDOMReader_h_

#include <itkDOMReader.h>
#include "sziSystemParametersTuner.h"

#include "sziMetricDOMReader.h"
#include "sziOptimizerDOMReader.h"
#include "sziSystemDOMReader.h"
#include "sziDataSetDOMReader.h"
#include "sziSystemParametersTunerMonitorDOMReader.h"

#include "sziLogService.h"

namespace szi
{

    class SystemParametersTunerDOMReader : public itk::DOMReader<SystemParametersTuner>
    {
    public:
        /** Standard class typedefs. */
        typedef SystemParametersTunerDOMReader Self;
        typedef itk::DOMReader<SystemParametersTuner> Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using an object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SystemParametersTunerDOMReader, DOMReader );

    protected:
        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "SystemParametersTuner" )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): The input DOM object is invalid!" << End;
            }

            OutputType* output = this->GetOutput();
            if ( output == 0 )
            {
                OutputType::Pointer object = OutputType::New();
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
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): System is not available!" << End;
            }

            // read the metric
            node = inputdom->GetChildByID( "metric" );
            if ( node )
            {
                typedef MetricDOMReader ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( output->getMetric() );
                reader->Update( node );
                output->setMetric( static_cast<OutputType::MetricType*>(reader->GetOutput()) );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Metric is not available!" << End;
            }

            // read the optimizer
            node = inputdom->GetChildByID( "optimizer" );
            if ( node )
            {
                typedef OptimizerDOMReader ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( output->getOptimizer() );
                reader->Update( node );
                output->setOptimizer( static_cast<OutputType::OptimizerType*>(reader->GetOutput()) );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Optimizer is not available!" << End;
            }

            // read the data (i.e. training examples)
            node = inputdom->GetChildByID( "data" );
            if ( node )
            {
                typedef OutputType::DataType DataType;
                typedef DataSetDOMReader<DataType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetOutput( output->getData() );
                reader->Update( node );
                output->setData( reader->GetOutput() );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): DataSet (training examples) is not available!" << End;
            }

            // read the monitor of the tuning process
            node = inputdom->GetChildByID( "monitor" );
            if ( node )
            {
                typedef SystemParametersTunerMonitorDOMReader ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( node );
                output->AddObserver( itk::IterationEvent(), reader->GetOutput() );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): System tuning monitor is not available!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

        SystemParametersTunerDOMReader() {}

    private:
        SystemParametersTunerDOMReader( const Self & ); // Purposely not implemented.
        SystemParametersTunerDOMReader& operator=( const Self & ); // Purposely not implemented.
    };

} // namespace szi

#endif // _sziSystemParametersTunerDOMReader_h_
