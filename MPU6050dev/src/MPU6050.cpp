//#include <iostream>

#include "MPU6050.h"
#include "mraa.hpp"
#include <sys/timeb.h>
//#include thread



void MPU6050 :: MPU6050_read(int start,unsigned char *buff, int size){
		mraa::I2c* i2c;
		i2c = new mraa::I2c(6);
	 	//i2c->frequency(10);
		i2c->address(MPU6050_I2C_ADDRESS);
		i2c->write(start);
		char bufferx[20] = "Hello";
		i2c->read(bufferx,size);

		for(int i =0;i<size;i++)
			buff[i] = bufferx[i];

		return ;
}
int MPU6050 :: MPU6050_write_reg(int reg, int data){
		mraa::I2c* i2c;
		i2c = new mraa::I2c(6);
		i2c->address(MPU6050_I2C_ADDRESS);
		i2c->writeReg(reg,data);
		return 0;
}
int MPU6050 :: initialize(){
		unsigned char c;
		MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
		id = c;
		MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0x00);
		MPU6050_read (MPU6050_PWR_MGMT_1, &c, 1);
		sleep(1);
		status = 1;
		return 1;
}

void MPU6050 :: refresh(){
		old_readings = readings;
		last_time_measure = time_measure;

		this->time_measure = getMilliCount();
		MPU6050_read (MPU6050_ACCEL_XOUT_H, (unsigned char *) &buffer, sizeof(buffer));

		readings.x_accel = mergeBytes(buffer[0],buffer[1]);
		readings.y_accel = mergeBytes(buffer[2],buffer[3]);
		readings.z_accel = mergeBytes(buffer[4],buffer[5]);
		readings.temperature = mergeBytes(buffer[6],buffer[7]);
		readings.x_gyro = mergeBytes(buffer[8],buffer[9]);
		readings.y_gyro = mergeBytes(buffer[10],buffer[11]);
		readings.z_gyro = mergeBytes(buffer[12],buffer[13]);
//		      printf("New Values:--X:%i Y:%i Z:%i ----- \n",
//		    		  readings.x_gyro,
//		    		  readings.y_gyro,
//		    		  readings.z_gyro);
//		      printf("Last Values:--X:%i Y:%i Z:%i ----- \n",
//		    		  	  last_accel_t_gyro.value.x_gyro,
//		    		  	  last_accel_t_gyro.value.y_gyro,
//		    		  old_readings.x_gyro,
//		    		  old_readings.y_gyro,
//		    		  old_readings.z_gyro);
//		      printf("Current time: %i, Last time: %i \n",this->time_measure,this->last_time_measure);
//		      printf("Old   Values: \n--X:%i Y:%i Z:%i \n",accel_t_gyro.value.x_accel,accel_t_gyro.value.y_accel,accel_t_gyro.value.z_accel);

      return ;
}

unsigned char MPU6050 :: MPU6050_readRegistry(int reg){
		unsigned char byte;
		MPU6050_read (reg, (unsigned char *) &byte, 1);
		return byte;
}

short MPU6050 :: getXGyro() {
	return readings.x_gyro / FS_SEL;
}
short MPU6050 :: getYGyro() {
	return readings.y_gyro / FS_SEL;
}

unsigned char MPU6050 :: readAccelerationConfiguration(){
	return MPU6050_readRegistry(MPU6050_ACCEL_CONFIG);
}
void MPU6050 :: startAccelerationSelfTest(){
	MPU6050_write_reg (MPU6050_ACCEL_CONFIG, 224);
}

unsigned char MPU6050 :: readGyroConfig(){
	return MPU6050_readRegistry(MPU6050_GYRO_CONFIG);
}

void MPU6050 :: startGyroSelfTest(){
	MPU6050_write_reg (MPU6050_GYRO_CONFIG, 224);
}

unsigned char MPU6050 ::  readInterruptEnable(){
	return MPU6050_readRegistry(MPU6050_INT_ENABLE);
}

void MPU6050 :: setInterruptEnable(unsigned char status){
	MPU6050_write_reg (MPU6050_INT_ENABLE, status);
}

unsigned char MPU6050 :: readInterruptStatus(){
	return MPU6050_readRegistry(MPU6050_INT_STATUS);
}

unsigned char MPU6050 :: readPowerManagement1(){
		return MPU6050_readRegistry(MPU6050_PWR_MGMT_1);
}

void MPU6050 :: setPowerManagement1(unsigned char status){
		MPU6050_write_reg (MPU6050_PWR_MGMT_1, status);
}

unsigned char MPU6050 ::  readPowerManagement2(){
		return MPU6050_readRegistry(MPU6050_PWR_MGMT_2);
}

void MPU6050 ::  setPowerManagement2(unsigned char status){
			MPU6050_write_reg (MPU6050_PWR_MGMT_2, status);
}

int MPU6050 :: getMilliCount(){
			timeb tb;
			ftime(&tb);
			int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
			return nCount;
}
int MPU6050 :: getMilliSpan(int nTimeStart){
		int nSpan = getMilliCount() - nTimeStart;
		if(nSpan < 0)
			nSpan += 0x100000 * 1000;
		return nSpan;
}

short MPU6050 :: mergeBytes(unsigned char high,unsigned char low){
		return ((high<<8) | (low & 0xFF));
}


