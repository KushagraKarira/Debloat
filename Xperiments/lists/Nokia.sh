#!/usr/bin/env bash

declare -a nokia=(
	# I NEVER HAD A NOKIA DEVICE ON HAND. 
	# I did some intensive searches on the web to find a list and I try my best to document it but I need Nokia users to really improve it.
	# I use [MORE INFO NEEDED] tag as a marker.
	
	# s600ww is a SKUID.
	# Stock Keeping Unit (SKU or SKUID) is a unique set of numbers and letters used to identify, locate and track a product internally in a company or store's warehouse.
	# It is here used to identify region specific firmwares.
	# ww = Worldwide users / cn = china / tw = Taiwan / id = Indonesia

	# I don't understand why there are so much overlay stuff.
	# overlay : https://budhdisharma.medium.com/rro-runtime-resource-overlay-in-android-aosp-e094be17f4bc
	# 			https://source.android.com/devices/architecture/rros
	
	# blt = Bell Telephone Laboratories owned by Nokia (https://en.wikipedia.org/wiki/Bell_Labs)

	"com.android.partnerbrowsercustomizations.btl.s600ww.overlay"
	# Add Nokia pinned bookmarks in your chromium based browser

	"com.android.providers.calendar.overlay.base.s600ww" # [MORE INFO NEEDED]
	# "com.android.providers.calendar" is necessary to sync stock Calendar app and lets it work correctly.
	# I don't know what does this overlay add.
	#
	# REMINDER : Content providers help an application manage access to data stored by itself, stored by other apps, 
	# and provide a way to share data with other apps. They encapsulate the data, and provide mechanisms for defining data security
	# Source : https://developer.android.com/guide/topics/providers/content-providers.html

	"com.android.providers.settings.btl.s600ww.overlay" # [MORE INFO NEEDED]
	"com.android.providers.settings.overlay.base.s600ww"
	# Still otther overlays for android settings provider.
	# "com.android.providers.settings" handles settings app datas (contentProvider)
	#
	# REMINDER : Content providers help an application manage access to data stored by itself, stored by other apps, 
	# and provide a way to share data with other apps. They encapsulate the data, and provide mechanisms for defining data security
	# Source : https://developer.android.com/guide/topics/providers/content-providers.html

	"com.android.retaildemo.overlay.base.s600ww" # [MORE INFO NEEDED]
	# Overlay for Retail demonstration mode
	# https://en.wikipedia.org/wiki/Demo_mode

	"com.data.overlay.base.s600ww" # [MORE INFO NEEDED]
	# Overlay for ???
	# Nokia users don't see any differences when removed.

	"com.evenwell.apnwidget.overlay.base.s600ww" # [MORE INFO NEEDED]
	# Another overlay for APN widget this time. Seems useless to me
	# REMINDER : APN means Access Point Name and must be configured with carrier values in order your device could acess carrier network. 

	##### WHAT IS EVENWELL? #####
	# Evenwell/FiH is chinese company. Specifically, they are a subsidary of the massive Foxconn group that manufactures 
	# consumer electronics for sale around the world.
	# You should read the Foxxconn wikipedia page and especially the Controversies section (https://en.wikipedia.org/wiki/Foxconn#Controversies)
	# Evenwell apps was caught to send personnal data to chinese servers
	# https://arstechnica.com/gadgets/2019/03/hmd-admits-the-nokia-7-plus-was-sending-personal-data-to-china/
	#
	# Majority of evenwell app are useless background apps.

	"com.evenwell.AprUploadService" # [MORE INFO NEEDED]
	"com.evenwell.AprUploadService.data.overlay.base"
	"com.evenwell.AprUploadService.data.overlay.base.s600ww"
	"com.evenwell.AprUploadService.data.overlay.base.s600id"
	# Apr Upload Service ????

	"com.evenwell.autoregistration" # [MORE INFO NEEDED]
	"com.evenwell.autoregistration.overlay.base"
	"com.evenwell.autoregistration.overlay.base.s600id"
	"com.evenwell.autoregistration.overlay.base.s600ww"
	"com.evenwell.autoregistration.overlay.base.s600ww"
	"com.evenwell.autoregistration.overlay.d.base.s600id"
	"com.evenwell.autoregistration.overlay.d.base.s600ww"
	# Spyware app which sends warranty details to China
	# https://milankragujevic.com/the-trade-of-privacy-for-convenience
	# https://nitter.net/drwetter/status/1108801189662130176

	"com.evenwell.batteryprotect"
	"com.evenwell.batteryprotect.overlay.base"
	"com.evenwell.batteryprotect.overlay.base.s600id"
	"com.evenwell.batteryprotect.overlay.base.s600ww"
	"com.evenwell.batteryprotect.overlay.d.base.s600e0"
	# Battery protect is advertised to improve battery performance but in practice it drains your battery and kills apps to aggressively.
	# https://dontkillmyapp.com/nokia
	# Nokia decided to stop using this app-killer in the future
	# https://www.androidpolice.com/2019/08/27/nokia-hmd-phones-disable-evenwell-background-process-app-killer/

	"com.evenwell.bboxsbox" # [MORE INFO NEEDED]
	"com.evenwell.bboxsbox.app"
	# ????

	"com.evenwell.bokeheditor" # [MORE INFO NEEDED]
	"com.evenwell.bokeheditor.overlay.base.s600ww"
	# Related to photos editing I think 

	"com.evenwell.CPClient" # [MORE INFO NEEDED]
	"com.evenwell.CPClient.overlay.base"
	"com.evenwell.CPClient.overlay.base.s600id"
	"com.evenwell.CPClient.overlay.base.s600ww"
	# CP = Client Provisioning.
	# Surely used to push new carrier internet/MMS settings automatically
	# Maybe it's useful if carriers change their APN... but you still can change it manually, it's not difficult.

	"com.evenwell.custmanager" # [MORE INFO NEEDED]
	"com.evenwell.custmanager.data.overlay.base"
	"com.evenwell.custmanager.data.overlay.base.s600id"
	"com.evenwell.custmanager.data.overlay.base.s600ww"
	"com.evenwell.customerfeedback.overlay.base.s600ww"
	# Customer manager
	# Given its name it is useless but I don't have more info.

	"com.evenwell.dataagent" # [MORE INFO NEEDED]
	"com.evenwell.dataagent.overlay.base"
	"com.evenwell.dataagent.overlay.base.s600id"
	"com.evenwell.dataagent.overlay.base.s600ww"
	# Data agent
	# Used for backup/restore ? 

	"com.evenwell.DbgCfgTool" # [MORE INFO NEEDED]
	"com.evenwell.DbgCfgTool.overlay.base"
	"com.evenwell.DbgCfgTool.overlay.base.s600id"
	"com.evenwell.DbgCfgTool.overlay.base.s600ww"
	# Debug config tool ? 

	"com.evenwell.defaultappconfigure.overlay.base.s600ww" # [MORE INFO NEEDED]
	# ????

	"com.evenwell.DeviceMonitorControl" # [MORE INFO NEEDED]
	"com.evenwell.DeviceMonitorControl.data.overlay.base"
	"com.evenwell.DeviceMonitorControl.data.overlay.base.s600id"
	"com.evenwell.DeviceMonitorControl.data.overlay.base.s600ww"
	# Monitor stuff obviously...

	"com.evenwell.email.data.overlay.base.s600ww" # [MORE INFO NEEDED]
	# Overlay for email app

	"com.evenwell.factorywizard"
	"com.evenwell.factorywizard.overlay.base"
	"com.evenwell.factorywizard.overlay.base.s600ww"
	# Most likely a configuration setup after a factory reset (and/or after first boot)
	# Guides you through the basics of setting up your device.

	"com.evenwell.foxlauncher.partner" # [MORE INFO NEEDED]
	# Partner Launcher Customization
	# Related to the Nokia launcher

	"com.evenwell.fqc"
	# FQC is a secret test menu. It lets you test the hardware (touch screen, speakers, SD card, SIM card, camera...)
	# You need to type *#*#372733#*#* in the Nokia dialer

	"com.evenwell.legalterm"
	"com.evenwell.legalterm.overlay.base.s600ww"
	# Provides terms and condition (legal notice)

	"com.evenwell.managedprovisioning" # [MORE INFO NEEDED]
	"com.evenwell.managedprovisioning.overlay.base"
	"com.evenwell.managedprovisioning.overlay.base.s600id"
	"com.evenwell.managedprovisioning.overlay.base.s600ww"
	# Seems to be a user interface related to settings description and wizard setup.
	# https://en.wikipedia.org/wiki/Wizard_(software)

	"com.evenwell.mappartner" # [MORE INFO NEEDED]
	# ????

	"com.evenwell.nps"
	"com.evenwell.nps.overlay.base"
	"com.evenwell.nps.overlay.base.s600id"
	"com.evenwell.nps.overlay.base.s600ww"
	# Net Promoter Score
	# Preinstalled survey.

	"com.evenwell.pandorasbox" # [MORE INFO NEEDED]
	"com.evenwell.pandorasbox.app"
	# WTF is this ? 

	"com.evenwell.partnerbrowsercustomizations"
	"com.evenwell.partnerbrowsercustomizations.overlay.base"
	"com.evenwell.partnerbrowsercustomizations.overlay.base.s600id"
	"com.evenwell.partnerbrowsercustomizations.overlay.base.s600ww"
	# Customize your browser with stuff you don't want.

	"com.evenwell.permissiondetection" # [MORE INFO NEEDED]
	"com.evenwell.permissiondetection.overlay.base.s600ww"
	# ????

	"com.evenwell.phone.overlay.base.s600ww"
	"com.evenwell.phone.overlay.base"
	# Overlay for the dialer app

	"com.evenwell.PowerMonitor"
	"com.evenwell.PowerMonitor.overlay.base"
	"com.evenwell.PowerMonitor.overlay.base.s600id"
	"com.evenwell.PowerMonitor.overlay.base.s600ww"
	# Drains more battery than it saves.

	#"com.evenwell.powersaving.g3"
	#"com.evenwell.powersaving.g3.overlay.base.s600id"
	#"com.evenwell.powersaving.g3.overlay.base.s600ww"
	#"com.evenwell.powersaving.g3.overlay.d.base.s600e0"
	# Is nokia powersaving really effective? 

	"com.evenwell.providers.downloads.overlay.base.s600ww"
	"com.evenwell.providers.downloads.ui.overlay.base.s600ww"
	# Add most likey a useless overlay on the download app.

	"com.evenwell.providers.partnerbookmarks.overlay.base.s600ww" # [MORE INFO NEEDED]
	# Provides Nokia bookmarks in Chrome/Nokia browser ?

	"com.evenwell.providers.weather"
	"com.evenwell.providers.weather.overlay.base.s600ww"
	# Provider for the Nokia weather app.
	# REMINDER : Content providers help an application manage access to data stored by itself, stored by other apps, 
	# and provide a way to share data with other apps. They encapsulate the data, and provide mechanisms for defining data security
	# Source : https://developer.android.com/guide/topics/providers/content-providers.html

	"com.evenwell.pushagent"
	"com.evenwell.pushagent.overlay.base"
	"com.evenwell.pushagent.overlay.base.s600id"
	"com.evenwell.pushagent.overlay.base.s600ww"
	# Surely related to push notifications for Nokia apps (only ?)

	"com.evenwell.retaildemoapp"
	"com.evenwell.retaildemoapp.overlay.base"
	"com.evenwell.retaildemoapp.overlay.base.s600id"
	"com.evenwell.retaildemoapp.overlay.base.s600ww"
	# Nokia retail demonstration mode 
	# https://en.wikipedia.org/wiki/Demo_mode

	"com.evenwell.screenlock.overlay.base.s600ww" # [MORE INFO NEEDED]
	# Overlay for the screenlock

	"com.evenwell.settings.data.overlay.base" # [MORE INFO NEEDED]
	"com.evenwell.settings.data.overlay.base.s600ww" 
	# Overlay related to settings

	"com.evenwell.SettingsUtils"
	"com.evenwell.SettingsUtils.overlay.base.s600ww"
	# Settings utils
	# (crapy) Audio rendering. 
	# See https://gitlab.com/W1nst0n/universal-android-debloater/-/issues/9#note_369056538

	"com.evenwell.SetupWizard"
	"com.evenwell.SetupWizard.overlay.base"
	"com.evenwell.setupwizard.btl.s600ww.overlay"
	"com.evenwell.SetupWizard.overlay.d.base.s600ww"
	"com.evenwell.SetupWizard.overlay.base.s600ww"
	# It's the basic configuration wizard that drives you through first boot and guides you through the basics of setting up your device.

	"com.evenwell.stbmonitor"
	"com.evenwell.stbmonitor.data.overlay.base"
	"com.evenwell.stbmonitor.data.overlay.base.s600id"
	"com.evenwell.stbmonitor.data.overlay.base.s600ww"
	# Apparently used to stabilize phone usage.
	# Seems to drain battery 

	"com.evenwell.telecom.data.overlay.base" # [MORE INFO NEEDED]
	"com.evenwell.telecom.data.overlay.base.s600id"
	"com.evenwell.telecom.data.overlay.base.s600ww"
	# Telecom telemetry ?

	"com.evenwell.UsageStatsLogReceiver"
	"com.evenwell.UsageStatsLogReceiver.data.overlay.back.s600id"
	"com.evenwell.UsageStatsLogReceiver.data.overlay.base.s600ww"
	# Logging stuff

	"com.evenwell.weather.overlay.base.s600ww" # [MORE INFO NEEDED]
	"com.evenwell.weatherservice"
	"com.evenwell.weatherservice.overlay.base.s600ww"
	# Overlay for the weather app

	"com.fih.infodisplay" # [MORE INFO NEEDED]
	# Foxconn info display
	# ????

	"com.fih.StatsdLogger"
	# Foxconn stats logger

	"com.foxconn.ifaa"
	# IFAA = China’s Internet Finance Authentication Alliance
	# Chinese organisation that aim to achieve a more simple way to verify the identity of human (like passwordless authentication)

	"com.hmdglobal.datago"
	"com.hmdglobal.datago.overlay.base"
	"com.hmdglobal.datago.overlay.base.s600ww" # [MORE INFO NEEDED]
	# Sends diagnostic data to HMD (Company behin Nokia) ?

	"com.hmdglobal.support"
	# My Phone (https://play.google.com/store/apps/details?id=com.hmdglobal.support)	
	# Lets you join the Nokia phones community, get app recommendations, explore your phone’s user guide and more.

	##############################  ADVANCED DEBLOAT ##############################

	#"com.android.cellbroadcastreceiver.overlay.base.s600ww"
	# Cell broadcast is a method of sending messages to multiple mobile telephone users in a defined area at the same time.
	# It is often used for regional emergency alerts. 
	# I think this package only handles notifications overlay for broadcast cell, not the implementation.
	# It seems to me that broadcast SMS use normal notifications so there is chances that this package provide special overlay for Nokia SMS app ?

	#"com.hmdglobal.camera2"
	# Nokia camera (https://play.google.com/store/apps/details?id=com.hmdglobal.camera2)

	#"com.evenwell.camera2"
	# Nokia camera by evenwell (https://play.google.com/store/apps/details?id=com.evenwell.camera2)

	#"com.evenwell.fmradio"
	#"com.evenwell.fmradio.overlay.base.s600ww"
	# Nokia radio app 

	#"com.evenwell.hdrservice"
	# HDR Service (https://play.google.com/store/apps/details?id=com.evenwell.hdrservice)
	# Enhances contrast and sharpness for normal photos, games and videos dynamically.

	#"com.evenwell.OTAUpdate" # [MORE INFO NEEDED]
	#"com.evenwell.OTAUpdate.overlay.base.s600ww"
	# Related to OTA Update (Over-The-Air updates, updates from Nokia)
	# May not be safe if you still receive updates.

	)
