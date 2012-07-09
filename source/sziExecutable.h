#ifndef _sziExecutable_h_
#define _sziExecutable_h_

namespace szi
{

    class Executable
    {
    public:
        /** Abstract method to prepare for execution. */
        virtual void initialize()
        {
            this->setInitializedOn();
        }

        /** Abstract method to perform execution. */
        virtual void execute() {}

        /** Abstract method to do house-keeping after execution. */
        virtual void finalize()
        {
            if ( this->isInitialized() ) this->setInitializedOff();
        }

        virtual ~Executable()
        {
            this->finalize();
        }

    protected:
        Executable() : m_Initialized(false) {}

        bool isInitialized() const { return this->m_Initialized; }
        void setInitializedOn() { this->m_Initialized = true; }
        void setInitializedOff() { this->m_Initialized = false; }

    private:
        bool m_Initialized;
    };

} // namespace szi

#endif // _sziExecutable_h_
