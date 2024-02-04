#! /bin/bash
# builder/scripts/cc.sh
# Copyright (C) 2023  Alex Zebra


#    ,----------------,
#    | BUILDER EXTEND |
#    '----------------'



#    COMPILATION COMMANDS

function cpp_compilation {
    ../builder/scripts/cc.sh $BLOCK_NAME $1 $2
}

function fasm_compilation {
    ../builder/scripts/fasmc.sh $BLOCK_NAME $1 $2
}



#    STATUS

# status format
# <block_name>:<failed/complete>

STATUS_FILE="../build/status"
function remove_status_file {
    rm -f $STATUS_FILE
}

function status_fail {
    echo -e "$(cat $STATUS_FILE 2> /dev/null)$BLOCK_NAME:failed\n" > $STATUS_FILE
}
function status_complete {
    echo -e "$(cat $STATUS_FILE 2> /dev/null)$BLOCK_NAME:complete\n" > $STATUS_FILE
}



#    LOGS
RESET="\033[0m"
BOLD="\033[1m"

REEED="\033[31m"
GREEN="\033[32m"
WHITE="\033[97m"
BLUEE="\033[36m"

CLEAR="\r$(printf '%.s ' {1..$(tput cols)})\r"

# statuses
DONE_STAT="$BOLD$WHITE[$GREEN DONE $WHITE]$RESET"
FAIL_STAT="$BOLD$WHITE[$REEED FAIL $WHITE]$RESET"
INFO_STAT="$BOLD$WHITE[$WHITE INFO $WHITE]$RESET"
LOAD_STAT="$BOLD$WHITE[$WHITE  **  $WHITE]$RESET"

function check_status {
    grep $BLOCK_NAME $STATUS_FILE | cut -d ':' -f 2
}

function log_block_start {
    printf "$INFO_STAT $BLOCK_NAME: Start.\n"
}

function log_block_done {
    printf "$DONE_STAT $BLOCK_NAME: Done.\n\n"
}

function log_block_fail {
    printf "$FAIL_STAT $BLOCK_NAME: Fail.\n\n"
}

function log_block_end {
    if [[ "$(check_status)" == "complete" ]]
    then
        log_block_done
    else
        log_block_fail
        exit 1
    fi
}