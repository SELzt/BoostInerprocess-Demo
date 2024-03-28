#include "struct_data.hpp"
#include "atomic_struct.hpp"
#include "shm_commom.hpp"
using namespace boost::interprocess;
int main(void){
    GaugeInfo gaugeInfo;
    TurnByTurnInfo tbtInfo;

    while(1){
        message_queue::size_type recvd_size;
        unsigned int priority;
        MqInfo info;
        // serverToClientMQ.timed_receive
        serverToClientMQ.receive(&info, MQ_INFO_LEN, recvd_size, priority); // 阻塞式
        printf("MSGID = %u, MSGACTION = %d, recvd_size = %lu, priority = %d\n", info.id, info.action, recvd_size, priority);
        // if(POOL_GaugeInfo.isModify()){
        //     gaugeInfo = POOL_GaugeInfo.getValue();
        //     ++gaugeInfo.speedValue;
        //     printf("[GaugeInfo]speedValue = %u, speedValid = %u, speedUnit = %u\n", gaugeInfo.speedValue, gaugeInfo.speedValid, gaugeInfo.speedUnit);
        // }
        // if(pool_tbtInfo.isModify()){
        //     tbtInfo = pool_tbtInfo.getValue();
        //     printf("[TurnByTurnInfo]direction = %d, distanceToNext = %u, nextStreet = %s, remainRange = %u, remainTime = %u, totalRange = %u\n", tbtInfo.direction, tbtInfo.distanceToNext, tbtInfo.nextStreet, tbtInfo.remainRange, tbtInfo.remainTime, tbtInfo.totalRange);
        // }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 0;
}