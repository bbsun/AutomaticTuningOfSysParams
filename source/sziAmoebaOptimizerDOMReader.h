#ifndef _sziAmoebaOptimizerDOMReader_h_
#define _sziAmoebaOptimizerDOMReader_h_

#include <itkAmoebaOptimizer.h>
#include <itkDOMReader.h>

namespace szi
{

    class AmoebaOptimizerDOMReader : public itk::DOMReader<itk::AmoebaOptimizer>
    {
    public:
        /** Standard class typedefs. */
        typedef AmoebaOptimizerDOMReader Self;
        typedef itk::DOMReader<itk::AmoebaOptimizer> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::AmoebaOptimizerDOMReader, DOMReader );

    protected:
        AmoebaOptimizerDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
            LoggerType* logger = this->GetLogger();

            OutputType* output = this->GetOutput();
            if ( output == NULL )
            {
                logger->Info( "creating the output object ...\n" );
                OutputType::Pointer object = OutputType::New();
                output = (OutputType*)object;
                this->SetOutput( output );
            }
            else
            {
                logger->Info( "filling an existing output object ...\n" );
            }

            itk::FancyString objname = output->GetNameOfClass();
            if ( objname != inputdom->GetName() )
            {
                logger->Info( "invalid input DOM object\n" );
                throw 0;
            }

            itk::FancyString info;
            itk::FancyString s;

            s = inputdom->GetAttribute("MaximumNumberOfIterations");
            if ( s != "" )
            {
                OutputType::NumberOfIterationsType value = 0;
                s >> value;
                output->SetMaximumNumberOfIterations( value );
                logger->Info( info << itk::ClearContent << "OK! MaximumNumberOfIterations = " << value << "\n" );
            }
            else
            {
                logger->Warning( info << itk::ClearContent << "MaximumNumberOfIterations not provided!\n" );
            }

            s = inputdom->GetAttribute("AutomaticInitialSimplex");
            if ( s != "" )
            {
                int value = 0;
                s >> value;
                output->SetAutomaticInitialSimplex( (bool)value );
                logger->Info( info << itk::ClearContent << "OK! AutomaticInitialSimplex = " << value << "\n" );
            }
            else
            {
                logger->Warning( info << itk::ClearContent << "AutomaticInitialSimplex not provided!\n" );
            }

            s = inputdom->GetAttribute("OptimizeWithRestarts");
            if ( s != "" )
            {
                int value = 0;
                s >> value;
                output->SetOptimizeWithRestarts( (bool)value );
                logger->Info( info << itk::ClearContent << "OK! OptimizeWithRestarts = " << value << "\n" );
            }
            else
            {
                logger->Warning( info << itk::ClearContent << "OptimizeWithRestarts not provided!\n" );
            }

            s = inputdom->GetAttribute("InitialSimplexDelta");
            if ( s != "" )
            {
                OutputType::ParametersType values;
                s.ToData( values );
                output->SetInitialSimplexDelta( values );
                logger->Info( info << itk::ClearContent << "OK! InitialSimplexDelta = " << values << "\n" );
            }
            else
            {
                logger->Warning( info << itk::ClearContent << "InitialSimplexDelta not provided!\n" );
            }

            s = inputdom->GetAttribute("ParametersConvergenceTolerance");
            if ( s != "" )
            {
                double value = 0;
                s >> value;
                output->SetParametersConvergenceTolerance( value );
                logger->Info( info << itk::ClearContent << "OK! ParametersConvergenceTolerance = " << value << "\n" );
            }
            else
            {
                logger->Warning( info << itk::ClearContent << "ParametersConvergenceTolerance not provided!\n" );
            }

            s = inputdom->GetAttribute("FunctionConvergenceTolerance");
            if ( s != "" )
            {
                double value = 0;
                s >> value;
                output->SetFunctionConvergenceTolerance( value );
                logger->Info( info << itk::ClearContent << "OK! FunctionConvergenceTolerance = " << value << "\n" );
            }
            else
            {
                logger->Warning( info << itk::ClearContent << "FunctionConvergenceTolerance not provided!\n" );
            }

            s = inputdom->GetAttribute("Scales");
            if ( s != "" )
            {
                OutputType::ScalesType values;
                s.ToData( values );
                output->SetScales( values );
                logger->Info( info << itk::ClearContent << "OK! Scales = " << values << "\n" );
            }
            else
            {
                logger->Warning( info << itk::ClearContent << "Scales not provided!\n" );
            }
        }

    private:
        AmoebaOptimizerDOMReader( const Self & ); // purposely not implemented
        AmoebaOptimizerDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziAmoebaOptimizerDOMReader_h_
