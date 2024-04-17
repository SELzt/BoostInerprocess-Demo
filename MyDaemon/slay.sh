#!/bin/bash  
  
# 检查参数数量  
if [ "$#" -ne 1 ]; then  
    echo "使用方法: $0 <进程名>"  
    exit 1  
fi  
  
# 获取进程名  
process_name=$1  
  
# 查找进程ID  
pid=$(pgrep -f "$process_name")  
  
if [ -z "$pid" ]; then  
    echo "没有找到名为 $process_name 的进程"  
    exit 0  
fi  
  
# 杀掉进程  
echo "正在杀掉进程 $process_name (PID: $pid)..."  
kill -9 $pid  
  
# 检查进程是否已被杀掉  
if ! pgrep -x "$process_name" > /dev/null; then  
    echo "进程 $process_name 已被成功杀掉"  
else  
    echo "无法杀掉进程 $process_name"  
    exit 1  
fi