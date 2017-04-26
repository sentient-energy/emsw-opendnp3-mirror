#include <opendnp3/DNP3/Master.h>
#include <opendnp3/DNP3/ScanScheduler.h>
#include <opendnp3/APL/DataTypes.h>

namespace apl {
namespace dnp {

void ScanScheduler::ScheduleOnDemandPoll(void)
{
    /*** Issue on demand integrity pool ***/
    LOG_BLOCK(LEV_DEBUG, "Schedule On Demand Integrity Poll.");

    Transaction tr(&mSuspendTimerSource); // need to pause execution so that this action is safe
    mMaster->ScheduleOnDemandIntegrityPoll();

    return;
}

void ScanScheduler::ScheduleFreeFormPoll(std::unordered_map<apl::DataTypes, std::vector<uint32_t>, std::EnumClassHash> ffInputPoints)
{
    /*** Issue free form pool ***/
    LOG_BLOCK(LEV_DEBUG, "Schedule Free Form Poll. #points" << ffInputPoints.size() << " : " );

    Transaction tr(&mSuspendTimerSource); // need to pause execution so that this action is safe
    mMaster->ScheduleFreeFormPoll(ffInputPoints);

    return;
}

} // namespace dnp
} // namespace apl
