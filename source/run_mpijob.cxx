#include <iostream>
#include "sziMPIWorkerLauncher.h"

int main ( int argc, char** argv )
{
    try
    {
        // create a MPI job launcher
        typedef szi::MPIWorkerLauncher WorkerType;
        WorkerType::Pointer worker = WorkerType::New();

        // start the launcher
        worker->initialize( argc, argv );
        worker->execute();
        worker->finalize();

        return EXIT_SUCCESS;
    }
    catch ( ... )
    {
    	std::cout << "Exit abnormally!" << std::endl;
        return EXIT_FAILURE;
    }
}
