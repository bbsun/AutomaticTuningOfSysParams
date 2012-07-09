#ifndef _sziSystem_h_
#define _sziSystem_h_

#include <itkSingleValuedCostFunction.h>
#include "sziMPIJob.h"
#include "sziTunable.h"

#include "sziSystemData.h"

namespace szi
{

    /**
    Abstract class to represent any system that can be iteratively tuned.
    A system consists of:
    - a set of data;
    - a set of internal parameters; and
    - the performance score under the current setting of parameters.
    */
    class System : public itk::SingleValuedCostFunction, public MPIJob, public Tunable
    {
    public:
        /** Standard class typedefs. */
        typedef System Self;
        typedef itk::SingleValuedCostFunction Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::System, SingleValuedCostFunction );

        /** User-defined types. */
        typedef SystemData DataType;
        //
        typedef Superclass::ParametersType ParametersType;
        typedef Superclass::MeasureType MeasureType;

        virtual void setData( DataType* data ) { MPIJob::setData( data ); }
        DataType* getData() { return static_cast<DataType*>( MPIJob::getData() ); }
        const DataType* getData() const { return static_cast<const DataType*>( MPIJob::getData() ); }

        virtual void updatePerformanceScore() = 0;

        /**
        Abstract method from MPIJob to run this system as an executable task.
        The default task is to compute the performance score of this system under current system parameters.
        */
        virtual void execute()
        {
            this->updatePerformanceScore();
        }

        virtual unsigned int GetNumberOfParameters() const
        {
            return this->getNumberOfTunableParameters();
        }

        virtual MeasureType GetValue( const ParametersType& params ) const
        {
            Self* self = const_cast<Self*>( this );
            self->setTunableParameters( params );
            self->updatePerformanceScore();
            return self->getPerformanceScore();
        }

        virtual void GetDerivative( const ParametersType& params, DerivativeType& deriv ) const
        {
        	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GetDerivative(): derivative calculation is not supported" << End;
        }

    protected:
        System() {}

    private:
        System( const Self & ); // purposely not implemented
        System& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziSystem_h_
