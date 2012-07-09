#ifndef _sziRegistrationSystemDOMReader_h_
#define _sziRegistrationSystemDOMReader_h_

#include <itkDOMReader.h>
#include "sziRegistrationSystem.h"

#include "sziLogService.h"

namespace szi
{

    class RegistrationSystemDataDOMReader : public itk::DOMReader<RegistrationSystemData>
    {
    public:
        /** Standard class typedefs. */
        typedef RegistrationSystemDataDOMReader Self;
        typedef itk::DOMReader<RegistrationSystemData> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::RegistrationSystemDataDOMReader, DOMReader );

    protected:
        RegistrationSystemDataDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "RegistrationSystemData" )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Input DOM object is invalid!" << End;
            }

            OutputType* output = this->GetOutput();
            if ( output == 0 )
            {
                OutputType::Pointer object = OutputType::New();
                output = object;
                this->SetOutput( output );
            }

            // read data fields

            itk::FancyString s;
            const DOMNodeType* node = 0;

            s = inputdom->GetAttribute( "home" );
            if ( s != "" )
            {
                output->datadir = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): home = " << s << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Data home directory is not available!" << End;
            }

            node = inputdom->GetChildByID( "fdata" );
            if ( node )
            {
                s = node->GetAttribute( "path" );
                output->fdata.sFolder = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): fdata.home = " << s << End;
                //
                s = node->GetAttribute( "ct" );
                output->fdata.sCT = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): fdata.ct = " << s << End;
                //
                s = node->GetAttribute( "seg" );
                output->fdata.sSegmentation = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): fdata.seg = " << s << End;
                //
                s = node->GetAttribute( "roi" );
                output->fdata.sROI = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): fdata.roi = " << s << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Fixed data is not available!" << End;
            }

            node = inputdom->GetChildByID( "mdata" );
            if ( node )
            {
                s = node->GetAttribute( "path" );
                output->mdata.sFolder = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): mdata.home = " << s << End;
                //
                s = node->GetAttribute( "ct" );
                output->mdata.sCT = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): mdata.ct = " << s << End;
                //
                s = node->GetAttribute( "seg" );
                output->mdata.sSegmentation = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): mdata.seg = " << s << End;
                //
                s = node->GetAttribute( "roi" );
                output->mdata.sROI = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): mdata.roi = " << s << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Moving data is not available!" << End;
            }

            s = inputdom->GetAttribute( "seglabel" );
            if ( s != "" )
            {
                short label = 0;
                s >> label;
                output->seglabel = label;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): seglabel = " << label << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Segmentation label is not available!" << End;
            }

            s = inputdom->GetAttribute( "tparams" );
            if ( s != "" )
            {
                //OutputType::ParametersType params;
                itk::Array<double> params;
                s.ToData( params );
                output->tparams = params;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): tparams = " << params << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Transformation parameters are not available!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        RegistrationSystemDataDOMReader( const Self & ); // purposely not implemented
        RegistrationSystemDataDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#include "sziRegistraterDOMReader.h"

namespace szi
{

    class RegistrationSystemDOMReader : public itk::DOMReader<RegistrationSystem>
    {
    public:
        /** Standard class typedefs. */
        typedef RegistrationSystemDOMReader Self;
        typedef itk::DOMReader<RegistrationSystem> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::RegistrationSystemDOMReader, DOMReader );

        typedef RegistrationSystem OutputType;

        typedef OutputType::RegistraterType RegistraterType;

        typedef Superclass::DOMNodeType DOMNodeType;

    protected:
        RegistrationSystemDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "RegistrationSystem" )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Input DOM object is invalid!" << End;
            }

            OutputType* output = this->GetOutput();
            if ( output == 0 )
            {
                OutputType::Pointer object = OutputType::New();
                output = object;
                this->SetOutput( output );
            }

            // read settings

            const DOMNodeType* node = 0;

            node = inputdom->GetChildByID( "registrater" );
            if ( node )
            {
                typedef OutputType::RegistraterType RegistraterType;
                typedef RegistraterDOMReaderOf<RegistraterType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( node );
                output->setRegistrater( reader->GetOutput() );
            }
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Registrater was not provided!" << End;
            }

            node = inputdom->GetChildByID( "data" );
			if ( node )
			{
				typedef RegistrationSystemDataDOMReader ReaderType;
				ReaderType::Pointer reader = ReaderType::New();
				reader->Update( node );
				output->setData( reader->GetOutput() );
			}
			else
			{
				getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Data was not provided!" << End;
			}

			// read the tunable parameters
			itk::FancyString s = inputdom->GetAttribute( "sysparams" );
			if ( s != "" )
			{
				itk::Array<double> params;
				s.ToData( params );
				output->setTunableParameters( params );
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): Tunable parameters " << params << End;
			}
			else
			{
				getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Tunable parameters were not provided!" << End;
			}

			getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        RegistrationSystemDOMReader( const Self & ); // purposely not implemented
        RegistrationSystemDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziRegistrationSystemDOMReader_h_
