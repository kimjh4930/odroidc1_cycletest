#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm-generic/uaccess.h>

#include <linux/slab.h>
#include <linux/pid.h>

#include <linux/sched.h>

#include <asm/string.h>

static char msg[1000];
static int len = 0;
static int len_check = 1;

static u64 get_cycle64(void);
u64 start_cycle=0;
u64 end_cycle=0;
int p_id;

static struct proc_dir_entry *proc_write_entry = NULL;
char *proc_name = "taskpid";


int simple_proc_open(struct inode *inode, struct file *file){

	printk(KERN_INFO"proc called open\n");

	return 0;
}

int simple_proc_release(struct inode *inode, struct file *file){

	end_cycle = get_cycle64();
	printk(KERN_INFO"end cycle : %llu\n", end_cycle);
	printk(KERN_INFO"elapse cycle : %llu\n", end_cycle - start_cycle);
	printk(KERN_INFO"proc called release\n");

	start_cycle = 0;
	end_cycle = 0;

	return 0;
}

int simple_proc_read(struct file *file, char __user *buf, size_t size, loff_t *offset){

	printk(KERN_INFO"proc called read\n");

	if(len_check){
		len_check = 0;
	}else{
		len_check = 1;
		return 0;
	}

	copy_to_user(buf, msg, len);

	return len;
}

int simple_proc_write(struct file *file, const char __user *buf, size_t size, loff_t *offset){

	int temp_pid = 0;

	printk(KERN_INFO"proc called write\n");

	len = size;

	copy_from_user(msg, buf, len);

	start_cycle = get_cycle64();
	printk(KERN_INFO"start cycle : %lld\n", start_cycle);
	temp_pid = simple_strtoul(msg, NULL, 0);

	if(temp_pid != 0){
		p_id = temp_pid;
	}

	printk(KERN_INFO"[statistics] p_id : %lu\n", p_id);
	
	return len;
}

static int get_PID(void){
	return p_id;
}
static void set_PID(int value){
	p_id = value;
}


struct file_operations fops ={
	.open = simple_proc_open,
	.read = simple_proc_read,
	.write = simple_proc_write,
	.release = simple_proc_release
};

static int __init init_simpleproc(void){
	printk(KERN_INFO"init simple proc\n");
	if(!proc_create("simpleproc", 0666, NULL, &fops)){
		printk(KERN_INFO"ERROR! proc_create\n");
		remove_proc_entry("simpleproc", NULL);
		return -1;
	}
	return 0;
}

static void __exit exit_simpleproc(void){
	remove_proc_entry("simpleproc", NULL);
	printk(KERN_INFO"exit simple proc\n");
}

EXPORT_SYMBOL( get_PID );
EXPORT_SYMBOL( set_PID );

module_init(init_simpleproc);
module_exit(exit_simpleproc);
MODULE_LICENSE("GPL");
