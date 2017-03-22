 #!/bin/bash

set -e -u

sed -i 's/#\(en_US\.UTF-8\)/\1/' /etc/locale.gen
sed -i 's/#\(fr_FR\.UTF-8\)/\1/' /etc/locale.gen
sed -i 's/#\(ar_DZ\.UTF-8\)/\1/' /etc/locale.gen
locale-gen

echo 'LANG="fr_FR.UTF-8"' >> /etc/locale.conf
export LANG=fr_FR.UTF-8

echo "KEYMAP=fr" >> /etc/vconsole.conf

rm /etc/localtime
ln -s /usr/share/zoneinfo/Africa/Algiers /etc/localtime

hwclock --systohc --utc

echo "ESI" > /etc/hostname

useradd -m -g users -G "adm,audio,floppy,log,network,rfkill,scanner,storage,optical,power,wheel" -s /bin/bash utilisateur

echo "utilisateur ALL=(ALL) ALL" >> /etc/sudoers

usermod -s /bin/bash root
cp -aT /etc/skel/ /root/
chmod 700 /root

sed -i 's/#\(PermitRootLogin \).\+/\1yes/' /etc/ssh/sshd_config
sed -i "s/#Server/Server/g" /etc/pacman.d/mirrorlist
sed -i 's/#\(Storage=\)auto/\1volatile/' /etc/systemd/journald.conf

sed -i 's/#\(HandleSuspendKey=\)suspend/\1ignore/' /etc/systemd/logind.conf
sed -i 's/#\(HandleHibernateKey=\)hibernate/\1ignore/' /etc/systemd/logind.conf
sed -i 's/#\(HandleLidSwitch=\)suspend/\1ignore/' /etc/systemd/logind.conf

systemctl enable pacman-init.service choose-mirror.service gdm.service NetworkManager.service
systemctl set-default graphical.target
