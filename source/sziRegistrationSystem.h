#ifndef _sziRegistrationSystem_h_
#define _sziRegistrationSystem_h_

#include "sziSystem.h"

#include <itkArray.h>

namespace szi
{

    class RegistrationSystemData : public SystemData
    {
    public:
        /** Standard class typedefs. */
        typedef RegistrationSystemData Self;
        typedef SystemData Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::RegistrationSystemData, szi::SystemData );

        // data directory
        std::string datadir;

        // each of the fixed and moving data is represented using an Actor
        struct Actor
        {
            // image file sub-directory in the data directory
            std::string sFolder;
            // CT image file name
            std::string sCT;
            // segmentation image file name
            std::string sSegmentation;
            // region-of-interest used for registration
            std::string sROI;
        };

        // the fixed data
        Actor fdata;

        // the moving data
        Actor mdata;

        // segmentation label
        short seglabel;

        // setting of the transformation parameters that determines the
        // alignment between the fixed and moving data
        typedef Superclass::ParametersType ParametersType;
        ParametersType tparams;

        // write self to a StreamBuffer
        virtual void streamOut( StreamBuffer& sb ) const
        {
            Superclass::streamOut( sb );

            sb << this->datadir;

            sb << this->fdata.sFolder;
            sb << this->fdata.sCT;
            sb << this->fdata.sSegmentation;
            sb << this->fdata.sROI;

            sb << this->mdata.sFolder;
            sb << this->mdata.sCT;
            sb << this->mdata.sSegmentation;
            sb << this->mdata.sROI;

            sb << this->seglabel;

            sb << (const itk::Array<double>&)this->tparams;
        }

        // read and update self from a StreamBuffer
        virtual void streamIn( StreamBuffer& sb )
        {
            Superclass::streamIn( sb );

            sb >> this->datadir;

            sb >> this->fdata.sFolder;
            sb >> this->fdata.sCT;
            sb >> this->fdata.sSegmentation;
            sb >> this->fdata.sROI;

            sb >> this->mdata.sFolder;
            sb >> this->mdata.sCT;
            sb >> this->mdata.sSegmentation;
            sb >> this->mdata.sROI;

            sb >> this->seglabel;

            sb >> (itk::Array<double>&)this->tparams;
        }

    protected:
        RegistrationSystemData() : seglabel( 0 ) {}

    private:
        RegistrationSystemData( const Self & ); // purposely not implemented
        RegistrationSystemData& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#include <itkImageRegistrationMethod.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkKappaStatisticImageToImageMetric.h>
#include <itkNearestNeighborInterpolateImageFunction.h>

#include <itkCenteredTransformInitializer.h>
#include <itkMatrixOffsetTransformBase.h>

#include "sziCommandInterface.h"
#include "sziTunable.h"
#include "sziBoundingBoxFinder.h"
#include "sziRegionOfInterestExtractor.h"

namespace szi
{

    class RegistrationSystem : public System, public CommandInterface
    {
    public:
        /** Standard class typedefs. */
        typedef RegistrationSystem Self;
        typedef System Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::RegistrationSystem, szi::System );

        typedef Superclass::ParametersType ParametersType;
        typedef Superclass::MeasureType MeasureType;

        typedef RegistrationSystemData DataType;

        static const unsigned int SpaceDimension = 3;

        typedef short CTPixelType;
        typedef itk::Image<CTPixelType,SpaceDimension> CTImageType;

        typedef unsigned char SegPixelType;
        typedef itk::Image<SegPixelType,SpaceDimension> SegImageType;

        typedef CTImageType::RegionType RegionType;

        typedef itk::ImageRegistrationMethod<CTImageType,CTImageType> RegistraterType;
        typedef RegistraterType::OptimizerType OptimizerType;

        typedef itk::KappaStatisticImageToImageMetric<SegImageType,SegImageType> ScorerType;

        virtual void setData( DataType* data ) { Superclass::setData( data ); }
        DataType* getData() { return static_cast<DataType*>( Superclass::getData() ); }
        const DataType* getData() const { return static_cast<const DataType*>( Superclass::getData() ); }

        virtual void setRegistrater( RegistraterType* r ) { this->m_Registrater = r; }
        RegistraterType* getRegistrater() { return this->m_Registrater; }
        const RegistraterType* getRegistrater() const { return this->m_Registrater; }

        virtual void initialize()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): =====start=====" << End;

