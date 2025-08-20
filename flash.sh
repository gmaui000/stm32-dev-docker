#!/bin/bash
set -e

openocd -f interface/stlink.cfg \
        -f target/stm32f1x.cfg \
        -c "init" \
        -c "reset halt" \
        -c "program \"build/stm32f1.elf\" verify reset exit"

