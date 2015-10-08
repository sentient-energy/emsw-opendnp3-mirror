#include <opendnp3/DNP3/Master.h>
#include <opendnp3/DNP3/ScanScheduler.h>

namespace apl {
namespace dnp {

void ScanScheduler::ScheduleOnDemandPoll(void)
{
    /*** Issue on demand integrity pool ***/
    LOG_BLOCK(LEV_DEBUG, "Schedule On Demand Integrity Poll.");

    mMaster->ScheduleOnDemandIntegrityPoll();

    return;
}

} // namespace dnp
} // namespace apl
