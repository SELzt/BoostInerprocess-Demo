#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <signal.h>
#include "common.h"
using namespace std;
void dispatchRegister(unordered_map<int, DaemonClientInfo>& clients, std::vector<int>& needRemoveVec, DaemonClientInfo& client){
    clients[client.pid] = client;
}
void dispatchUnRegister(std::vector<int>& needRemoveVec, int pid){
    needRemoveVec.push_back(pid);
}
void dispatchMessage(unordered_map<int, DaemonClientInfo>& clients, std::vector<int>& needRemoveVec, DaemonClientInfo& client){
    if(client.type == DaemonType::Register){
        dispatchRegister(clients, needRemoveVec, client);
    }
    else if(client.type == DaemonType::UnRegister){
        dispatchUnRegister(needRemoveVec, client.pid);
    }
    printf("pid -> %d, type -> %d, condition -> %d, scriptPath -> %s \n", client.pid, client.type, client.condition, client.scriptPath);
}
void working(unordered_map<int, DaemonClientInfo>& clients, std::vector<int>& needRemoveVec, DaemonClientInfo& client){
    switch (client.condition)
    {
        case DaemonCondition::ConditionDeath:
            if(kill(client.pid, 0) == -1){
                printf("[info]death pid = %d\n", client.pid);
                system(client.scriptPath);
                dispatchUnRegister(needRemoveVec, client.pid);
            }
            break;
        case DaemonCondition::ConditionTimeout:
            break;
        case DaemonCondition::ConditionNull:
            break;
        
        default:
            break;
    }
}
int main(){
    boost::interprocess::message_queue::remove(mqName);
    boost::interprocess::message_queue m_daemonMq(boost::interprocess::open_or_create, mqName, QUEUE_SIZE, MQ_INFO_LEN);
    std::vector<int> needRemoveVec;
    int pid = 0;
    if(pid = fork()){
        // 父进程
        if(pid > 0){
            exit(0);
        }
        else{
            printf("fork fail 1\n");
        }
    }
    setsid();
    chdir("/");
    umask(0);

    if(pid = fork()){
        // 父进程
        if(pid > 0){
            exit(0);
        }
        else{
            printf("fork fail 2\n");
        }
    }
    printf("become daemon\n");
    unordered_map<int, DaemonClientInfo> clients;
    close(0);
    close(1);
    close(2);
    DaemonClientInfo info;
    unsigned long receiveSize;
    unsigned int priority;
    while(1){
        if(m_daemonMq.try_receive(&info, MQ_INFO_LEN, receiveSize, priority)){
            dispatchMessage(clients, needRemoveVec, info);
        }
        // printf("looping\n");
        while(!needRemoveVec.empty()){
            int tempPid = needRemoveVec[needRemoveVec.size() - 1];
            needRemoveVec.pop_back();
            int res = clients.erase(tempPid);
            // printf("res = %d\n", res);
        }
        for(auto& it : clients){
            // printf("pid -> %d, type -> %d, condition -> %d, scriptPath -> %s \n", it.second.pid, it.second.type, it.second.condition, it.second.scriptPath);
            working(clients, needRemoveVec, it.second);
        }
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    return 0;
}