cmd_/home/odroid/usercycle/cpucycle.ko := ld -EL -r  -T /home/odroid/kernel_cycle_timer/scripts/module-common.lds --build-id  -o /home/odroid/usercycle/cpucycle.ko /home/odroid/usercycle/cpucycle.o /home/odroid/usercycle/cpucycle.mod.o