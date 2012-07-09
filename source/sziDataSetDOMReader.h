#ifndef _sziDataSetDOMReader_h_
#define _sziDataSetDOMReader_h_

#include <itkDOMReader.h>
#include "sziDataSet.h"

namespace szi
{

    template < class TDataSet >
    class DataSetDOMReader : public itk::DOMReader<TDataSet>
    {
    public:
        /** Standard class typedefs. */
        typedef DataSetDOMReader Self;
        typedef itk::DOMReader<TDataSet> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::DataSetDOMReader, DOMReader );

        typedef TDataSet OutputType;

        typedef typename Superclass::DOMNodeType DOMNodeType;

    protected:
        DataSetDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* );

    private:
        DataSetDOMReader( const Self & ); // purposely not implemented
        DataSetDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#include "sziDataSetDOMReader.hxx"

#endif // _sziDataSetDOMReader_h_
