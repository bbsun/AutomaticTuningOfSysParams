#ifndef _sziRegistraterDOMReader_h_
#define _sziRegistraterDOMReader_h_

#include <itkDOMReader.h>
#include <itkImageRegistrationMethod.h>

#include "sziExampleRegistrater1.h"
#include "sziExampleRegistrater2.h"

#include "sziImageDOMReader.h"
#include "sziTransformDOMReader.h"
#include "sziInterpolatorDOMReader.h"
#include "sziImageToImageMetricDOMReader.h"
#include "sziOptimizerDOMReader.h"

#include "sziLogService.h"

namespace szi
{

    /**
    DOM-based reader to read an itk::ImageRegistrationMethod object.
    */
    template < typename TRegistrater >
    class RegistraterDOMReaderOf : public itk::DOMReader<TRegistrater>
    {
    public:
        /** Standard class typedefs. */
        typedef RegistraterDOMReaderOf Self;
        typedef itk::DOMReader<TRegistrater> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::RegistraterDOMReaderOf, DOMReader );

        typedef TRegistrater OutputType;

        typedef typename TRegistrater::FixedImageType FixedImageType;
        typedef typename TRegistrater::MovingImageType MovingImageType;

        typedef typename Superclass::DOMNodeType DOMNodeType;

    protected:
        RegistraterDOMReaderOf() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            OutputType* output = this->GetOutput();

            itk::FancyString tagname = inputdom->GetName();

            if ( tagname == "ExampleRegistrater1" )
            {
                typedef ExampleRegistrater1<FixedImageType,MovingImageType> RealOutputType;
                typename RealOutputType::Pointer ro = RealOutputType::New();
                output = (RealOutputType*)ro;
                this->SetOutput( output );
            }

            else if ( tagname == "ExampleRegistrater2" )
            {
                typedef ExampleRegistrater2<FixedImageType,MovingImageType> RealOutputType;
                typename RealOutputType::Pointer ro = RealOutputType::New();
                output = (RealOutputType*)ro;
                this->SetOutput( output );

                const DOMNodeType* node = 0;

                // read the first level
                node = inputdom->GetChildByID( "level-1" );
                if ( node )
                {
					typedef RegistraterDOMReaderOf<TRegistrater> ReaderType;
					typename ReaderType::Pointer reader = ReaderType::New();
					reader->Update( node );
					ro->setRegistrater( reader->GetOutput(), 1 );
                }
                else
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): level-1 settings not available!" << End;
                }

                // read the second level
				node = inputdom->GetChildByID( "level-2" );
				if ( node )
				{
					typedef RegistraterDOMReaderOf<TRegistrater> ReaderType;
					typename ReaderType::Pointer reader = ReaderType::New();
					reader->Update( node );
					ro->setRegistrater( reader->GetOutput(), 2 );
				}
				else
				{
					getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): level-2 settings not available!" << End;
				}
            }

            else if ( tagname.MatchWith("Registrater") )
			{
				typedef itk::ImageRegistrationMethod<FixedImageType,MovingImageType> RealOutputType;
				typename RealOutputType::Pointer ro = RealOutputType::New();
				output = (RealOutputType*)ro;
				this->SetOutput( output );
			}

            // the registrater type is not supported
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this registrater type is not supported!" << End;
            }

            const DOMNodeType* node = 0;

            // read fixed image
            node = inputdom->GetChildByID( "fdata" );
            if ( node )
            {
                typedef ImageDOMReaderOf<FixedImageType> ReaderType;
                typename ReaderType::Pointer reader = ReaderType::New();
                reader->Update( node );
                output->SetFixedImage( reader->GetOutput() );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Fixed image is not available!" << End;
            }

            // read moving image
            node = inputdom->GetChildByID( "mdata" );
            if ( node )
            {
                typedef ImageDOMReaderOf<MovingImageType> ReaderType;
                typename ReaderType::Pointer reader = ReaderType::New();
                reader->Update( node );
                output->SetMovingImage( reader->GetOutput() );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Moving image is not available!" << End;
            }

            // read transform
            node = inputdom->GetChildByID( "transform" );
            if ( node )
            {
                typedef typename OutputType::TransformType TransformType;
                typedef TransformDOMReaderOf<TransformType> ReaderType;
                typename ReaderType::Pointer reader = ReaderType::New();
                reader->Update( node );
                output->SetTransform( reader->GetOutput() );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Transform is not available!" << End;
            }

            // read interpolator
            node = inputdom->GetChildByID( "interpolator" );
            if ( node )
            {
                typedef typename OutputType::InterpolatorType InterpolatorType;
                typedef InterpolatorDOMReaderOf<InterpolatorType> ReaderType;
                typename ReaderType::Pointer reader = ReaderType::New();
                reader->Update( node );
                output->SetInterpolator( reader->GetOutput() );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Interpolator is not available!" << End;
            }

            // read metric
            node = inputdom->GetChildByID( "metric" );
            if ( node )
            {
                typedef typename OutputType::MetricType MetricType;
                typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                typename ReaderType::Pointer reader = ReaderType::New();
                reader->Update( node );
                output->SetMetric( reader->GetOutput() );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Metric is not available!" << End;
            }

            // read optimizer
            node = inputdom->GetChildByID( "optimizer" );
            if ( node )
            {
                typedef typename OutputType::OptimizerType OptimizerType;
                typedef OptimizerDOMReader ReaderType;
                typename ReaderType::Pointer reader = ReaderType::New();
                reader->Update( node );
                OptimizerType* o = dynamic_cast<OptimizerType*>( reader->GetOutput() );
                output->SetOptimizer( o );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Optimizer is not available!" << End;
            }

            // read monitor

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        RegistraterDOMReaderOf( const Self & ); // purposely not implemented
        RegistraterDOMReaderOf& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziRegistraterDOMReader_h_
