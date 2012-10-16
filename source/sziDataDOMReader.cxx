#include "sziDataDOMReader.h"

#include "sziTestSystemDOMReader.h"
#include "sziRegistrationSystemDOMReader.h"
#include "sziLiverSegmentationSystemDOMReader.h"
#include "sziDataSetDOMReader.h"

#include "sziLogService.h"

namespace szi
{

    /** Function to generate the output object from an input DOM object. */
    void DataDOMReader::GenerateData( const DOMNodeType* inputdom, const void* )
    {
    	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

        itk::FancyString tagname = inputdom->GetName();

        if ( tagname == "DataSet" )
        {
            /*
            typedef DataSetDOMReader<> ReaderType;
            typedef ReaderType::OutputType RealOutputType;
            //
            OutputType* o = this->GetOutput();
            RealOutputType* output = dynamic_cast<RealOutputType*>( o );
            if ( o && output == 0 )
            {
                syslog->Warning( "The user-specified output is invalid and will be ignored!" );
            }
            //
            ReaderType::Pointer reader = ReaderType::New();
            reader->SetOutput( output );
            reader->Update( inputdom );
            output = reader->GetOutput();
            //
            this->SetOutput( output );
            //*/
        }

        else if ( tagname == "TestSystemData" )
        {
            typedef TestSystemDataDOMReader ReaderType;
            typedef ReaderType::OutputType RealOutputType;
            //
            OutputType* o = this->GetOutput();
            RealOutputType* output = dynamic_cast<RealOutputType*>( o );
            if ( o && output == 0 )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output is invalid and will be ignored!" << End;
            }
            //
            ReaderType::Pointer reader = ReaderType::New();
            reader->SetOutput( output );
            reader->Update( inputdom );
            output = reader->GetOutput();
            //
            this->SetOutput( output );
        }

        else if ( tagname == "RegistrationSystemData" )
        {
            typedef RegistrationSystemDataDOMReader ReaderType;
            typedef ReaderType::OutputType RealOutputType;
            //
            OutputType* o = this->GetOutput();
            RealOutputType* output = dynamic_cast<RealOutputType*>( o );
            if ( o && output == 0 )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output is invalid and will be ignored!" << End;
            }
            //
            ReaderType::Pointer reader = ReaderType::New();
            reader->SetOutput( output );
            reader->Update( inputdom );
            output = reader->GetOutput();
            //
            this->SetOutput( output );
        }
        
        else if ( tagname == "LiverSegmentationSystemData" )
        {
            typedef LiverSegmentationSystemDataDOMReader ReaderType;
            typedef ReaderType::OutputType RealOutputType;
            //
            OutputType* o = this->GetOutput();
            RealOutputType* output = dynamic_cast<RealOutputType*>( o );
            if ( o && output == 0 )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output is invalid and will be ignored!" << End;
            }
            //
            ReaderType::Pointer reader = ReaderType::New();
            reader->SetOutput( output );
            reader->Update( inputdom );
            output = reader->GetOutput();
            //
            this->SetOutput( output );
        }

        // the metric type is not supported
        else
        {
        	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this data type is not supported!" << End;
        }

        getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
    }

} // namespace szi
