#ifndef _sziExampleRegistrater2_h_
#define _sziExampleRegistrater2_h_

#include <itkImageRegistrationMethod.h>
#include "sziTunable.h"

#include <itkMatrixOffsetTransformBase.h>
#include <itkSimilarity3DTransform.h>
#include <itkBSplineDeformableTransform.h>
#include "sziBSplineDeformableTransformInitializer.h"

#include <itkRegularStepGradientDescentOptimizer.h>

#include <vector>

#include "sziLogService.h"

namespace szi
{

    /**
    This example registration is similar to ExampleRegistrater1 but includes an additional level of deformable registration.
    In the first level of registration, the transformation and system parameters are the same as the previous example.
    In the second level, B-spline free-form deformation (FFD) is used to model the local deformation of the moving image, and
    the optimizer remains the same (i.e. GD). We include an additional parameter, i.e. the relaxation factor in GD, as the tunable
    system parameter. The setting of this parameter can greatly affect the convergence speed of such applications as
    FFD registration is an expensive operation and users want to obtain the best result with the least computation time.
    */
    template < typename TFixedImage, typename TMovingImage >
    class ExampleRegistrater2 : public itk::ImageRegistrationMethod<TFixedImage,TMovingImage>, public Tunable
    {
    public:
        /** Standard class typedefs. */
        typedef ExampleRegistrater2 Self;
        typedef itk::ImageRegistrationMethod<TFixedImage,TMovingImage> Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ExampleRegistrater2, ImageRegistrationMethod );

        typedef TFixedImage FixedImageType;
        typedef TMovingImage MovingImageType;

        static const unsigned int SpaceDimension = 3;
        static const unsigned int SplineOrder = 3;

        typedef typename Tunable::ParametersType ParametersType;
        typedef typename Tunable::MeasureType MeasureType;

        typedef itk::Similarity3DTransform<double> CoarseTransformType;
        typedef itk::BSplineDeformableTransform<double,SpaceDimension,SplineOrder> FineTransformType;

        typedef typename Superclass::TransformType TransformType;

        typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
        typedef typename OptimizerType::ScalesType ScalesType;

        typedef Superclass RegistraterType;
        typedef typename RegistraterType::Pointer RegistraterPointer;

        void setRegistrater( RegistraterType* registrater, int level )
        {
        	if ( level >0 && level <= 2 )
        	{
        		this->m_Registraters[level-1] = registrater;
        	}
        	else
        	{
        		getSystemLogger() << StartFatal(this->GetNameOfClass()) << "setRegistrater(): invalid level number!" << End;
        	}
        }

        virtual void setTunableParameters( const ParametersType& params )
        {
            Tunable::setTunableParameters( params );

            RegistraterType* registrater = 0;

            // there are three system parameters to be tuned in the first registration level

            registrater = this->m_Registraters[0];

            OptimizerType* optimizer1 = dynamic_cast<OptimizerType*>( registrater->GetOptimizer() );
			if ( optimizer1 == 0 )
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "setTunableParameters(): invalid level-1 optimizer!" << End;
			}

