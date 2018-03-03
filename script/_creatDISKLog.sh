#!/bin/bash

#============================================================
#
# 脚本说明 : 
#     生成关于监察磁盘日志文件的脚本
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

# 检查文件是否存在
if [[ ! -f "$dsk_log" ]]; then
    touch ./log/dsk.log
fi

# 过滤信息
# _dsk_info : 记录过滤出的信息
_dsk_info=(`df -m | grep '^/dev/' | awk '{printf("%d %d %s %s\n", $2, $3, $5, $6)}'`)

# _dsk_sum  : 记录磁盘总量
# _dsk_avl  : 记录磁盘总占有量
# _ind      : 记录_dsk_info数组下标
# _cnt      : 记录_dsk_info数组长度
_dsk_sum=0
_dsk_avl=0
_ind=0
_cnt=${#_dsk_info[*]}

# 统计信息
# 信息格式:
#     时间 标志(disk/sd) 名称(disk/sd) 总容量 剩余容量 占用比例
# _tmp_info : 记录每条汇总信息
_tmp_info=("time[None]", 0, "disk?[None]", 0, 0, "Ave%[None]")
while (( $_ind < $_cnt ))
do
    _tmp_mod=`expr $_ind % 4`
    
    # 统计分区总量
    if [[ $_tmp_mod -eq 0 ]]; then
        _tmp_num=${_dsk_info[$_ind]}
        _dsk_sum=`expr $_dsk_sum + $_tmp_num`
    fi
    
    # 统计总占用量
    if [[ $_tmp_mod -eq 1 ]]; then
        _tmp_num=${_dsk_info[$_ind]}
        _dsk_avl=`expr $_dsk_avl + $_tmp_num`
    fi
    
    # 收集分区信息
    _tmp_info[$_tmp_mod]=${_dsk_info[$_ind]}
    if [[ $_tmp_mod -eq 3 ]]; then
        _tmp_time=`date +%Y-%m-%d__%H:%M:%S`
        echo $_tmp_time 1 ${_tmp_info[3]} ${_tmp_info[0]} ${_tmp_info[1]} ${_tmp_info[2]} >> $dsk_log
    fi

    _ind=`expr $_ind + 1`
done

# 磁盘信息
_tmp_time=`date +%Y-%m-%d__%H:%M:%S`
_tmp_avl=`echo "scale=3;($_dsk_avl)*100/$_dsk_sum" | bc`
echo $_tmp_time 0 "disk" $_dsk_sum $_dsk_avl $_tmp_avl"%" >> $dsk_log


# 新写法
# NowTime=`date +"%Y-%m-%d__%H:%M:%S"`
# eval $(df -T -m -x tmpfs -x devtmpfs | tail -n +2 | \
#     awk '{printf("paramount["NR"]=%d; parleft["NR"]=%d; parname["NR"]=%s; \
#     usedperc["NR"]=%s;", $3, $5, $7, $6)}\
#     END {printf("parnum=%d\n", NR)}')
# 
# for (( i = 1 ; i <= $parnum ; i++ )); do
#     echo "$NowTime 1 ${parname[$i]} ${paramount[$i]} ${parleft[$i]} ${usedperc[$i]}" >> $dsk_log
#     diskSum=$[ $diskSum + ${paramount[$i]} ]
#     leftSum=$[ $leftSum + ${parleft[$i]} ]
# done 
#
# usedPercSum=$[ (100-$leftSum*100/$diskSum) ]
# echo "$NowTime 0 disk $diskSum $leftSum ${usedPercSum}%" >> $dsk_log
