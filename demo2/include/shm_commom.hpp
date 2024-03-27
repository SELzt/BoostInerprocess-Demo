#include "atomic_struct.hpp"
#include <vector>
static std::string poolName = "StructPool2";
#ifdef SHMSERVER
// 需确保server先启动，否则client将收不到数据，因为原有的共享内存被移除了，然后在新的地址开辟了另一个内存块，而client持有的还是旧的内存块地址
struct shm_remove
    {
        shm_remove() { shared_memory_object::remove(poolName.c_str()); }
        ~shm_remove(){ shared_memory_object::remove(poolName.c_str()); }
    } remover;
#endif
static const int poolLength = 655350*4;
Pool pool(open_or_create, poolName.c_str(), poolLength);
AtomicStruct<GaugeInfo> pool_gaugeInfo(pool, "GaugeInfo", GaugeInfo());
AtomicStruct<TurnByTurnInfo> pool_tbtInfo(pool, "TurnByTurnInfo", TurnByTurnInfo());

std::vector<AtomicStruct<TurnByTurnInfo>> pool_tbtInfoVector;
void construct(){
    int length = 7500;
    unsigned long constructLength = length * sizeof(TurnByTurnInfo);
    printf("poolLength = %d, constructLength = %lu, allocate %d\n", poolLength, constructLength, pool.get_size());
    if(poolLength < constructLength){
        printf("construct error\n");
        return;
    }
    for(int i = 0; i < length; ++i){
        printf("%d\n", i);
        AtomicStruct<TurnByTurnInfo> pool_tbtInfo(pool,(std::to_string(i) + "TurnByTurnInfo").c_str(), TurnByTurnInfo());
        pool_tbtInfoVector.push_back(pool_tbtInfo);
    }
}