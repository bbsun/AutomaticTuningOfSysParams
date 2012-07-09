#ifndef _sziTransformDOMReader_h_
#define _sziTransformDOMReader_h_

#include <itkTransform.h>
#include <itkDOMReader.h>

#include <itkEuler3DTransform.h>
#include <itkQuaternionRigidTransform.h>
#include <itkVersorRigid3DTransform.h>
#include <itkSimilarity3DTransform.h>

#include <itkBSplineDeformableTransform.h>

#include <itkIdentityTransform.h>
#include <itkRigid3DPerspectiveTransform.h>

#include <fstream>
#include <sstream>

#include "sziLogService.h"

namespace szi
{

    // DOM reader for itk::Transform
    template < typename TTransform >
    class TransformDOMReaderOf : public itk::DOMReader<TTransform>
    {
    public:
        /** Standard class typedefs. */
        typedef TransformDOMReaderOf Self;
        typedef itk::DOMReader<TTransform> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::TransformDOMReaderOf, DOMReader );

        typedef TTransform OutputType;

        typedef typename OutputType::ScalarType ScalarType;
        static const unsigned int InputSpaceDimension = OutputType::InputSpaceDimension;
        static const unsigned int OutputSpaceDimension = OutputType::OutputSpaceDimension;

        typedef typename Superclass::DOMNodeType DOMNodeType;

    protected:
        TransformDOMReaderOf() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            // create the transform object

            OutputType* output = this->GetOutput();
            itk::FancyString tagname = inputdom->GetName();

