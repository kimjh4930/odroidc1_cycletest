cmd_/home/odroid/usercycle/monitor.ko := ld -EL -r  -T /home/odroid/kernel_cycle_timer/scripts/module-common.lds --build-id  -o /home/odroid/usercycle/monitor.ko /home/odroid/usercycle/monitor.o /home/odroid/usercycle/monitor.mod.o