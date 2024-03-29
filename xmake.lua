add_rules("mode.release", "mode.debug", "mode.releasedbg")
set_languages("cxx14")
add_cxxflags("-std=c++14")  -- 添加c++编译选项
add_includedirs("3rd/")
includes("demo1", "demo2")