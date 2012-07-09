#ifndef _sziDataSet_h_
#define _sziDataSet_h_

#include <itkDataObject.h>
#include <vector>
#include "sziStreamable.h"

namespace szi
{

    /**
    Class to store a set of streamable data.
    */
    template < class TElement >
    class DataSet : public itk::DataObject, public std::vector<typename TElement::Pointer>, public Streamable
    {
    public:
        /** Standard class typedefs. */
        typedef DataSet Self;
        typedef itk::DataObject Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::DataSet, DataObject );

        /** User-defined types. */
        typedef TElement ElementType;

        virtual void streamOut( StreamBuffer& sb ) const
        {
            unsigned int n = this->size();
            sb << n;
            //
            for ( unsigned int i = 0; i < n; i++ )
            {
                const ElementType* e = this->at(i);
                //
                typename ElementType::Pointer tempobj;
                if ( e == 0 )
                {
                    tempobj = ElementType::New();
                    e = tempobj;
                }
                //
                sb << (*e);
            }
        }

        virtual void streamIn( StreamBuffer& sb )
        {
            unsigned int n = 0;
            sb >> n;
            //
            this->resize( n );
            for ( unsigned int i = 0; i < n; i++ )
            {
                ElementType* e = this->at(i);
                //
                typename ElementType::Pointer tempobj;
                if ( e == 0 )
                {
                    tempobj = ElementType::New();
                    e = tempobj;
                }
                //
                sb >> (*e);
            }
        }

    protected:
        DataSet() {}

    private:
        DataSet( const Self & ); // purposely not implemented
        DataSet& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziDataSet_h_
