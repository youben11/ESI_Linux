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
POST_INSTALL='post-install.sh'
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

# making ext4 file system
mkfs.ext4 -F $INSTALL_PART

# mounting INSTALL_PART to the /mnt mount point
mount $INSTALL_PART /mnt

# copying files to the new root
rsync -aAXvP /* /mnt --exclude={/dev/*,/proc/*,/sys/*,/tmp/*,/run/*,/mnt/*,/media/*,/lost+found,/.gvfs}
Send2Daddy "1#"

# copying files to the new root
cp -avT /run/archiso/bootmnt/arch/boot/$(uname -m)/vmlinuz /mnt/boot/vmlinuz-linux-lts
Send2Daddy "2#"

# changing storage volatility
sed -i 's/Storage=volatile/#Storage=auto/' /mnt/etc/systemd/journald.conf

# deleting old configuration files
rm -r /mnt/etc/systemd/system/{choose-mirror.service,pacman-init.service,etc-pacman.d-gnupg.mount,getty@tty1.service.d}
rm /mnt/etc/systemd/scripts/choose-mirror

# changing the permissions of root home directory
chmod 700 /mnt/root

# mounting efi partition if exist
if test -d '/sys/firmware/efi/efivars'
then
	EFI_PART=$INSTALL_DISK$(parted /dev/sda print | grep EFI | tr -d ' ' | head -c 1)
	mkdir /mnt/boot/efi
	mount $EFI_PART /mnt/boot/efi
fi

# turn on the swap partition if exist
SWAP_PART=$INSTALL_DISK$(parted /dev/sda print | grep swap | tr -d ' ' | head -c 1)
if [ "$SWAP_PART" != "$INSTALL_DISK" ]
then
	swapon $SWAP_PART
fi

# we need to mount efi and swap partitions before genfstab
genfstab -U /mnt > /mnt/etc/fstab
Send2Daddy "3#"

# copy the post-install script and run it inside the new root
cp $POST_INSTALL /mnt/
arch-chroot /mnt $POST_INSTALL $@

# deleting the post-install script
rm "/mnt/$POST_INSTALL"

# unmount all the the partition recursivly
umount -R /mnt

############################################################
