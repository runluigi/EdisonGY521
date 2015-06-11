#include <iostream>
#include "MPU6050.h"
#include <unistd.h>
//#include <sys/socket.h>


using std::cout;

int main(){
	cout << "WELCOME TO MPU6050 MARK 2"<<std::endl;

	MPU6050 gyro;

	if( gyro.initialize() == 1)
	    		cout <<"INITIALIZED ID:"<< (int) gyro.getId() << std::endl;
	cout<<"SAMPLE RATE IS:"<<gyro.getSamplerate()<<" kHz"<<std::endl;

	gyro.setGyroConfig(24);//set the sensitivity for gyroscope

	cout<<"GYRO CONFIG:"<<(int)gyro.readGyroConfig()<< std::endl;
	gyro.calibrate();

	//cout<<"Calibration values:"<<gyro.getCalibrations()<<std::endl;
	//gyro.refresh();
	//cout<<"Current reading:"<<gyro.getReadings()<<std::endl;

	for(int i = 0;i<500;i++){//polling values every 10ms
		gyro.refresh();
		usleep(10000);
	}

	cout<<"Raw values:"<<gyro.getReadings()<<std::endl;
	cout<<"Calibrrated values:"<<gyro.getCalibratedReading()<<std::endl;
	gyro.refresh();

	cout<<"Raw values:"<<gyro.getReadings()<<std::endl;
	cout<<"Calibrrated values:"<<gyro.getCalibratedReading()<<std::endl;
	gyro.refresh();

	cout<<"Raw values:"<<gyro.getReadings()<<std::endl;
	cout<<"Calibrrated values:"<<gyro.getCalibratedReading()<<std::endl;
	gyro.refresh();
}
