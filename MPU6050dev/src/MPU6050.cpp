	#include <unistd.h>
	#include <signal.h>
	#include <stdint.h>
	#include "MPU6050.h"
	#include <stdio.h>

    int running = 0;



    int main(){
    	MPU6050 gyro;
    	gyro.init();
    	usleep(80000);
    	//gyro.calibrate();
    	printf("Gyro Config: %i \n",gyro.readGyroConfig());
    	printf("Accel Config: %i \n",gyro.readAccelerationConfiguration());
    	gyro.readMeasurements();
    	//gyro.startGyroSelfTest();
    	//gyro.startAccelerationSelfTest();
    	//sleep(1);
    	gyro.setInterruptEnable(1);
    	printf("Int Enable: %i \n",gyro.readInterruptEnable());
    	printf("Int Status: %i \n",gyro.readInterruptStatus());
    	printf("Power Management 1: %i \n",gyro.readPowerManagement1());
    	printf("Power Management 2: %i \n",gyro.readPowerManagement2());
    	gyro.readMeasurements();
    	printf("------------------------new status----------------------\n");

    		FILE * pFile;
    		pFile = fopen ("MPU6050.txt","w");
    		if (pFile!=NULL){
    			printf("File is open.");
    			double temp_gyrox = 0,temp_gyroy = 0;

    		for(int i = 0; i<500;){
    			usleep(100000);
    			if(gyro.readInterruptStatus()>=1){
    				gyro.readMeasurements();
    				double newgyroX = gyro.getXGyro();
    				newgyroX = newgyroX/131;

    				if (temp_gyrox != newgyroX){
    					//printf("Different values: %f,%f\n",temp_gyrox, newgyroX);
    				}

    				temp_gyrox = (double) gyro.getXGyro()/131;
    				temp_gyroy = (double) gyro.getYGyro()/131;

    				fprintf(pFile,"X,%f,Y,%f\n",temp_gyrox,temp_gyroy);
    				if(gyro.getXGyro() == 0 && gyro.getYGyro() == 0){
    					gyro.init();
    				    printf("Gyro is acting weird\n");
    				}
    				i++;
    			}
    		}
      		fclose(pFile);
      		printf("Closing Files and exiting");
    		}else{
    			printf("File is not open");
    		}
    }

