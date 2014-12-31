#!/bin/bash

# 	run as root
#	sets pin modes to 7 for digital output for 
# hbridge directions 
# All pins are on the P9 header 
# Pins are mapped to inputs on the H-bridge as:
# IN1 IN2 IN3 IN4
# 11  12  13  17
echo 7 > /sys/kernel/debug/omap_mux/gpmc_wait0
echo 7 > /sys/kernel/debug/omap_mux/gpmc_ben1
echo 7 > /sys/kernel/debug/omap_mux/gpmc_wpn
echo 7 > /sys/kernel/debug/omap_mux/spi0_cs0

#	sets input pin modes to ADC
# also pins on P9 header
# Accelerometer: 40
# Gyroscope: 38
echo 6 > /sys/kernel/debug/omap_mux/gpmc_a2
echo 6 > /sys/kernel/debug/omap_mux/gpmc_a3

modprobe ti_tscadc

# PWM pins are 14 and 16
./pwm-enable -e 0 1 2

