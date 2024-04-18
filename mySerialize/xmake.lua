target("mySerialize")
    set_kind("binary")
    add_files("main.cpp")
    add_links("boost_serialization")