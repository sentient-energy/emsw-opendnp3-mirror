#ifndef __SCAN_SCHEDULER__
#define __SCAN_SCHEDULER__

#include <opendnp3/APL/Loggable.h>

namespace apl {
namespace dnp {

class Master;

class ScanScheduler : private Loggable
{
public:
    ScanScheduler(Master* apMaster, Logger* apLogger) :
        Loggable(apLogger),
        mMaster(apMaster) {};

    void ScheduleOnDemandPoll(void);

private:
    Master *mMaster; 
};

} // namespace dnp
} // namespace apl

#endif
