#ifndef _sziRegionOfInterestExtractor_h_
#define _sziRegionOfInterestExtractor_h_

#include <itkObject.h>
#include <itkResampleImageFilter.h>
#include <itkIdentityTransform.h>
#include <itkNearestNeighborInterpolateImageFunction.h>

namespace szi
{

    template < class TImage >
    class RegionOfInterestExtractor : public itk::Object
    {
    public:
        /** Standard class typedefs. */
        typedef RegionOfInterestExtractor Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::RegionOfInterestExtractor, Object );

        /** User-defined types. */
        typedef TImage ImageType;
        typedef typename ImageType::PixelType PixelType;
        typedef typename ImageType::RegionType RegionType;
        //
        typedef ImageType InputType;
        typedef ImageType OutputType;

        void setInput( InputType* input )
        {
            _input = input;
        }

        void setRegionOfInterest( const RegionType& roi )
        {
            _roi = roi;
        }

        void setDefaultPixelValue( PixelType value )
        {
            _defvalue = value;
        }

        void update()
        {
            _output = 0;
            if ( !(InputType*)_input ) return;
            //
            _input->Update();
            //
            typedef itk::ResampleImageFilter<ImageType,ImageType,double> ResamplerType;
            typename ResamplerType::Pointer resampler = ResamplerType::New();
            //
            typedef itk::IdentityTransform<double,3> IdentityTransformType;
            typename IdentityTransformType::Pointer I = IdentityTransformType::New();
            resampler->SetTransform( I );
            //
            resampler->SetInput( _input );
            resampler->SetSize( _roi.GetSize() );
            //
            typename ImageType::PointType origin = _input->GetOrigin();
            for ( int i=0; i<ImageType::ImageDimension; i++ )
            {
                origin[i] = _input->GetSpacing()[i] * ( _roi.GetIndex(i) - _input->GetLargestPossibleRegion().GetIndex(i) ) + origin[i];
            }
            resampler->SetOutputOrigin( origin );
            //
            resampler->SetOutputSpacing( _input->GetSpacing() );
            resampler->SetOutputDirection( _input->GetDirection() );
            resampler->SetDefaultPixelValue( _defvalue );
            //
            typedef itk::NearestNeighborInterpolateImageFunction<ImageType,double> InterpolatorType;
            typename InterpolatorType::Pointer nninterp = InterpolatorType::New();
            resampler->SetInterpolator( nninterp );
            //
            resampler->Update();
            //
            _output = resampler->GetOutput();
        }

        OutputType* getOutput()
        {
            return _output;
        }
        //
        const OutputType* getOutput() const
        {
            return _output;
        }

    protected:
        RegionOfInterestExtractor() : _defvalue() {}

    private:
        RegionOfInterestExtractor( const Self & ); // purposely not implemented
        RegionOfInterestExtractor& operator=( const Self & ); // purposely not implemented

        typename InputType::Pointer _input;
        RegionType _roi;
        PixelType _defvalue;
        //
        typename OutputType::Pointer _output;
    };

} // namespace szi

#endif // _sziRegionOfInterestExtractor_h_
