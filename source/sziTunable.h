#ifndef _sziTunable_h_
#define _sziTunable_h_

#include <itkSingleValuedCostFunction.h>
#include <itkEventObject.h>

namespace szi
{

	itkEventMacro( ProgressEvent, itk::IterationEvent );

    class Tunable
    {
    public:
        typedef itk::SingleValuedCostFunction::ParametersType ParametersType;
        typedef itk::SingleValuedCostFunction::MeasureType MeasureType;

        /**
        Abstract method to be, if necessary, reimplemented in subclasses of tunable systems
        to get the number of system parameters to be tuned.
        */
        virtual unsigned int getNumberOfTunableParameters() const
        {
            return (unsigned int)this->getTunableParameters().GetSize();
        }

        /**
        Abstract methods to be extended or reimplemented in subclasses of tunable systems
        to set/get the system parameters to be tuned.
        */
        virtual void setTunableParameters( const ParametersType& params ) { this->m_TunableParameters = params; }
        virtual const ParametersType& getTunableParameters() const { return this->m_TunableParameters; }

        /**
        Abstract method to be reimplemented in subclasses of tunable systems
        to compute the performance score under current setting of system parameters.
        */
        virtual void updatePerformanceScore() = 0;

        /**
        Abstract methods to be extended or reimplemented in subclasses of tunable systems
        to set/get the current performance score.
        */
        virtual void setPerformanceScore( MeasureType score ) { this->m_PerformanceScore = score; }
        virtual MeasureType getPerformanceScore() const { return this->m_PerformanceScore; }

        Tunable() : m_PerformanceScore( 0 ) {}
        virtual ~Tunable() {}

    private:
        ParametersType m_TunableParameters;
        MeasureType m_PerformanceScore;
    };

} // namespace szi

#endif // _sziTunable_h_
