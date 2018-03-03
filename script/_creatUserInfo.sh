#!/bin/bash

#============================================================================
# 日志文件说明:
#   获取本机用户信息
# 日志文件格式：
#   时间 用户总数 近期活跃用户(3人) 具有root权限的用户 当前在线用户及登录IP 
#
#============================================================================

source ../config 

_show_msg=$show_msg
_init_check=$init_check
_usr_log=$usr_log

# 检查是否初始化成功
$_init_check
if [[ $? -eq 1 ]]; then
    exit 1
fi

# 检查日志文件是否存在
if [[ ! -f $_usr_log ]]; then
    touch $_usr_log
fi 


# 获取时间
_now_time=`date +"%Y-%m-%d__%H:%M:%S"`


# 获取用户总数
# _usr_lst : 用户名列表
# _usr_cnt : 非系统用户个数
eval $(cat /etc/passwd | awk -F: 'BEGIN{cnt = 0;}{if ($3 >= 1000 && $3 < 65534) {printf("_usr_lst["NR"]=%s\n", $1); cnt++;}} END{printf("_usr_cnt=%d\n", cnt);}')


# 获取近期活跃用户 
# _act_usr : 活跃用户列表
_act_usr=(`last | cut -d " " -f 1 | grep -v root | sort | uniq -c | sort -k 1 -n -r \
                | grep -v wtmp | grep -v reboot | head -n -1 | head -n 3 \
                |awk '{printf("%s\n", $2)}'`)
# echo ${_act_usr[*]}


# 获取具有root权限的用户
# _usr_rot : 具有root权限的用户
eval $(awk -F : '{if ($3 == 1000) printf("_usr_rot=%s", $1)}' /etc/passwd)
if [[ -r /etc/sudoers ]]; then 
    for (( i = 1 ; i <= $_usr_cnt ; i++ )); do
        grep -q "${_usr_lst[$i]}" /etc/sudoers 
        if [[ $? -eq 0 ]]; then
            _usr_rot="$_usr_rot,${_usr_lst[$i]}"
        fi
    done
fi 
# echo $_usr_rot


# 获取当前在线用户及登录IP
_now_usr=(`w -h | awk '{printf(",%s_%s_%s", $1, $2, $3)}' | cut -c 2-`)
# echo $_now_usr

echo "$_now_time $_usr_cnt [${_act_usr[*]}] [${_usr_rot[*]}] [${_now_usr[*]}]" >> $_usr_log
