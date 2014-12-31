//Gyroscope and accelerometer reading functions
#include <stdio.h>
#include <stdlib.h>

int readGyro (void)
{
FILE* pin = NULL;
int reading = -1;

	if((pin = fopen("/sys/devices/platform/omap/tsc/ain3", "r")) == NULL)
		printf("Can't open ain3\n");
	else {
		fscanf(pin, "%d", &reading);
		fclose(pin);
	}

	return reading;
}

int readYacc (void)
{
FILE* pin = NULL;
int reading = -1;

	if((pin = fopen("/sys/devices/platform/omap/tsc/ain1", "r")) == NULL)
		printf("Can't open ain1\n");
	else {
		fscanf(pin, "%d", &reading);
		fclose(pin);
	}

	return reading;
}
