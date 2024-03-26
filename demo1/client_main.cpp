#include <stdio.h>
#include <thread>
#include <chrono>
#include <boost/container/string.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
using namespace boost::container;
using namespace boost::interprocess;
using namespace std;
int main(void){
    shared_memory_object ssm_obj{open_only, "testObj", read_write};
    // ssm_obj.truncate(65530);
    mapped_region region{ssm_obj, read_write};
    while(1){
        printf("read %s\n", region.get_address());
        std::this_thread::sleep_for(chrono::milliseconds(50));
    }
    return 0;
}