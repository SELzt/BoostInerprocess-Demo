#include "daemon_client.h"
#include "common.h"
boost::interprocess::message_queue m_daemonMq(boost::interprocess::open_or_create, mqName, QUEUE_SIZE, MQ_INFO_LEN);

bool regiestDaemon(int pid, int condition, char* scriptPath){
    if(pid <= 0){
        printf("[ERROR]%s pid < 0\n", __func__);
        return false;
    }
    DaemonClientInfo info;
    info.pid = pid;
    info.type = DaemonType::Register;
    info.condition = condition;
    strcpy(info.scriptPath, scriptPath);
    return m_daemonMq.try_send(&info, MQ_INFO_LEN, 1);
}

bool unRegisterDaemon(int pid){
    if(pid <= 0){
        printf("[ERROR]%s pid < 0\n", __func__);
        return false;
    }
    DaemonClientInfo info;
    info.pid = pid;
    info.type = DaemonType::UnRegister;
    return m_daemonMq.try_send(&info, MQ_INFO_LEN, 1);
}