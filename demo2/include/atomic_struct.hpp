#ifndef ATOMIC_STRUCT_HPP
#define ATOMIC_STRUCT_HPP
#include "struct_data.hpp"
#include <string>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
using namespace boost::interprocess;
typedef managed_shared_memory Pool;
template <typename T>
class AtomicStruct{
    static void asign(T& src, const T& dst){
        memcpy(&src, &dst, sizeof(T));
    }
    static bool equals(T& src, T& dst){
        return !memcmp(&src, &dst, sizeof(T));
    }
    public:
        AtomicStruct(Pool& pool, const char* name, const T& initStruct):m_pool(pool){
            // m_pool = pool;
            auto res = m_pool.find_no_lock<T>(name);
            if(res.first != nullptr){
                m_ptr = res.first;
            }
            else{
                m_ptr = m_pool.construct<T>(name)(initStruct);
            }
            assert(m_ptr != nullptr);
            asign(m_local, initStruct);
            mtxName = name;
            mtxName.append(".locker");
            #ifdef SHMSERVER
            named_mutex::remove(mtxName.c_str());
            #endif
            m_mutex = new named_mutex(open_or_create, mtxName.c_str());
        }
        ~AtomicStruct(){
            #ifdef SHMSERVER
            named_mutex::remove(mtxName.c_str());
            #endif
        }
        void setValue(T newData){
            scoped_lock<named_mutex> lock(*m_mutex);
            asign(*m_ptr, newData);
            // printf("[%s]\n", __func__);
        }
        void syncValue(){
            if(isModify()){
                m_local = getValue();
            }
        }
        T getValue(){
            scoped_lock<named_mutex> lock(*m_mutex);
            asign(m_local, *m_ptr);
            return m_local;
        }

        bool isModify(){
            // printf("%#x\n", m_mutex);
            bool res = false;
            res = !equals(m_local, *m_ptr);
            return res;
        }
        void printMutexAddr(){
            printf("%#x\n", m_mutex);
        }
        void clientLock(){
            scoped_lock<named_mutex> lock(*m_mutex);
            while(1);
        }
    private:
        Pool& m_pool;
        T* m_ptr;
        T m_local;
        std::string mtxName;
        named_mutex* m_mutex;
};

#endif