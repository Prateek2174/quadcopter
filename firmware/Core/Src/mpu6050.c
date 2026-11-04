/*
 * mpu6050.c
 *
 *  Created on: Nov 1, 2026
 *      Author: prate
 */

#include "main.h"
#include "mpu6050.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

void mpu6050_init(void){

	//1.	Reset the device
	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_1, DEVICE_RESET);
	//2.	Wake from sleep
	i2c_clear_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_1, SLEEP);
	//3.	Set clock source
	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x01);
	//4.	Enable I2C interface
	i2c_clear_bits(MPU6050_ADDR, MPU6050_USER_CTRL, I2C_IF_DIS);
	//5.	Set sample rate
	//6.	Configure DLPF
	mpu6050_set_sample_rate(3, 4); //DLPF = Mode 3, Sample Rate = 200 Hz
	//7.	Set gyro full scale range
	i2c_update_bitfield(MPU6050_ADDR, MPU6050_GYRO_CONFIG, FS_SEL_MASK, FS_SEL(3));
	//8.	Set accel full scale range
	i2c_update_bitfield(MPU6050_ADDR, MPU6050_ACCEL_CONFIG, AFS_SEL_MASK, AFS_SEL(2));
	//9.	Disable FIFO -> Reset -> Enable, Clear FIFO_EN
	i2c_clear_bits(MPU6050_ADDR, MPU6050_USER_CTRL, FIFO_EN);
	i2c_set_bits(MPU6050_ADDR, MPU6050_USER_CTRL, FIFO_RESET);
	i2c_set_bits(MPU6050_ADDR, MPU6050_USER_CTRL, FIFO_EN);
	i2c_clear_bits(MPU6050_ADDR, MPU6050_FIFO_EN, 0xFF);
	//10.	Disable interrupts (optional)

	//11.	Verify WHO_AM_I register (optional)


}

//Power Management Functions

void mpu6050_clock_source_Xgyro(void){

	i2c_update_bitfield(MPU6050_ADDR, MPU6050_PWR_MGMT_1, CLKSEL_MASK, CLKSEL(1));
}

void mpu6050_clock_source_Ygyro(void){

	i2c_update_bitfield(MPU6050_ADDR, MPU6050_PWR_MGMT_1, CLKSEL_MASK, CLKSEL(2));
}

void mpu6050_clock_source_Zgyro(void){

	i2c_update_bitfield(MPU6050_ADDR, MPU6050_PWR_MGMT_1, CLKSEL_MASK, CLKSEL(3));
}

void mpu6050_sleep(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_1, SLEEP);
}

void mpu6050_device_reset(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_1, DEVICE_RESET);
}

void mpu6050_temp_sensor_disable(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_1, TEMP_DIS);
}

bool mpu6050_accel_only_low_power_mode(float wakeup_freq){

	bool check = mpu6050_set_wakeup_freq(wakeup_freq);
	if(!check) return false;

	//Set CYCLE
	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_1, CYCLE);

	//Disable sleep
	i2c_clear_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_1, SLEEP);

	//Set TEMP_DIS bit to 1
	mpu6050_temp_sensor_disable();

	//Set STBY_XG, STBY_YG, STBY_ZG bits to 1
	mpu6050_gyro_standby();

	return true;
}

bool mpu6050_set_wakeup_freq(float wakeup_freq){

	// LP_WAKE_CTRL   Wake-up Frequency
	//      0           1.25 Hz
	//      1              5 Hz
	//      2             20 Hz
	//      3             40 Hz

	if(wakeup_freq == 1.25){

		i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, LP_WAKE_CTRL_1_25);

	}else if(wakeup_freq == 5){

		i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, LP_WAKE_CTRL_5);

	}else if(wakeup_freq == 20){

		i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, LP_WAKE_CTRL_20);

	}else if(wakeup_freq == 40){

		i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, LP_WAKE_CTRL_40);

	}else{

		return false;
		//**********Return an error statement
	}

	return true;
}

void mpu6050_gyro_standby(void){

	uint8_t enable = STBY_XG | STBY_YG | STBY_ZG;
	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, enable);
}

void mpu6050_Xgyro_standby(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, STBY_XG);
}

void mpu6050_Ygyro_standby(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, STBY_YG);
}

void mpu6050_Zgyro_standby(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, STBY_ZG);
}

void mpu6050_accel_standby(void){

	mpu6050_Xaccel_standby();
	mpu6050_Yaccel_standby();
	mpu6050_Zaccel_standby();

}

void mpu6050_Xaccel_standby(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, STBY_XA);
}

void mpu6050_Yaccel_standby(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, STBY_YA);
}

void mpu6050_Zaccel_standby(void){

	i2c_set_bits(MPU6050_ADDR, MPU6050_PWR_MGMT_2, STBY_ZA);
}

void mpu6050_temp_read(MPU6050_data *data){

	//Enable FIFO for temp
	i2c_set_bits(MPU6050_ADDR, MPU6050_FIFO_EN, TEMP_FIFO_EN);

	//Check to make sure 2 bytes are available
	if(!(mpu6050_fifo_count_check(2))){
		i2c_clear_bits(MPU6050_ADDR, MPU6050_FIFO_EN, TEMP_FIFO_EN);
		return;
	}

	uint8_t buffer[2];
	i2c_read(MPU6050_ADDR, MPU6050_FIFO_R_W, buffer, 2);
	data->temp = (((int16_t)((buffer[0] << 8) | buffer[1]))/340 + 36.53);

	//Disable FIFO for temp
	i2c_clear_bits(MPU6050_ADDR, MPU6050_FIFO_EN, TEMP_FIFO_EN);
}

void mpu6050_gyro_read(MPU6050_data *data){

	i2c_set_bits(MPU6050_ADDR, MPU6050_FIFO_EN, XG_FIFO_EN | YG_FIFO_EN | ZG_FIFO_EN);

	if(!(mpu6050_fifo_count_check(6))){
		i2c_clear_bits(MPU6050_ADDR, MPU6050_FIFO_EN, XG_FIFO_EN | YG_FIFO_EN | ZG_FIFO_EN);
		return;
	}

	uint8_t low, high;
	i2c_read_reg(MPU6050_ADDR, MPU6050_FIFO_R_W, &high);
	i2c_read_reg(MPU6050_ADDR, MPU6050_FIFO_R_W, &low);
	data->gyro_x = (int16_t)((high << 8) | low);

	i2c_read_reg(MPU6050_ADDR, MPU6050_FIFO_R_W, &high);
	i2c_read_reg(MPU6050_ADDR, MPU6050_FIFO_R_W, &low);
	data->gyro_y = (int16_t)((high << 8) | low);

	i2c_read_reg(MPU6050_ADDR, MPU6050_FIFO_R_W, &high);
	i2c_read_reg(MPU6050_ADDR, MPU6050_FIFO_R_W, &low);
	data->gyro_z = (int16_t)((high << 8) | low);

	i2c_clear_bits(MPU6050_ADDR, MPU6050_FIFO_EN, XG_FIFO_EN | YG_FIFO_EN | ZG_FIFO_EN);
}

void mpu6050_compute_orientation(MPU6050_data *data){

	float ratio;

	//Compute Pitch
	ratio = data->accel_x / 9.81f;

	if(ratio > 1.0f){

		ratio = 1.0f;

	}else if(ratio < -1.0f){

		ratio = -1.0f;

	}

	data->pitch = asinf(ratio);

	//Compute Roll
	data->roll = atan2f(data->accel_y, data->accel_z);



}
