#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <limits>
#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

using namespace std;

//g++ Synthetic.cpp -lrt -o Synthetic

static float *results;

timespec diff(timespec s, timespec e);

int main(int argc, char** argv){

	const int NUM_THREADS = 16;

	//pid_t parent = getpid();
	//pid_t pid = fork();

	//map the space for shared memory
	results = (float*)mmap(NULL, NUM_THREADS*sizeof(float), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	memset((void*)results, 0, NUM_THREADS*sizeof(float));

	//printf("PID: %d",getpid());
	
	while(1){

		pid_t parent = getpid();
		pid_t *pid = (pid_t*)malloc(NUM_THREADS*sizeof(pid));
		pid_t thispid = getpid();
		int pidIndex;
		for (int i = 0;i<NUM_THREADS;i++){
			pid[i] = fork();
			if (pid[i] == 0){
				thispid = pid[i];
				pidIndex = i;
				break;
			}
		}

		if (thispid == 0){
			timespec startT, endT;
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startT);

			//do stuff,
			int tmp = 0;
			for (int i = 0;i<std::numeric_limits<int>::max()/2;i++){
				tmp++;
			}

			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endT);

			timespec r = diff(startT,endT);

			//cout<<"ELAPSED sec: "<<r.tv_sec <<", nsec: " << r.tv_nsec << "\n";
			//cout<<r.tv_sec<<"."<<r.tv_nsec<<"\n";
			float totaltime = r.tv_sec + (r.tv_nsec/1000000000.0f);
			results[pidIndex] = totaltime;
			return 0;
		}
		else{
			int status;
			//waitpid(pid,&status, 0);
			float avg = 0;
			for (int i = 0;i<NUM_THREADS;i++){
				waitpid(pid[i],&status, 0);
				avg += results[i];
			}
			avg = avg/NUM_THREADS;
			cout<<"Avg: "<<avg<<endl;
			
			sleep(1);
		}

		free(pid);
	}

	return 0;
}

timespec diff(timespec s, timespec e){
	timespec result;
	//guyrutenberg.com
	if (e.tv_nsec < s.tv_nsec){
		//we rolled over into the next second
		result.tv_sec = e.tv_sec - s.tv_sec - 1;
		result.tv_nsec = 1000000000 + e.tv_nsec - s.tv_nsec;
	}
	else{
		result.tv_sec = e.tv_sec - s.tv_sec;
		result.tv_nsec = e.tv_nsec - s.tv_nsec;
	}
	return result;
}
