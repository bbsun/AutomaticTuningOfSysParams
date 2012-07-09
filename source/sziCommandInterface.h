#ifndef _sziCommandInterface_h_
#define _sziCommandInterface_h_

#include <itkCommand.h>

namespace szi
{

    class CommandInterface
    {
    public:
        /** Abstract method to be implemented in subclasses to process events from a monitor-only source. */
        virtual void Execute( const itk::Object * caller, const itk::EventObject & eo ) = 0;

        /** Abstract method to be implemented in subclasses to process events from a controllable source. */
        virtual void Execute( itk::Object * caller, const itk::EventObject & eo )
        {
            this->Execute( (const itk::Object *)caller, eo ); // by default just treat the event source as monitor-only
        }

    public:
        CommandInterface();

        /** Return the szi::Command wrap of this object. */
        itk::Command * GetCommandAdapter()
        {
            return (itk::Command *)this->m_Command;
        }

    private:
        /** Variable to wrap this CommandInterface object as an szi::Command. */
        itk::Command::Pointer m_Command;
    };

    class CommandAdapter : public itk::Command
    {
    public:
        /** Standard class typedefs. */
        typedef CommandAdapter Self;
        typedef itk::Command Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::CommandAdapter, Command );

        /** Set the observer to be called upon receiving an event. */
        void SetObserver( CommandInterface * ci )
        {
            this->m_CommandInterface = ci;
        }

        /** This method is called when an event is produced from a monitor-only source, just route it to the observer. */
        virtual void Execute( const itk::Object * caller, const itk::EventObject & eo )
        {
            if ( this->m_CommandInterface ) this->m_CommandInterface->Execute( caller, eo );
        }

        /** This method is called when an event is produced from a controllable source, just route it to the observer. */
        virtual void Execute( itk::Object * caller, const itk::EventObject & eo )
        {
            if ( this->m_CommandInterface ) this->m_CommandInterface->Execute( caller, eo );
        }

    protected:
        CommandAdapter() : m_CommandInterface( 0 ) {}

    private:
        CommandAdapter( const Self & ); // purposely not implemented
        CommandAdapter & operator=( const Self & ); // purposely not implemented

        CommandInterface * m_CommandInterface; // the observer to process incoming events
    };

    inline CommandInterface::CommandInterface()
    {
        CommandAdapter::Pointer ca = CommandAdapter::New();
        ca->SetObserver( this );
        this->m_Command = (CommandAdapter *)ca;
    }

} // namespace szi

#endif // _sziCommandInterface_h_
