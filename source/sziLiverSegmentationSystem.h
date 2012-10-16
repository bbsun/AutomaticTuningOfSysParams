#ifndef _sziLiverSegmentationSystem_h_
#define _sziLiverSegmentationSystem_h_

#include "sziSystem.h"

#include <itkArray.h>
#include <fstream>

namespace szi
{

    class LiverSegmentationSystemData : public SystemData
    {
    public:
        /** Standard class typedefs. */
        typedef LiverSegmentationSystemData Self;
        typedef SystemData Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::LiverSegmentationSystemData, szi::SystemData );

        std::string sName;
        
        // name of the settings file
        std::string fnSettings;

        // name of the resulting segmentation image file
        std::string fnResultingSegmentation;

        // name of the reference segmentation image file
        std::string fnReferenceSegmentation;

        // write self to a StreamBuffer
        virtual void streamOut( StreamBuffer& sb ) const
        {
            Superclass::streamOut( sb );

            sb << this->sName;
            sb << this->fnSettings;
            sb << this->fnResultingSegmentation;
            sb << this->fnReferenceSegmentation;
        }

        // read and update self from a StreamBuffer
        virtual void streamIn( StreamBuffer& sb )
        {
            Superclass::streamIn( sb );

            sb >> this->sName;
            sb >> this->fnSettings;
            sb >> this->fnResultingSegmentation;
            sb >> this->fnReferenceSegmentation;
        }

    protected:
        LiverSegmentationSystemData() {}

    private:
        LiverSegmentationSystemData( const Self & ); // purposely not implemented
        LiverSegmentationSystemData& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi


#include <cstdlib>

namespace szi
{

    class LiverSegmentationSystem : public System
    {
    public:
        /** Standard class typedefs. */
        typedef LiverSegmentationSystem Self;
        typedef System Superclass;
        typedef itk::SmartPointer< Self > Pointer;
        typedef itk::SmartPointer< const Self > ConstPointer;

        /** Method for object creation without using the object factory. */
        itkFactorylessNewMacro( Self );

        /** Run-time type information (and related methods). */
        itkTypeMacro( szi::LiverSegmentationSystem, szi::System );

        typedef Superclass::ParametersType ParametersType;

        typedef LiverSegmentationSystemData DataType;

        std::string fnSegmentationProg;
        std::string fnEvaluationProg;
        
        virtual void setData( DataType* data ) { Superclass::setData( data ); }
        DataType* getData() { return static_cast<DataType*>( Superclass::getData() ); }
        const DataType* getData() const { return static_cast<const DataType*>( Superclass::getData() ); }

        virtual void updatePerformanceScore()
        {
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): =====start=====" << End;

            DataType* data = this->getData();

            // set tunable parameters
            const ParametersType& tparams = this->getTunableParameters();
            //getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): parameters = " << tparams << End;
            
            // check whether the low and high thresholds are valid
            double MinimumIntensityRange = 25.0;
            if ( (tparams[0]+MinimumIntensityRange) > tparams[1] )
            {
                double score = 100.0;
                this->setPerformanceScore( score );
                return;
            }

            // perform segmentation
            {
                itk::FancyString cmd;
                cmd << this->fnSegmentationProg;
                cmd << " \"" << data->fnSettings << "\"";
                cmd << " " << tparams[0] << " " << tparams[1];
                cmd << " " << (const itk::Array<double>&)tparams;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): " << cmd << End;
                system( cmd );
            }

            // evaluate the segmentation result
            double score = 100.0; // 100 means no overlap at all
            {
                itk::FancyString fnResult;
                fnResult << "result-" << data->sName << ".txt";
                std::ofstream ofs( fnResult );
                ofs << score;
                ofs.close();
                //
                itk::FancyString cmd;
                cmd << this->fnEvaluationProg;
                cmd << " \"" << data->fnResultingSegmentation << "\"";
                cmd << " \"" << data->fnReferenceSegmentation << "\"";
                cmd << " -o " << fnResult;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): " << cmd << End;
                score = system( cmd );
                score /= 100.0;
                // assume that the score is less than 0 if there is an error during the shell execution
                if ( score < 0.0 ) score = 100.0;
                getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): score = " << score << End;
                //
                std::ifstream ifs( fnResult );
                ifs >> score;
                ifs.close();
            }
            this->setPerformanceScore( score );
            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): score = " << score << End;

            getSystemLogger() << StartInfo(this->GetNameOfClass()) << "updatePerformanceScore(): -----e-n-d-----" << End;
        }

    protected:
        LiverSegmentationSystem()
        {
            DataType::Pointer data = DataType::New();
            this->setData( (DataType*)data );
        }

    private:
        LiverSegmentationSystem( const Self & ); // purposely not implemented
        LiverSegmentationSystem& operator=( const Self & ); // purposely not implemented
    };

} // namespace szi

#endif // _sziLiverSegmentationSystem_h_
