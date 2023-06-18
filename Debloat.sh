#!/bin/sh
echo Make sure ADB is installed
echo it can be installed by
echo Debian / Ubuntu : sudo apt install adb

manufacturer=$(adb shell getprop ro.product.manufacturer ) #Get Manufacture name from build.prop
cd Android
model=$(adb shell getprop ro.product.model)
echo The device is $model by $manufacturer

read -p "Continue Debug (y/n)?" CONT
if [ "$CONT" = "y" ]; then
	echo Starting Debug !!
	sh $manufacturer # Execute the Mfg. file
else
	exit 1
fi

echo Source : https://github.com/KushagraKarira/Debloat

