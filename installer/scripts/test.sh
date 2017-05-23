#!/bin/bash

##########################################################
LOG_STDOUT='install_log_stdout.log'
LOG_STDERR='install_log_stderr.log'
POST_INSTALL='post-test.sh'
##########################################################
# saves stdout in FD_INSTALLER
exec {FD_INSTALLER}>&1
# stdout replaced with the LOG_STDOUT file
exec 1>${LOG_STDOUT}
# stderr replaced with the LOG_STDERR file
exec 2>${LOG_STDERR}
##########################################################
# print to stdout stream
Send2Daddy () {
	echo $* >&$FD_INSTALLER
}
##########################################################
echo "test"

sleep 1
Send2Daddy "1#"
sleep 1
Send2Daddy "2#"
sleep 1
Send2Daddy "3#"
sleep 1
scripts/post-test.sh $FD_INSTALLER
