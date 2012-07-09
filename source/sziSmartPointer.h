#ifndef _sziSmartPointer_h_
#define _sziSmartPointer_h_

#include <itkSmartPointer.h>

namespace szi
{

    /**
    A modification of the ITK version of smart pointer, which accepts any non-smart object type
    as the template parameter. If the object type is indeed a smart object, this class behaves the
    same as the ITK version; otherwise, the class does not provide reference counting and automatic deleting functions.
    */
    template < class TObject >
    class SmartPointer : public itk::SmartPointer<itk::LightObject>
    {
    public:
        typedef itk::SmartPointer<itk::LightObject> SuperClass;
        typedef typename SuperClass::ObjectType SuperObjectType;

        typedef TObject ObjectType;

        SmartPointer() : SuperClass(), m_Object(0)
        {
        }

        SmartPointer( const SmartPointer& sp )
        {
            this->operator=( sp );
        }

        SmartPointer( ObjectType* o )
        {
            this->operator=( o );
        }

        ObjectType* operator->() const
        {
            return this->m_Object;
        }

        operator ObjectType*() const
        {
            return this->m_Object;
        }

        ObjectType* GetPointer() const
        {
            return this->m_Object;
        }

        SmartPointer& operator=( const SmartPointer& sp )
        {
            return this->operator=( sp.GetPointer() );
        }

        SmartPointer& operator=( ObjectType* o )
        {
            this->m_Object = o;
            SuperClass::operator=( dynamic_cast<SuperObjectType*>(o) );
            return *this;
        }

        bool IsNotNull() const
        {
            return ( this->m_Object != 0 );
        }

        bool IsNull() const
        {
            return ( this->m_Object == 0 );
        }

        template< typename TR >
        bool operator==( TR r ) const
        {
            return ( this->m_Object == static_cast<const ObjectType*>(r) );
        }

        template< typename TR >
        bool operator!=( TR r ) const
        {
            return ( this->m_Object != static_cast<const ObjectType*>(r) );
        }

        bool operator<( const SmartPointer& r ) const
        {
            return ( (void*)this->m_Object < (void*)r.m_Object );
        }

        bool operator>( const SmartPointer& r ) const
        {
            return ( (void*)this->m_Object > (void*)r.m_Object );
        }

        bool operator<=( const SmartPointer& r ) const
        {
            return ( (void*)this->m_Object <= (void*)r.m_Object );
        }

        bool operator>=( const SmartPointer& r ) const
        {
            return ( (void*)this->m_Object >= (void*)r.m_Object );
        }

        ObjectType* Print( std::ostream& os ) const
        {
            SuperClass::Print( os );
            return this->m_Object;
        }

    private:
        ObjectType* m_Object;
    };

} // namespace szi

#endif // _sziSmartPointer_h_
