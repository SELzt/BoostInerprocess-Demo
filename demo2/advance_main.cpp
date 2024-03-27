#include "struct_data.hpp"
#include "atomic_struct.hpp"
#include "shm_commom.hpp"
using namespace boost::interprocess;
int main(void){
    // construct();
    GaugeInfo gaugeInfo;
    gaugeInfo.speedUnit = 1;
    gaugeInfo.speedValid = 1;
    gaugeInfo.speedValue = 0u;
    
    TurnByTurnInfo tbtInfo;
    tbtInfo.direction = 23;
    tbtInfo.distanceToNext = 3333;
    strcpy(tbtInfo.nextStreet, "guilin railway station");
    tbtInfo.remainRange = 30000000;
    tbtInfo.remainTime = 65555;
    tbtInfo.totalRange = 40000000;

    while(1){
        ++gaugeInfo.speedValue;
        if(gaugeInfo.speedValue > 240)
            gaugeInfo.speedValue = 0;
        pool_gaugeInfo.setValue(gaugeInfo);
        // printf("speedValue = %d\n", gaugeInfo.speedValue);
        --tbtInfo.remainRange;
        pool_tbtInfo.setValue(tbtInfo);
        // for(auto it : pool_tbtInfoVector){
        //     it.setValue(tbtInfo);
        // }
        // printf("remainRange = %d\n", tbtInfo.remainRange);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}