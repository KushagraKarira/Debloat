#!/usr/bin/env bash
adb shell pm uninstall --user 0 com.android.keyguard #Huwei Magazine Unlock
adb shell pm uninstall --user 0 com.android.hwmirror #Mirror
adb shell pm uninstall --user 0 com.baidu.input_huawei #Keyboard ( and spyware )
adb shell pm uninstall --user 0 com.hicloud.android.clone #Hawvei Phone Clone
adb shell pm uninstall --user 0 com.huawei.android.chr #Huawei Call History Record 
adb shell pm uninstall --user 0 com.huawei.android.FloatTasks #Floating dock function 
adb shell pm uninstall --user 0 com.huawei.android.hsf # Huawei Services Framework
	# 3 permissions : DELETE_PACKAGES, INSTALL_PACKAGES, PACKAGE_USAGE_STATS
	# Safe to remove according to huawei users

	"com.huawei.android.hwpay" 
	# Huawei Pay
	# Mobile payment and e-wallet service for Huawei devices that offers the same services as Apple Pay, Samsung Pay etc...
	# https://consumer.huawei.com/en/mobileservices/huawei-wallet/

	"com.huawei.android.instantonline" # [MORE INFO NEEDED]
	# no noticable consequences

	"com.huawei.android.instantshare" 
	# Huawei Share features.
	# File transfer tool between Huawei mobiles, using Bluetooth connection and WiFi Direct technology.

	"com.huawei.android.karaoke" 
	# Karaoke mode feature.

	"com.huawei.android.mirrorshare" 
	# MirrorShare feature (Miracast rebranded by Huawei)
	# Used to mirror screen of you smartphone on a TV.

	"com.huawei.android.pushagent" # [MORE INFO NEEDED]
	# push notification agent
	# Seems to only be used for Huawei apps
	# The recompiled java code makes it look like it's once again mainly used for analytics.

	"com.huawei.android.remotecontroller" 
	# Huawei Smart Controller app.
	# Lets you you add, customize, and set up remote controls, allowing control of your electronic appliances through your phone. 

	"com.huawei.android.tips" 
	# HUAWEI Feature Advisor
	# Periodically gives you notifications on how to use certain features on your phone.

	"com.huawei.android.totemweather" 
	"com.huawei.android.totemweatherapp"
	"com.huawei.android.totemweatherwidget"
	# Huawei Weather app (and its widget)

	"com.huawei.android.wfdft"
	"com.huawei.android.wfdirect" 
	# Wi-Fi Direct feature.
	# Note: Wifi direct enables devices to establish a direct Wi-Fi connection (without a router) over which the two can send and receive files. 

	"com.huawei.appmarket" 
	# Huawei app store (AppGallery)
	# https://www.xda-developers.com/appgallery-huawei-alternative-google-play-store-android/

	"com.huawei.arengine.service" 
	# Augmented reality service.

	"com.huawei.bd"
	# HwUE (Huawei UserExperience)
	# When a company call a something 'UserExperience' you know you don't need this.
	# Analytics service, run at boot. Collect information about packages/apps usages.
	# Has a nice custom permission called com.huawei.permission.BIG_DATA

	"com.huawei.bluetooth"
	# Lets you import your contacts via Bluetooth
	# Bluetooth will still work if you remove this package.

	"com.huawei.browser" 
	# Huawei Browser app.

	"com.huawei.browserhomepage"
	# Huawei Browser component.

	"com.huawei.compass" 
	# Huawei Compass app.

	"com.huawei.contactscamcard" 
	# CamCard is a business card reader app.

	"com.huawei.contacts.sync" # [MORE INFO NEEDED]
	# Huawei Contacts sync
	# My guess (can't have the apk on hand) is this enables you to synchronise your contacts with your Huawei account.

	"com.huawei.desktop.explorer" # [MORE INFO NEEDED]
	# From XDA thread : "Service that is been used when you wanna use your phone as an operative system on a PC."
	# I don't understand what does it mean.

	"com.huawei.email"
	# Huawei Email app.

	"com.huawei.fido.uafclient" 
	# UAF client for FIDO.
	# Fido is a set of open technical specifications for mechanisms of authenticating users to online services that do not depend on passwords.
	# https://fidoalliance.org/specs/u2f-specs-1.0-bt-nfc-id-amendment/fido-glossary.html
	# https://fidoalliance.org/specs/fido-v2.0-rd-20170927/fido-overview-v2.0-rd-20170927.html
	#
	# The UAF protocol is designed to enable online services to offer passwordless and multi-factor security by allowing users to register their device 
	# to the online service and using a local authentication mechanism such as iris or fingerprint recognition. .
	# https://developers.google.com/identity/fido/android/native-apps
	# Safe to remove if you don't use password-less authentification to access online servics.

	"com.huawei.game.kitserver" # [MORE INFO NEEDED]
	# Probably safe to remove if you don't play games

	"com.huawei.gameassistant" 
	# Huawei Game Suite (HiGame).
	# Mobile game app store.
	# https://club.hihonor.com/in/topic/16341/detail.htm

	"com.huawei.geofence" 
	# GeofenceService.
	# Allows you to do something when a user enters an area that has been defined as a trigger.
	# A geofence is a virtual perimeter set on a real geographic area. Combining a user position with a geofence perimeter, 
	# it is possible to know if the user is inside or outside the geofence or even if he is exiting or entering the area.

	"com.huawei.hwsearch"
	# Petal Search widget. Used for finding apps/apks on serveral online sources (introduced after Google Mobile Services Ban).

	"com.huawei.hwid" 
	# Huawei Mobile Services (https://play.google.com/store/apps/details?id=com.huawei.hwid)
	# Needed to access advanced Huawei features.
	# A Huawei ID is required to access services, like Themes, Mobile Cloud, HiCare, Huawei Wear, Huawei Health  

	"com.huawei.hiaction" # [MORE INFO NEEDED]
	# no noticable consequences

	"com.huawei.hiai" # [MORE INFO NEEDED]
	# no noticable consequences

	"com.huawei.hiassistantoversea"
	# HiVoice. Huawei's voice assistant to replace "Hey Google"

	"com.huawei.hicard" # [MORE INFO NEEDED]
	# Huawei Cards, no noticable consequences

	"com.huawei.hicloud"
	# Huawei cloud features

	"com.huawei.hifolder" 
	# Huawei Online Cloud folder service
	# https://consumer.huawei.com/en/mobileservices/mobilecloud/

	#"com.huawei.hiview"
	#"com.huawei.hiviewtunnel"
	# This displays details/attributes of pictures in the gallery (ISO, exposure time, etc.).

	"com.huawei.himovie.overseas"
	# Huawei stock video application (https://play.google.com/store/apps/details?id=com.huawei.himovie.overseas). Replace with VLC, which integrates well with the stock gallery.

	"com.huawei.hitouch" 
	# Huawei HiTouch
	# Assistant capable to recognize the objects in a photo and to search them through various shopping sites.
	# https://consumer.huawei.com/uk/support/faq/have-you-tried-the-new-hitouch-assistant/

	"com.huawei.hwasm" 
	# FIDO UAF Autenthicator-Specific Module.
	# See 'com.huawei.fido.uafclient' for FIDO explaination.
	# The UAF Authenticator-Specific Module (ASM) is a software interface on top of UAF authenticators which gives a standardized way for FIDO UAF clients 
	# to detect and access the functionality of UAF authenticators and hides internal communication complexity from FIDO UAF Client.
	# Source : https://fidoalliance.org/specs/fido-uaf-v1.0-ps-20141208/fido-uaf-asm-api-v1.0-ps-20141208.html

	"com.huawei.hwblockchain" # [MORE INFO NEEDED]
	# probably blockchain related, no noticable consequences

	"com.huawei.hwdetectrepair" 
	# Huawei Smart diagnosis (https://play.google.com/store/apps/details?id=com.huawei.hwdetectrepair)
	# Useless features and run in background.

	#"com.huawei.HwMultiScreenShot"
	# Scrolling screenshot feature

	"com.huawei.hwstartupguide"
	# A one-time setup app that is no longer needed
	
	"com.huawei.hwvoipservice"
	# MeeTime (https://consumer.huawei.com/en/support/content/en-us00956296/)

	"com.huawei.iaware"
	# App Prioritizer. Prioritizes apps to avoid slowdown
	# Up to you but there is apparently no noticeable difference when deleted.

	"com.huawei.ihealth" 
	# MotionService package, it's required for actions like shaking the phone to shut off the alarm, ecc.

	"com.huawei.intelligent"
	# Huawei Assistant. Shopping recommendations

	"com.huawei.health"
	# Huawei Health (https://play.google.com/store/apps/details?id=com.huawei.health)
	# Connect Huawei wearables to your phone and all sorts of stats like all fitness tracking apps.

	# Live wallpapers
	"com.huawei.livewallpaper.paradise" 
	"com.huawei.livewallpaper.artflower"
	"com.huawei.livewallpaper.flowersbloom"
	"com.huawei.livewallpaper.mountaincloud"
	"com.huawei.livewallpaper.naturalgarden"
	"com.huawei.livewallpaper.ripplestone"
	
	"com.huawei.magazine"
	# Magazine unlock. Downloads wallpapers for your lock screen.

	"com.huawei.mirror" # [MORE INFO NEEDED]
	# Huawei Mirror app. 
	# Mirror like "Glass"
	
	"com.huawei.mirrorlink"
	# Huawei mirrorlink implementation
	# Used to connect your phone to a car (with https://mirrorlink.com/ support) in order to provide audio streaming, GPS navigation...

	"com.huawei.music"
	# Huawei Music app.

	"com.huawei.parentcontrol" 
	# Parental controls functions.

	"com.huawei.pcassistant" 
	# HiSuite service. Used by HiSuite PC software.
	# HiSuite enables you to backup your data and restore them from/to your phone.
	# https://consumer.huawei.com/en/support/hisuite/

	"com.huawei.phoneservice" 
	# HiCare (https://play.google.com/store/apps/details?id=com.huawei.phoneservice)
	# Provides you common online services including customer services, issue feedback, user guides, service centers and self-service. 

	"com.huawei.scanner"
	# AI Lens. Shop for objects that you take a picture of. This de-clutters the camera interface by removing the AI Lens button on the top left corner and does not break the AR Measure app.
 	
	"com.huawei.stylus.floatmenu" 
	# Floating menu with M-Pen feature.

	"com.huawei.synergy" 
	# Huawei Cloud & Network Synergy.
	# Seems to be related to B2B (Business To Business) cloud stuff.
	# https://www.huawei.com/en/press-events/news/2016/10/Cloud-Network-Synergy-Whitepaper

	"com.huawei.tips"
	# HUAWEI Feature Advisor
	# Periodically gives you notifications on how to use certain features on your phone.

	"com.huawei.trustagent" # [MORE INFO NEEDED]
	# Smart unlock feature.
	# Enables you to unlock your phone with a Bluetooth device, like a smart band. 
	# When a compatible Bluetooth device is detected, you can unlock your phone with a simple swipe (without a password).

	"com.huawei.vassistant" 
	# HiVoice app.
	# Vocal assistant like Siri or Alexa
	# http://tadviser.com/index.php/Product:Huawei_HiAssistant_(HiVoice)

	"com.huawei.videoeditor" 
	# Huawei Video editor.

	"com.huawei.vassistant" 
	# HiVoice app
	# Huawei voice assistant (like Siri or Google assistant)
	# Huge privacy risk. Keep in mind that the app keeps the microphone *on* non-stop.
	# Is now Celia (https://consumer.huawei.com/en/emui/celia/)

	"com.huawei.wallet" 
	# Huawei Wallet (renammed Huawei Pay)
	# Mobile payment and e-wallet service for Huawei devices that offers the same services as Apple Pay, Samsung Pay etc...
	# https://consumer.huawei.com/en/mobileservices/huawei-wallet/

	"com.huawei.watch.sync" # [MORE INFO NEEDED]
	# Huawei Watch sync function
	# Is it only used to sync Huawei watch ?
	# Safe to remove according to several users

	"com.iflytek.speechsuite" 
	# Default voice input method from iflytek, a big chinese company (https://en.wikipedia.org/wiki/IFlytek)
	# IFLytek was implicated in human rights violations : 
	# https://asia.nikkei.com/Economy/Trade-war/US-sanctions-8-China-tech-companies-over-role-in-Xinjiang-abuses
	# Archive: https://web.archive.org/save/https://asia.nikkei.com/Economy/Trade-war/US-sanctions-8-China-tech-companies-over-role-in-Xinjiang-abuses

	"com.nuance.swype.emui" # [MORE INFO NEEDED]
	# Huawei Swype functions.
	# Is it the full Swype keyboard or only the Swype function on Huawei keyboard ? 
	# NOTE : Nuance company said it would discontinue support of the Swype keyboard app.

	##############################  ADVANCED DEBLOAT ##############################

	#"com.hisi.mapcon" 
	# Provides wifi calling feature (call or text on Wi-Fi networks using your SIM card)
	# NOTE: Instant messaging video/voice call does not use this "wifi calling" feature. 
	# Btw, you should use a E2EE messaging app like Signal/Session/Element(https://element.io/) instead of the non-secure wifi-calling feature
	# provided by your carrier.

	#"com.huawei.KoBackup"
	# As of writing this, Huawei phones cannot be rooted. 
	# This Backup application is probably able to backup more than any other 3rd party backup app.

	#"com.huawei.android.thememanager" # [MORE INFO NEEDED]
	# Huawei Themes (https://play.google.com/store/apps/details?id=com.huawei.android.thememanager)
	# Lets you use Huawei themes
	# You should still be able to set wallapers without it. Can someone check?

	#"com.huawei.aod" [MORE INFO NEEDED]
	# Always On Display feature.
	# Drain battery (though not much on OLED screens) for nothing really useful.
	# RedSkull23 says it's unsafe to remove it. Does it bootloop ? 

	#"com.huawei.android.internal.app" # [MORE INFO NEEDED]
	# Component of Huawei sharing. I read someone saying "Do not remove or you won't be able to send files to apps".
	# Can someone test it ? 

	#"com.huawei.android.launcher" 
	# Huawei launcher app.
	# It's basically the home screen, the way icons apps are organized and displayed.
	# DON'T REMOVE THIS IF YOU DIDN'T INSTALL ANOTHER LAUNCHER !
	# You will maybe need this package for the recent apps feature to work (even if you have another launcher)

	#"com.huawei.calculator"
	# Huawei Calculator app.

	#"com.huawei.calendar"
	# Huawei Calendar app.

	#"com.huawei.contacts"
	# Huawei Contacts app

	#"com.huawei.deskclock"
	# Huawei Clock App.

	#"com.huawei.photos"
	# Huawei Gallery app.
	# Note: The official camera app refuses to open photos in another gallery (you can't review your picture from the camera app)

	#"com.android.mediacenter" 	
	# Huawei music app. (Yeah they messed up with the package name)

	#"com.huawei.screenrecorder" 
	# Huawei Screen recorder feature (with internal mic record toggle)
	
	#"com.huawei.hidisk" 
	# Huawei File Manager app.

	#"com.huawei.search" 
	# HiSearch
	# Allows you to search through settings, files, contacts and notes while keeping a record of your search history.
	# Hi Search is really annonying because it's triggered as soon as you wipe down from the middle part of the home.

	#"com.huawei.securitymgr" # [MORE INFO NEEDED]
	# PrivateSpace
	# Lets you store private information in a hidden space within your device that can only be accessed 
	# with your fingerprint or password.
	# TODO: Data at rest encryption? If not, this is useless
	# https://consumer.huawei.com/en/support/content/en-us00754246/
	)


##################### YOU SHOULDN'T MESS WITH THEM (core packages and may cause bootloop)  #####################

# "com.huawei.systemmanager"
# Huawei System Manager app
# Manage apps behavior and authorize them to run (or not) in background.
