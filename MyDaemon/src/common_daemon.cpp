#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <exception>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#define ENTITY_LEN 2
bool getProcessPID(const std::string& processName, int& pid) {
    DIR* dir;
    struct dirent* ent;
    std::ifstream cmdline;
    std::string line;
    pid = -1;
    if ((dir = opendir("/proc")) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string filename = ent->d_name;
            if (filename != "." && filename != "..") {
                std::string path = "/proc/" + filename + "/cmdline";
                cmdline.open(path);
                if (cmdline.is_open()) {
                    std::getline(cmdline, line);
                    if (!line.empty() && line.find(processName) != std::string::npos) {
                        cmdline.close();
                        closedir(dir);
                        try{
                            pid = stoi(filename);
                        }
                        catch(std::exception e){
                            printf("error %s\n", e.what());
                        }
                        return true;
                    }
                    cmdline.close();
                }
            }
        }
        closedir(dir);
    }
    return false;
}
bool checkProcessExist(std::string& processName, int& pid){
    bool isExist = false;
    if(pid != -1){
        if (kill(pid, 0)) {
            isExist = true;
            printf("进程 %s 存在, pid = %d\n", processName.c_str(), pid);
        }
        else {
            printf("进程 %s 不存在\n", processName.c_str());
        }
    }
    else{
        if(getProcessPID(processName, pid)){
            isExist = true;
        }
        else{
            printf("进程 %s 不存在\n", processName.c_str());
        }
    }
    return isExist;
}
int main() {
    daemon(0, 1);
    std::pair<std::string, std::string> entity[ENTITY_LEN] = {
        std::pair<std::string, std::string>("hud_service", "/dim/usr/bin/dhu-cluster/hud/hud_service&"),
        std::pair<std::string, std::string>("hud_hmi", "/dim/usr/bin/dhu-cluster/hud/hud_hmi&")
    };
    int pids[ENTITY_LEN], errorCounts[ENTITY_LEN];
    memset(pids, 0, sizeof(int) * ENTITY_LEN);
    memset(errorCounts, 0, sizeof(int) * ENTITY_LEN);

    std::chrono::steady_clock::time_point startTime, endTime;
    startTime = std::chrono::steady_clock::now();
    while(1){
        endTime = std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() >= 1000){
            startTime = std::chrono::steady_clock::now();
            endTime = std::chrono::steady_clock::now();
            for(int i = 0; i < ENTITY_LEN; ++i){
                errorCounts[i] += abs(checkProcessExist(entity[i].first, pids[i]) - 1);
                if(errorCounts[i] >= 5){
                    errorCounts[i] = 0;
                    pids[i] = -1;
                    printf("restart %s. result = %d\n", entity[i].first.c_str(), system(entity[i].second.c_str()));
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
