#include <boost/interprocess/ipc/message_queue.hpp>
typedef struct DaemonClientInfo{
    int pid;
    int type;// 0 regiest, 1 unRegiest
    int condition;
    char scriptPath[512];
}__attribute__((packed)) DaemonClientInfo;
enum DaemonType{
    Register = 0,
    UnRegister,
};
enum DaemonCondition{
    ConditionDeath = 0,
    ConditionTimeout,
    ConditionNull
};
const char* mqName = "server_to_client_queue";
const std::size_t MQ_INFO_LEN = sizeof(DaemonClientInfo);
const std::size_t QUEUE_SIZE = 3000;
