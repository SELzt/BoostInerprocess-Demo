#ifndef STRUCT_ADAT_HPP
#define STRUCT_ADAT_HPP
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/thread/thread.hpp>
typedef struct MqInfo
{
    unsigned int id;
	unsigned char action;
}__attribute__((packed)) MqInfo;

typedef struct GaugeInfo
{
    unsigned char speedUnit;
    unsigned char speedValid;
    unsigned short speedValue;
}__attribute__((packed)) GaugeInfo;

typedef struct TurnByTurnInfo
{
	unsigned int remainRange;    
	unsigned int totalRange;     
	unsigned short direction;    
	unsigned int distanceToNext; 
	unsigned int remainTime;     
	char nextStreet[24];         
}__attribute__((packed)) TurnByTurnInfo;


enum MessageID{
    ID_GaugeInfo = 0x20000000,
    ID_TurnByTurnInfo = 0x20000001,
	ID_UNKNOW = 0xFFFFFFFF
};
enum SHMAction{
	Action_modify = 0,
	Action_remove,
	Action_unknow
};
#endif