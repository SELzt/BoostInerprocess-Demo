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
    for(auto it : poolMsgIDMap){
        printf("MSGID = %u, MSGNAME = %s\n", it.first, it.second);
    }
    while(1){
        ++gaugeInfo.speedValue;
        if(gaugeInfo.speedValue > 240)
            gaugeInfo.speedValue = 0;
        POOL_GaugeInfo.setValue(gaugeInfo);
        {
            MqInfo mqInfo{(unsigned int)ID_GaugeInfo, (unsigned char)Action_modify};
            serverToClientMQ.send(&mqInfo, MQ_INFO_LEN, 0);
        }
        {
            MqInfo mqInfo{(unsigned int)ID_TurnByTurnInfo, (unsigned char)Action_modify};
            serverToClientMQ.send(&mqInfo, MQ_INFO_LEN, 127);
        }
        {
            MqInfo mqInfo{(unsigned int)ID_UNKNOW, (unsigned char)Action_unknow};
            serverToClientMQ.send(&mqInfo, MQ_INFO_LEN, 255);
            boost::posix_time::ptime timeout = boost::get_system_time() + boost::posix_time::millisec(50);
            serverToClientMQ.timed_send(&mqInfo, MQ_INFO_LEN, 255, timeout);
        }
        // printf("speedValue = %d\n", gaugeInfo.speedValue);
        --tbtInfo.remainRange;
        POOL_TurnByTurnInfo.setValue(tbtInfo);
        // for(auto it : pool_tbtInfoVector){
        //     it.setValue(tbtInfo);
        // }
        // printf("remainRange = %d\n", tbtInfo.remainRange);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}