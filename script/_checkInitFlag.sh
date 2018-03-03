#!/bin/bash

# 此文件用来检测系统是否初始化成功

source ../config

_show_msg=$show_msg
_init_flag=$init_flag

if [[ ! -f $_init_flag ]]; then
    $_show_msg "31" "* [ERROR] 未检测到系统成功初始化标记 系统中断"
    exit 1
fi
exit 0
