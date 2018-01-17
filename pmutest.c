#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define SLEEP_USECS	(100*1000)

unsigned long long int totalcycle = 0;
unsigned int precycle = 0;
unsigned int cycles = 0;

/*void init_pmu(){
	asm volatile("mcr p15, 0, %0, c9, c12, 0" : : "r"(0x00000007));
	asm volatile("mcr p15, 0, %0, c9, c12, 1" : : "r"(0x8000000f));
}*/

uint32_t read_cycle(){
	uint32_t cycle_count;

	asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(cycle_count));

	return cycle_count;
}

int main(){

	//init_pmu();

	unsigned int temp = 0;
	int child, status, pid;

	pid = vfork();

	if(pid == 0){
		execl("./sleep", "./sleep", NULL);
		exit(1);
	}else{

		while(1){
			/*if(child == wait(&status)){
				break;
			}*/
			usleep(SLEEP_USECS);
	
			cycles = read_cycle();
		
			if(precycle > cycles){
				temp = 0xFFFFFFFF - precycle;
				temp = temp + cycles;
				totalcycle += temp;
			}else{
				temp = cycles - precycle;
				totalcycle += temp;
			}

			precycle = cycles;

			printf("totalcycle : %llu\b", totalcycle);

		}
	}

	return 0;

}
