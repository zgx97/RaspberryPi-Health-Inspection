#!/bin/bash

# TODO
#=======================================
# 文件说明:
#   使用root权限cat一个文件
#=======================================

source ../config 

_show_msg=$show_msg
_init_check=$init_check

# 检测系统初始化是否成功
$_init_check 
if [[ $? = 1 ]]; then
    $_show_msg "31" "init error"
    exit 1
fi

# 检测参数传入是否正确
if [[ 'x'$1 = 'x' ]]; then
    $_show_msg "33" "_tmp_cat.sh need two arguments!" 
    exit 1
fi

cat $1
