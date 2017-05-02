#!/bin/bash

#"/dev/sda1"
install_part=${1}
#"/dev/sda"
install_disk=$(echo "$install_part" | head -c 8)

user_name=${2}
user_pass=${3}
root_pass=${4}

#"en_US.UTF-8"
lang=${5}
#"fr"
keymap=${6}

hostname=${7}

timezone=${8}
#'yes' or 'no' 
autologin=${9}


##########################################################
#echo -e '\n[#@1]'

mount $install_part /mnt

rsync -aAXvP /* /mnt --exclude={/dev/*,/proc/*,/sys/*,/tmp/*,/run/*,/mnt/*,/media/*,/lost+found,/.gvfs}

cp -avT /run/archiso/bootmnt/arch/boot/$(uname -m)/vmlinuz /mnt/boot/vmlinuz-linux-lts

sed -i 's/Storage=volatile/#Storage=auto/' /mnt/etc/systemd/journald.conf

#rm /mnt/etc/udev/rules.d/81-dhcpd.rules

#rm /mnt/root/{.automated_script.sh,.zlogin}

rm -r /mnt/etc/systemd/system/{choose-mirror.service,pacman-init.service,etc-pacman.d-gnupg.mount,getty@tty1.service.d}

rm /mnt/etc/systemd/scripts/choose-mirror

chmod 700 /mnt/root

# we need to mount efi and swap partitions before that
if test -d '/sys/firmware/efi/efivars'
then
	efi_part=$(fdisk -l | grep EFI | head -c 9)
	mkdir /mnt/boot/efi
	mount $efi_part /mnt/boot/efi
fi

swap_part=$(fdisk -l | grep swap | head -c 9)
if [ "$swap_part" != "" ]
then
	swapon $swap_part
fi

genfstab -U /mnt > /mnt/etc/fstab

cp 'post-install.sh' /mnt/

arch-chroot /mnt "/post-install.sh" ${1} ${2} ${3} ${4} ${5} ${6} ${7} ${8} ${9}

rm '/mnt/post-install.sh'
umount -R /mnt

############################################################
