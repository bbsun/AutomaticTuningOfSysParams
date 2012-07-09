#ifndef _sziDataDOMReader_h_
#define _sziDataDOMReader_h_

#include <itkDOMReader.h>
#include <itkDataObject.h>

namespace szi
{

    class DataDOMReader : public itk::DOMReader<itk::DataObject>
    {
    public:
        /** Standard class typedefs. */
        typedef DataDOMReader Self;
        typedef itk::DOMReader<itk::DataObject> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::DataDOMReader, DOMReader );

    protected:
        DataDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* );

    private:
        DataDOMReader( const Self & ); // purposely not implemented
        DataDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziDataDOMReader_h_
