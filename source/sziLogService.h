#ifndef _sziLogService_h_
#define _sziLogService_h_

#include "sziFancyLogger.h"

namespace szi
{

    /** To get the system logger where it is needed, either directly use the global variable, */
    extern FancyLogger::Pointer syslog;

    /** or use the following function call. */
    inline FancyLogger& getSystemLogger() { return *(FancyLogger*)syslog; }

} // namespace szi

#endif // _sziLogService_h_
