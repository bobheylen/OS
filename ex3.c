#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_FREQ 1

void readSensor(){
	float f= rand() % 45 - 10; //Generates number between -10 and 35
	time_t t = time(&t);
	printf("Temperature = %1.2f @ %s\n", f, ctime(&t));
	fflush(stdout);
}

int main(){
	//for(int i=0; i<10; i++)
	while(1)
	{
		readSensor();
		sleep(MAX_FREQ);
	}
	return 0;
}
