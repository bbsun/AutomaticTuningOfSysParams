#ifndef _sziRegularStepGradientDescentOptimizerDOMReader_h_
#define _sziRegularStepGradientDescentOptimizerDOMReader_h_

#include <itkRegularStepGradientDescentOptimizer.h>
#include <itkDOMReader.h>

#include "sziLogService.h"

namespace szi
{

    class RegularStepGradientDescentOptimizerDOMReader : public itk::DOMReader<itk::RegularStepGradientDescentOptimizer>
    {
    public:
        /** Standard class typedefs. */
        typedef RegularStepGradientDescentOptimizerDOMReader Self;
        typedef itk::DOMReader<itk::RegularStepGradientDescentOptimizer> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::RegularStepGradientDescentOptimizerDOMReader, DOMReader );

    protected:
        RegularStepGradientDescentOptimizerDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            OutputType* output = this->GetOutput();
            if ( output == NULL )
            {
            	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): creating the output object ..." << End;
                OutputType::Pointer object = OutputType::New();
                output = (OutputType*)object;
                this->SetOutput( output );
            }
            else
            {
            	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): filling an existing output object ..." << End;
            }

            itk::FancyString objname = output->GetNameOfClass();
            if ( objname != inputdom->GetName() )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): invalid input DOM object" << End;
            }

            itk::FancyString info;
            itk::FancyString s;

            s = inputdom->GetAttribute("MaximumNumberOfIterations");
            if ( s != "" )
            {
                unsigned int value = 0;
                s >> value;
                output->SetNumberOfIterations( value );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): MaximumNumberOfIterations = " << value << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): MaximumNumberOfIterations not provided!" << End;
            }

            s = inputdom->GetAttribute("MaximumStepLength");
            if ( s != "" )
            {
                double value = 0;
                s >> value;
                output->SetMaximumStepLength( value );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): MaximumStepLength = " << value << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): MaximumStepLength not provided!" << End;
            }

            s = inputdom->GetAttribute("MinimumStepLength");
            if ( s != "" )
            {
                double value = 0;
                s >> value;
                output->SetMinimumStepLength( value );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): MinimumStepLength = " << value << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): MinimumStepLength not provided!" << End;
            }

            s = inputdom->GetAttribute("RelaxationFactor");
            if ( s != "" )
            {
                double value = 0;
                s >> value;
                output->SetRelaxationFactor( value );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): RelaxationFactor = " << value << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): RelaxationFactor not provided!" << End;
            }

            s = inputdom->GetAttribute("GradientMagnitudeTolerance");
            if ( s != "" )
            {
                double value = 0;
                s >> value;
                output->SetGradientMagnitudeTolerance( value );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): GradientMagnitudeTolerance = " << value << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): GradientMagnitudeTolerance not provided!" << End;
            }

            const DOMNodeType* node = inputdom->GetChildByID( "scales" );
            s = node ? node->GetAttribute("value") : "";
            if ( s != "" )
            {
                OutputType::ScalesType values;
                s.ToData( values );
                output->SetScales( values );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): Scales = " << values << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): Scales not provided!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        RegularStepGradientDescentOptimizerDOMReader( const Self & ); // purposely not implemented
        RegularStepGradientDescentOptimizerDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziRegularStepGradientDescentOptimizerDOMReader_h_
