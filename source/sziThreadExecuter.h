#ifndef _sziThreadExecuter_h_
#define _sziThreadExecuter_h_

#include <itkObject.h>
#include <itkMultiThreader.h>
#include <itkEventObject.h>

#include "sziExecutable.h"

namespace szi
{

    itkEventMacro( ThreadEvent, itk::AnyEvent );
    itkEventMacro( ThreadStartEvent, ThreadEvent );
    itkEventMacro( ThreadEndEvent, ThreadEvent );

    /**
    Class to wrap existing executables for running tasks in threads, internally
    using the thread service provided by ITK.
    Users can also derive from this class to create threaded executions.
    */
    class Thread
    {
    public:
        /** Standard class typedefs. */
        typedef Thread Self;
        typedef SmartPointer<Self> Pointer;
        typedef SmartPointer<const Self> ConstPointer;

        typedef itk::MultiThreader ThreaderType;

        static ITK_THREAD_RETURN_TYPE thread_callback( void* arg )
        {
            typedef ThreaderType::ThreadInfoStruct ThreaderInfoType;
            ThreaderInfoType* tinfo = (ThreaderInfoType*)arg;

            Self* self = (Self*)( tinfo->UserData );
            itk::Object* o = dynamic_cast<itk::Object*>( self );

            if (o) o->InvokeEvent( ThreadStartEvent() );

            self->setRunningOn();
            self->run();
            self->setRunningOff();

            if (o) o->InvokeEvent( ThreadEndEvent() );

            return ITK_THREAD_RETURN_VALUE;
        }

        /** Method to start the thread executation. */
        virtual void start()
        {
            if ( this->isRunning() ) return;
            /*
            this->m_Threader->SetNumberOfThreads( 1 );
            this->m_Threader->SetSingleMethod( this->thread_callback, (void*)this );
            this->m_Threader->SingleMethodExecute();
            //*/
            this->m_Threader->SpawnThread( this->thread_callback, (void*)this );
        }

        bool isRunning() const
        {
            return this->m_Running;
        }

    protected:
        /**
        Abstract method to be implemented in subclasses to execute a user task in a thread.
        */
        virtual void run() = 0;

        void setRunningOff() { this->m_Running = false; }
        void setRunningOn() { this->m_Running = true; }

        Thread() : m_Running(false)
        {
            this->m_Threader = ThreaderType::New();
        }

    private:
        /** Variable to indicate whether the thread is running or not. */
        bool m_Running;

        /** Variable to hold the thread implementation that provides threading support. */
        ThreaderType::Pointer m_Threader;
    };

    /**
    Class to run an executable object in a thread.
    */
    class ThreadExecuter : public itk::Object, public Thread
    {
    public:
        /** Standard class typedefs. */
        typedef ThreadExecuter Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ThreadExecuter, Object );

        void setExecutable( Executable* e ) { this->m_Executable = e; }
        Executable* getExecutable() { return this->m_Executable; }
        const Executable* getExecutable() const { return this->m_Executable; }

        virtual void initialize()
        {
            Executable* e = this->getExecutable();
            if ( e )
            {
                e->initialize();
            }
        }

        virtual void finalize()
        {
            Executable* e = this->getExecutable();
            if ( e )
            {
                e->finalize();
            }
        }

    protected:
        /** Thread method to run the user task specified by the input executable. */
        virtual void run()
        {
            Executable* e = this->getExecutable();
            if ( e )
            {
                e->execute();
            }
        }

        ThreadExecuter() : m_Executable(0) {}

    private:
        ThreadExecuter( const Self & ); // purposely not implemented
        ThreadExecuter& operator=( const Self & ); // purposely not implemented

        Executable* m_Executable;
    };

} // namespace szi

#endif // _sziThreadExecuter_h_
