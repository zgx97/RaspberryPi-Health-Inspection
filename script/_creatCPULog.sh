#!/bin/bash

source ../config 

_show_msg=$show_msg
_init_check=$init_check
_cpu_log=$cpu_log

# 检测系统初始化是否成功
$_init_check
if [[ $? -eq 1 ]]; then
    exit 1
fi

# 引入用户信息
# _usr_pas_log=$usr_pas_log
# _root_pas=`cat "$_usr_pas_log"`

# 检测文件是否存在
if [[ ! -f "$_cpu_log" ]]; then
    touch $_cpu_log
fi

# 获得CPU温度
# 注意一定要获得一个整型
_now_T=`cat /sys/class/thermal/thermal_zone0/temp`
_now_T=`echo "scale=0;$_now_T/1000+1" | bc`
_now_T=$[$_now_T]

# 获得CPU温度提示等级
_war_level="[None]"
if [ $_now_T -le 50 ]; then
    _war_level="Normal"
elif [ $_now_T -le 70 ]; then
    _war_level="Note"
elif [ $_now_T -le 85 ]; then
    _war_level="Warning"
else 
    _war_level="Fatal"
fi


# TODO
# 调用中转脚本, cat频率文件, 选择使用sar命令来获取cpu相关信息
# _ord_0="cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq"
# ./_tmp_.sh $_root_pas $_ord_0

# 获取5分钟平均负载
_loadavg=(`cat /proc/loadavg | awk '{printf("%s\n", $1)}'`)

# 获得当前时间
_now_time=`date +%Y-%m-%d__%H:%M:%S`

echo $_now_time $_loadavg $_now_T $_war_level >> $_cpu_log


# 新写法
# NowTime=`date +"%Y-%m-%d__%H:%M:%S"`

# LoadAvg=`cut -d " " -f 1-3 /proc/loadavg`
# CpuTemp=`cat /sys/class/thermal/thermal_zone0/temp`
# CpuTemp=`echo "scale=2;$CpuTemp/1000" | bc`
#
# eval $(head -n 1 /proc/stat | awk -v sum1=0 -v idle1=0 \
#     '{for (i=2; i<=8; i++) {sum1=sum1+$i} printf("sum1=%d; idle1=%d;", sum1, $5)}')

# sleep 0.5

# eval $(head -n 1 /proc/stat | awk -v sum2=0 -v idle2=0 \
#     '{for (i=2; i<=8; i++) {sum2=sum2+$i} printf("sum2=%d; idle2=%d;", sum2, $5)}')

# CpuUsedPerc=`echo "scale=4;(1-($idle2-$idle1)/($sum2-$sum1))*100" | bc`
# CpuUsedPerc=`printf "%.2f\n" "$CpuUsedPerc"`

# WarnLevel="normal"
# if [[ `echo $CpuTemp '>=' 70 | bc -l` = 1 ]]; then
#     WarnLevel="warning"
# elif [[ `echo $CpuTemp '>=' 50 | bc -l` = 1 ]]; then
#     WarnLevel="note"
# fi

# echo "$NowTime $LoadAvg $CpuUsedPerc ${CpuTemp}°C $WarnLevel" >> $_cpu_log


