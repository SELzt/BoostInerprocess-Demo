#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
using namespace std;
using namespace boost::container;
using namespace boost::interprocess;
static int i = 0;
mapped_region* m_region;
void setValue(){
    memcpy(m_region->get_address(), std::to_string(i).c_str(), sizeof(std::to_string(i).c_str()));
    ++i;
}
int main(void){
    shared_memory_object ssm_obj{open_or_create, "testObj", read_write};
    ssm_obj.truncate(65535);
    offset_t poolSize = 0;
    ssm_obj.get_size(poolSize);
    if(65535 != poolSize){
        printf("truncate error!\n");
        return -1;
    }
    mapped_region region{ssm_obj, read_write};
    m_region = &region;
    while(1){
        setValue();
        std::this_thread::sleep_for(chrono::milliseconds(50));
    };
    return 0;
}