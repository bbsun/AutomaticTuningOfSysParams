#ifndef _sziSystemTrainingMetric_h_
#define _sziSystemTrainingMetric_h_

#include <itkSingleValuedCostFunction.h>

#include "sziSystem.h"
#include "sziDataSet.h"

namespace szi
{

    /**
    Class to evaluate the performance of a system using a number of training examples.
    For each training example, a score is computed for the system, and the overall system performance
    is computed as the mean of the individual scores.
    */
    class SystemTrainingMetric : public itk::SingleValuedCostFunction
    {
    public:
        /** Standard class typedefs. */
        typedef SystemTrainingMetric Self;
        typedef itk::SingleValuedCostFunction Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SystemTrainingMetric, SingleValuedCostFunction );

        /** User-defined types. */
        typedef System SystemType;
        typedef SystemType::DataType SystemDataType;
        //
        typedef DataSet<SystemDataType> DataType;
        //
        typedef Superclass::MeasureType MeasureType;

        virtual void setSystem( SystemType* system ) { this->m_System = system; }
        SystemType* getSystem() { return this->m_System; }
        const SystemType* getSystem() const { return this->m_System; }

        virtual void setData( DataType* data ) { this->m_Data = data; }
        DataType* getData() { return this->m_Data; }
        const DataType* getData() const { return this->m_Data; }

        virtual void initialize()
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): =====start=====" << End;

            SystemType* system = this->getSystem();
            if ( system == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): system is null" << End;
            }

            if ( this->getData() == 0 )
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "initialize(): the training data set is null" << End;
            }

            if ( this->getData()->size() == 0 )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "initialize(): number of training examples is zero" << End;
            }

            if ( this->GetNumberOfParameters() == 0 )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "initialize(): number of parameters to be tuned is zero" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "initialize(): -----e-n-d-----" << End;
        }

        virtual unsigned int GetNumberOfParameters() const
        {
            const SystemType* system = this->getSystem();
            return system->getNumberOfTunableParameters();
        }

        virtual MeasureType GetValue( const ParametersType& params ) const
        {
            Self* self = const_cast<Self*>( this );

            SystemType* system = self->getSystem();

            // compute the score for each training example, and then aggregate them
            MeasureType value = 0;

            // first, start score computation for each training example
            for ( unsigned int i = 0; i < self->getData()->size(); i++ )
            {
                SystemDataType* data = self->getData()->at(i);
                system->setData( data );
                system->setTunableParameters( params );
                system->updatePerformanceScore();
            }

            // then, collect and sum individual scores
            for ( unsigned int i = 0; i < self->getData()->size(); i++ )
            {
                SystemDataType* data = self->getData()->at(i);
                system->setData( data );
                MeasureType score = system->getPerformanceScore();
                value += score;
            }

            // finally, compute the average
            value /= (MeasureType)self->getData()->size();

            return value;
        }

        virtual void GetDerivative( const ParametersType& params, DerivativeType& deriv ) const
        {
        	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GetDerivative(): derivative calculation is not supported" << End;
        }

    protected:
        SystemTrainingMetric() {}

    private:
        SystemTrainingMetric( const Self & ); // purposely not implemented
        SystemTrainingMetric& operator=( const Self & ); // purposely not implemented

        SystemType::Pointer m_System;
        DataType::Pointer m_Data;
    };

} // namespace szi

#endif // _sziSystemTrainingMetric_h_
