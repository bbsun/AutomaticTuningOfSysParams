#ifndef _sziLiverSegmentationSystemDOMReader_h_
#define _sziLiverSegmentationSystemDOMReader_h_

#include <itkDOMReader.h>
#include "sziLiverSegmentationSystem.h"

#include "sziLogService.h"

namespace szi
{

    class LiverSegmentationSystemDataDOMReader : public itk::DOMReader<LiverSegmentationSystemData>
    {
    public:
        /** Standard class typedefs. */
        typedef LiverSegmentationSystemDataDOMReader Self;
        typedef itk::DOMReader<LiverSegmentationSystemData> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::LiverSegmentationSystemDataDOMReader, DOMReader );

    protected:
        LiverSegmentationSystemDataDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "LiverSegmentationSystemData" )
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

            s = inputdom->GetID();
            output->sName = s;
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): name = " << s << End;

            node = inputdom->GetChildByID( "settings" );
            if ( node )
            {
                s = node->GetAttribute( "value" );
                output->fnSettings = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): settings file = " << s << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): settings file is not available!" << End;
            }
            
            node = inputdom->GetChildByID( "result" );
            if ( node )
            {
                s = node->GetAttribute( "value" );
                output->fnResultingSegmentation = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): resulting segmentation file = " << s << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): resulting segmentation file is not available!" << End;
            }
            
            node = inputdom->GetChildByID( "reference" );
            if ( node )
            {
                s = node->GetAttribute( "value" );
                output->fnReferenceSegmentation = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): reference segmentation file = " << s << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): reference segmentation file is not available!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        LiverSegmentationSystemDataDOMReader( const Self & ); // purposely not implemented
        LiverSegmentationSystemDataDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi


namespace szi
{

    class LiverSegmentationSystemDOMReader : public itk::DOMReader<LiverSegmentationSystem>
    {
    public:
        /** Standard class typedefs. */
        typedef LiverSegmentationSystemDOMReader Self;
        typedef itk::DOMReader<LiverSegmentationSystem> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::LiverSegmentationSystemDOMReader, DOMReader );

        typedef LiverSegmentationSystem OutputType;
        typedef Superclass::DOMNodeType DOMNodeType;

    protected:
        LiverSegmentationSystemDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "LiverSegmentationSystem" )
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
            itk::FancyString s;

            node = inputdom->GetChildByID( "segmentation" );
            if ( node )
            {
                s = node->GetAttribute( "cmd" );
                output->fnSegmentationProg = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): segmentation program = " << s << End;
            }
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): segmentation program file is not available!" << End;
            }
            
            node = inputdom->GetChildByID( "evaluation" );
            if ( node )
            {
                s = node->GetAttribute( "cmd" );
                output->fnEvaluationProg = s;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): evaluation program = " << s << End;
            }
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): evaluation program file is not available!" << End;
            }

			// read the tunable parameters
			s = inputdom->GetAttribute( "sysparams" );
			if ( s != "" )
			{
				itk::Array<double> params;
				s.ToData( params );
				output->setTunableParameters( params );
				getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): starting parameters = " << params << End;
			}
			else
			{
				getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): starting parameters are not available!" << End;
			}

			getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        LiverSegmentationSystemDOMReader( const Self & ); // purposely not implemented
        LiverSegmentationSystemDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziLiverSegmentationSystemDOMReader_h_
