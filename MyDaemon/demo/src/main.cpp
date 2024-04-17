#include <stdio.h>
#include "unistd.h"
#include "daemon_client.h"
int main(){
    bool res = regiestDaemon(getpid(), 0, "/home/MyDaemon/test.sh");
    printf("regiestDaemon res = %d\n", res);
    while(1);
    return 0;
}