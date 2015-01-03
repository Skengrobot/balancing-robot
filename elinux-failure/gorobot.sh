#!/bin/bash
#run as root

SLOTS=/sys/devices/bone_capemgr.8/slots 

#	sets pin modes to 7 for digital output for 
# hbridge directions 
# All pins are on the P9 header 
# Pins are mapped to inputs on the H-bridge as:
# h-bridge pin:   IN1 IN2 IN3 IN4
# bone pin:       11  12  13  17
# OS gpio number: 30  60  31   5
echo pinctrl-test-0 > $SLOTS
gpio_outputs=
for pin in 30 60 31 5 ;
do
    echo $pin > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio$pin/direction
done

# also pins on P9 header
# Accelerometer: 40
# Gyroscope: 38
# Seems the touchscreen kernel module was renamed
modprobe ti_am335x_tscadc

# PWM pins are 14 and 16
modprobe pwm_test
echo am33xx_pwm > $SLOTS
echo bone_pwm_P9_14 > $SLOTS
echo bone_pwm_P9_16 > $SLOTS
echo 500 > /sys/devices/ocp.3/pwm_test_P9_14.11/period
echo 500 > /sys/devices/ocp.3/pwm_test_P9_16.12/period

# Make sure that everything is ok
lsmod
cat /sys/kernel/debug/pwm
cat $SLOTS
