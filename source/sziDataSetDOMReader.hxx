#include "sziDataSetDOMReader.h"

#include "sziTestSystem.h"
#include "sziRegistrationSystem.h"
#include "sziDataDOMReader.h"

#include "sziLogService.h"

namespace szi
{

    /** Function to generate the output object from an input DOM object. */
    template < class TDataSet >
    void DataSetDOMReader<TDataSet>::GenerateData( const DOMNodeType* inputdom, const void* )
    {
    	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

        itk::FancyString tagname = inputdom->GetName();

        if ( tagname != "DataSet" )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): Invalid input DOM object!" << End;
        }

        OutputType* output = this->GetOutput();
        if ( output == 0 )
        {
            typename OutputType::Pointer object = OutputType::New();
            output = object;
            this->SetOutput( output );
        }

        typedef typename OutputType::ElementType ElementType;
        typedef typename ElementType::Pointer ElementPointer;

        output->clear();

        // read the elements
        typename DOMNodeType::ConstChildrenListType children;
        inputdom->GetAllChildren( children );
        for ( int i = 0; i < (int)children.size(); i++ )
        {
            typedef DataDOMReader ReaderType;
            typename ReaderType::Pointer reader = ReaderType::New();
            reader->Update( children[i] );
            ElementType* e = dynamic_cast<ElementType*>( reader->GetOutput() );
            output->push_back( ElementPointer(e) );
        }

        getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
    }

} // namespace szi
