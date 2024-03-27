#include "struct_data.hpp"
#include "atomic_struct.hpp"
#include "shm_commom.hpp"
using namespace boost::interprocess;
int main(void){
    // construct();
    GaugeInfo gaugeInfo;
    TurnByTurnInfo tbtInfo;

    while(1){
        int i = 0;
        TurnByTurnInfo tempTbtInfo;
        // for(auto it : pool_tbtInfoVector){
        //     if(it.isModify()){
        //         tempTbtInfo = it.getValue();
        //         // printf("remainRange %d\n", tempTbtInfo.remainRange);
        //     }
        //     ++i;
        // }
        // printf("i = %d\n", i);
        // pool_gaugeInfo.printMutexAddr();
        // pool_gaugeInfo.clientLock();

        if(pool_gaugeInfo.isModify()){
            gaugeInfo = pool_gaugeInfo.getValue();
            ++gaugeInfo.speedValue;
            printf("[GaugeInfo]speedValue = %u, speedValid = %u, speedUnit = %u\n", gaugeInfo.speedValue, gaugeInfo.speedValid, gaugeInfo.speedUnit);
        }
        if(pool_tbtInfo.isModify()){
            tbtInfo = pool_tbtInfo.getValue();
            printf("[TurnByTurnInfo]direction = %d, distanceToNext = %u, nextStreet = %s, remainRange = %u, remainTime = %u, totalRange = %u\n", tbtInfo.direction, tbtInfo.distanceToNext, tbtInfo.nextStreet, tbtInfo.remainRange, tbtInfo.remainTime, tbtInfo.totalRange);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 0;
}