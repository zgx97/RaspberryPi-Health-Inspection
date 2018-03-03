#!/bin/bash

#============================================================
#
# 脚本说明 : 
#     生成关于监察内存日志文件的脚本
#
# 脚本参数 : 
#     需要Master传入上一时间段动态内存平均值
#
#============================================================

source ./config

# 检查系统初始化是否成功
_show_msg=$show_msg
_check_check=$init_check

#$_check_check
#if [[ $? -eq 1 ]]; then
#    exit 1
#fi

# 检测传入参数
if [[ _x"$1" = _x ]]; then 
    echo -e "\033[31mERROR:\033[0m" "MEMLog script requires a parameter [_dyn_aver]"
    exit 1
fi

# 检测是否有备份文件
if [[ ! -f "$mem_log" ]]; then
    touch ./log/mem.log
fi 

# 得到上一时间段的动态的内存使用平均值
_dny_aver=$1

# 切割得到内存总空间和剩余空间
_mem_info=(`free -m | head -n 2 | tail -n 1 | awk '{printf("%d %d %d\n", $2, $3, $7)}'`)

# 计算百分比
_tmp_use=`echo "scale=3;${_mem_info[1]}*100/${_mem_info[0]}" | bc`
_per_use=`echo "$_tmp_use%"`

# 更新动态平均值
_dny_temp=`echo "scale=3;0.875*$_dny_aver+0.125*$_tmp_use" | bc`
_dny_aver=`echo "$_dny_temp%"`

# 计算当前时间
_now_time=`date +"%Y-%m-%d__%H:%M:%S"`

# 内存剩余空间
_ept_mem=${_mem_info[2]}

# 输出 当前时间 内存总量 内存剩余空间 内存占用百分比 内存平均占用百分比
echo $_now_time ${_mem_info[0]} $_ept_mem $_per_use $_dny_aver >> $mem_log


