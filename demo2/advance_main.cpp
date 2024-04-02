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
    for(auto& it : poolMsgIDMap){
        printf("MSGID = %u\n", it.first);
    }
    
    std::chrono::steady_clock::time_point mStartTime;
    std::chrono::steady_clock::time_point mEndTime;
    mStartTime = std::chrono::steady_clock::now();
    mEndTime = std::chrono::steady_clock::now();
    int duration = 0;
    int count = 0;
    while(1){
        ++gaugeInfo.speedValue;
        if(gaugeInfo.speedValue > 240)
            gaugeInfo.speedValue = 0;
        --tbtInfo.remainRange;
        {
            auto instance = FindInstance<GaugeInfo>(ID_GaugeInfo, poolMsgIDMap);
            if(instance){
                instance->setValue(gaugeInfo);
            }
        }
        {
            auto instance = FindInstance<TurnByTurnInfo>(ID_TurnByTurnInfo, poolMsgIDMap);
            if(instance){
                instance->setValue(tbtInfo);
            }
        }
        // POOL_GaugeInfo.setValue(gaugeInfo);
        for(int i = 0; i < 1500; ++i){
            {
                static int s_totalSend = 0;
                static int s_success = 0;
                MqInfo mqInfo{(unsigned int)ID_GaugeInfo, (unsigned char)Action_modify};
                boost::posix_time::ptime timeout = boost::get_system_time() + boost::posix_time::millisec(1000);
                // bool res = serverToClientMQ.timed_send(&mqInfo, MQ_INFO_LEN, 256, timeout);
                bool res = serverToClientMQ.try_send(&mqInfo, MQ_INFO_LEN, 256);
                ++s_totalSend;
                s_success += res;
                count += res;
                // printf("s_totalSend = %d, s_success = %d, res = %d\n", s_totalSend, s_success, res);
            }
            {
                MqInfo mqInfo{(unsigned int)ID_TurnByTurnInfo, (unsigned char)Action_modify};
                bool res = serverToClientMQ.try_send(&mqInfo, MQ_INFO_LEN, 127);
                count += res;
                // printf(" res = %d\n", res);
            }
        }
        // {
        //     MqInfo mqInfo{(unsigned int)ID_UNKNOW, (unsigned char)Action_unknow};
        //     bool res = serverToClientMQ.try_send(&mqInfo, MQ_INFO_LEN, 255);
        //     printf("id = %u,  res = %d\n", (unsigned int)ID_UNKNOW, res);
        // }
        // printf("speedValue = %d\n", gaugeInfo.speedValue);
        // POOL_TurnByTurnInfo.setValue(tbtInfo);
        
        mEndTime = std::chrono::steady_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(mEndTime - mStartTime).count();
        
        if(duration >= 1000){
            printf("msg send %d/s\n", count);
            duration = 0;
            count = 0;
            mStartTime = std::chrono::steady_clock::now();
            mEndTime = std::chrono::steady_clock::now();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}