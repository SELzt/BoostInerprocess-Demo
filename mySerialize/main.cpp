#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// 定义一个简单的数据结构
struct MyData {
    int x;
    float y;
    std::string name;

    // 序列化函数
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & x;
        ar & y;
        ar & name;
    }
};

int main() {
    // 创建一个MyData对象
    MyData data;
    data.x = 10;
    data.y = 3.14;
    data.name = "Boost.Serialization Example";

    // 将数据序列化到文件
    {
        std::ofstream ofs("data.txt");
        boost::archive::text_oarchive oa(ofs);
        oa << data;
    }

    // 从文件中反序列化数据
    MyData loadedData;
    {
        std::ifstream ifs("data.txt");
        boost::archive::text_iarchive ia(ifs);
        ia >> loadedData;
    }

    // 打印反序列化后的数据
    std::cout << "Loaded Data: x=" << loadedData.x << ", y=" << loadedData.y << ", name=" << loadedData.name << std::endl;

    return 0;
}
