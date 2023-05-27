#include <stdio.h>
#include <time.h>

int main()
{
	int i, itr;
	struct timespec start_time, stop_time, sleep_time, res;
	double tmp, elapsed_time[10];
	
	clock_getres(CLOCK_REALTIME, &res); //fill res with clock period resolution in seconds and nanoseconds
	printf("The clock resolution of CLOCK %d is %d sec and %d nsec\n", CLOCK_REALTIME, res.tv_sec, res.tv_nsec);
	
	//make sleep time interval 20 milliseconds
	sleep_time.tv_sec = 0; //seconds is set to 0 because time interval is less than a second
	sleep_time.tv_nsec = 20000000; //2*10^7 nsec = 2*10^7 * 10^-9 = 2*10^-2 = 20*10^-3 = 20 msec
	printf("Sleep time requested is %d sec  %d nsec\n", sleep_time.tv_sec, sleep_time.tv_nsec);
	
	itr = 10; //do 10 trials
	for(i = 0; i < itr; i++)
	{
		clock_gettime(CLOCK_REALTIME, &start_time); //get time from computer's real time clock
		nanosleep(&sleep_time, NULL); //sleep for 20 msec
		clock_gettime(CLOCK_REALTIME, &stop_time); //get time from computer's real time clock
		
		//calculate elapsed time in nsec
		tmp = (stop_time.tv_sec-start_time.tv_sec)*1e9;
		tmp += stop_time.tv_nsec-start_time.tv_nsec;
		elapsed_time[i] = tmp; //store elapsed time of this trial in array
	}
	
	for(i = 0; i < itr; i++)
	{
		printf("Iteration %2d ... slept for %lf nsec\n", i, elapsed_time[i]);
	}
	
	return 0;
}
