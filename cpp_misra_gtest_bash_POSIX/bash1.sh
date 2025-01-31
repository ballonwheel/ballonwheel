







#du -sh .
#du -sh /home
#df -h
#Also useful is sorting by size:
#du -sh /* | sort -h


#uname -a
#cat /proc/version : Show Linux kernel version with help of a special file
#hostnamectl | grep Kernel : For systemd based Linux distro you can use hotnamectl to display hostname and running Linux kernel version



#!/bin/bash

# Check if a number is positive, negative, or zero
#number=-5

#if [ $number -gt 0 ]; then
#    echo "$number is positive"
# elif [ $number -lt 0 ]; then
#    echo "$number is negative"
# else
#    echo "$number is zero"
#fi



imre@dob01:~$ rsync -av /path/to/source_directory/ /path/to/destination_directory/


imre@dob01:~$ sudo dd if=/dev/sdb of=/dev/sdc status=progress


imre@dob01:~$ sudo testdisk /dev/sdc1

https://www.cs.montana.edu/courses/309/topics/4-disks/debugfs_example.html
> debugfs /dev/hda6

imre@dob01:~$ sudo ddrescue -d -v /dev/sdc1 /media/imre/KINGSTON/test.img /media/imre/KINGSTON/test.logfile


2024aprilis, usb pendrive halott lett, csak olvashatokent
atcopyztam dd-vel egy masikra es ott:
ez segitett:
imre@dob01:~$ sudo dosfsck -w -r -l -a -v -t /dev/sdb1

meg kene nezni, hogy az eredeti formazhato e vagy total read only lett?



szoveget keresni konyvtar fajlokban
imre@dob01:~$ find /media/imre/CA41-3130/ -type f -exec grep -H "win laptop" {} +




# firefox update
sudo apt-get purge firefox-esr
sudo apt-get install firefox-esr
#a default browser elallitodik




