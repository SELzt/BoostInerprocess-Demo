#include "atomic_struct.hpp"
#include <vector>
#include <unordered_map>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <memory>
#define constructMessageID(_pool, _name) std::pair<MessageID, std::shared_ptr<void>>(ID_##_name, CreateInstance<_name>(_pool, #_name, ID_##_name))
using namespace boost::interprocess;

static const int poolLength = 655350*4;
static std::string poolName = "StructPool2";
Pool pool(open_or_create, poolName.c_str(), poolLength);

const char* mqName = "server_to_client_queue";
const std::size_t MQ_INFO_LEN = sizeof(MqInfo);
const std::size_t QUEUE_SIZE = 3000;
boost::interprocess::message_queue serverToClientMQ(boost::interprocess::open_or_create, mqName, QUEUE_SIZE, MQ_INFO_LEN);


template<typename T>
std::shared_ptr<AtomicStruct<T>> CreateInstance(Pool& _pool, const char* name, MessageID id) {
    return std::make_shared<AtomicStruct<T>>(_pool, name, T(), id);
}
 
// 查找模板类实例的函数
template<typename T>
std::shared_ptr<AtomicStruct<T>> FindInstance(const MessageID& key, std::unordered_map<MessageID, std::shared_ptr<void>>& instances) {
    auto it = instances.find(key);
    if (it != instances.end()) {
        return std::static_pointer_cast<AtomicStruct<T>>(it->second);
    }
    return nullptr;
}
std::unordered_map<MessageID, std::shared_ptr<void>> poolMsgIDMap{
    constructMessageID(pool, GaugeInfo),
    constructMessageID(pool, TurnByTurnInfo),
};


