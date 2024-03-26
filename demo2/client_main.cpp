#include "struct_data.hpp"
using namespace boost::interprocess;
int main(void){
    struct shm_remove
    {
        shm_remove(){/*shared_memory_object::remove("StructPool");*/}
        ~shm_remove(){shared_memory_object::remove("StructPool");}
    }remover;
    managed_shared_memory sharedMemoryPool{open_or_create, "StructPool", 65535};

    GaugeInfo* gaugeInfo = sharedMemoryPool.find<GaugeInfo>(("GaugeInfo")).first;
    if(gaugeInfo != nullptr){
        printf("[GaugeInfo]speedValue = %u, speedValid = %u, speedUnit = %u\n", gaugeInfo->speedValue, gaugeInfo->speedValid, gaugeInfo->speedUnit);
    }
    else{
        printf("if not found in pool, nullptr\n");
    }
    TurnByTurnInfo* tbtInfo = sharedMemoryPool.find<TurnByTurnInfo>(("TurnByTurnInfo")).first;
    if(tbtInfo != nullptr){
        printf("[TurnByTurnInfo]direction = %d, distanceToNext = %u, nextStreet = %s, remainRange = %u, remainTime = %u, totalRange = %u\n", tbtInfo->direction, tbtInfo->distanceToNext, tbtInfo->nextStreet, tbtInfo->remainRange, tbtInfo->remainTime, tbtInfo->totalRange);
    }
    while(1);
    return 0;
}