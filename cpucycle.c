#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/spinlock.h>

static int keres_init(void);
static void keres_exit(void);
static void init_pmu(void);

static void cycle_main(unsigned long data);

unsigned long delay;

static u32 read_cycles();

static u64 get_cycle64(void);

static u64 totalcycles = 0;
static u32 current_cycle = 0;
static u64 prev_cycle = 0;

DEFINE_TIMER(mytimer, cycle_main, 0, 0);

/* approach armv7 registers */
static void init_pmu(){

	/* Enable counter in Control Register and reset cycle count and event count */
	asm volatile("mcr   p15, 0, %0, c9, c12, 0" : : "r"(0x00000007));
	//asm volatile("mcr   p15, 0, %0, c9, c12, 0" : : "r"(0x0000000F));

        /* Event counter selection register, which counter to access */
        //asm volatile("mcr   p15, 0, %0, c9, c12, 5" : : "r"(0x0));

        /* selected event type to record, instructions executed */
        //asm volatile("mcr   p15, 0, %0, c9, c13, 1" : : "r"(0x00000008));

        /* count enable set register, bit 31 enables the cycle counter, 
           and bit 0 enables the first counter */
        asm volatile("mcr   p15, 0, %0, c9, c12, 1" : : "r"(0x8000000f));
}

static u32 read_cycles(){
	
	unsigned long cycle_count;

	asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(cycle_count));

	return cycle_count;
}

static u64 get_cycle64(){
	return totalcycles;
}

static void cycle_main(unsigned long data){
	unsigned int temp = 0;

	current_cycle = read_cycles();

	if(prev_cycle > current_cycle){
		temp = 0xFFFFFFFF - prev_cycle;
		temp += current_cycle;
		totalcycles += temp;
	}else{
		temp = current_cycle - prev_cycle;
		totalcycles += temp;
	}

	prev_cycle = current_cycle;

	mod_timer(&mytimer, jiffies + delay);
}

static int __init keres_init(void){

	init_pmu();

	delay = 1;

	mod_timer(&mytimer, jiffies + delay);

	printk("init_pmu()\n");

	return 0;
}

static void __exit keres_exit(void){

	del_timer(&mytimer);

	printk("keres exit\n");

}

EXPORT_SYMBOL(get_cycle64);

module_init(keres_init);
module_exit(keres_exit);

MODULE_LICENSE("Dual BSD/GPL");
