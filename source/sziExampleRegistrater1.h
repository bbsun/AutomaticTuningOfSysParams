#ifndef _sziExampleRegistrater1_h_
#define _sziExampleRegistrater1_h_

#include <itkImageRegistrationMethod.h>
#include "sziTunable.h"

#include <itkMatrixOffsetTransformBase.h>
#include <itkSimilarity3DTransform.h>

#include <itkRegularStepGradientDescentOptimizer.h>

#include "sziLogService.h"

namespace szi
{

    /**
    In this example registration, similarity transformation with three types of parameters,
    i.e. rotation (in versor format), translation and isotropic scaling,
    is used to model the motion of the moving image, and gradient descent (GD) is used to search for an optimal setting
    of the transformation parameters. The system or internal parameters to be tuned by our proposed method are
    the scaling factors for those three types of transformation parameters, as they affect whether the registration
    is biased to a special type of transformation parameters, and the overall peformance of the registration including
    failure rate, computation speed, and so on.
    */
    template < typename TFixedImage, typename TMovingImage >
    class ExampleRegistrater1 : public itk::ImageRegistrationMethod<TFixedImage,TMovingImage>, public Tunable
    {
    public:
        /** Standard class typedefs. */
        typedef ExampleRegistrater1 Self;
        typedef itk::ImageRegistrationMethod<TFixedImage,TMovingImage> Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ExampleRegistrater1, ImageRegistrationMethod );

        static const unsigned int SpaceDimension = TFixedImage::ImageDimension;

        typedef TFixedImage FixedImageType;
        typedef TMovingImage MovingImageType;

        typedef typename Tunable::ParametersType ParametersType;
        typedef typename Tunable::MeasureType MeasureType;

        typedef typename Superclass::TransformType SuperTransformType;
        typedef itk::Similarity3DTransform<double> TransformType;

        typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
        typedef typename OptimizerType::ScalesType ScalesType;

        virtual void setTunableParameters( const ParametersType& params )
        {
            Tunable::setTunableParameters( params );

            // there are three system parameters to be tuned

            OptimizerType* optimizer = dynamic_cast<OptimizerType*>( this->GetOptimizer() );
			if ( optimizer == 0 )
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "setTunableParameters(): invalid optimizer type!" << End;
			}

			TransformType* transform = dynamic_cast<TransformType*>( this->GetTransform() );
			if ( transform == 0 )
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "setTunableParameters(): invalid transform type!" << End;
			}

            ScalesType scales( transform->GetNumberOfParameters() );
            // set the optimization scaling factor for the rotation components (Versor)
            for ( int i=0; i<3; i++ )
            {
                scales[i] = ( params[0] < 1e-30 ? 1e-30 : params[0] );
            }
            // set the optimization scaling factor for the translation components (in millimeters)
            for ( int i=3; i<6; i++ )
            {
                scales[i] = ( params[1] < 1e-30 ? 1e-30 : params[1] );
            }
            // set the optimization scaling factor for the isotropic scaling (in the range of for example [0.5,2])
            scales[6] = ( params[2] < 1e-30 ? 1e-30 : params[2] );

            optimizer->SetScales( scales );
        }

        virtual void updatePerformanceScore()
        {
        	OptimizerType* optimizer = dynamic_cast<OptimizerType*>( this->GetOptimizer() );

        	optimizer->InvokeEvent( ProgressEvent() );

        	double initialStepLength = optimizer->GetMaximumStepLength();

            // perform registration
            this->Update();

            optimizer->SetMaximumStepLength( initialStepLength );

            this->GetTransform()->SetParameters( this->GetLastTransformParameters() );

            // update the performance score, though we'll not use this one
            MeasureType score = optimizer->GetValue();
            this->setPerformanceScore( score );
        }

        virtual void SetTransform( SuperTransformType* t )
        {
        	// the input transform can be an initial transform for initializing the full transform
        	typedef itk::MatrixOffsetTransformBase<double,SpaceDimension,SpaceDimension> InitialTransformType;
        	InitialTransformType* tInitial = dynamic_cast<InitialTransformType*>( t );
        	if ( tInitial )
        	{
        		// the input transform is an initial transform for initializing the transform to be estimated
        		InitialTransformType* transform = dynamic_cast<InitialTransformType*>( this->GetTransform() );
        		if ( transform == 0 )
        		{
        			getSystemLogger() << StartFatal(this->GetNameOfClass()) << "SetTransform(): transform not available!" << End;
        		}
        		//
				transform->SetIdentity();
				transform->SetCenter( tInitial->GetCenter() );
				transform->Compose( tInitial );
        		//
        		this->SetInitialTransformParameters( transform->GetParameters() );
        	}
        	else
        	{
        		getSystemLogger() << StartFatal(this->GetNameOfClass()) << "SetTransform(): transform type not supported!" << End;
        	}
        }

    protected:
        ExampleRegistrater1()
        {
            // initialize tunable parameters
            ParametersType params( 3 );
            params.Fill( 1 );
            Tunable::setTunableParameters( params );

            typename TransformType::Pointer transform = TransformType::New();
            Superclass::SetTransform( transform );
        }

    private:
        ExampleRegistrater1( const Self & ); // purposely not implemented
        ExampleRegistrater1& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziExampleRegistrater1_h_
