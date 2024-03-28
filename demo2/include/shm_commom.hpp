#include "atomic_struct.hpp"
#include <vector>
#include <map>
#include <boost/interprocess/ipc/message_queue.hpp>
#define constructMessageID(_name) std::pair<MessageID, const char*>(ID_##_name, #_name)
#define constructAtomicStruct(_pool, _name) AtomicStruct<_name> POOL_##_name(_pool, #_name, _name(), ID_##_name) 
using namespace boost::interprocess;
static std::string poolName = "StructPool2";
static std::string mqName = "server_to_client_queue";
#ifdef SHMSERVER
// 需确保server先启动，否则client将收不到数据，因为原有的共享内存被移除了，然后在新的地址开辟了另一个内存块，而client持有的还是旧的内存块地址
struct shm_remove
    {
        shm_remove() {
            shared_memory_object::remove(poolName.c_str()); 
            message_queue::remove(mqName.c_str());
        }
        ~shm_remove(){
            shared_memory_object::remove(poolName.c_str()); 
            message_queue::remove(mqName.c_str());
        }
    } remover;
#endif
static const int poolLength = 655350*4;
const unsigned int MQ_INFO_LEN = sizeof(MqInfo);
Pool pool(open_or_create, poolName.c_str(), poolLength);

#ifdef SHMSERVER
message_queue serverToClientMQ(create_only, mqName.c_str(), 3000, MQ_INFO_LEN);
#else
message_queue serverToClientMQ(open_only, mqName.c_str());
#endif

constructAtomicStruct(pool, GaugeInfo);
constructAtomicStruct(pool, TurnByTurnInfo);

const std::map<MessageID, const char*> poolMsgIDMap{
    constructMessageID(GaugeInfo),
    constructMessageID(TurnByTurnInfo),
};