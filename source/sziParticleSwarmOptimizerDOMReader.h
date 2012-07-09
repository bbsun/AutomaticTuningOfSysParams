#ifndef _sziParticleSwarmOptimizerDOMReader_h_
#define _sziParticleSwarmOptimizerDOMReader_h_

#include <itkDOMReader.h>
#include <itkParticleSwarmOptimizer.h>

namespace szi
{

    class ParticleSwarmOptimizerDOMReader : public itk::DOMReader<itk::ParticleSwarmOptimizer>
    {
    public:
        /** Standard class typedefs. */
        typedef ParticleSwarmOptimizerDOMReader Self;
        typedef itk::DOMReader<itk::ParticleSwarmOptimizer> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::ParticleSwarmOptimizerDOMReader, DOMReader );

        typedef Superclass::OutputType OutputType;
        typedef Superclass::DOMNodeType DOMNodeType;

    protected:
        ParticleSwarmOptimizerDOMReader() {}

        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            OutputType* output = this->GetOutput();
            if ( output == NULL )
            {
            	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): Creating a new output PSO object ..." << End;
                OutputType::Pointer object = OutputType::New();
                output = (OutputType*)object;
                this->SetOutput( output );
            }
            else
            {
            	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): Filling an existing output PSO object ..." << End;
            }

            itk::FancyString objname = "ParticleSwarmOptimizer"; //output->GetNameOfClass();
            if ( objname != inputdom->GetName() )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Input DOM object is invalid!" << End;
            }

            itk::FancyString info;
            itk::FancyString s;

            const DOMNodeType* node = 0;

            s = inputdom->GetAttribute("NumberOfParticles");
            if ( s != "" )
            {
                int nop = 0; s >> nop;
                output->SetNumberOfParticles( nop );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): NumberOfParticles = " << nop << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): NumberOfParticles not provided!" << End;
            }

            s = inputdom->GetAttribute( "SamplingSeed" );
            if ( s != "" )
            {
                int n = 0; s >> n;
                output->SetUseSeed( true );
                output->SetSeed( n );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): SamplingSeed = " << n << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): SamplingSeed not provided!" << End;
            }

            s = inputdom->GetAttribute("MaximumNumberOfIterations");
            if ( s != "" )
            {
                int noi = 0; s >> noi;
                output->SetMaximalNumberOfIterations( noi );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): MaximumNumberOfIterations = " << noi << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): MaximumNumberOfIterations not provided!" << End;
            }

            s = inputdom->GetAttribute("InertiaCoefficient");
            if ( s != "" )
            {
                double icoef = 0; s >> icoef;
                output->SetInertiaCoefficient( icoef );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): InertiaCoefficient = " << icoef << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): InertiaCoefficient not provided!" << End;
            }

            s = inputdom->GetAttribute("GlobalCoefficient");
            if ( s != "" )
            {
                double gcoef = 0; s >> gcoef;
                output->SetGlobalCoefficient( gcoef );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): GlobalCoefficient = " << gcoef << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): GlobalCoefficient not provided!" << End;
            }

            s = inputdom->GetAttribute("PersonalCoefficient");
            if ( s != "" )
            {
                double pcoef = 0; s >> pcoef;
                output->SetPersonalCoefficient( pcoef );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): PersonalCoefficient = " << pcoef << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): PersonalCoefficient not provided!" << End;
            }

            // read the lower bound of the sampling domain
            node = inputdom->GetChildByID( "lbound" );
            s = node ? node->GetAttribute("value") : "";
            if ( s == "" )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Lower bound of the parameters is missing!" << End;
            }
            std::vector<double> lbound;
            s.ToData( lbound );
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): lbound = " << lbound << End;
            // get the upper bound of the sampling domain
            node = inputdom->GetChildByID( "ubound" );
            s = node ? node->GetAttribute("value") : "";
            if ( s == "" )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Upper bound of the parameters is missing!" << End;
            }
            std::vector<double> ubound;
            s.ToData( ubound );
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): ubound = " << ubound << End;
            // combine the two
            if ( lbound.size() > ubound.size() )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Sizes of lower and upper parameters bounds mismatch!" << End;
            }
            OutputType::ParameterBoundsType bounds;
            for ( size_t i = 0; i < lbound.size(); i++ )
            {
                std::pair<double,double> value;
                value.first = lbound[i];
                value.second = ubound[i];
                bounds.push_back( value );
            }
            output->SetParameterBounds( bounds );

            node = inputdom->GetChildByID( "ParametersConvergenceTolerance" );
            s = node ? node->GetAttribute("value") : "";
            if ( s != "" )
            {
                itk::Array<double> ptols;
                s.ToData( ptols );
                output->SetParametersConvergenceTolerance( ptols );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): ParametersConvergenceTolerance = " << ptols << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): ParametersConvergenceTolerance not provided!" << End;
            }

            // read a double value from an attribute
            s = inputdom->GetAttribute("FunctionConvergenceTolerance");
            if ( s != "" )
            {
                double ftol = 0; s >> ftol;
                output->SetFunctionConvergenceTolerance( ftol );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): FunctionConvergenceTolerance = " << ftol << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): FunctionConvergenceTolerance not provided!" << End;
            }

            // read a double value from an attribute
            s = inputdom->GetAttribute("ConvergedPercentageToStop");
            if ( s != "" )
            {
                double stoppercent = 0; s >> stoppercent;
                output->SetPercentageParticlesConverged( stoppercent );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): ConvergedPercentageToStop = " << stoppercent << End;
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): ConvergedPercentageToStop not provided!" << End;
            }

            node = inputdom->GetChildByID( "scales" );
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
        ParticleSwarmOptimizerDOMReader( const Self & ); // purposely not implemented
        ParticleSwarmOptimizerDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziParticleSwarmOptimizerDOMReader_h_
