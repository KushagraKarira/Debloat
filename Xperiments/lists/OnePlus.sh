#!/usr/bin/env bash

declare -a oneplus=(
	# I NEVER HAD A ONEPLUS DEVICE ON HAND. 
	# I did some intensive searches on the web to find a list and I try my best to document it but I need OnePlus users to really improve it.
	# I use [MORE INFO NEEDED] tag as a marker.
	#
	# In any case, OnePlus has a lot of shady logging apps ! 

	"cn.oneplus.photos"
	# Shot On OnePlus
	# Accessible through the Wallpapers selection menu.
	# Provide photos uploaded by OnePlus users, allowing you to set them as your current wallpaper. 
	# Each day, one new photo appears within the application.

	"com.example.wifirftest"
	# Wifi Radio Frequency test
	# Probably used in factory. No hidden test menu to use it.

	"com.fingerprints.fingerprintsensortest"
	# Sensor Test Tool 
	# Provide fingerprint hidden test menu (type *#806 in OnePlus dialer)

	"com.oem.autotest"
	# Auto Test Server
	# Used to test the hardware of your device and change hidden settings.

	"com.oem.logkitsdservice"
	# Used by a Shady logging app (com.oem.oemlogkit) which can be activated a bit too easily.
	# No good reason why this app is on customer devices.
	# It can log WiFi traffic, Bluetooth traffic, NFC activity, GPS coordinates over time, power consumption, modem signal/data details, "lag issues," and more.
	# https://thehackernews.com/2017/11/oneplus-logkit-app.html
	# https://www.bleepingcomputer.com/news/security/second-oneplus-factory-app-discovered-this-one-dumps-photos-wifi-and-gps-logs/
	# Source : https://nitter.net/fs0c131y/status/930773795656396801

	"com.oem.nfc"
	# OnePlus NFC tester

	"com.oem.oemlogkit"
	# OnePlusLogKit 
	# See "com.oem.logkitsdservice"

	"com.oneplus.backuprestore"
	# OnePlus Switch (https://play.google.com/store/apps/details?id=com.oneplus.backuprestore)
	# Lets you migrate your contacts, text messages, photos, and other data from your previous phone to a OnePlus phone. 
	# It can also help backup your data of the OnePlus phone as a compressed archive.

	"com.oneplus.brickmode"
	# OnePlus Zen Mode (https://play.google.com/store/apps/details?id=com.oneplus.brickmode)
	# Zen Mode will help you put down your phone and enjoy your life.
	# In Zen Mode you will only be able to take photos and answer calls.

	"com.oneplus.bttestmode"
	# OnePlus Bluetooth test mode
	# Type *#*#232339#*#* in the OnePlus dialer to access this hidden test menu.

	"com.oneplus.card"
	# Card Package
	# Widget which lets you add membership card in Shelf.
	# You enter numbers for a club card or something and it'll store it and generate a barcode for you.
	#
	# Note : Shelf is essentially a page on your home screen that allows you to take memos, add widgets, gain access to your most-used apps, 
	# and get a quick glimpse of the weather. Swipe right (from the left edge of your OnePlus screen) and you'll see Shelf in action.

	"com.oneplus.factorymode"
	# EngineeringMode/ FactoryMode
	# Used by the operator in the factory to test the devices.
	# You only need to type *#808# in the OnePlus dialer to acess the hidden menu.
	# Potential security risk : https://nitter.net/fs0c131y/status/930115188988182531
	# It's now possible for an app to enable root access on any device with the APK preinstalled. 
	# For now, this only works in ADB, which requires local access to the device.
	#
	# OnePlus decided to remove this app.

	"com.oneplus.factorymode.specialtest"
	# Engineering Mode Special Test
	# Used by the operator in the factory to test the devices.
	#
	# See above.

	"com.oneplus.gamespace"
	# OnePlus Game Space
	# Useless. Game launcher.
	# Allows you to launch your game library as well as checking out several stats about the game, such as how long you have played.

	"com.oneplus.note"
	# OnePlus Notes app

	"com.oneplus.opbugreportlite"
	# OPBugReportLite 
	# Sends silently, every 6 hours, the battery stats, kernel panics, watchdogs, ANRs and all crashes of your device to Singapore.
	# https://www.androidpit.com/oneplus-opbugreportlite-data-collection
	# Source (yeah it's Elliot Alderson again :D) : https://nitter.net/fs0c131y/status/933037531066785797

	"com.oneplus.soundrecorder"
	# OnePlus voice recording app 

	"com.tencent.soter.soterserver"
	# Soter is a biometric authentication standard and platform in Android held by Tencent.
	# https://github.com/Tencent/soter#a-quick-look-at-tencent-soter
	# FYI : Tencent is a Chinese multinational conglomerate holding company (https://en.wikipedia.org/wiki/Tencent#Controversies)

	"com.wapi.wapicertmanage"
	# WAPI certificate manager
	# WAPI = WLAN Authentication and Privacy Infrastructure. 
	# It's a Chinese National Standard for Wireless LAN (local area network : within a limited area such as a home)
	# Not very useful if you don't live in China.
	# FYI : https://en.wikipedia.org/wiki/WLAN_Authentication_and_Privacy_Infrastructure
	#
	# Digital certificates identify computers, phones, and apps for security. Just like you'd use your driver’s license 
	# to show that you can legally drive, a digital certificate identifies your device and confirms that it should be able to access something.
	# FYI : https://security.stackexchange.com/questions/102550/what-are-wifi-certificates-used-for-what-are-they 

	"net.oneplus.commonlogtool"
	# OnePlus Common Log Tool
	# 9 permissions and given what we know about OnePlus logging apps, it's a good idea to remove this 
	# See "com.oneplus.opbugreportlite", "com.oneplus.factorymode", "com.oem.logkitsdservice".

	"net.oneplus.forums"
	# OnePlus Community (https://play.google.com/store/apps/details?id=net.oneplus.forums)
	# Lets you access to OnePlus forum... wah that great !

	"com.oneplus.opsports"
	# Cricket Scores (https://play.google.com/store/apps/details?id=com.oneplus.opsports)
	# Lets you access and follow cricket teams and tournaments

	"net.oneplus.odm"
	"net.oneplus.odm.provider"
	# Shady analytic app... again.
	# Sends A LOT of data to OnePlus' servers including the phone's IMEI number, the phone number, MAC addresses, 
	# mobile network names and IMSI prefixes, Wi-Fi connection info, the phone's serial number and every time an app was opened.
	# Press : https://www.androidpolice.com/2017/10/10/never-settle-oneplus-found-collecting-personally-identifiable-analytics-data-phone-owners/
	# Source : https://www.chrisdcmoore.co.uk/post/oneplus-analytics/

	"net.oneplus.provider.appcategoryprovider" # [MORE INFO NEEDED]
	# AppCategoryProvider
	# Used to regroup app in category in the OnePlus launcher ?

	"net.oneplus.push"
	# OnePlus push notification. 
	# It only concern OnePlus useless preinstalled apps ("surveys and other junks" according a user)
	# https://forums.oneplus.com/threads/psa-non-root-root-stop-oneplus-push-notifications.580058/
	# OnePlus can remotely sends you push notification :
	# https://www.androidpolice.com/2019/07/01/oneplus-accidentally-pushed-a-cryptic-notification-to-all-7-pro-users/

	"net.oneplus.weather"
	# OnePlus weather app (https://play.google.com/store/apps/details?id=net.oneplus.weather)

	"net.oneplus.widget"
	# OnePlus Widget
	# Lets you use OnePlus widgets on the home screen.

	########## ADVANCED DEBLOAT ##########

	#"com.oneplus.iconpack.circle"
	# OnePlus Icon Pack - Round (https://play.google.com/store/apps/details?id=com.oneplus.iconpack.circle)

	#"com.oneplus.iconpack.oneplus"
	# OnePlus Icon Pack (https://play.google.com/store/apps/details?id=com.oneplus.iconpack.oneplus)

	#"com.oneplus.iconpack.square"
	# OnePlus Icon Pack - Square (https://play.google.com/store/apps/details?id=com.oneplus.iconpack.square)

	#"cn.oneplus.oem_tcma"
	# TCMA stands for Tiered Contention Multiple Access, which is a cellular traffic management protocol.
	# TCMA is a CSMA/CA protocol which schedules transmission of different types of traffic. 
	# I don't know if it's a good idea to remove given a CSMA/CA protocol improve QoS (https://en.wikipedia.org/wiki/Quality_of_service)
	# At the same time, it is most likely only used in China (ch.oneplus ==> China version) and for OnePlus apps.
	# FYI : https://en.wikipedia.org/wiki/Carrier-sense_multiple_access_with_collision_avoidance

	#"com.oneplus.filemanager"
	# OnePlus file manager

	)

