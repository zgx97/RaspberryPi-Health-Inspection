#!/bin/bash 

#===================================================================================================================================
# 日志格式
# 时间 主机名 OS版本 内核版本 运行时间 平均负载 磁盘总量 磁盘已用% 内存大小 内存已用% CPU温度 磁盘报警级别 内存报警级别 CPU报警级别
#===================================================================================================================================

source ./config 

_show_msg=$show_msg
_init_check=$init_check
_clc_log=$clc_log

# 检查初始化是否成功
#$_init_check
#if [[ $? -eq 1 ]]; then 
#    exit 1
#fi

# 检查文件是否存在
if [[ ! -f $_clc_log ]]; then
    touch $_clc_log
fi


# 获得时间
_now_time=`date +"%Y-%m-%d__%H:%M:%S"`

# 获取主机名
_local_host_name=`hostname`

# 获得OS版本
_os_version=`cat /etc/issue.net | sed 's/ /_/g'`

# 获得内核版本 
_kn_version=`uname -r`

# 运行时间
_run_time=`uptime -s | sed 's/ /_/g'`

# 平均负载
_load_avg=`cat /proc/loadavg | awk '{printf("%s_%s_%s\n", $1, $2, $3)}'`

# 磁盘大小

# 磁盘已用 

# 内存大小 
_mem_size=(`free -m | tail -n +2 | awk '{printf("%s\n", $2)}'`)
_mem_size=${_mem_size[0]}

# 内存已用 
_mem_used=(`free -m | tail -n +2 | awk '{printf("%.2f\n", $3*100/$2)}'`)
_mem_used=`echo "${_mem_used[0]}%"`


# CPU温度和CPU报警级别
_cpu_temp=`cat /sys/class/thermal/thermal_zone0/temp`
_cpu_temp=`echo "scale=2;$_cpu_temp/1000" | bc`

eval $(head -n 1 /proc/stat | awk -v sum1=0 -v idle1=0 \
    '{for (i=2; i<=8; i++) {sum1=sum1+$i} printf("sum1=%d; idle1=%d;", sum1, $5)}')

sleep 0.5

eval $(head -n 1 /proc/stat | awk -v sum2=0 -v idle2=0 \
    '{for (i=2; i<=8; i++) {sum2=sum2+$i} printf("sum2=%d; idle2=%d;", sum2, $5)}')

_cpu_used_perc=`echo "scale=4;(1-($idle2-$idle1)/($sum2-$sum1))*100" | bc`
_cpu_used_perc=`printf "%.2f\n" "$CpuUsedPerc"`

_cpu_warn="normal"
if [[ `echo $_cpu_temp '>=' 70 | bc -l` = 1 ]]; then
    _cpu_warn="warning"
elif [[ `echo $_cpu_temp '>=' 50 | bc -l` = 1 ]]; then
    _cpu_warn="note"
fi


# 磁盘报警级别 

# 内存报警级别 


