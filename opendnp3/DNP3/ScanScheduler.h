#ifndef __SCAN_SCHEDULER__
#define __SCAN_SCHEDULER__

#include <opendnp3/APL/Loggable.h>
#include <opendnp3/APL/ITimerSource.h>
#include <opendnp3/APL/SuspendTimerSource.h>

namespace apl {
namespace dnp {

class Master;

class ScanScheduler : private Loggable
{
public:
    ScanScheduler(Master* apMaster, ITimerSource* apTimerSrc, Logger* apLogger) :
        Loggable(apLogger),
        mMaster(apMaster),
        mSuspendTimerSource(apTimerSrc)
        {};

    void ScheduleOnDemandPoll(void);

private:
    Master *mMaster;
    SuspendTimerSource mSuspendTimerSource;
};

} // namespace dnp
} // namespace apl

#endif