            RegistraterType* registrater = this->getRegistrater();
            if ( registrater == 0 )
            {
                getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): Registrater is null!" << End;
            }

            OptimizerType* optimizer = registrater->GetOptimizer();
            if ( optimizer == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): Optimizer is null!" << End;
            }
            optimizer->AddObserver( itk::IterationEvent(), this->GetCommandAdapter() );

            Tunable* tunable = dynamic_cast<Tunable*>( registrater );
            if ( tunable == 0 )
            {
                getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): Registrater is not tunable!" << End;
            }

            // if user hasn't provide an initial setting for the tunable parameters,
            // use the one from the registrater
            if ( this->getNumberOfTunableParameters() == 0 )
            {
                this->setTunableParameters( tunable->getTunableParameters() );
            }

            // initialize performance score calculator
            ScorerType::Pointer scorer = ScorerType::New();
            scorer->ComplementOn();
            scorer->ComputeGradientOff();
            {
                typedef itk::NearestNeighborInterpolateImageFunction<SegImageType,double> NNInterpolatorType;
                NNInterpolatorType::Pointer nn = NNInterpolatorType::New();
                scorer->SetInterpolator( nn );
            }
            //
            this->m_Scorer = scorer;

            Superclass::initialize();

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): -----e-n-d-----" << End;
        }

        virtual void updatePerformanceScore()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): =====start=====" << End;

            DataType* data = this->getData();

            // load the data from disk
            this->loadData();

            // set the registrater
            RegistraterType* registrater = this->getRegistrater();
            registrater->SetFixedImage( this->m_FixedImage );
			registrater->SetFixedImageRegion( this->m_FixedImage->GetBufferedRegion() );
			registrater->SetMovingImage( this->m_MovingImage );

            // initialize the transform if necessary
            if ( data->tparams.GetSize() == 0 )
            {
                // the user did not provide an initial transformation setting,
                // so perform an automatic initial alignment using the two segmentations
                typedef itk::MatrixOffsetTransformBase<double,SpaceDimension,SpaceDimension> TransformType;
                TransformType::Pointer t = TransformType::New();
                //
                typedef itk::CenteredTransformInitializer<TransformType,SegImageType,SegImageType> InitializerType;
                InitializerType::Pointer initializer = InitializerType::New();
                initializer->SetTransform( (TransformType*)t );
                initializer->SetFixedImage( this->m_FixedSegImage );
                initializer->SetMovingImage( this->m_MovingSegImage );
                initializer->MomentsOn();
                initializer->InitializeTransform();
                //
                registrater->SetTransform( (TransformType*)t );
                //
                getSystemLogger() << StartWarning(this->GetNameOfClass()) << "updatePerformanceScore(): automatically compute the initial transform!" << End;
            }
            else
            {
            	registrater->SetInitialTransformParameters( data->tparams );
            }

            // set tunable parameters
            Tunable* tunable = dynamic_cast<Tunable*>( registrater );
            const ParametersType& tparams = this->getTunableParameters();
            tunable->setTunableParameters( tparams );
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): tunable parameters " << tparams << End;

            // set/initialize the score calculator
			ScorerType* scorer = this->m_Scorer;
			scorer->SetFixedImage( this->m_FixedSegImage );
			scorer->SetFixedImageRegion( this->m_FixedSegImage->GetBufferedRegion() );
			scorer->SetMovingImage( this->m_MovingSegImage );
			scorer->SetForegroundValue( data->seglabel );

            // compute the initial performance score
            const ParametersType& iparams = registrater->GetInitialTransformParameters();
            {
				scorer->SetTransform( registrater->GetTransform() );
				scorer->Initialize();
				MeasureType score = scorer->GetValue( iparams );
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): initial score = " << score << End;
            }

            // perform score computation, i.e., the registration
            tunable->updatePerformanceScore();

            // We will not use the performance score computed by the registration;
            // instead, we use the resulting transform to compute the overlap percentage between
            // the fixed and moving image segmentations.
            //
            // compute the final performance score
            const ParametersType& fparams = this->m_FinalParams;
            {
				scorer->SetTransform( registrater->GetTransform() );
				scorer->Initialize();
				MeasureType score = scorer->GetValue( fparams );
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): final score = " << score << End;
	            //
	            this->setPerformanceScore( score );
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): -----e-n-d-----" << End;
        }

        virtual void Execute( const itk::Object * caller, const itk::EventObject & eo )
        {
        	if ( ProgressEvent().CheckEvent(&eo) )
        	{
        		this->m_IterCount = 0;
        		return;
        	}

        	RegistraterType* registrater = this->getRegistrater();
        	OptimizerType* optimizer = registrater->GetOptimizer();

        	const ParametersType& params = optimizer->GetCurrentPosition();
        	double value = optimizer->GetValue( params );

        	if ( this->m_IterCount == 0 || this->m_FinalValue > value )
        	{
        		this->m_FinalParams = params;
        		this->m_FinalValue = value;
        	}

        	this->m_IterCount++;

        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "Execute(): " << this->m_IterCount << " " << this->m_FinalValue << " " << value << End;
        }

    protected:
        /**
        Read CT and segmentation images from disk.
        */
        void loadData()
        {
            DataType* data = this->getData();

            // read and crop the fixed image segmentation
            RegionType fROI;
            {
                std::string fn = data->datadir + data->fdata.sFolder + data->fdata.sSegmentation;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "loadData(): read fixed image segmentation from " << fn << End;

                typedef itk::ImageFileReader<SegImageType> ImageReaderType;
                ImageReaderType::Pointer imgReader = ImageReaderType::New();
                imgReader->SetFileName( fn.c_str() );
                imgReader->Update();
                SegImageType* fseg = imgReader->GetOutput();

                typedef BoundingBoxFinder<SegImageType> BBFinderType;
                BBFinderType::Pointer finder = BBFinderType::New();
                finder->setInput( fseg );
                finder->setLabelValue( data->seglabel );
                finder->update();
                fROI = finder->getOutput();

                typedef RegionOfInterestExtractor<SegImageType> ExtractorType;
                ExtractorType::Pointer extractor = ExtractorType::New();
                extractor->setInput( fseg );
                extractor->setRegionOfInterest( fROI );
                extractor->update();
                this->m_FixedSegImage = extractor->getOutput();
            }

            // read and crop the fixed image
            {
                std::string fn = data->datadir + data->fdata.sFolder + data->fdata.sCT;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "loadData(): read fixed image from " << fn << End;

                typedef itk::ImageFileReader<CTImageType> ImageReaderType;
                ImageReaderType::Pointer imgReader = ImageReaderType::New();
                imgReader->SetFileName( fn.c_str() );
                imgReader->Update();
                CTImageType* fImage = imgReader->GetOutput();

                typedef RegionOfInterestExtractor<CTImageType> ExtractorType;
                ExtractorType::Pointer extractor = ExtractorType::New();
                extractor->setInput( fImage );
                extractor->setRegionOfInterest( fROI );
                extractor->update();
                this->m_FixedImage = extractor->getOutput();
            }

            // read and crop the moving image segmentation
            RegionType mROI;
            {
                std::string fn = data->datadir + data->mdata.sFolder + data->mdata.sSegmentation;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "loadData(): read moving image segmentation from " << fn << End;

                typedef itk::ImageFileReader<SegImageType> ImageReaderType;
                ImageReaderType::Pointer imgReader = ImageReaderType::New();
                imgReader->SetFileName( fn.c_str() );
                imgReader->Update();
                SegImageType* mseg = imgReader->GetOutput();

                typedef BoundingBoxFinder<SegImageType> BBFinderType;
                BBFinderType::Pointer finder = BBFinderType::New();
                finder->setInput( mseg );
                finder->setLabelValue( data->seglabel );
                finder->update();
                mROI = finder->getOutput();

                typedef RegionOfInterestExtractor<SegImageType> ExtractorType;
                ExtractorType::Pointer extractor = ExtractorType::New();
                extractor->setInput( mseg );
                extractor->setRegionOfInterest( mROI );
                extractor->update();
                this->m_MovingSegImage = extractor->getOutput();
            }

            // read and crop the moving image
            {
                std::string fn = data->datadir + data->mdata.sFolder + data->mdata.sCT;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "loadData(): read moving image from " << fn << End;

                typedef itk::ImageFileReader<CTImageType> ImageReaderType;
                ImageReaderType::Pointer imgReader = ImageReaderType::New();
                imgReader->SetFileName( fn.c_str() );
                imgReader->Update();
                CTImageType* mImage = imgReader->GetOutput();

                typedef RegionOfInterestExtractor<CTImageType> ExtractorType;
                ExtractorType::Pointer extractor = ExtractorType::New();
                extractor->setInput( mImage );
                extractor->setRegionOfInterest( mROI );
                extractor->update();
                this->m_MovingImage = extractor->getOutput();
            }
        }

        RegistrationSystem() : m_IterCount(0), m_FinalValue(0)
        {
            DataType::Pointer data = DataType::New();
            this->setData( (DataType*)data );
        }

    private:
        RegistrationSystem( const Self & ); // purposely not implemented
        RegistrationSystem& operator=( const Self & ); // purposely not implemented

        RegistraterType::Pointer m_Registrater;
        ScorerType::Pointer m_Scorer;

        CTImageType::Pointer m_FixedImage;
        SegImageType::Pointer m_FixedSegImage;

        CTImageType::Pointer m_MovingImage;
        SegImageType::Pointer m_MovingSegImage;

        int m_IterCount;
        ParametersType m_FinalParams;
        double m_FinalValue;
    };

} // namespace szi

#endif // _sziRegistrationSystem_h_
