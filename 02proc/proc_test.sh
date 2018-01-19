#set -x
echo Please run this script as a super user!!!
ls -al /proc/mymem*
echo mymemdev is not shown under /proc
insmod mymemdev.ko
ls -al /proc/mymemdev
echo Now you see the /proc/mymemdev is created after the module is inserted!!!
set -x
echo 123 > /proc/mymemdev
echo tteesstt > /proc/mymemdev
cat /proc/mymemdev
echo aaa > /proc/mymemdev
cat /proc/mymemdev
rmmod mymemdev.ko

