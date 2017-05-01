#!/usr/bin/env bash

#"sda1"
install_part=${1}
#"sda"
install_disk=$(echo "$install_part" | head -c 3)

user_name=${2}
user_pass=${3}
root_pass=${4}

#"en_US.UTF-8"
lang=${5}
#"fr"
keymap=${6}
hostname=${7}

###########################################################
echo
echo

echo "hello esi-linux "
echo

echo "install_part: $install_part"
echo "install_disk: $install_disk "
sleep 5s
echo "user_name: $user_name"
echo "user_pass: $user_pass"
echo "root_pass: $root_pass"
echo "lang: $lang"
echo "keymap: $keymap"
echo "hostname $hostname"
