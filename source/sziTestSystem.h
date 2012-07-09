#ifndef _sziTestSystem_h_
#define _sziTestSystem_h_

#include "sziSystem.h"
#include <cmath>

namespace szi
{

    class TestSystemData : public SystemData
    {
    public:
        /** Standard class typedefs. */
        typedef TestSystemData Self;
        typedef SystemData Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::TestSystemData, szi::SystemData );

    protected:
        TestSystemData() {}

    private:
        TestSystemData( const Self & ); // purposely not implemented
        TestSystemData& operator=( const Self & ); // purposely not implemented
    };

    class TestSystem : public System
    {
    public:
        /** Standard class typedefs. */
        typedef TestSystem Self;
        typedef System Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::TestSystem, szi::System );

        //typedef TestSystemData DataType;

        virtual void updatePerformanceScore()
        {
            double score = 0;
            //
            for ( int i = 1; i <= 1000; i++ ) score += log( (double)i );
            //
            DataType* data = this->getData();
			//
            this->setPerformanceScore( score );
        }

    protected:
        TestSystem()
        {
            TestSystemData::Pointer data = TestSystemData::New();
            this->setData( (TestSystemData*)data );
        }

    private:
        TestSystem( const Self & ); // purposely not implemented
        TestSystem& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziTestSystem_h_
