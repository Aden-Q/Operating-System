/*get system info*/

#include <sys/time.h>
#include <stdio.h>
#include <time.h>		// function ctime d'st exist in sys/time.h while in time.h
#include <string.h>


void get_time();	// get system time and print
void get_cpu(FILE *procFile, char *lineBuf, int LB_SIZE);	// get cpu info and print
void get_kernel(FILE *procFile, char *lineBuf, int LB_SIZE);	// get kernel info and print
void get_optime(FILE *procFile, char *lineBuf, int LB_SIZE);	// get system operates time
void get_cpu_stat(FILE *procFile, char *lineBuf, int LB_SIZE); // get cpu stat
void get_memory(FILE *procFile, char *lineBuf, int LB_SIZE);	// get memory info
void get_load(FILE *procFile, char *lineBuf, int LB_SIZE);	// get cpu average load


int main(){
	// get sys time
	get_time();

	FILE* procFile;
	char lineBuf[128];
	int LB_SIZE = sizeof(lineBuf);

	// CPU info
	get_cpu(procFile, lineBuf, LB_SIZE);

	// kernel info
	get_kernel(procFile, lineBuf, LB_SIZE);

	// system operates time
	get_optime(procFile, lineBuf, LB_SIZE);

	// get cpu stat(time in user/kernel mode)
	get_cpu_stat(procFile, lineBuf, LB_SIZE);

	// get memory info
	get_memory(procFile, lineBuf, LB_SIZE);

	// get cpu average load
	get_load(procFile, lineBuf, LB_SIZE);

	return 0;
}

// get system time
void get_time(){
	struct timeval now;
	gettimeofday(&now, NULL);
	printf("\e[32mSystem time:\n\e[0m");
	printf("Kernel Status Report at %s\n", ctime(&(now.tv_sec)));
}

// get cpu info
void get_cpu(FILE *procFile, char *lineBuf, int LB_SIZE){
	int count = 0;

	procFile = fopen("/proc/cpuinfo", "r");

	if(procFile == NULL){
		printf("open error!\n");
		return 1;
	}

	printf("\e[32mCPU info:\n\e[0m");
	
	while(fgets(lineBuf, LB_SIZE+1, procFile)){
		// processor number
		if(strstr(lineBuf, "processor") != NULL){
			printf("%s", lineBuf);
			count++;
		}
		// model info
		else if(strstr(lineBuf, "model name") != NULL){
			printf("%s", lineBuf);
			count++;
		}
		// frequency
		else if(strstr(lineBuf, "cpu MHz") != NULL){
			printf("%s", lineBuf);
			count++;
		}
		// cache
		else if(strstr(lineBuf, "cache size") != NULL){
			printf("%s", lineBuf);
			count++;
		}
		if(count == 4){
			printf("\n");
			count = 0;
		}
	}

	fclose(procFile);
}

// get kernel version
void get_kernel(FILE *procFile, char *lineBuf, int LB_SIZE){
	procFile = fopen("/proc/version", "r");

	if(procFile == NULL){
		printf("open error!\n");
		return 1;
	}

	printf("\e[32mKernel info:\n\e[0m");

	fgets(lineBuf, LB_SIZE+1, procFile);
	char *temp = strtok(lineBuf," ");
	for(int i = 0; i < 3; i++){
		if(i == 2)
			printf("%s\n\n", temp);
		else
			printf("%s ", temp);
		temp = strtok(NULL, " ");
	}

	fclose(procFile);
}

// get sys operating time
void get_optime(FILE *procFile, char *lineBuf, int LB_SIZE){
	procFile = fopen("/proc/uptime", "r");

	if(procFile == NULL){
		printf("open error!\n");
		return 1;
	}

	printf("\e[32mStart time:\n\e[0m");

	fgets(lineBuf, LB_SIZE+1, procFile);
	// extract the first element
	char *temp = strtok(lineBuf, " ");
	printf("%s seconds\n\n", temp);

	fclose(procFile);
}

// get CPU time in user/kernel mode and free time
void get_cpu_stat(FILE *procFile, char *lineBuf, int LB_SIZE){
	char *utime, *ktime;
	procFile = fopen("/proc/stat", "r");

	if(procFile == NULL){
		printf("open error!\n");
		return 1;
	}

	printf("\e[32mCPU stat:\n\e[0m");

	fgets(lineBuf, LB_SIZE+1, procFile);
	// discard the first item
	strtok(lineBuf, " ");
	utime = strtok(NULL, " ");
	// discard the third item
	strtok(NULL, " ");
	ktime = strtok(NULL, " ");

	printf("CPU time in user mode: %s jiffies\n", utime);
	printf("CPU time in kernel mode: %s jiffies\n\n", ktime);

	fclose(procFile);
}

// get memory info
void get_memory(FILE *procFile, char *lineBuf, int LB_SIZE){
	procFile = fopen("/proc/meminfo", "r");

	if(procFile == NULL){
		printf("open error!\n");
		return 1;
	}

	printf("\e[32mMemory info:\n\e[0m");

	for(int i = 0; i < 5; i++){
		fgets(lineBuf, LB_SIZE+1, procFile);
		printf("%s", lineBuf);
	}

	printf("\n");

	fclose(procFile);
}

// get loadavg info
void get_load(FILE *procFile, char *lineBuf, int LB_SIZE){
	char *load_1, *load_5, *load_15;
	procFile = fopen("/proc/loadavg", "r");

	if(procFile == NULL){
		printf("open error!\n");
		return 1;
	}

	printf("\e[32mCPU load:\n\e[0m");

	fgets(lineBuf, LB_SIZE+1, procFile);
	// average load in one minute
	load_1 = strtok(lineBuf, " ");
	// average load in five minutes
	load_5 = strtok(NULL, " ");
	// average load in fifteen minutes
	load_15 = strtok(NULL, " ");
	printf("CPU average load in one minute: %s\n", load_1);
	printf("CPU average load in five minutes: %s\n", load_5);
	printf("CPU average load in fifteen minutes: %s\n", load_15);

	fclose(procFile);
}