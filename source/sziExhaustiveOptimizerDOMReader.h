#ifndef _sziExhaustiveOptimizerDOMReader_h_
#define _sziExhaustiveOptimizerDOMReader_h_

#include <itkExhaustiveOptimizer.h>
#include <itkDOMReader.h>

#include "sziLogService.h"

namespace szi
{

    class ExhaustiveOptimizerDOMReader : public itk::DOMReader<itk::ExhaustiveOptimizer>
    {
    public:
        /** Standard class typedefs. */
        typedef ExhaustiveOptimizerDOMReader Self;
        typedef itk::DOMReader<itk::ExhaustiveOptimizer> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ExhaustiveOptimizerDOMReader, DOMReader );

    protected:
        ExhaustiveOptimizerDOMReader() {}

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

            itk::FancyString s;

            s = inputdom->GetAttribute("StepLength");
            if ( s != "" )
            {
                double value = 0.0;
                s >> value;
                output->SetStepLength( value );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): StepLength = " << value << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): StepLength not provided!" << End;
            }

            s = inputdom->GetAttribute("NumberOfSteps");
            if ( s != "" )
            {
                itk::Array<double> values;
                s.ToData( values );
                output->SetNumberOfSteps( values );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): NumberOfSteps = " << values << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): NumberOfSteps not provided!" << End;
            }

            s = inputdom->GetAttribute("Scales");
            if ( s != "" )
            {
                itk::Array<double> values;
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
        ExhaustiveOptimizerDOMReader( const Self & ); // purposely not implemented
        ExhaustiveOptimizerDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziExhaustiveOptimizerDOMReader_h_
