/*get system info*/

#include <sys/time.h>
#include <stdio.h>
#include <time.h>		// function ctime d'st exist in sys/time.h while in time.h
#include <string.h>


void get_time();	// get system time and print
void get_cpu(FILE *procFile, char *lineBuf, int LB_SIZE);	// get cpu info and print
void get_kernel(FILE *procFile, char *lineBuf, int LB_SIZE);	// get kernel info and print


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
			printf("%s", temp);
		else
			printf("%s", temp);
		temp = strtok(NULL, " ");
	}

	fclose(procFile);
}