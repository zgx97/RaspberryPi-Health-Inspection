#!/bin/bash

# 检查系统初始化是否成功
_show_msg=$show_msg
_check_check=$init_check

$_check_check
if [[ $? -eq 1 ]]; then
    exit 1
fi

# 传入两个参数
# 第一个参数为颜色, 第二个参数为字符串

_msg_end="\033[0m"

# 黑色
_msg_blk="\033[30m" 
# 红色
_msg_red="\033[31m"
# 绿色
_msg_grn="\033[32m"
# 黄色
_msg_yel="\033[33m"
# 蓝色
_msg_blu="\033[34m"
# 紫色
_msg_pup="\033[35m"
# 天蓝色
_msg_sbl="\033[36m"
# 白色
_msg_wht="\033[37m"


if [[ "x"$1 = "x" || "x"$2 = "x" ]]; then
    echo -e "$_msg_red""function showMessage need two arguments""$_msg_end"
    exit 1
fi

echo -e "\033[$1m$2$_msg_end"

