#ifndef _sziSimpleSystemParametersTunerMonitor_h_
#define _sziSimpleSystemParametersTunerMonitor_h_

#include <itkCommand.h>
#include "sziSystemParametersTuner.h"

#include <iostream>

namespace szi
{

    class SimpleSystemParametersTunerMonitor : public itk::Command
    {
    public:
        /** Standard class typedefs. */
        typedef SimpleSystemParametersTunerMonitor Self;
        typedef itk::Command Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::SimpleSystemParametersTunerMonitor, Command );

        typedef SystemParametersTuner TunerType;

        /** This method is called when an event is produced from a monitor-only source, just route it to the observer. */
        virtual void Execute( const itk::Object * caller, const itk::EventObject & eo )
        {
            const TunerType* tuner = static_cast<const TunerType*>( caller );

            std::cout << tuner->getCurrentIteration() << " : " << tuner->getFinalValue() << " <- " << tuner->getFinalParameters() << std::endl;
        }

        /** This method is called when an event is produced from a controllable source, just route it to the observer. */
        virtual void Execute( itk::Object * caller, const itk::EventObject & eo )
        {
            this->Execute( (const itk::Object *)caller, eo );
        }

    protected:
        SimpleSystemParametersTunerMonitor() {}

    private:
        SimpleSystemParametersTunerMonitor( const Self & ); // purposely not implemented
        SimpleSystemParametersTunerMonitor& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziSimpleSystemParametersTunerMonitor_h_
