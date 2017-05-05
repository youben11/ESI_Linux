#!/bin/bash

##########################################################
# "/dev/sda1"
install_part=${1}
# "/dev/sda"
install_disk=$(echo "$install_part" | head -c 8)

user_name=${2}
user_pass=${3}
root_pass=${4}

# "en_US.UTF-8"
lang=${5}
# "fr"
keymap=${6}
hostname=${7}
# /Region/City ex: /Africa/Algiers
timezone=${8}
# 'yes' or 'no'
autologin=${9}
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

# making ext4 file system
mkfs.ext4 -F $install_part

# mounting install_part to the /mnt mount point
mount $install_part /mnt

# copying files to the new root
rsync -aAXvP /* /mnt --exclude={/dev/*,/proc/*,/sys/*,/tmp/*,/run/*,/mnt/*,/media/*,/lost+found,/.gvfs}

# copying files to the new root
cp -avT /run/archiso/bootmnt/arch/boot/$(uname -m)/vmlinuz /mnt/boot/vmlinuz-linux-lts

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
	efi_part=$install_disk$(parted /dev/sda print | grep EFI | tr -d ' ' | head -c 1)
	mkdir /mnt/boot/efi
	mount $efi_part /mnt/boot/efi
fi

# turn on the swap partition if exist
swap_part=$install_disk$(parted /dev/sda print | grep swap | tr -d ' ' | head -c 1)
if [ "$swap_part" != "$install_disk" ]
then
	swapon $swap_part
fi

# we need to mount efi and swap partitions before genfstab
genfstab -U /mnt > /mnt/etc/fstab

# copy the post-install script and run it inside the new root
cp 'post-install.sh' /mnt/
arch-chroot /mnt "/post-install.sh" ${1} ${2} ${3} ${4} ${5} ${6} ${7} ${8} ${9}

# deleting the post-install script
rm '/mnt/post-install.sh'

# unmount all the the partition recursivly
umount -R /mnt

############################################################