			CoarseTransformType* transform1 = dynamic_cast<CoarseTransformType*>( registrater->GetTransform() );
			if ( transform1 == 0 )
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "setTunableParameters(): invalid level-1 transform!" << End;
			}

            ScalesType scales( transform1->GetNumberOfParameters() );
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

            optimizer1->SetScales( scales );

            // there is one additional system parameter to be tuned in the second registration level

            registrater = this->m_Registraters[1];

            OptimizerType* optimizer2 = dynamic_cast<OptimizerType*>( registrater->GetOptimizer() );
			if ( optimizer2 == 0 )
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "setTunableParameters(): invalid level-2 optimizer!" << End;
			}

			FineTransformType* transform2 = dynamic_cast<FineTransformType*>( registrater->GetTransform() );
			if ( transform2 == 0 )
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "setTunableParameters(): invalid level-2 transform!" << End;
			}

            // set the optimization relaxation factor between iterations
            double rfactor = params[3];
            if ( rfactor > 0.9 ) rfactor = 0.9;
            if ( rfactor < 0.1 ) rfactor = 0.1;

            optimizer2->SetRelaxationFactor( rfactor );
        }

        virtual void updatePerformanceScore()
        {
            // perform registration
            this->Update();
        }

        virtual void Update()
        {
        	OptimizerType* optimizer = dynamic_cast<OptimizerType*>( this->GetOptimizer() );
			if ( optimizer == 0 )
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "Update(): invalid optimizer!" << End;
			}

            RegistraterType* registrater = 0;

            // perform the coarse-level registration

            optimizer->InvokeEvent( ProgressEvent() );

            registrater = this->m_Registraters[0];

            Superclass::SetTransform( registrater->GetTransform() );
            Superclass::SetInitialTransformParameters( registrater->GetInitialTransformParameters() );

            this->SetMetric( registrater->GetMetric() );
            this->SetInterpolator( registrater->GetInterpolator() );

            OptimizerType* optimizer1 = dynamic_cast<OptimizerType*>( registrater->GetOptimizer() );
            optimizer->SetMaximumStepLength( optimizer1->GetMaximumStepLength() );
            optimizer->SetMinimumStepLength( optimizer1->GetMinimumStepLength() );
            optimizer->SetNumberOfIterations( optimizer1->GetNumberOfIterations() );
            optimizer->SetGradientMagnitudeTolerance( optimizer1->GetGradientMagnitudeTolerance() );
            optimizer->SetRelaxationFactor( optimizer1->GetRelaxationFactor() );
            //
            optimizer->SetScales( optimizer1->GetScales() );

        	Superclass::Update();

        	this->GetTransform()->SetParameters( this->GetLastTransformParameters() );

            // perform the fine-level registration

        	optimizer->InvokeEvent( ProgressEvent() );

        	registrater = this->m_Registraters[1];

			FineTransformType* transform2 = dynamic_cast<FineTransformType*>( registrater->GetTransform() );
			transform2->SetBulkTransform( this->GetTransform() );
			//
			typename FineTransformType::SizeType gridSizeOnImage;
			gridSizeOnImage = transform2->GetGridRegion().GetSize();
			for ( int i = 0; i < SpaceDimension; i++ ) gridSizeOnImage[i] -= SplineOrder;
			//
			typedef BSplineDeformableTransformInitializer<FineTransformType,FixedImageType> InitializerType;
			typename InitializerType::Pointer initializer = InitializerType::New();
			initializer->SetTransform( transform2 );
			initializer->SetImage( this->GetFixedImage() );
			initializer->SetGridSizeInsideTheImage( gridSizeOnImage );
			initializer->InitializeTransform();
			//
			ParametersType iparams2( transform2->GetNumberOfParameters() );
			iparams2.Fill( 0 );
			Superclass::SetInitialTransformParameters( iparams2 );
			transform2->SetParameters( this->GetInitialTransformParameters() );
			//
			Superclass::SetTransform( transform2 );

            this->SetMetric( registrater->GetMetric() );
            this->SetInterpolator( registrater->GetInterpolator() );

            OptimizerType* optimizer2 = dynamic_cast<OptimizerType*>( registrater->GetOptimizer() );
            optimizer->SetMaximumStepLength( optimizer2->GetMaximumStepLength() );
            optimizer->SetMinimumStepLength( optimizer2->GetMinimumStepLength() );
            optimizer->SetNumberOfIterations( optimizer2->GetNumberOfIterations() );
            optimizer->SetGradientMagnitudeTolerance( optimizer2->GetGradientMagnitudeTolerance() );
            optimizer->SetRelaxationFactor( optimizer2->GetRelaxationFactor() );
            //
            ScalesType scales2( transform2->GetNumberOfParameters() );
            scales2.Fill( 1 );
            optimizer->SetScales( scales2 );

        	Superclass::Update();

        	this->GetTransform()->SetParameters( this->GetLastTransformParameters() );

            // update the performance score, though we'll not use this one
            MeasureType score = optimizer->GetValue();
            this->setPerformanceScore( score );
        }

        virtual void SetTransform( TransformType* t )
		{
        	RegistraterType* registrater = this->m_Registraters[0];

			// the input transform can be an initial transform for initializing the full transform
			typedef itk::MatrixOffsetTransformBase<double,SpaceDimension,SpaceDimension> InitialTransformType;
			InitialTransformType* tInitial = dynamic_cast<InitialTransformType*>( t );
			if ( tInitial )
			{
				// the input transform is an initial transform for initializing the transform to be estimated
				InitialTransformType* transform = dynamic_cast<InitialTransformType*>( registrater->GetTransform() );
        		if ( transform == 0 )
        		{
        			getSystemLogger() << StartFatal(this->GetNameOfClass()) << "SetTransform(): level-1 transform not available!" << End;
        		}
				//
				transform->SetIdentity();
				transform->SetCenter( tInitial->GetCenter() );
				transform->Compose( tInitial );
				//
				registrater->SetInitialTransformParameters( transform->GetParameters() );
			}
			else
			{
				getSystemLogger() << StartFatal(this->GetNameOfClass()) << "SetTransform(): transform type not supported!" << End;
			}

        	Superclass::SetTransform( registrater->GetTransform() );
        	Superclass::SetInitialTransformParameters( registrater->GetInitialTransformParameters() );
		}

        virtual void SetInitialTransformParameters( const ParametersType& params )
        {
        	RegistraterType* registrater = this->m_Registraters[0];
        	registrater->GetTransform()->SetParameters( params );
        	registrater->SetInitialTransformParameters( params );

        	Superclass::SetTransform( registrater->GetTransform() );
        	Superclass::SetInitialTransformParameters( registrater->GetInitialTransformParameters() );
        }

    protected:
        ExampleRegistrater2() : m_Registraters(2)
        {
            // initialize tunable parameters
            ParametersType params( 4 );
            params.Fill( 1 );
            params[3] = 0.5;
            Tunable::setTunableParameters( params );

            this->m_Registraters[0] = RegistraterType::New();
            //
            typename CoarseTransformType::Pointer tCoarse = CoarseTransformType::New();
            this->m_Registraters[0]->SetTransform( tCoarse );
            Superclass::SetTransform( tCoarse );

            this->m_Registraters[1] = RegistraterType::New();
            //
            typename FineTransformType::Pointer tFine = FineTransformType::New();
            tFine->SetBulkTransform( tCoarse );
            this->m_Registraters[1]->SetTransform( tFine );

            typename OptimizerType::Pointer optimizer = OptimizerType::New();
            this->SetOptimizer( optimizer );
        }

    private:
        ExampleRegistrater2( const Self & ); // purposely not implemented
        ExampleRegistrater2& operator=( const Self & ); // purposely not implemented

        std::vector<RegistraterPointer> m_Registraters;
    };

} // namespace szi

#endif // _sziExampleRegistrater2_h_
