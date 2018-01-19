#set -x
echo Please run this script as a super user!!!
ls -al /proc/mymem*
echo mymemdev is not shown under /proc
insmod mymemdev.ko
ls -al /proc/mymemdev
echo Now you see the /proc/mymemdev is created after the module is inserted!!!
set -x
cat /proc/mymemdev
rmmod mymemdev.ko

