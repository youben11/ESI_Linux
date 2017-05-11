#!/bin/bash

##########################################################
# "/dev/sda1"
INSTALL_PART=${1}
# "/dev/sda"
INSTALL_DISK=$(echo "$INSTALL_PART" | head -c 8)

USER_NAME=${2}
USER_PASS=${3}
ROOT_PASS=${4}

# "en_US.UTF-8"
LANG=${5}
# "fr"
KEYMAP=${6}
HOSTNAME=${7}
# /Region/City ex: Africa/Algiers
TIMEZONE=${8}
# 'yes' or 'no'
AUTOLOGIN=${9}
##########################################################
LOG_STDOUT='install_log_stdout.log'
LOG_STDERR='install_log_stderr.log'
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
# check the exit status of the last command executed
CheckIt () {
	if [ $? != 0 ]
	then
		Send2Daddy "error: $1"
		exit 1
	fi
}
##########################################################
# checking the args number
if [ $# != 9 ]
then
	Send2Daddy "error: check the args"
	exit 1
fi
##########################################################

#updating the KEYMAP
echo "KEYMAP=$KEYMAP" > /etc/vconsole.conf
CheckIt "changing the KEYMAP to $KEYMAP"

#update the LANGuage
echo "LANG=$LANG" > /etc/locale.conf
CheckIt "changing the language to $LANG"

#updating the HOSTNAME
echo "$HOSTNAME" > /etc/hostname
CheckIt "updating the hostname"

#TIMEZONE
ln -sf /usr/share/zoneinfo/$TIMEZONE /etc/localtime
CheckIt "configure the timezone to $TIMEZONE"

# AUTOLOGIN
if [ "$AUTOLOGIN" == "yes" ]
then
	sed -i "s/etudiant/$USER_NAME/" /etc/gdm/custom.conf
else
	# disable AUTOLOGIN
	sed -i "s/.*AutomaticLogin=.*/#AutomaticLogin=$USER_NAME/" /etc/gdm/custom.conf
	sed -i 's/.*AutomaticLoginEnable=.*/#AutomaticLoginEnable=True/' /etc/gdm/custom.conf
fi

#updating the USER_NAME login and home folder
usermod -l "$USER_NAME" -d /home/$USER_NAME -m "etudiant"
CheckIt "changing user login and home folder"
#updating the group_name
groupmod -n "$USER_NAME" "users"
CheckIt "updating the group name"
#replacing the USER_NAME "etudiant" with the new user_name in the sudoers file
sed -i "s/etudiant/$USER_NAME/" /etc/sudoers
CheckIt "adding user to sudoers"

#updating the passwords
echo -e "$USER_NAME:$USER_PASS\nroot:$ROOT_PASS" | chpasswd -c SHA512
CheckIt "changing the passwords"
Send2Daddy "4#"

#creat the initial ramdisk env
mkinitcpio -p linux-lts
CheckIt "creating the init ramdisk env"
Send2Daddy "5#"

#installing grub
if test -d '/sys/firmware/efi/efivars'
then
	grub-install $INSTALL_DISK --target=x86_64-efi --recheck --no-floppy
else
	grub-install $INSTALL_DISK --target=i386-pc --recheck -no-floppy
fi
CheckIt "installing the bootloader grub"

grub-mkconfig -o /boot/grub/grub.cfg
CheckIt "generate grub configuration"
Send2Daddy "6#"
