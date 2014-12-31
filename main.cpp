//motor pins and write functions are decalred in circuitTest_test.cpp
//analog read pins and functions are decalred in getAngle.h



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include "circuitTest_test.cpp"
#include "getAngle.h"

#define PWM_OFFSET 14
//values below this don't start motors
#define ANGLE_GAIN 6
//gain for proprtional part of motor controller
#define GYRO_GAIN 0.55
//gain for derivative part of motor controller
#define gyroBias 1389
//0-degree bias point
#define gyroSensitivity -0.0027166
//bulk constant for integration timestep and voltage-angular rate relationship
#define accBias 2020
//0-degree bias point
#define accSensitivity 0.086579
//bulk constant for small angle approximation of sine and voltage-tilt relationship

using namespace std;

//FIR filter coefficients
const float hlp[21] = {0.007252481, 0.009322776, 0.015307679, 0.024649499, 0.036451130, 0.049564465, 0.062704538, 0.074577930, 0.084012427, 0.090074477, 0.092164591, 0.090074477, 0.084012427, 0.074577930, 0.062704538, 0.049564465, 0.036451130, 0.024649499, 0.015307679, 0.009322776, 0.007252481,};
const float hhp[21] = {0.031944, 0.0044177, -0.0020706, -0.013609, -0.029706, -0.049104, -0.069866, -0.089627, -0.10595, -0.1167, 0.87954, -0.1167, -0.10595, -0.089627, -0.069866, -0.049104, -0.029706, -0.0136709, -0.0020706, 0.0044177, 0.031944};
//past sensor reads for filtering
float gyro[21] = {0};
float acc[21] = {0};
float temp[21];
int gyroread;

//functions for filtering, basically array shift, multiply and add
float highPass (float newVal)
{
float angle = 0;

	for (int i=0; i<21; i++)
		temp[i] = gyro[i];
	for (int i=1; i<21; i++)
		gyro[i] = temp[i-1];
	gyro[0] = newVal;

	for (int i=0; i<21; i++)
		angle += gyro[i]*hhp[i];

	return angle;
}

float lowPass (float newVal)
{
float angle = 0;

	for (int i=0; i<21; i++)
		temp[i] = acc[i];
	for (int i=0; i<21; i++)
		acc[i] = temp[i-1];
	acc[0] = newVal;

	for (int i=0; i<21; i++)
		angle += acc[i]*hlp[i];

	return angle;
}


// mutex to lock angle estimates
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
//function that updates each motor value
void *updatePWM1(void*);
void *updatePWM2(void*);
//angle estimate from filtering
float angle;

int main()
{
	pthread_t thread1, thread2;
	float y = 0;
	float gyro = 0;
	float integral = 0;
	float acc_est;
	float gyro_est;

	exportPin(IN1);
	exportPin(IN2);
	exportPin(IN3);
	exportPin(IN4);
	requestPWM();

	int motor1;
	int motor2;
	//int count = 0;


	while(1){

		pthread_mutex_lock(&mutex1);
		gyroread = readGyro() - gyroBias;;
		pthread_mutex_unlock(&mutex1);

	        integral = integral+(readGyro()-gyroBias)*gyroSensitivity;
	        y=(readYacc()-accBias)*accSensitivity;

		gyro_est = highPass(integral);
		acc_est = lowPass(y);

		pthread_mutex_lock(&mutex1);
		angle=acc_est+gyro_est;
		pthread_mutex_unlock(&mutex1);

		//printf("%f\t%f\t%f\n", acc_est, gyro_est, angle);

        	usleep(10000);

		//count++;
		//printf("Cycle %d: \n", count);


		motor1 = pthread_create(&thread1, NULL, updatePWM1, (void*) NULL);
		motor2 = pthread_create(&thread2, NULL, updatePWM2, (void*) NULL);
		pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);



	}

	exit(0);
}




void *updatePWM1(void*)
{
	int g = gyroread;
	int a = angle;
	float pwm1 = ANGLE_GAIN*a - GYRO_GAIN*g;


	if(pwm1 <= 0){
	setDirection1(BACKWARD);
	pwm1 = -pwm1;
	}

	else
	setDirection1(FORWARD);

	pwm1 = pwm1 + PWM_OFFSET;
	if(pwm1 > 100)
	pwm1 = 100;

//  	printf("updating PWM 1 to %.7f\n", pwm1);
//  	printf("%ld\n",pthread_self());

	dutyCycle1((int) pwm1);
//	usleep(250000);
	return 0;
}


void *updatePWM2(void*)
{
	int g = gyroread;
	int a = angle;
	float pwm2 = ANGLE_GAIN*a - GYRO_GAIN*g;

	if(pwm2 <= 0){
	setDirection2(BACKWARD);
	pwm2 = -pwm2;
	}

	else
	setDirection2(FORWARD);

	pwm2 = pwm2 + PWM_OFFSET;
	if(pwm2 > 100)
	pwm2 = 100;

//	printf("updating PWM 2 to %.7f\n", pwm2);
//	printf("%ld\n",pthread_self());

	dutyCycle2((int) pwm2);
//	usleep(250000);

    return 0;
}
