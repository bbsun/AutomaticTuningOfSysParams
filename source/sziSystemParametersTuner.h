#ifndef _sziSystemParametersTuner_h_
#define _sziSystemParametersTuner_h_

#include <itkObject.h>
#include "sziMPIJob.h"
#include "sziCommandInterface.h"

#include "sziSystemTrainingMetric.h"
#include <itkSingleValuedNonLinearOptimizer.h>

namespace szi
{

    class SystemParametersTuner : public itk::Object, public MPIJob, public CommandInterface
    {
    public:
        /** Standard class typedefs. */
        typedef SystemParametersTuner Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SystemParametersTuner, Object );

        typedef SystemTrainingMetric MetricType;
        //
        typedef MetricType::SystemType SystemType;
        typedef MetricType::DataType DataType;
        typedef MetricType::ParametersType ParametersType;

        typedef itk::SingleValuedNonLinearOptimizer OptimizerType;

        virtual void setSystem( SystemType* system ) { this->m_System = system; }
        virtual SystemType* getSystem() { return this->m_System; }
        virtual const SystemType* getSystem() const { return this->m_System; }

        virtual void setData( DataType* data ) { this->m_Data = data; }
        virtual DataType* getData() { return this->m_Data; }
        virtual const DataType* getData() const { return this->m_Data; }

        virtual void setMetric( MetricType* metric ) { this->m_Metric = metric; }
        virtual MetricType* getMetric() { return this->m_Metric; }
        virtual const MetricType* getMetric() const { return this->m_Metric; }

        virtual void setOptimizer( OptimizerType* optimizer ) { this->m_Optimizer = optimizer; }
        virtual OptimizerType* getOptimizer() { return this->m_Optimizer; }
        virtual const OptimizerType* getOptimizer() const { return this->m_Optimizer; }

        virtual void setInitialParameters( const ParametersType& params ) { this->m_InitialParameters = params; }
        virtual const ParametersType& getInitialParameters() const { return this->m_InitialParameters; }

        virtual const ParametersType& getFinalParameters() const { return this->m_FinalParameters; }

        virtual double getFinalValue() const { return this->m_FinalValue; }

        virtual int getCurrentIteration() const { return this->m_IterationCount; }

        /**
        Executable method to prepare for parameters tuning.
        */
        virtual void initialize()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): =====start=====" << End;

            SystemType* system = this->getSystem();
            if ( system == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): system is null" << End;
            }
            system->initialize();

            MetricType* metric = this->getMetric();
            if ( metric == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): metric is null" << End;
            }
            metric->setSystem( system );
            metric->setData( this->getData() );
            metric->initialize();

            OptimizerType* optimizer = this->getOptimizer();
            if ( optimizer == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): optimizer is null" << End;
            }
            optimizer->SetCostFunction( metric );
            optimizer->AddObserver( itk::IterationEvent(), this->GetCommandAdapter() );

            this->setInitialParameters( system->getTunableParameters() );

            MPIJob::initialize();

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): -----e-n-d-----" << End;
        }

        /**
        Executable method to start parameters tuning.
        */
        virtual void execute()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): =====start=====" << End;

            OptimizerType* optimizer = this->getOptimizer();

            this->m_IterationCount = 0;

            optimizer->SetInitialPosition( this->m_InitialParameters );

            optimizer->StartOptimization();

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "execute(): -----e-n-d-----" << End;
        }

        /**
        CommandInterface method to process iteration events.
        */
        virtual void Execute( const itk::Object * caller, const itk::EventObject & eo )
        {
            const OptimizerType* optimizer = this->getOptimizer();

            const ParametersType& curpos = optimizer->GetCurrentPosition();
            double curval = optimizer->GetValue( curpos );

            if ( this->m_IterationCount == 0 || this->m_FinalValue > curval )
            {
                this->m_FinalParameters = curpos;
                this->m_FinalValue = curval;
            }

            this->m_IterationCount++;

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "Execute(): " << this->m_IterationCount << " " << this->m_FinalValue << " " << curval << End;

            // forward the event to observers of this object
            this->InvokeEvent( eo );
        }

    protected:
        SystemParametersTuner() : m_FinalValue(0), m_IterationCount(0) {}

    private:
        SystemParametersTuner( const Self & ); // purposely not implemented
        SystemParametersTuner& operator=( const Self & ); // purposely not implemented

        SystemType::Pointer m_System;

        DataType::Pointer m_Data;

        MetricType::Pointer m_Metric;

        OptimizerType::Pointer m_Optimizer;

        ParametersType m_InitialParameters;

        ParametersType m_FinalParameters;
        double m_FinalValue;

        int m_IterationCount;
    };

} // namespace szi

#endif // _sziSystemParametersTuner_h_
