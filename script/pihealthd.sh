#!/bin/bash

set -e

umask 022


# 使定义在init-functions文件中的所有shell函数在当前脚本(实际是shell)中生效
. /lib/lsb/init-functions

# Are we running from init?
# previous? runlevel?
run_by_init() {
    ([ "$previous" ] && [ "$runlevel" ]) || [ "$runlevel" = S ]
}

# 检查是否启动
check_for_upstart() {
    if init_is_upstart; then
    exit $1
    fi
}

# 检查没有启动
check_for_no_start() {
    # forget it if we're trying to start, and /etc/pihealth/pihealt_not_to_be_run exists
    if [ -e /et/pihealth/pihealt_not_to_be_run ]; then
    fi 
    fi


}


case "$1" in
    # 退出分正常退出和异常退出
    # 设置一个文件 pihealth_to_run, 里面放置一个pid
    # 如果文件存在说明已经运行
    # 对于异常退出的情况，pihealth_to_run 是不会被删除的，但是异常退出后下次重新进入
    # pid一定会改变，所以需要对比pid
    # 程序没有运行的情况有两种
    # 1. pihealth_to_run not exists 
    # 2. pihealth_to_run exists but pre_pid != now_pid
    
    # flag = 0 代表程序已经运行
    # flag = 1 代表pihealth_to_run不存在
    # flag = 2 代表pihealth_to_run存在，但是里面记录的是之前的pid
    start)
        sleep 1
        now_pid=`ps | grep "pihealth" | awk '{printf("%s", $1)}'`
        flag=0
        if ! [ -n ../pihealt_to_run ]; then
            flag=1
        else
            pre_pid=`cat ../pihealt_to_run | grep "pre_pid" | awk '{printf("%s", $2)}'`
            if [[ $pre_pid != now_pid ]]; then 
                flag=2
            else
                flag=0
            fi
        fi
        if [[ $flag == 0 ]]; then 
            echo "PiHealth Had Start!"
            exit 1
        elif [[ $flag == 1 ]]; then
            echo "pre_pid $now_pid" ../pihealth_to_run
            echo "PiHealth Start!"
            # 启动脚本
        else
            

        fi
    ;;
    stop)
        pkill client
    ;;
    restart)
    
    ;;
    status)

    ;;
    *)

    ;;
esac
