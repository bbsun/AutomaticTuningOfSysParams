#ifndef _sziFancyLogger_h_
#define _sziFancyLogger_h_

#include <itkLogger.h>
#include <itkStdStreamLogOutput.h>
#include <fstream>

#include <itkFancyString.h>

namespace szi
{

    /**
    A logger implementation that always logs incoming messages to a file.
    It also includes some minor revisions to the original implementation for easier use.
    */
    class FancyLogger : public itk::Logger
    {
    public:
        /** Standard class typedefs. */
        typedef FancyLogger Self;
        typedef itk::Logger Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::FancyLogger, Logger );

        void AddOutput( std::ostream& os )
        {
            itk::StdStreamLogOutput::Pointer output = itk::StdStreamLogOutput::New();
            output->SetStream( os );
            this->AddLogOutput( (itk::StdStreamLogOutput*)output );
        }

        void StartLogging( const char * name )
        {
            this->EndLogging();

            std::string fn( name );
            fn.append( ".log" );
            this->m_FileStream.open( fn.c_str() );
            if ( !this->m_FileStream.is_open() )
            {
                this->Warning( "Not able to create the log file!" );
            }
        }

        void EndLogging()
        {
            if ( this->m_FileStream.is_open() )
            {
                this->m_FileStream.close();
            }
        }

        virtual void Write( PriorityLevelType level, std::string const & content )
        {
            std::string message( content + "\n" );
            Superclass::Write( level, message );

            if ( level == FATAL )
            {
                this->EndLogging();
                throw "Fatal error was encountered!";
            }
        }

        void StartLine( PriorityLevelType level )
        {
            if ( this->m_LineText.size() )
            {
                this->EndLine();
            }

            this->m_LinePriority = level;
        }

        void EndLine()
        {
            this->Write( this->m_LinePriority, this->m_LineText );
            this->m_LineText.clear();
        }

        virtual ~FancyLogger()
        {
            this->EndLogging();
        }

        /*
        template < typename T >
        FancyLogger& operator<<( T const & data ) { this->m_LineText << data; return (*this); }
        /*/
        FancyLogger& operator<<( char const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( unsigned char const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( short const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( unsigned short const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( int const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( unsigned int const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( long const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( unsigned long const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( float const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( double const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( std::string const & data ) { this->m_LineText << data; return (*this); }
        FancyLogger& operator<<( char const * data ) { this->m_LineText << data; return (*this); }
        template < typename T > FancyLogger& operator<<( std::vector<T> const & data ) { this->m_LineText << data; return (*this); }
        template < typename T > FancyLogger& operator<<( itk::Array<T> const & data ) { this->m_LineText << data; return (*this); }
        //*/

        FancyLogger& operator<<( void (*mf)(FancyLogger&) )
        {
            (*mf)(*this);
            return (*this);
        }

        class Manipulator
        {
        public:
			typedef FancyLogger ObjectType;
			typedef std::string ArgumentType;
			Manipulator( const ArgumentType& a ) : m_Argument( a ) {}
			virtual void mf( ObjectType&, const ArgumentType& ) const = 0;
			ArgumentType m_Argument;
        };
        //
        FancyLogger& operator<<( const Manipulator& manip )
		{
            manip.mf( (*this), manip.m_Argument );
			return (*this);
		}

    protected:
        FancyLogger() : m_LinePriority( NOTSET )
        {
            this->SetPriorityLevel( NOTSET );
            this->SetLevelForFlushing( INFO );

            this->SetTimeStampFormat( HUMANREADABLE );
            this->SetHumanReadableFormat( "%Y-%b-%d %H:%M:%S" );

            this->SetName( this->GetNameOfClass() );

            // add the default file output
            this->AddOutput( this->m_FileStream );
        }

    private:
        FancyLogger( const Self & ); // purposely not implemented
        FancyLogger& operator=( const Self & ); // purposely not implemented

        std::ofstream m_FileStream;

        PriorityLevelType m_LinePriority;
        itk::FancyString m_LineText;
    };

    // some manipulators for FancyLogger

    class StartFatal : public FancyLogger::Manipulator
    {
    public:
        typedef FancyLogger::Manipulator SuperClass;
        StartFatal( const ArgumentType& a = "" ) : SuperClass( a ) {}
        virtual void mf( ObjectType& logger, const ArgumentType& name ) const
        {
            if ( name.size() )
            {
                logger.SetName( name.c_str() );
            }
            logger.StartLine( FancyLogger::FATAL );
        }
    };

    class StartCritical : public FancyLogger::Manipulator
    {
    public:
        typedef FancyLogger::Manipulator SuperClass;
        StartCritical( const ArgumentType& a = "" ) : SuperClass( a ) {}
        virtual void mf( ObjectType& logger, const ArgumentType& name ) const
        {
            if ( name.size() )
            {
                logger.SetName( name.c_str() );
            }
            logger.StartLine( FancyLogger::CRITICAL );
        }
    };

    class StartWarning : public FancyLogger::Manipulator
    {
    public:
        typedef FancyLogger::Manipulator SuperClass;
        StartWarning( const ArgumentType& a = "" ) : SuperClass( a ) {}
        virtual void mf( ObjectType& logger, const ArgumentType& name ) const
        {
            if ( name.size() )
            {
                logger.SetName( name.c_str() );
            }
            logger.StartLine( FancyLogger::WARNING );
        }
    };

    class StartInfo : public FancyLogger::Manipulator
    {
    public:
        typedef FancyLogger::Manipulator SuperClass;
        StartInfo( const ArgumentType& a = "" ) : SuperClass( a ) {}
        virtual void mf( ObjectType& logger, const ArgumentType& name ) const
        {
            if ( name.size() )
            {
                logger.SetName( name.c_str() );
            }
            logger.StartLine( FancyLogger::INFO );
        }
    };

    class StartDebug : public FancyLogger::Manipulator
    {
    public:
        typedef FancyLogger::Manipulator SuperClass;
        StartDebug( const ArgumentType& a = "" ) : SuperClass( a ) {}
        virtual void mf( ObjectType& logger, const ArgumentType& name ) const
        {
            if ( name.size() )
            {
                logger.SetName( name.c_str() );
            }
            logger.StartLine( FancyLogger::DEBUG );
        }
    };

    inline void Fatal( FancyLogger& logger )
    {
        logger.StartLine( FancyLogger::FATAL );
    }

    inline void Critical( FancyLogger& logger )
    {
        logger.StartLine( FancyLogger::CRITICAL );
    }

    inline void Warning( FancyLogger& logger )
    {
        logger.StartLine( FancyLogger::WARNING );
    }

    inline void Info( FancyLogger& logger )
    {
        logger.StartLine( FancyLogger::INFO );
    }

    inline void Debug( FancyLogger& logger )
    {
        logger.StartLine( FancyLogger::DEBUG );
    }

    inline void End( FancyLogger& logger )
    {
        logger.EndLine();
    }

} // namespace szi

#endif // _sziFancyLogger_h_
