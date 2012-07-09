#ifndef _sziInterpolatorDOMReader_h_
#define _sziInterpolatorDOMReader_h_

#include <itkInterpolateImageFunction.h>
#include <itkDOMReader.h>

#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkBSplineInterpolateImageFunction.h>

#include "sziLogService.h"

namespace szi
{

    // DOM reader for itk::InterpolateImageFunction
    template < typename TInterpolator >
    class InterpolatorDOMReaderOf : public itk::DOMReader<TInterpolator>
    {
    public:
        /** Standard class typedefs. */
        typedef InterpolatorDOMReaderOf Self;
        typedef itk::DOMReader<TInterpolator> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::InterpolatorDOMReaderOf, DOMReader );

        typedef TInterpolator OutputType;

        typedef typename OutputType::InputImageType InputImageType;
        typedef typename OutputType::CoordRepType CoordRepType;

        typedef typename Superclass::DOMNodeType DOMNodeType;

    protected:
        InterpolatorDOMReaderOf() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            if ( this->GetOutput() )
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): The user-specified output will be ignored!" << End;
            }

            OutputType* output = this->GetOutput();
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname.StartWith("NearestNeighbor") )
            {
                typedef itk::NearestNeighborInterpolateImageFunction<InputImageType,CoordRepType> RealOutputType;
                typename RealOutputType::Pointer ro = RealOutputType::New();
                output = dynamic_cast<OutputType*>( (RealOutputType*)ro );
                this->SetOutput( output );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): use NearestNeighbor!" << End;
            }

            else if ( tagname.StartWith("Linear") )
            {
                typedef itk::LinearInterpolateImageFunction<InputImageType,CoordRepType> RealOutputType;
                typename RealOutputType::Pointer ro = RealOutputType::New();
                output = dynamic_cast<OutputType*>( (RealOutputType*)ro );
                this->SetOutput( output );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): use Linear!" << End;
            }

            else if ( tagname.StartWith("BSpline") )
            {
                typedef itk::BSplineInterpolateImageFunction<InputImageType,CoordRepType> RealOutputType;
                typename RealOutputType::Pointer ro = RealOutputType::New();
                output = dynamic_cast<OutputType*>( (RealOutputType*)ro );
                this->SetOutput( output );
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): use BSpline!" << End;
            }

            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this interpolator is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        InterpolatorDOMReaderOf( const Self & ); // purposely not implemented
        InterpolatorDOMReaderOf& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziInterpolatorDOMReader_h_