            if ( tagname.MatchWith("Euler3DTransform") )
            {
                if ( InputSpaceDimension != 3 || OutputSpaceDimension != 3 )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Tag name and output type mismatch!" << End;
                }
                typedef itk::Euler3DTransform<ScalarType> TransformType;
                typename TransformType::Pointer transform = TransformType::New();
                output = dynamic_cast<OutputType*>( (TransformType*)transform );
                this->SetOutput( output );
            }

            else if ( tagname.MatchWith("QuaternionRigidTransform") )
            {
                if ( InputSpaceDimension != 3 || OutputSpaceDimension != 3 )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Tag name and output type mismatch!" << End;
                }
                typedef itk::QuaternionRigidTransform<ScalarType> TransformType;
                typename TransformType::Pointer transform = TransformType::New();
                output = dynamic_cast<OutputType*>( (TransformType*)transform );
                this->SetOutput( output );
            }

            else if ( tagname.MatchWith("VersorRigid3DTransform") )
            {
                if ( InputSpaceDimension != 3 || OutputSpaceDimension != 3 )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Tag name and output type mismatch!" << End;
                }
                typedef itk::VersorRigid3DTransform<ScalarType> TransformType;
                typename TransformType::Pointer transform = TransformType::New();
                output = dynamic_cast<OutputType*>( (TransformType*)transform );
                this->SetOutput( output );
            }

            else if ( tagname.MatchWith("Similarity3DTransform") )
            {
                if ( InputSpaceDimension != 3 || OutputSpaceDimension != 3 )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Tag name and output type mismatch!" << End;
                }
                typedef itk::Similarity3DTransform<ScalarType> TransformType;
                typename TransformType::Pointer transform = TransformType::New();
                output = dynamic_cast<OutputType*>( (TransformType*)transform );
                this->SetOutput( output );
            }

            else if ( tagname.MatchWith("BSplineDeformableTransform") )
            {
                if ( InputSpaceDimension != OutputSpaceDimension )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Output type is invalid!" << End;
                }
                typedef itk::BSplineDeformableTransform<ScalarType,InputSpaceDimension> TransformType;
                typename TransformType::Pointer transform = TransformType::New();
                output = dynamic_cast<OutputType*>( (TransformType*)transform );
                this->SetOutput( output );
            }

            else if ( tagname.MatchWith("IdentityTransform") )
            {
                if ( InputSpaceDimension != OutputSpaceDimension )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Output type is invalid!" << End;
                }
                typedef itk::IdentityTransform<ScalarType,InputSpaceDimension> TransformType;
                typename TransformType::Pointer transform = TransformType::New();
                output = dynamic_cast<OutputType*>( (TransformType*)transform );
                this->SetOutput( output );
            }

            else if ( tagname.MatchWith("Rigid3DPerspectiveTransform") )
            {
                if ( InputSpaceDimension != 3 || OutputSpaceDimension != 3 )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Tag name and output type mismatch!" << End;
                }
                typedef itk::Rigid3DPerspectiveTransform<ScalarType> TransformType;
                typename TransformType::Pointer transform = TransformType::New();
                output = dynamic_cast<OutputType*>( (TransformType*)transform );
                this->SetOutput( output );
            }

            // the transform type is not supported
            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading this transform type is not supported!" << End;
            }

            // read and set the fixed and optimizable parameters

            itk::FancyString fs;

            fs = inputdom->GetAttribute( "fparams" );
            if ( fs != "" )
            {
                typename OutputType::ParametersType params = output->GetFixedParameters();
                fs >> params;
                output->SetFixedParameters( params );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): fixed parameters not provided!" << End;
            }

            fs = inputdom->GetAttribute( "params" );
            //
            if ( fs.StartWith("file://") )
            {
                // read the parameters from a disk file
                fs.erase( 0, strlen("file://") );
                std::ifstream ifs( fs );
                if ( !ifs.is_open() )
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Not able to read parameters from file \"" << fs << "\"!" << End;
                }
                std::stringstream buffer;
                buffer << ifs.rdbuf();
                ifs.close();
                fs = buffer.str();
            }
            //
            if ( fs != "" )
            {
                // read the parameters from a string
                typename OutputType::ParametersType params( output->GetNumberOfParameters() );
                fs >> params;
                output->SetParameters( params );
            }
            else
            {
            	getSystemLogger() << StartWarning(this->GetNameOfClass()) << "GenerateData(): parameters not provided!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        TransformDOMReaderOf( const Self & ); // purposely not implemented
        TransformDOMReaderOf& operator=( const Self & ); // purposely not implemented
    };

    // DOM reader for itk::Transform
    class TransformDOMReader : public itk::DOMReader<itk::TransformBase>
    {
    public:
        /** Standard class typedefs. */
        typedef TransformDOMReader Self;
        typedef itk::DOMReader<itk::TransformBase> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::TransformDOMReader, DOMReader );

    protected:
        TransformDOMReader() {}

        /** Function to generate the output object from an input DOM object. */
        virtual void GenerateData( const DOMNodeType* inputdom, const void* )
        {
        	getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): =====start=====" << End;

            itk::FancyString type = inputdom->GetAttribute( "type" );

            if ( type == "" )
            {
                typedef itk::Transform<double> TransformType;
                typedef TransformDOMReaderOf<TransformType> ReaderType;
                ReaderType::Pointer reader = ReaderType::New();
                reader->Update( inputdom );
                this->SetOutput( reader->GetOutput() );
            }

            else if ( type.StartWith("double") )
            {
                if ( type.EndWith("2,2") )
                {
                    typedef itk::Transform<double,2,2> TransformType;
                    typedef TransformDOMReaderOf<TransformType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( type.EndWith("2,3") )
                {
                    typedef itk::Transform<double,2,3> TransformType;
                    typedef TransformDOMReaderOf<TransformType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( type.EndWith("3,2") )
                {
                    typedef itk::Transform<double,3,2> TransformType;
                    typedef TransformDOMReaderOf<TransformType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( type.EndWith("3,3") )
                {
                    typedef itk::Transform<double,3,3> TransformType;
                    typedef TransformDOMReaderOf<TransformType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this double transform type is not supported!" << End;
                }
            }

            else if ( type.StartWith("float") )
            {
                if ( type.EndWith("2,2") )
                {
                    typedef itk::Transform<float,2,2> TransformType;
                    typedef TransformDOMReaderOf<TransformType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( type.EndWith("2,3") )
                {
                    typedef itk::Transform<float,2,3> TransformType;
                    typedef TransformDOMReaderOf<TransformType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( type.EndWith("3,2") )
                {
                    typedef itk::Transform<float,3,2> TransformType;
                    typedef TransformDOMReaderOf<TransformType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else if ( type.EndWith("3,3") )
                {
                    typedef itk::Transform<float,3,3> TransformType;
                    typedef TransformDOMReaderOf<TransformType> ReaderType;
                    ReaderType::Pointer reader = ReaderType::New();
                    reader->Update( inputdom );
                    this->SetOutput( reader->GetOutput() );
                }
                else
                {
                	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this float transform type is not supported!" << End;
                }
            }

            else
            {
            	getSystemLogger() << StartFatal(this->GetNameOfClass()) << "GenerateData(): Reading of this transform type is not supported!" << End;
            }

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "GenerateData(): -----e-n-d-----" << End;
        }

    private:
        TransformDOMReader( const Self & ); // purposely not implemented
        TransformDOMReader& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziTransformDOMReader_h_
