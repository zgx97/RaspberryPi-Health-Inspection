#!/bin/bash

_show_msg=$show_msg
_check_check=$init_check

$_check_check
if [[ $? -eq 1 ]]; then
    exit 1
fi

cp ../log/*.log ../log/old.log/
