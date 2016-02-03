#include "MPU6050.h"
#include <mraa.hpp>

MPU6050 :: MPU6050(){
	this->time_measure = 0;
	this->last_time_measure = 0;
	this->id = 0;
	this->status = 0;
	this->samplerate = 8; //measurement in kHz
	clearMPU6050Reading(calibrations);
	samplesize = 200;
	clearbuffer();

}

void MPU6050 :: clearbuffer(){
	for (size_t i = 0;i<14;i++)
		buffer[i] = 0;
}
void MPU6050 :: MPU6050_read(int start, char buff[], int size){
		mraa::I2c* i2c;
		i2c = new mraa::I2c(6);
	 	//mraa_result_t result = i2c->frequency(250); //for future use
	 	//if(result == MRAA_ERROR_FEATURE_NOT_SUPPORTED)
	 	//std::cout<<"feature not supported by MRAA";
		i2c->address(MPU6050_I2C_ADDRESS);
		i2c->writeByte(start);
		i2c->read((uint8_t *)buff,size);
		i2c->~I2c();
		//delete[] i2c;
		return;
}

int MPU6050 :: MPU6050_write_reg(int reg, int data){
		mraa::I2c* i2c;
		i2c = new mraa::I2c(6);

		i2c->address(MPU6050_I2C_ADDRESS);
		i2c->writeReg(reg,data);
		i2c->~I2c();

		return 0;
}

int MPU6050 :: initialize(){

		char c[2] = {0,0};

		MPU6050_read (MPU6050_WHO_AM_I, c, 1); //look out for passing the address of char c
		id = c[0];

		if(id == MPU6050_I2C_ADDRESS) //look for MPU6050 onboard
			status = 1;
		else
			return status;

		MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0x00);
		MPU6050_read (MPU6050_PWR_MGMT_1, c, 1);
		if(this->readConfig()!= 0)
			this->samplerate = 1; //our sample rate changes to 1 regardless

		usleep(100000);
		return status;
}
void MPU6050 :: calibrate(){
	clearMPU6050Reading(calibrations);
	for (int i = 0;i<this->samplesize;i++){
		this->refresh();
		calibrations.x_accel += readings.x_accel;
		calibrations.y_accel += readings.y_accel;
		calibrations.z_accel += readings.z_accel;
		calibrations.temperature +=readings.temperature;
		calibrations.x_gyro += readings.x_gyro;
		calibrations.y_gyro += readings.y_gyro;
		calibrations.z_gyro += readings.z_gyro;
		usleep(samplerate*1000);
	}


	calibrations.x_accel = calibrations.x_accel/samplesize;
	calibrations.y_accel = calibrations.y_accel/samplesize;
	calibrations.z_accel = calibrations.z_accel/samplesize;

	calibrations.temperature = calibrations.temperature/samplesize;

	calibrations.x_gyro = calibrations.x_gyro/samplesize;
	calibrations.y_gyro = calibrations.y_gyro/samplesize;
	calibrations.z_gyro = calibrations.z_gyro/samplesize;

}

int MPU6050:: getId(){
	return id;
}
void MPU6050 :: refresh(){
		old_readings = readings;
		last_time_measure = time_measure;

		this->time_measure = getMilliCount();

		MPU6050_read (MPU6050_ACCEL_XOUT_H,  buffer, sizeof(buffer));

		readings.x_accel = mergeBytes(buffer[0],buffer[1]);

		readings.y_accel = mergeBytes(buffer[2],buffer[3]);
		readings.z_accel = mergeBytes(buffer[4],buffer[5]);
		readings.temperature = mergeBytes(buffer[6],buffer[7]);
		readings.x_gyro = mergeBytes(buffer[8],buffer[9]);
		readings.y_gyro = mergeBytes(buffer[10],buffer[11]);
		readings.z_gyro = mergeBytes(buffer[12],buffer[13]);

//

      return ;
}

unsigned char MPU6050 :: MPU6050_readRegistry(int reg){
		char byte;
		MPU6050_read (reg, &byte, 1);
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

unsigned char MPU6050::readConfig()  {
	return MPU6050_readRegistry(MPU6050_PWR_MGMT_2);
}

void MPU6050::setConfig(unsigned char status) {
	MPU6050_write_reg (MPU6050_CONFIG, status);
}

void MPU6050::setGyroConfig(unsigned char status) {
	MPU6050_write_reg (MPU6050_GYRO_CONFIG, status);
}

void MPU6050::setAccelerationConfiguration(unsigned char status) {
	MPU6050_write_reg (MPU6050_ACCEL_CONFIG, status);
}

MPU6050Reading MPU6050::getCalibratedReading() {
	MPU6050Reading adjusted;
	adjusted.x_accel = adjustInt(this->readings.x_accel,this->calibrations.x_accel);
	adjusted.y_accel = adjustInt(this->readings.y_accel,this->calibrations.y_accel);
	adjusted.z_accel = adjustInt(this->readings.z_accel,this->calibrations.z_accel);
	adjusted.temperature = this->readings.temperature;

	adjusted.x_gyro = adjustInt(this->readings.x_gyro,this->calibrations.x_gyro);
	adjusted.y_gyro = adjustInt(this->readings.y_gyro,this->calibrations.y_gyro);
	adjusted.z_gyro = adjustInt(this->readings.z_gyro,this->calibrations.z_gyro);

	return adjusted;

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

char MPU6050 :: mergeBytes(unsigned char high,unsigned char low){
	return ((high&15) | low);
}

short MPU6050 :: getTemperature(){
			return this->readings.temperature;
}

short MPU6050 :: getXAccel() {
			return this->readings.x_accel;
}
short MPU6050 :: getYAccel() {
			return this->readings.y_accel;
}
short MPU6050 :: getZAccel() {
			return this->readings.z_accel;
}

std::ostream& operator<<(std::ostream& out,const MPU6050Reading& data){
	out<<"x_accel: "<<data.x_accel<<" ";
	out<<"y_accel: "<<data.y_accel<<" ";
	out<<"z_accel: "<<data.z_accel<<" ";
	out<<"temperature: " << (data.temperature/340.00)+36.53 << " ";

	out<<"x_gyro: "<<data.x_gyro<<" ";
	out<<"y_gyro: "<<data.y_gyro<<" ";
	out<<"z_gyro: "<<data.z_gyro<<" ";

	return out;
}

void clearMPU6050Reading(MPU6050Reading& data) {
	data.x_accel = 0;
	data.y_accel = 0;
	data.z_accel = 0;

	data.temperature = 0;

	data.x_gyro = 0;
	data.y_gyro = 0;
	data.z_gyro = 0;

}
int adjustInt(int number, int adjustment){
	int adjusted = 0;

	//if(adjustment>=0){
//		std::cout<<"adjustment is positive so we are substracting "<<adjustment<<std::endl;
		adjusted = number-adjustment;
	//}else{
		//std::cout<<"adjustment is negate so we are adding "<<(unsigned int)adjustment<<std::endl;
		//adjusted = number - adjustment;
	//}
//	std::cout<<"returning adjusted number: "<<adjusted<<std::endl;
	return adjusted;
}
