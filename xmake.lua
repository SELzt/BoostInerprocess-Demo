add_rules("mode.release", "mode.debug", "mode.releasedbg")
set_languages("cxx14")
add_cxxflags("-std=c++14")  -- 添加c++编译选项
add_defines("QNX")
add_defines("QNX_SCREEN")
add_defines("_QNX_SOURCE")
add_defines("BOOST_THREAD_NO_LIB")
add_includedirs("3rd/")
includes("demo2")