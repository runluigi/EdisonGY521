	#include <unistd.h>
	#include <signal.h>
	#include <stdint.h>
	#include "MPU6050.h"

    int running = 0;



    int main(){
    	MPU6050 gyro;
    	gyro.init();
    	gyro.calibrate();

    	int lastmeasurement = 0;

    	int i = 0;
    	while(i<100){
    		gyro.readMeasurements();
    		if(lastmeasurement != gyro.getX() ){
    			lastmeasurement = gyro.getX();
    			printf("change number: %i x: %i: \n",i,lastmeasurement);
    			i++;
    		}
    		printf("no change x: %i: \n",i,gyro.getX());
    		usleep(300);
    		 //gyro.printLastReadTime();

    	}


    }

