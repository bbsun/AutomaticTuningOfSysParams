#ifndef _sziMPIWorkerLauncher_h_
#define _sziMPIWorkerLauncher_h_

#include <itkObject.h>
#include "sziMPIWorker.h"
#include "sziMPIWorkerDOMReader.h"

namespace szi
{

    class MPIWorkerLauncher : public itk::Object, public MPIWorker
    {
    public:
        /** Standard class typedefs. */
        typedef MPIWorkerLauncher Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::MPIWorkerLauncher, Object );

        typedef MPIWorker::RankType RankType;

        /** Initialize the MPI system. */
        virtual void initialize( int argc, char** argv )
        {
            // Temporary variable logging information.
            itk::FancyString msg;

            // Initialization can only be done once.
            if ( this->m_InputFileName )
            {
                getSystemLogger() << StartWarning(this->GetNameOfClass()) << "initialize(): Worker has already been initialized!" << End;
                return;
            }

            // Initialize MPI.
            MPI_Init( &argc, &argv );

            // Find out this worker's identity in the default communicator.
            RankType rank = MPIContext::getWorkerRank();

            // Store it as an internal state for future reference.
            this->setRank( rank );

            // Check whether an input XML job file has been provided or not.
            if ( argc < 2 )
            {
                MPI_Finalize();
                throw "Input job file is missing!";
            }

            // start the system logging
            itk::FancyString name( argv[1] );
            name << ".Worker-" << rank;
 			getSystemLogger().StartLogging( name );

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): =====start=====" << End;

            // Log the auditing information: command-line arguments.
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): Worker was initialized with following arguments:" << End;

            for ( int i = 0; i < argc; i++ )
            {
            	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize():   argv[" << i << "] = \"" << argv[i] << "\"" << End;
            }

            // Store the input XML job file for subsequent job processing.
            this->m_InputFileName = argv[1];

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): -----e-n-d-----" << End;
        }

        /**
        Load the job into this worker from an XML file, and execute it using a cluster of MPI-enabled workstations.
        The job will be done in a master-slave style. The first worker is served as the master. It assigns
        job pieces to other workers that are served as slaves, and then collects and aggregates the
        results produced by the slaves.
        */
        virtual void execute()
        {
            // Check whether this worker has been initialized or not.
            if ( this->m_InputFileName == 0 )
            {
                throw "Worker was not initialized!";
            }

            // Log the auditing information.
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): =====start=====" << End;

            // Get the pre-stored ID of this worker.
            RankType rank = this->getRank();

            // Create a new worker and load the job into it.
            typedef MPIWorker WorkerType;
            WorkerType::Pointer worker;
            //
            typedef MPIWorkerDOMReader ReaderType;
            ReaderType::Pointer reader = ReaderType::New();
            reader->SetFileName( this->m_InputFileName );
            if ( rank == 0 )
            {
                // The first worker will be the master, so read the master.
                reader->SetOutputToMaster();
            }
            else
            {
                // Each of the rest will be a slave, so please return a slave worker.
                reader->SetOutputToSlave();
            }
            reader->Update();
            worker = reader->GetOutput();

            // Control will be delegated to the newly created worker, so copy my ID to it.
            worker->setRank( rank );

            // Pass the control to the newly created worker and start the service.
            // If it is the master, it will send job pieces to the slaves; otherwise,
            // this slave will listen to service requests from the master.
            try
            {
				worker->initialize();
				worker->execute();
            }
            catch (...)
            {
            }
			worker->finalize();

            // Log the auditing information.
			getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): -----e-n-d-----" << End;
        }

        /** Shut down the MPI system. */
        virtual void finalize()
        {
            // Perform house-keeping if this worker has been initialized previously.
            if ( this->m_InputFileName )
            {
                MPI_Finalize();
                this->m_InputFileName = 0;
            }
        }

        virtual ~MPIWorkerLauncher()
        {
            this->finalize();
        }

    protected:
        MPIWorkerLauncher() : m_InputFileName(0) {}

    private:
        MPIWorkerLauncher( const Self & ); // Purposely not implemented.
        MPIWorkerLauncher& operator=( const Self & ); // Purposely not implemented.

        /**
        Variable to store the input file name, which points to an XML file that represents the job to be executed.
        It is also served as a flag to indicate whether the worker is initialized (when !=0) or not (when ==0).
        */
        const char* m_InputFileName;
    };

} // namespace szi

#endif // _sziMPIWorkerLauncher_h_
