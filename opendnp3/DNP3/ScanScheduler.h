#ifndef __SCAN_SCHEDULER__
#define __SCAN_SCHEDULER__

#include <opendnp3/APL/Loggable.h>
#include <opendnp3/APL/ITimerSource.h>
#include <opendnp3/APL/SuspendTimerSource.h>
#include <opendnp3/APL/DataTypes.h>

#include <vector>
#include <mutex>
#include <unordered_map>

using std::shared_ptr;

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
    void ScheduleFreeFormPoll(std::unordered_map<apl::DataTypes, std::vector<uint32_t>, std::EnumClassHash> ffInputPoints);

private:
    Master *mMaster;
    SuspendTimerSource mSuspendTimerSource;
};

} // namespace dnp
} // namespace apl

#endif
