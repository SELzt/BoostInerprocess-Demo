#include "struct_data.hpp"
using namespace boost::interprocess;
int main(void){
    struct shm_remove
    {
        shm_remove(){shared_memory_object::remove("StructPool");}
        ~shm_remove(){shared_memory_object::remove("StructPool");}
    }remover;
    
    managed_shared_memory sharedMemoryPool{open_or_create, "StructPool", 65535};
    // 参数：构造类型、名称和初始值
    GaugeInfo* gaugeInfo = sharedMemoryPool.construct<GaugeInfo>("GaugeInfo")(GaugeInfo());
    TurnByTurnInfo* tbtInfo = sharedMemoryPool.construct<TurnByTurnInfo>("TurnByTurnInfo")(TurnByTurnInfo());
    gaugeInfo->speedUnit = 1;
    gaugeInfo->speedValid = 1;
    gaugeInfo->speedValue = 22u;
    tbtInfo->direction = 23;
    tbtInfo->distanceToNext = 3333;
    strcpy(tbtInfo->nextStreet, "guilin railway station");
    tbtInfo->remainRange = 30000000;
    tbtInfo->remainTime = 65555;
    tbtInfo->totalRange = 40000000;
    while(1);
    return 0;
}