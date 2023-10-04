#!/bin/bash
echo "ins gpio driver"
insmod /lib/modules/5.10.*/extra/gpio-driver.ko
while true
do
        echo The current time is $(date)
        sleep 1
done