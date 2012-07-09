#ifndef _sziMPISystemParametersTunerSlave_h_
#define _sziMPISystemParametersTunerSlave_h_

#include <itkObject.h>
#include "sziMPIWorker.h"
#include "sziSystem.h"
#include "sziMPISystemParametersTunerContext.h"

namespace szi
{

    /**
    Slave worker that provides service to the master. It repeatedly listens to messages from the master,
    take the actions, and returns the results to the master.
    */
    class MPISystemParametersTunerSlave : public itk::Object, public MPIWorker
    {
    public:
        /** Standard class typedefs. */
        typedef MPISystemParametersTunerSlave Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPISystemParametersTunerSlave, Object );

        typedef System SystemType;
        typedef SystemType::DataType SystemDataType;

        virtual void setSystem( SystemType* system ) { this->setJob( system ); }
        SystemType* getSystem() { return static_cast<SystemType*>( this->getJob() ); }
        const SystemType* getSystem() const { return static_cast<const SystemType*>( this->getJob() ); }

        virtual void initialize()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): =====start=====" << End;

            SystemType* system = this->getSystem();
            if ( system == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): system is null" << End;
            }
            system->initialize();

            MPIWorker::initialize();

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): -----e-n-d-----" << End;
        }

        /** Receive messages from the master, do the work, and send back the results. */
        virtual void execute()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): =====start=====" << End;

            if ( !this->isInitialized() )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "execute(): worker not initialized" << End;
            }

            SystemType* system = this->getSystem();

            SystemDataType* data = system->getData();
            if ( data == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "execute(): system data is null" << End;
            }

            // listen to messages from the master, and perform data processing accordingly
            while ( true )
            {
                // receive a command message from the master
                int tag = MPIContext::receive( 0 );
                //getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): received tag " << tag << End;

                if ( tag == MPIContext::TAG_EXIT )
                {
                    break;
                }

                else if ( tag == MPISystemParametersTunerContext::TAG_SPT_UPDATE_SCORE )
                {
                    SystemDataType* data = system->getData();

                    // receive a SystemData from the master
                    MPIContext::receive( (Streamable&)(*data), 0, MPISystemParametersTunerContext::TAG_SPT_UPDATE_SCORE );

                    try
                    {
						// associate the data with the system
						system->setData( data );
						// update the values for tunable parameters
						system->setTunableParameters( data->m_Parameters );
						// compute the performance score
						system->updatePerformanceScore();

						MPIContext::send( 0, MPIContext::TAG_OK );
                    }
                    catch (...)
                    {
                    	MPIContext::send( 0, MPIContext::TAG_FAIL );
                    }
                }

                else if ( tag == MPISystemParametersTunerContext::TAG_SPT_GET_SCORE )
                {
                	try
                	{
						double score = system->getPerformanceScore();

						MPIContext::send( 0, MPIContext::TAG_OK );

	                    // send the score back to the master
	                    MPIContext::send( score, 0, MPISystemParametersTunerContext::TAG_SPT_GET_SCORE );
					}
					catch (...)
					{
						MPIContext::send( 0, MPIContext::TAG_FAIL );
					}
                }

                else
                {
                	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "execute(): unrecognized tag received from the master" << End;
                }
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): -----e-n-d-----" << End;
        }

    protected:
        MPISystemParametersTunerSlave() {}

    private:
        MPISystemParametersTunerSlave( const Self & ); // Purposely not implemented.
        MPISystemParametersTunerSlave& operator=( const Self & ); // Purposely not implemented.
    };

} // namespace szi

#endif // _sziMPISystemParametersTunerSlave_h_
