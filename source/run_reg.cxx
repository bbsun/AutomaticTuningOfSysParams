
#include "sziRegistrationSystem.h"
#include "sziRegistrationSystemDOMReader.h"
#include "sziLogService.h"

int main ( int argc, char** argv )
{
    szi::getSystemLogger().SetName( "RegistrationSystem" );
    szi::getSystemLogger().StartLogging( argv[1] );

    int retcode = 0;

    try
    {
        if ( argc < 2 )
        {
            szi::getSystemLogger() << szi::Fatal << "Need an XML input registration job file to start!" << szi::End;
        }

        typedef szi::RegistrationSystem JobType;
        JobType::Pointer job;

        typedef szi::RegistrationSystemDOMReader ReaderType;
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName( argv[1] );
        reader->Update();
        job = reader->GetOutput();

        job->initialize();
        job->execute();
        job->finalize();

        retcode = EXIT_SUCCESS;
    }
    catch ( ... )
    {
        retcode = EXIT_FAILURE;
    }

    szi::getSystemLogger().EndLogging();

    return retcode;
}
