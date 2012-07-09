#ifndef _sziBoundingBoxFinder_h_
#define _sziBoundingBoxFinder_h_

#include <itkObject.h>
#include <itkImage.h>
#include <itkImageRegionIteratorWithIndex.h>

namespace szi
{

    template < class TImage >
    class BoundingBoxFinder : public itk::Object
    {
    public:
        /** Standard class typedefs. */
        typedef BoundingBoxFinder Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::BoundingBoxFinder, Object );

        /** User-defined types. */
        typedef TImage ImageType;
        typedef typename ImageType::PixelType PixelType;
        typedef typename ImageType::RegionType RegionType;
        //
        typedef ImageType InputType;
        typedef RegionType OutputType;

        void setInput( InputType* input )
        {
            _input = input;
        }

        void setLabelValue( PixelType value )
        {
            _value = value;
        }

        void update()
        {
            _output = OutputType();
            if ( !(InputType*)_input ) return;
            //
            _input->Update();
            //
            typedef typename ImageType::IndexType IndexType;
            IndexType minIndex;
            IndexType maxIndex;
            //
            bool first_time = true;
            //
            typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;
            IteratorType it( _input, _input->GetBufferedRegion() );
            for ( it.GoToBegin(); !it.IsAtEnd(); ++it )
            {
                if ( it.Get() != _value ) continue;
                //
                IndexType idx = it.GetIndex();
                if ( first_time )
                {
                    minIndex = idx;
                    maxIndex = idx;
                    first_time = false;
                }
                else
                {
                    for ( int i=0; i<ImageType::ImageDimension; i++ )
                    {
                        if ( minIndex[i] > idx[i] ) minIndex[i] = idx[i];
                    }
                    //
                    for ( int i=0; i<ImageType::ImageDimension; i++ )
                    {
                        if ( maxIndex[i] < idx[i] ) maxIndex[i] = idx[i];
                    }
                }
            }
            //
            _output.SetIndex( minIndex );
            //
            typedef typename ImageType::SizeType SizeType;
            SizeType size;
            for ( int i=0; i<ImageType::ImageDimension; i++ ) size[i] = maxIndex[i] - minIndex[i] + 1;
            _output.SetSize( size );
        }

        OutputType& getOutput()
        {
            return _output;
        }
        //
        const OutputType& getOutput() const
        {
            return _output;
        }

    protected:
        BoundingBoxFinder() : _value() {}

    private:
        BoundingBoxFinder( const Self & ); // purposely not implemented
        BoundingBoxFinder& operator=( const Self & ); // purposely not implemented

        typename InputType::Pointer _input;
        PixelType _value;
        //
        OutputType _output;
    };

} // namespace szi

#endif // _sziBoundingBoxFinder_h_
