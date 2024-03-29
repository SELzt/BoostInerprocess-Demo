#include "struct_data.hpp"
#include "atomic_struct.hpp"
#include "shm_commom.hpp"
#define getAtomic(_name) _name
using namespace boost::interprocess;
int main(void){
    GaugeInfo gaugeInfo;
    TurnByTurnInfo tbtInfo;
    
    std::chrono::steady_clock::time_point mStartTime;
    std::chrono::steady_clock::time_point mEndTime;
    mStartTime = std::chrono::steady_clock::now();
    mEndTime = std::chrono::steady_clock::now();
    int duration = 0;
    int count = 0;
    while(1){
        message_queue::size_type recvd_size;
        unsigned int priority;
        MqInfo info;
        while(serverToClientMQ.try_receive(&info, MQ_INFO_LEN, recvd_size, priority)){
            switch(info.id){
                case ID_GaugeInfo:
                    {
                        auto ins = FindInstance<GaugeInfo>(ID_GaugeInfo, poolMsgIDMap);
                        if(ins){
                            gaugeInfo = ins->getValue();
                            // printf("[GaugeInfo]speedValue = %u, speedValid = %u, speedUnit = %u\n", gaugeInfo.speedValue, gaugeInfo.speedValid, gaugeInfo.speedUnit);
                        }
                    }
                    break;
                case ID_TurnByTurnInfo:
                    {
                        auto ins = FindInstance<TurnByTurnInfo>(ID_TurnByTurnInfo, poolMsgIDMap);
                        if(ins){
                            tbtInfo = ins->getValue();
                            // printf("[TurnByTurnInfo]direction = %d, distanceToNext = %u, nextStreet = %s, remainRange = %u, remainTime = %u, totalRange = %u\n", tbtInfo.direction, tbtInfo.distanceToNext, tbtInfo.nextStreet, tbtInfo.remainRange, tbtInfo.remainTime, tbtInfo.totalRange);
                        }
                    }
                    break;
                default:
                    break;
            }
            ++count;
            mEndTime = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(mEndTime - mStartTime).count();
            if(duration >= 1000){
                printf("msg recv %d/s\n", count);
                duration = 0;
                count = 0;
                mStartTime = std::chrono::steady_clock::now();
                mEndTime = std::chrono::steady_clock::now();
            }
            // printf("MSGID = %u, MSGACTION = %d, recvd_size = %lu, priority = %d\n", info.id, info.action, recvd_size, priority);

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 0;
}