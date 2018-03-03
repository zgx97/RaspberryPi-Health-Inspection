#!/bin/bash

# 交给上层去清空交换区的缓存文件！

source ./config 

_show_msg=$show_msg
_init_check=$init_check

# 获得交换区文件夹和mpc_log文件
_mpc_log=$mpc_log
_mpc_slog=$mpc_slog

# 检测系统是否正常启动
#$_init_check
#if [[ $? -eq 1 ]]; then
#    exit 1
#fi

# 检测文件是否存在
if [[ ! -f $_mpc_log ]]; then
    touch $_mpc_log
fi
if [[ ! -f $_mpc_slog ]]; then
    touch $_mpc_slog
fi

# 检测CPU占用情况
# _mpc_cpu_info (USR PID CPU CNT)
_mpc_cpu_indx=2
_mpc_cpu_info=(`ps -aux --sort=-%cpu | tail -n +2 | awk '{printf("%s %s %s %d\n", $1, $2, $3, 0)}'`)
_mpc_cpu_count=${#_mpc_cpu_info[*]}
# echo $_mpc_cpu_count

# echo ${_mpc_cpu_info[*]} > ../log/a.log

while (( $_mpc_cpu_indx < $_mpc_cpu_count ))
do
    _tmp=`expr $_mpc_cpu_indx + 0`
    echo -e "mpc cpu index : " "$_mpc_cpu_indx" " \c"
    echo "mpc cpu : " ${_mpc_cpu_info[$_tmp]}
    
    if [[ `echo "${_mpc_cpu_info[$_tmp]} <= 20" | bc` = 1 ]]; then
        break
    fi

    # CPU利用率到达界限以上则将数据传入交换区
    if [[ `echo "${_mpc_cpu_info[$_tmp]} >= 20" | bc` = 1 ]]; then
        # 首先找到待查找PID
        _sch_str=${_mpc_cpu_info[`expr $_tmp - 1`]}
        _grp_val=`grep $_sch_str $_mpc_slog`
        # echo -e "sch str is " "$_sch_str" "\c"
        # echo   ",grp val is " "$_grp_val"
        
        # 如果不是第一次出现, 则搜索到上次保存的最大次数
        # 否则则设置成第一次出现
        if [[ $_grp_val"x" != "x" ]]
        then
            #_pid_cnt=(`grep $_sch_str $_mpc_slog | sed -i -e "s/${}"`)
            _pid_cnt=(`grep $_sch_str $_mpc_slog | sort -t " " -n -r -k 4 | awk '{printf("%d\n", $4)}'`)
            _pid_cnt=${_pid_cnt[0]}
            _pid_cnt=`expr $_pid_cnt + 1`
        else
            _pid_cnt=1
        fi

        # 将数据存放到mpc_slog中
        echo "${_mpc_cpu_info[`expr $_tmp - 2`]}" \
             "${_mpc_cpu_info[`expr $_tmp - 1`]}" \
             "${_mpc_cpu_info[$_tmp]}" $_pid_cnt >> $_mpc_slog
        
        # 如果某个进程在mpc_slog中出现了10+, 则放入mpc_log中
        if [[ $_pid_cnt -ge 10 ]]; then
            # 现在在过滤出MEM COMMAND
            _now_info=(`ps --pid $_sch_str | tail -n +2 | awk '{printf("%d %-20s", -1, $4)}'`)
            # echo "now info is " ${_now_info[*]}
            _now_time=`date +%Y-%m-%d__%H:%M:%S`
            _now_cout=$_pid_cnt
            _now_proc=${_now_info[1]}
            _now_user=${_mpc_cpu_info[`expr $_tmp - 2`]}
            _now_cpuu=${_mpc_cpu_info[`expr $_tmp`]}
            _now_memu=${_now_info[0]}
            echo $_now_time $_now_cout $_now_proc \
                 $_now_user $_now_cpuu $_now_memu > $_mpc_log
            # 清空mpc_swap.log中的相关缓存条目
            echo "_sch_str is " "$_sch_str"
            sed -i -e  "/$_sch_str/d" $_mpc_slog
            # cat $_mpc_slog
        fi
    fi
    _mpc_cpu_indx=`expr $_mpc_cpu_indx + 4`
done 


_now_time=`date +%Y-%m-%d__%H:%M:%S`

# echo $_now_time
