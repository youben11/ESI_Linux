#!/bin/bash

#"/dev/sda1"
install_part=${1}
#"/dev/sda"
install_disk=$(echo "$install_part" | head -c 3)

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

#post install  :

#updating the keymap
echo "KEYMAP=$keymap" > /etc/vconsole.conf

#update the language
echo "LANG=$lang" > /etc/locale.conf

#updating the hostname
echo "$hostname" > /etc/hostname

#timezone
ln -sf /usr/share/zoneinfo$timezone /etc/localtime

# autologin
if [ "$autologin" == "yes" ]
then
	sed -i "s/etudiant/$user_name/" /etc/gdm/custom.conf
else
	# disable autologin
	sed -i "s/.*AutomaticLogin=.*/#AutomaticLogin=$user_name/" /etc/gdm/custom.conf
	sed -i 's/.*AutomaticLoginEnable=.*/#AutomaticLoginEnable=True/' /etc/gdm/custom.conf
fi

#updating the user_name login and home folder
usermod -l "$user_name" -d /home/$user_name -m "etudiant"
#updating the group_name
groupmod -n "$user_name" "users"
#replacing the user_name "etudiant" with the new user_name in the sudoers file
sed -i "s/etudiant/$user_name/" /etc/sudoers

#updating the passwords
echo -e "$user_name:$user_pass\nroot:$root_pass" | chpasswd -c SHA512

#creat the initial ramdisk env
mkinitcpio -p linux-lts

#installing grub
if test -d '/sys/firmware/efi/efivars'
then
	grub-install $install_disk --target=x86_64-efi --recheck --no-floppy
else
	grub-install $install_disk --target=i386-pc --recheck -no-floppy
fi

# activate splash screen for grub
sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT=.*/GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"/' /etc/default/grub

sed -i 's/GRUB_DISTRIBUTOR=.*/GRUB_DISTRIBUTOR="ESI"/' /etc/default/grub

grub-mkconfig -o /boot/grub/grub.cfg
