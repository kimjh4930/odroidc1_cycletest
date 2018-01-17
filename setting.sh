echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo 0 > /sys/devices/system/cpu/cpu1/online
echo 0 > /sys/devices/system/cpu/cpu2/online
echo 0 > /sys/devices/system/cpu/cpu3/online

rmmod getpid
rmmod cpucycle

insmod ./cpucycle.ko
insmod ./getpid.ko
