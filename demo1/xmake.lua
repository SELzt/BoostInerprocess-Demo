-- add_includedirs(os.dirs(path.join(os.scriptdir(), "3rd/boost/container/**")))
target("demo1_server")
    set_kind("binary")
    add_files("main.cpp")

target("demo1_client")
    set_kind("binary")
    add_files("client_main.cpp")