#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <functional>
#include <unordered_map>
/**
 * Example 
 * 
#include "HighPrecisionTimer.hpp"
using namespace util::timer;
int main() {
    TimerSharedPtr timerBase = HighPrecisionTimer::create(500);
    TimerSharedPtr timer1 = HighPrecisionTimer::create(200, true);
    timer1->registerCallback([&](){
        printf("timer1 id = %ld, duration = %ld\n", timer1->getId(), timer1->getElapsedTime());
    });
    TimerSharedPtr timer2 = HighPrecisionTimer::create(500);
    timer2->registerCallback([&](){
        printf("timer2 id = %ld, duration = %ld\n", timer2->getId(), timer2->getElapsedTime());
    });
    timer1->start();
    timer2->start();
    // timer2->stop();
    int count = 0;
    while(1){
        timerBase->loop();
        ++count;
        if(count == 300){
            timer1->stop();
        }
        if(count == 500){
            timer1->start();
            count = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    return 0;
}
*/
namespace util{
    namespace timer{
        class HighPrecisionTimer;
        typedef std::shared_ptr<HighPrecisionTimer> TimerSharedPtr;
        std::unordered_map<size_t, std::weak_ptr<HighPrecisionTimer>> timerMap;
        size_t uniqueID = 0;
        inline size_t generateID(){
            return ++uniqueID;
        }
        class HighPrecisionTimer {
        public:
            HighPrecisionTimer(size_t id, size_t interval, bool isRepeat) : m_isRunning(false), m_startTime(), m_endTime() {
                m_id = id;
                m_isRepeat = isRepeat;
                m_interval = interval;
            }
            ~HighPrecisionTimer(){
                stop();
                timerMap.erase(m_id);
            }
            static TimerSharedPtr create(size_t interval, bool isRepeat = false){
                size_t id = generateID();
                TimerSharedPtr ptr = std::make_shared<HighPrecisionTimer>(id, interval, isRepeat);
                timerMap[id] = ptr;
                return ptr;
            }
            void start() {
                m_isRunning = true;
                m_startTime = std::chrono::high_resolution_clock::now();
            }

            void stop() {
                printf("stop id = %ld\n", m_id);
                m_endTime = std::chrono::high_resolution_clock::now();
                m_isRunning = false;
            }

            void loop(){
                for(const auto& it : timerMap){
                    TimerSharedPtr timer = it.second.lock();
                    if(timer)
                        timer->invoke();
                }
            }
            void invoke(){
                if(m_isRunning && getElapsedTime() >= m_interval){
                    m_cb();
                    m_startTime = std::chrono::high_resolution_clock::now();
                    if(!m_isRepeat){
                        stop();
                    }
                }
            }
            inline void registerCallback(std::function<void(void)> cb){
                m_cb = cb;
            }
            inline size_t getId() { return m_id; }
            inline size_t getElapsedTime() {
                return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_startTime).count();
            }

        private:
            bool m_isRunning;
            bool m_isRepeat;
            size_t m_interval;//ms
            size_t m_id;
            std::function<void(void)> m_cb; 
            std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime, m_endTime;
        };
    }
}

