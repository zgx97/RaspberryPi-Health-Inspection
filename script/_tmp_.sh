#!/usr/bin/expect -f

# TODO
# 中转脚本说明
# 传入参数 :
#   0. root密码
#   1. 执行的命令
#   other 为其他命令参数

# 设置等待时间
set timeout 0.1

if { $argc < 2 } {
    puts stdout "ERROR: _tmp_ script file need two arguments!"
    exit 1
}

set _rot_pas [lindex $argv 0]
set _ord_str [lindex $argv 1]

if { $_ord_str == "cat" } {
    set _ord_agm [lindex $argv 2]
    spawn sudo -S ./_tmp_cat.sh $_ord_agm
    expect ":"
    send "$_rot_pas\r"
    interact
}


