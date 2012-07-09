#ifndef _sziStreamable_h_
#define _sziStreamable_h_

#include <cstdlib>
#include <cstring>
#include <string>
#include <itkArray.h>

namespace szi
{

    /**
    Class to hold the data for I/O operations.
    */
    class StreamBuffer
    {
        char* _pointer;
        long _head;
        long _tail;
        long _capacity;

        int _stride;

    protected:
        long getCapacity() const { return _capacity; }

        void changeCapacity( long capacity )
        {
            if ( capacity < _stride )
            {
                _capacity = _stride;
            }
            else
            {
                long n = ( capacity / _stride );
                if ( capacity % _stride ) n++;
                _capacity = ( n * _stride );
            }
            //
            _pointer = (char*)realloc( _pointer, _capacity );
            if ( !_pointer )
            {
                throw "memory allocation failed";
            }
        }

    public:
        StreamBuffer( long capacity=0, int stride=256 ) : _pointer(0), _head(0), _tail(0), _capacity(0), _stride(stride)
        {
            this->changeCapacity( capacity );
        }

        virtual ~StreamBuffer()
        {
            if ( _pointer ) free( _pointer );
        }

        long getSize() const { return ( _tail - _head ); }

        void* getPointer() { return ( _pointer + _head ); }
        const void* getPointer() const { return ( _pointer + _head ); }

        long getInPosition() const { return _tail; }
        void setInPosition( long pos ) { _tail = pos; } // add pos check?

        long getOutPosition() const { return _head; }
        void setOutPosition( long pos ) { _head = pos; } // add pos check?

        void streamIn( const void* pointer, long size )
        {
            if ( !pointer || !size ) return;
            long newcap = _tail + size;
            if ( newcap > _capacity ) this->changeCapacity( newcap );
            std::memcpy( _pointer + _tail, pointer, size );
            _tail = newcap;
        }
        //
        void streamOut( void* pointer, long size )
        {
            if ( !pointer || !size ) return;
            long newcap = _head + size;
            if ( newcap > _tail ) return;
            std::memcpy( pointer, _pointer + _head, size );
            _head = newcap;
        }

        void streamIn( const StreamBuffer& sb )
        {
            this->streamIn( sb.getPointer(), sb.getSize() );
        }
        //
        void streamOut( StreamBuffer& sb )
        {
            this->streamOut( sb.getPointer(), sb.getSize() );
        }

        template < typename T >
        void streamIn( const T& data )
        {
            this->streamIn( &data, sizeof(T) );
        }
        //
        template < typename T >
        void streamOut( T& data )
        {
            this->streamOut( &data, sizeof(T) );
        }

        void streamIn( const std::string& s )
        {
            size_t n = s.size();
            this->streamIn( n );
            //
            if ( n ) this->streamIn( s.data(), n );
        }
        //
        void streamOut( std::string& s )
        {
            size_t n = 0;
            this->streamOut( n );
            //
            if ( n )
            {
                char* buf = new char[n];
                this->streamOut( buf, n );
                s = std::string( buf, n );
                delete [] buf;
            }
            else s = "";
        }

        void flush() { _head = _tail = 0; }
    };

    /**
    Abstract class (interface) to represent any object that is capable of I/O operations.
    This class is particularly useful for I/O operations over network.
    */
    class Streamable
    {
    public:
        virtual void streamOut( StreamBuffer& sb ) const = 0;
        virtual void streamIn( StreamBuffer& sb ) = 0;
    };

} // namespace szi

// for streaming in/out data of type szi::Streamable
inline szi::StreamBuffer& operator<<( szi::StreamBuffer& sb, const szi::Streamable& s )
{
    s.streamOut( sb );
    return sb;
}
//
inline szi::StreamBuffer& operator>>( szi::StreamBuffer& sb, szi::Streamable& s )
{
    s.streamIn( sb );
    return sb;
}

// for streaming in/out data of fundamental types (char, int, float, etc)
template < typename T >
inline szi::StreamBuffer& operator<<( szi::StreamBuffer& sb, const T& data )
{
    sb.streamIn( data );
    return sb;
}
//
template < typename T >
inline szi::StreamBuffer& operator>>( szi::StreamBuffer& sb, T& data )
{
    sb.streamOut( data );
    return sb;
}

// for streaming in/out data array of fundamental types (char, int, float, etc)
template < typename T >
inline szi::StreamBuffer& operator<<( szi::StreamBuffer& sb, const itk::Array<T>& data )
{
    unsigned int n = data.GetSize();
    sb << n;
    //
    for ( unsigned int i = 0; i < n; i++ ) sb << data[i];
    //
    return sb;
}
//
template < typename T >
inline szi::StreamBuffer& operator>>( szi::StreamBuffer& sb, itk::Array<T>& data )
{
    unsigned int n = 0;
    sb >> n;
    //
    data.SetSize( n );
    for ( unsigned int i = 0; i < n; i++ ) sb >> data[i];
    //
    return sb;
}

// for streaming in/out data of type std::string
inline szi::StreamBuffer& operator<<( szi::StreamBuffer& sb, const std::string& s )
{
    sb.streamIn( s );
    return sb;
}
//
inline szi::StreamBuffer& operator>>( szi::StreamBuffer& sb, std::string& s )
{
    sb.streamOut( s );
    return sb;
}

#endif // _sziStreamable_h_
