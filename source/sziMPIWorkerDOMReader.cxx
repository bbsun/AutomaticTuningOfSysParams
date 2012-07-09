#include "sziMPIWorkerDOMReader.h"

#include "sziMPISystemParametersTunerDOMReader.h"

#include "sziLogService.h"

namespace szi
{

    void MPIWorkerDOMReader::GenerateData( const DOMNodeType* inputdom, const void* )
    {
    	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

        OutputType* output = this->GetOutput();
        itk::FancyString tagname = inputdom->GetName();

        if ( tagname == "SystemParametersTuner" )
        {
            typedef MPISystemParametersTunerDOMReader ReaderType;
            ReaderType::Pointer reader = ReaderType::New();
            reader->SetOutput( output );
            reader->SetOutputToMaster( this->GetOutputToMaster() );
            reader->Update( inputdom );
            output = reader->GetOutput();
            //
            this->SetOutput( output );
        }

        else
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): Reading of this MPIWorker type is not supported!" << End;
        }

        getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
    }

} // namespace szi
