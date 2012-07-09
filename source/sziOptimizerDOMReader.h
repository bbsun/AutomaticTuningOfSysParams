#ifndef _sziOptimizerDOMReader_h_
#define _sziOptimizerDOMReader_h_

#include <itkOptimizer.h>
#include <itkDOMReader.h>

#include "sziExhaustiveOptimizerDOMReader.h"
#include "sziAmoebaOptimizerDOMReader.h"
#include "sziRegularStepGradientDescentOptimizerDOMReader.h"
#include "sziParticleSwarmOptimizerDOMReader.h"

#include "sziLogService.h"

namespace szi
{

    class OptimizerDOMReader : public itk::DOMReader<itk::Optimizer>
    {
    public:
        /** Standard class typedefs. */
        typedef OptimizerDOMReader Self;
        typedef itk::DOMReader<itk::Optimizer> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::OptimizerDOMReader, DOMReader );

    protected:
        OptimizerDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString tagname = inputdom->GetName();

            // the optimizer type is ExhaustiveOptimizer
            if ( tagname == "ExhaustiveOptimizer" )
            {
                typedef ExhaustiveOptimizerDOMReader ReaderType;
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

            // the optimizer type is AmoebaOptimizer
            else if ( tagname == "AmoebaOptimizer" )
            {
                typedef AmoebaOptimizerDOMReader ReaderType;
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

            // the optimizer type is RegularStepGradientDescentOptimizer
            else if ( tagname == "RegularStepGradientDescentOptimizer" )
            {
                typedef RegularStepGradientDescentOptimizerDOMReader ReaderType;
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

            // the optimizer type is ParticleSwarmOptimizer
            else if ( tagname.MatchWith("ParticleSwarmOptimizer") )
            {
                typedef ParticleSwarmOptimizerDOMReader ReaderType;
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

            // the optimizer type is not supported
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this optimizer type is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        OptimizerDOMReader( const Self & ); // purposely not implemented
        OptimizerDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziOptimizerDOMReader_h_
