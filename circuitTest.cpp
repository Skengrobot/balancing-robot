//code for testing the circuit connection to the
//motors and sensors

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <unistd.h>

#define FORWARD 0
#define BACKWARD 1
#define IN1 30
#define IN2 60
#define IN3 31
#define IN4 5

using namespace std;


void exportPin(int pin)
{
    FILE *pinHandle = NULL;
    char setValue[4], GPIOString[4], GPIODirection[64];
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", pin);
    sprintf(GPIOString, "%d", pin);
	// Export the pin
	if ((pinHandle = fopen("/sys/class/gpio/export", "ab")) == NULL)
		printf("Can't export the GPIO pin.\n");
	else
		printf("Exported pin\n");

	strcpy(setValue, GPIOString);
	fwrite(&setValue, sizeof(char), 2, pinHandle);
	fclose(pinHandle);

	//Set the direction of the pin
	if ((pinHandle = fopen(GPIODirection, "rb+")) == NULL)
		printf("Can't open direction handle.\n");
	else
		printf("Set direction\n");

	strcpy(setValue,"out");
	fwrite(&setValue, sizeof(char), 3, pinHandle);
	fclose(pinHandle);
}

void unexportPin(int pin)
{
    FILE *pinHandle = NULL;
    char setValue[4], GPIOString[4];
	// Unexport the pin
	if ((pinHandle = fopen("/sys/class/gpio/unexport", "ab")) == NULL)
		printf("Can't export the GPIO pin.\n");
	else
		printf("Unexported pin\n");

	strcpy(setValue, GPIOString);
	fwrite(&setValue, sizeof(char), 2, pinHandle);
	fclose(pinHandle);
}

void requestPWM(void)
{
    FILE *requestPWM = NULL;
    FILE *runPWM = NULL;
    FILE *duty = NULL;
    FILE *period = NULL;

    char* one =  new char('1');
	if((requestPWM = fopen("/sys/class/pwm/ehrpwm.1:0/request", "w")) == NULL)
                printf("Can't request PWM 1.0  operation.\n");
        else
                printf("Got PWM 1.0\n");

	fprintf(requestPWM, "%d", 1);
	fclose(requestPWM);

	if((runPWM = fopen("/sys/class/pwm/ehrpwm.1:0/run", "w")) == NULL)
                printf("Can't run PWM 1.0  operation.\n");
        else
                printf("Running PWM 1.0\n");

        fprintf(runPWM,"%d", 1);
        fclose(runPWM);


	if ((requestPWM = fopen("/sys/class/pwm/ehrpwm.1:1/request", "w")) == NULL)
                printf("Can't request PWM 1.1  operation.\n");
        else
                printf("Got PWM 1.1\n");

        fprintf(runPWM, "%d", 1);
        fclose(requestPWM);

        if((runPWM = fopen("/sys/class/pwm/ehrpwm.1:1/run", "w")) == NULL)
                printf("Can't run PWM 1.1  operation.\n");
        else
                printf("Running PWM 1.1\n");

        fprintf(runPWM, "%d", 1);
        fclose(runPWM);

        if((period = fopen("/sys/class/pwm/ehrpwm.1:0/period_freq", "w")) == NULL)
                printf("Can't change period on 1.0.\n");
        else
                printf("Period set\n");

     	fprintf(period, "%d", 100);
        fclose(period);
}

void dutyCycle2(int setCycle)
{
FILE* dutyCycle = NULL;

        if((dutyCycle = fopen("/sys/class/pwm/ehrpwm.1:0/duty_percent", "w")) == NULL)
                printf("Can't change duty on 1.0.\n");
        else
                printf("Duty 0 set to %d\n", setCycle);

	fprintf(dutyCycle, "%d", setCycle);
	fclose(dutyCycle);
}

void dutyCycle1(int setCycle)
{
FILE* dutyCycle = NULL;

        if((dutyCycle = fopen("/sys/class/pwm/ehrpwm.1:1/duty_percent", "w")) == NULL)
                printf("Can't change duty on 1.1.\n");
        else
                printf("Duty 1 set to %d\n",setCycle);

        fprintf(dutyCycle, "%d", setCycle);
        fclose(dutyCycle);
}

void setDirection1 (bool forward)
{
int one = 1;
int zero = 0;

FILE* in1 = NULL;
FILE* in2 = NULL;
	if((in1 = fopen("/sys/class/gpio/gpio30/value", "w")) == NULL)
		printf("Can't open GPIO pin 30\n");
	else {
		if (forward)
			fprintf(in1, "%d", zero);
		else
			fprintf(in1, "%d", one);
		fclose(in1);
	}

	if((in2 = fopen("/sys/class/gpio/gpio60/value", "w")) == NULL)
		printf("Can't open GPIO pin 60\n");
	else {
		if (forward)
			fprintf(in2, "%d", one);
		else
			fprintf(in2, "%d", zero);
		fclose(in2);
	}
}

void setDirection2 (bool forward)
{
FILE* in3 = NULL;
FILE* in4 = NULL;
	if((in3 = fopen("/sys/class/gpio/gpio31/value", "w")) == NULL)
		printf("Can't open GPIO pin 31\n");
	else {
		if (forward)
			fprintf(in3, "%d", 0);
		else
			fprintf(in3, "%d", 1);
		fclose(in3);
	}

	if((in4 = fopen("/sys/class/gpio/gpio5/value", "w")) == NULL)
		printf("Can't open GPIO pin 5\n");
	else {
		if (forward)
			fprintf(in4, "%d", 1);
		else
			fprintf(in4, "%d", 0);
		fclose(in4);
	}
}



int main (void)
{
exportPin(IN1);
exportPin(IN2);
exportPin(IN3);
exportPin(IN4);
requestPWM();

setDirection1(FORWARD);
dutyCycle1(35);
sleep(5);
dutyCycle1(80);
sleep(5);
setDirection1(BACKWARD);
sleep(5);
dutyCycle1(0);

setDirection2(FORWARD);
dutyCycle2(35);
sleep(5);
dutyCycle2(80);
sleep(5);
setDirection2(BACKWARD);
sleep(5);
dutyCycle2(0);

return 0;
}
