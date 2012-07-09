#ifndef _sziImageToImageMetricDOMReader_h_
#define _sziImageToImageMetricDOMReader_h_

#include <itkSingleValuedCostFunction.h>
#include <itkImageToImageMetric.h>
#include <itkDOMReader.h>

#include <itkGradientDifferenceImageToImageMetric.h>
//
#include <itkKullbackLeiblerCompareHistogramImageToImageMetric.h>
#include <itkCorrelationCoefficientHistogramImageToImageMetric.h>
#include <itkMeanSquaresHistogramImageToImageMetric.h>
#include <itkMutualInformationHistogramImageToImageMetric.h>
#include <itkNormalizedMutualInformationHistogramImageToImageMetric.h>
//
#include <itkKappaStatisticImageToImageMetric.h>
#include <itkMatchCardinalityImageToImageMetric.h>
#include <itkMattesMutualInformationImageToImageMetric.h>
#include <itkMeanReciprocalSquareDifferenceImageToImageMetric.h>
#include <itkMeanSquaresImageToImageMetric.h>
#include <itkMutualInformationImageToImageMetric.h>
#include <itkNormalizedCorrelationImageToImageMetric.h>

#include "sziLogService.h"

namespace szi
{

    // DOM reader for itk::ImageToImageMetric
    template < typename TMetric >
    class ImageToImageMetricDOMReaderOf : public itk::DOMReader<TMetric>
    {
    public:
        /** Standard class typedefs. */
        typedef ImageToImageMetricDOMReaderOf Self;
        typedef itk::DOMReader<TMetric> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ImageToImageMetricDOMReaderOf, DOMReader );

        typedef TMetric OutputType;

        typedef typename OutputType::FixedImageType FixedImageType;
        typedef typename OutputType::MovingImageType MovingImageType;

        typedef typename Superclass::DOMNodeType DOMNodeType;

