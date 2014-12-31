#!/bin/bash

# 	run as root
#	sets pin modes to 7 for digital output for 
# hbridge directions 
echo 7 > /sys/kernel/debug/omap_mux/gpmc_wait0
echo 7 > /sys/kernel/debug/omap_mux/gpmc_ben1
echo 7 > /sys/kernel/debug/omap_mux/gpmc_wpn
echo 7 > /sys/kernel/debug/omap_mux/spi0_cs0

#	sets input pin modes to ADC
echo 6 > /sys/kernel/debug/omap_mux/gpmc_a2
echo 6 > /sys/kernel/debug/omap_mux/gpmc_a3

modprobe ti_tscadc

./pwm-enable -e 0 1 2

