#ifndef DAEMON_CLIENT
#define DAEMON_CLIENT
#ifdef __cplusplus
extern "C"{
#endif
    bool regiestDaemon(int pid, int condition, char* scriptPath);
    bool unRegisterDaemon(int pid);
#ifdef __cplusplus
}
#endif

#endif