    protected:
        ImageToImageMetricDOMReaderOf() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            OutputType* output = this->GetOutput();
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname == "GradientDifferenceImageToImageMetric" )
            {
                this->GenerateGradientDifferenceImageToImageMetric( inputdom );
            }

            else if ( tagname.EndWith("HistogramImageToImageMetric") )
            {
                this->GenerateHistogramImageToImageMetric( inputdom );
            }

            else if ( tagname == "KappaStatisticImageToImageMetric" )
            {
                this->GenerateKappaStatisticImageToImageMetric( inputdom );
            }

            else if ( tagname == "MatchCardinalityImageToImageMetric" )
            {
                this->GenerateMatchCardinalityImageToImageMetric( inputdom );
            }

            else if ( tagname == "MattesMutualInformationImageToImageMetric" )
            {
                this->GenerateMattesMutualInformationImageToImageMetric( inputdom );
            }

            else if ( tagname == "MeanReciprocalSquareDifferenceImageToImageMetric" )
            {
                this->GenerateMeanReciprocalSquareDifferenceImageToImageMetric( inputdom );
            }

            else if ( tagname == "MeanSquaresImageToImageMetric" )
            {
                this->GenerateMeanSquaresImageToImageMetric( inputdom );
            }

            else if ( tagname == "MutualInformationImageToImageMetric" )
            {
                this->GenerateMutualInformationImageToImageMetric( inputdom );
            }

            else if ( tagname == "NormalizedCorrelationImageToImageMetric" )
            {
                this->GenerateNormalizedCorrelationImageToImageMetric( inputdom );
            }

            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this image-to-image metric type is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

        void GenerateGradientDifferenceImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::GradientDifferenceImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateHistogramImageToImageMetric( const DOMNodeType* inputdom )
        {
            OutputType* output = this->GetOutput();
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname == "KullbackLeiblerCompareHistogramImageToImageMetric" )
            {
                this->GenerateKullbackLeiblerCompareHistogramImageToImageMetric( inputdom );
            }

            else if ( tagname == "CorrelationCoefficientHistogramImageToImageMetric" )
            {
                this->GenerateCorrelationCoefficientHistogramImageToImageMetric( inputdom );
            }

            else if ( tagname == "MeanSquaresHistogramImageToImageMetric" )
            {
                this->GenerateMeanSquaresHistogramImageToImageMetric( inputdom );
            }

            else if ( tagname == "MutualInformationHistogramImageToImageMetric" )
            {
                this->GenerateMutualInformationHistogramImageToImageMetric( inputdom );
            }

            else if ( tagname == "NormalizedMutualInformationHistogramImageToImageMetric" )
            {
                this->GenerateNormalizedMutualInformationHistogramImageToImageMetric( inputdom );
            }

            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "This histogram-based image-to-image metric type is not supported!" << End;
            }
        }

        void GenerateKullbackLeiblerCompareHistogramImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::KullbackLeiblerCompareHistogramImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateCorrelationCoefficientHistogramImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::CorrelationCoefficientHistogramImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateMeanSquaresHistogramImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::MeanSquaresHistogramImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateMutualInformationHistogramImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::MutualInformationHistogramImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateNormalizedMutualInformationHistogramImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::NormalizedMutualInformationHistogramImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateKappaStatisticImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::KappaStatisticImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }

            itk::FancyString s;

            s = inputdom->GetAttribute( "fgvalue" );
            if ( s != "" )
            {
            	typename MetricType::RealType fgvalue = 0;
				s >> fgvalue;
				metric->SetForegroundValue( fgvalue );
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "KappaStatistic: fgvalue = " << fgvalue << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "KappaStatistic: fgvalue not available!" << End;
            }

            s = inputdom->GetAttribute( "complement" );
            if ( s == "1" || s == "on" )
            {
				metric->ComplementOn();
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "KappaStatistic: complement = on" << End;
            }
            else if ( s == "0" || s == "off" )
            {
				metric->ComplementOff();
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "KappaStatistic: complement = off" << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "KappaStatistic: complement not available!" << End;
            }
        }

        void GenerateMatchCardinalityImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::MatchCardinalityImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateMattesMutualInformationImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::MattesMutualInformationImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }

            itk::FancyString s;

            s = inputdom->GetAttribute( "NumberOfHistogramBins" );
            if ( s != "" )
            {
            	unsigned int value = 0;
            	s >> value;
            	metric->SetNumberOfHistogramBins( value );
            	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "MattesMutualInformation: NumberOfHistogramBins = " << value << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "MattesMutualInformation: NumberOfHistogramBins not available!" << End;
            }

            s = inputdom->GetAttribute( "NumberOfSpatialSamples" );
			if ( s != "" )
			{
				unsigned long value = 0;
				s >> value;
				metric->SetNumberOfSpatialSamples( value );
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "MattesMutualInformation: NumberOfSpatialSamples = " << value << End;
			}
			else
			{
				getSystemLogger() << StartWarning(this->GetNameOfClass()) << "MattesMutualInformation: NumberOfSpatialSamples not available!" << End;
			}

			/*
			s = inputdom->GetAttribute( "SamplingSeed" );
			if ( s != "" )
			{
				long value = 0;
				s >> value;
				metric->SetSamplingSeed( value );
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "MattesMutualInformation: SamplingSeed = " << value << End;
			}
			else
			{
				getSystemLogger() << StartWarning(this->GetNameOfClass()) << "MattesMutualInformation: SamplingSeed not available!" << End;
			}
			//*/
        }

        void GenerateMeanReciprocalSquareDifferenceImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::MeanReciprocalSquareDifferenceImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateMeanSquaresImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::MeanSquaresImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateMutualInformationImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::MutualInformationImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

        void GenerateNormalizedCorrelationImageToImageMetric( const DOMNodeType* inputdom )
        {
            typedef itk::NormalizedCorrelationImageToImageMetric<FixedImageType,MovingImageType> MetricType;
            //
            MetricType* metric = dynamic_cast<MetricType*>( this->GetOutput() );
            if ( !metric )
            {
                typename MetricType::Pointer object = MetricType::New();
                metric = (MetricType*)object;
                this->SetOutput( metric );
            }
            // no fields to read
        }

    private:
        ImageToImageMetricDOMReaderOf( const Self & ); // purposely not implemented
        ImageToImageMetricDOMReaderOf& operator=( const Self & ); // purposely not implemented
    };

    // DOM reader for itk::ImageToImageMetric
    class ImageToImageMetricDOMReader : public itk::DOMReader<itk::SingleValuedCostFunction>
    {
    public:
        /** Standard class typedefs. */
        typedef ImageToImageMetricDOMReader Self;
        typedef itk::DOMReader<itk::SingleValuedCostFunction> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ImageToImageMetricDOMReader, DOMReader );

    protected:
        ImageToImageMetricDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();
            if ( !tagname.EndWith("ImageToImageMetric") )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Invalid input DOM object!" << End;
            }

            itk::FancyString type = inputdom->GetAttribute( "type" );
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): metric type: " << type << End;

            itk::FancyString ftype; // fixed image type
            itk::FancyString mtype; // moving image type
            type.Split( ftype, mtype, ";" );

            // metics with default fixed image
            if ( ftype == "" )
            {
                typedef itk::Image<unsigned char> FixedImageType;
                getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for fixed image: <uchar>" << End;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <uchar>" << End;
                    typedef itk::Image<unsigned char> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }

            // metrics with 3d fixed image
            else if ( ftype.MatchWith("uchar,3") || ftype.MatchWith("unsigned char,3") )
            {
                typedef itk::Image<unsigned char,3> FixedImageType;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <uchar,3>" << End;
                    typedef itk::Image<unsigned char,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("uchar,3") || mtype.MatchWith("unsigned char,3") )
                {
                    typedef itk::Image<unsigned char,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("short,3") )
                {
                    typedef itk::Image<short,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("float,3") )
                {
                    typedef itk::Image<float,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("double,3") )
                {
                    typedef itk::Image<double,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }
            else if ( ftype.MatchWith("short,3") )
            {
                typedef itk::Image<short,3> FixedImageType;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <short,3>" << End;
                    typedef itk::Image<short,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("uchar,3") || mtype.MatchWith("unsigned char,3") )
                {
                    typedef itk::Image<unsigned char,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("short,3") )
                {
                    typedef itk::Image<short,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("float,3") )
                {
                    typedef itk::Image<float,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("double,3") )
                {
                    typedef itk::Image<double,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }
            else if ( ftype.MatchWith("float,3") )
            {
                typedef itk::Image<float,3> FixedImageType;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <float,3>" << End;
                    typedef itk::Image<float,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("uchar,3") || mtype.MatchWith("unsigned char,3") )
                {
                    typedef itk::Image<unsigned char,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("short,3") )
                {
                    typedef itk::Image<short,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("float,3") )
                {
                    typedef itk::Image<float,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("double,3") )
                {
                    typedef itk::Image<double,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }
            else if ( ftype.MatchWith("double,3") )
            {
                typedef itk::Image<double,3> FixedImageType;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <double,3>" << End;
                    typedef itk::Image<double,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("uchar,3") || mtype.MatchWith("unsigned char,3") )
                {
                    typedef itk::Image<unsigned char,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("short,3") )
                {
                    typedef itk::Image<short,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("float,3") )
                {
                    typedef itk::Image<float,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("double,3") )
                {
                    typedef itk::Image<double,3> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }

            // metrics with 2d fixed image
            else if ( ftype.MatchWith("uchar,2") || ftype.MatchWith("unsigned char,2") )
            {
                typedef itk::Image<unsigned char,2> FixedImageType;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <uchar,2>" << End;
                    typedef itk::Image<unsigned char,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("uchar,2") || mtype.MatchWith("unsigned char,2") )
                {
                    typedef itk::Image<unsigned char,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("short,2") )
                {
                    typedef itk::Image<short,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("float,2") )
                {
                    typedef itk::Image<float,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("double,2") )
                {
                    typedef itk::Image<double,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }
            else if ( ftype.MatchWith("short,2") )
            {
                typedef itk::Image<short,2> FixedImageType;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <short,2>" << End;
                    typedef itk::Image<short,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("uchar,2") || mtype.MatchWith("unsigned char,2") )
                {
                    typedef itk::Image<unsigned char,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("short,2") )
                {
                    typedef itk::Image<short,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("float,2") )
                {
                    typedef itk::Image<float,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("double,2") )
                {
                    typedef itk::Image<double,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }
            else if ( ftype.MatchWith("float,2") )
            {
                typedef itk::Image<float,2> FixedImageType;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <float,2>" << End;
                    typedef itk::Image<float,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("uchar,2") || mtype.MatchWith("unsigned char,2") )
                {
                    typedef itk::Image<unsigned char,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("short,2") )
                {
                    typedef itk::Image<short,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("float,2") )
                {
                    typedef itk::Image<float,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("double,2") )
                {
                    typedef itk::Image<double,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }
            else if ( ftype.MatchWith("double,2") )
            {
                typedef itk::Image<double,2> FixedImageType;
                //
                if ( mtype == "" )
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Use default type for moving image: <double,2>" << End;
                    typedef itk::Image<double,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("uchar,2") || mtype.MatchWith("unsigned char,2") )
                {
                    typedef itk::Image<unsigned char,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("short,2") )
                {
                    typedef itk::Image<short,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("float,2") )
                {
                    typedef itk::Image<float,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( mtype.MatchWith("double,2") )
                {
                    typedef itk::Image<double,2> MovingImageType;
                    typedef itk::ImageToImageMetric<FixedImageType,MovingImageType> MetricType;
                    typedef ImageToImageMetricDOMReaderOf<MetricType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Moving image type is not supported!" << End;
                }
            }

            // not supported fixed image type
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Fixed image type is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        ImageToImageMetricDOMReader( const Self & ); // purposely not implemented
        ImageToImageMetricDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziImageToImageMetricDOMReader_h_
