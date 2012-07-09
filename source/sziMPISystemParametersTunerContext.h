#ifndef _sziMPISystemParametersTunerContext_h_
#define _sziMPISystemParametersTunerContext_h_

#include "sziMPIContext.h"

namespace szi
{

    struct MPISystemParametersTunerContext : public MPIContext
    {
        enum
        {
            TAG_SPT_BASE = 100,
            TAG_SPT_UPDATE_SCORE,
            TAG_SPT_GET_SCORE
        };
    };

} // namespace szi

#endif // _sziMPISystemParametersTunerContext_h_
