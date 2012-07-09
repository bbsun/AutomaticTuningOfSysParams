#ifndef _sziTestSystemDOMReader_h_
#define _sziTestSystemDOMReader_h_

#include <itkDOMReader.h>
#include "sziTestSystem.h"

namespace szi
{

    class TestSystemDataDOMReader : public itk::DOMReader<TestSystemData>
    {
    public:
        /** Standard class typedefs. */
        typedef TestSystemDataDOMReader Self;
        typedef itk::DOMReader<TestSystemData> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::TestSystemDataDOMReader, DOMReader );

    protected:
        TestSystemDataDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "TestSystemData" )
            {
                this->GetLogger()->Fatal( "Input DOM object is invalid!\n" );
                throw 0;
            }

            OutputType* output = this->GetOutput();
            if ( output == 0 )
            {
                OutputType::Pointer object = OutputType::New();
                output = object;
                this->SetOutput( output );
            }

            // read data fields
        }

    private:
        TestSystemDataDOMReader( const Self & ); // purposely not implemented
        TestSystemDataDOMReader& operator=( const Self & ); // purposely not implemented
    };

    class TestSystemDOMReader : public itk::DOMReader<TestSystem>
    {
    public:
        /** Standard class typedefs. */
        typedef TestSystemDOMReader Self;
        typedef itk::DOMReader<TestSystem> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::TestSystemDOMReader, DOMReader );

    protected:
        TestSystemDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname != "TestSystem" )
            {
                this->GetLogger()->Fatal( "Input DOM object is invalid!\n" );
                throw 0;
            }

            OutputType* output = this->GetOutput();
            if ( output == 0 )
            {
                OutputType::Pointer object = OutputType::New();
                output = object;
                this->SetOutput( output );
            }

            // read settings
        }

    private:
        TestSystemDOMReader( const Self & ); // purposely not implemented
        TestSystemDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziTestSystemDOMReader_h_
