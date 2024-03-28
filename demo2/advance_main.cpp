#include "struct_data.hpp"
#include "atomic_struct.hpp"
#include "shm_commom.hpp"
using namespace boost::interprocess;
int main(void){
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
            static int s_totalSend = 0;
            static int s_success = 0;
            MqInfo mqInfo{(unsigned int)ID_GaugeInfo, (unsigned char)Action_modify};
            boost::posix_time::ptime timeout = boost::get_system_time() + boost::posix_time::millisec(1000);
            bool res = serverToClientMQ.timed_send(&mqInfo, MQ_INFO_LEN, 0, timeout);
            ++s_totalSend;
            s_success += res;
            printf("s_totalSend = %d, s_success = %d, res = %d\n", s_totalSend, s_success, res);
        }
        {
            MqInfo mqInfo{(unsigned int)ID_TurnByTurnInfo, (unsigned char)Action_modify};
            bool res = serverToClientMQ.try_send(&mqInfo, MQ_INFO_LEN, 127);
            printf("id = %u,  res = %d\n", (unsigned int)ID_TurnByTurnInfo, res);
        }
        {
            MqInfo mqInfo{(unsigned int)ID_UNKNOW, (unsigned char)Action_unknow};
            bool res = serverToClientMQ.try_send(&mqInfo, MQ_INFO_LEN, 255);
            printf("id = %u,  res = %d\n", (unsigned int)ID_UNKNOW, res);
        }
        // printf("speedValue = %d\n", gaugeInfo.speedValue);
        --tbtInfo.remainRange;
        POOL_TurnByTurnInfo.setValue(tbtInfo);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 0;
}