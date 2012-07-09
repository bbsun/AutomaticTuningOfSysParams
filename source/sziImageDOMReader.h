#ifndef _sziImageDOMReader_h_
#define _sziImageDOMReader_h_

#include <itkDataObject.h>
#include <itkDOMReader.h>

#include <itkImage.h>
#include <itkImageFileReader.h>

#include "sziLogService.h"

namespace szi
{

    // DOM reader for itk::Image
    template < typename TImage >
    class ImageDOMReaderOf : public itk::DOMReader<TImage>
    {
    public:
        /** Standard class typedefs. */
        typedef ImageDOMReaderOf Self;
        typedef itk::DOMReader<TImage> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ImageDOMReaderOf, DOMReader );

        typedef TImage OutputType;

        typedef typename Superclass::DOMNodeType DOMNodeType;

    protected:
        ImageDOMReaderOf() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            if ( this->GetOutput() )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output will be ignored!" << End;
            }

            typedef OutputType ImageType;

            itk::FancyString s = inputdom->GetAttribute( "source" );

            if ( s.StartWith("file://") )
            {
                // remove the head
                s.erase( 0, strlen("file://") );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): reading image from file \"" << s << "\" ..." << End;
                //
                typedef itk::ImageFileReader<ImageType> ReaderType;
                typename ReaderType::Pointer reader = ReaderType::New();
                reader->SetFileName( s );
                reader->Update();
                ImageType* output = reader->GetOutput();
                this->SetOutput( output );
            }

            else if ( s.StartWith("create://") )
            {
                // remove the head
                s.erase( 0, strlen("create://") );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): creating image from text \"" << s << "\" ..." << End;
                // parse the string into a dictionary with separator ";" (between entries)
                // and separator "=" (between keys and their values)
                std::map<std::string,std::string> items;
                s.Split( items );
                //
                typename ImageType::Pointer image = ImageType::New();
                // read the size and allocate memory
                typename ImageType::SizeType size;
                {
                    s = items["size"];
                    getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): image size = " << s << End;
                    //
                    std::vector<int> vsize( ImageType::ImageDimension );
                    s >> vsize;
                    for ( int i = 0; i < (int)vsize.size(); i++ ) size[i] = vsize[i];
                }
                typename ImageType::RegionType region;
                {
                    region.SetSize( size );
                    //
                    typename ImageType::IndexType start;
                    start.Fill( 0 );
                    region.SetIndex( start );
                }
                image->SetRegions( region );
                image->Allocate();
                // read/set the spacing
                typename ImageType::SpacingType spacing;
                {
                    s = items["spacing"];
                    getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): image spacing = " << s << End;
                    //
                    std::vector<double> vspacing( ImageType::ImageDimension );
                    s >> vspacing;
                    for ( int i = 0; i < (int)vspacing.size(); i++ ) spacing[i] = vspacing[i];
                }
                image->SetSpacing( spacing );
                // read/set the origin
                typename ImageType::PointType origin;
                {
                    s = items["origin"];
                    getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): image origin = " << s << End;
                    //
                    if ( s[0u] == '*' )
                    {
                        for ( int i = 0; i < (int)ImageType::ImageDimension; i++ ) origin[i] = double(size[i]) * spacing[i] / -2.0;
                    }
                    else
                    {
                        std::vector<double> vorigin( ImageType::ImageDimension );
                        s >> vorigin;
                        for ( int i = 0; i < (int)vorigin.size(); i++ ) origin[i] = vorigin[i];
                    }
                }
                image->SetOrigin( origin );
                // read/set the background pixel value
                typename ImageType::PixelType bgvalue;
                {
                    s = items["bgvalue"];
                    getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): image bgvalue = " << s << End;
                    //
                    s >> bgvalue;
                }
                image->FillBuffer( bgvalue );
                //
                this->SetOutput( (ImageType*)image );
            }

            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this image source type is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        ImageDOMReaderOf( const Self & ); // purposely not implemented
        ImageDOMReaderOf& operator=( const Self & ); // purposely not implemented
    };

    // DOM reader for itk::Image
    class ImageDOMReader : public itk::DOMReader<itk::DataObject>
    {
    public:
        /** Standard class typedefs. */
        typedef ImageDOMReader Self;
        typedef itk::DOMReader<itk::DataObject> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ImageDOMReader, DOMReader );

    protected:
        ImageDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString type = inputdom->GetAttribute( "type" );
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): type = \"" << type << "\"" << End;

            // default
            if ( type == "" )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): type not provided, use the default \"uchar\"!" << End;
                //
                typedef itk::Image<unsigned char> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }

            // a 3d image
            else if ( type.MatchWith("uchar,3") || type.MatchWith("unsigned char,3") )
            {
                typedef itk::Image<unsigned char,3> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }
            else if ( type.MatchWith("short,3") )
            {
                typedef itk::Image<short,3> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }
            else if ( type.MatchWith("float,3") )
            {
                typedef itk::Image<float,3> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }
            else if ( type.MatchWith("double,3") )
            {
                typedef itk::Image<double,3> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }

            // a 2d image
            else if ( type.MatchWith("uchar,2") || type.MatchWith("unsigned char,2") )
            {
                typedef itk::Image<unsigned char,2> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }
            else if ( type.MatchWith("short,2") )
            {
                typedef itk::Image<short,2> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }
            else if ( type.MatchWith("float,2") )
            {
                typedef itk::Image<float,2> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }
            else if ( type.MatchWith("double,2") )
            {
                typedef itk::Image<double,2> ImageType;
                typedef ImageDOMReaderOf<ImageType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }

            // not supported image type
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this image type is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        ImageDOMReader( const Self & ); // purposely not implemented
        ImageDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziImageDOMReader_h_
