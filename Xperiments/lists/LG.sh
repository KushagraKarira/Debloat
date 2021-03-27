#!/usr/bin/env bash

declare -a lge=(
	# I NEVER HAD A LG DEVICE ON HAND. 
	# I did some intensive searches on the web to find a list and I try my best to document it. But I need LG users to really improve it.
	# I use [MORE INFO NEEDED] tag as a marker.

	"com.android.LGSetupWizard"
	# The first time you turn your device on, a Welcome screen is displayed. It guides you through the basics of setting up your device.
	# It's the setup for LG services.

	"com.lge.appbox.client" # [MORE INFO NEEDED]
	# LG Application manager
	# Installs/Updates LG related apps ? 

	"com.lge.bnr"
	# LG Backup
	# Can backup your mobile devices LG Home screen, device settings, apps, and contacts to your computer.
	# https://www.lg.com/us/support/help-library/lg-android-backup-CT10000025-20150104708841

	"com.lge.cic.eden.service"
	# Memories album
	# Gallery automatically creates a Memories album with pictures and videos saved in the phone. 
	# Memories is a virtual album of pictures saved in the phone or SD card.
	# Source : https://www.lg.com/hk_en/support/product-help/CT30019000-1433767985158-others

	"com.lge.cloudhub"
	# LG Cloud
	# Backup tool, which gives you the option to backup LG phone settings, apps, contacts and Home screen 
	# to your phone internal storage, SD card, computer or cloud.
	# Source : https://www.apeaksoft.com/backup/lg-cloud-backup.html
	
	"com.lge.drmservice"
	# DRM Service
	# Needed to read DRM content playback. It manages the DRM Client, which holds a particular type of information required to get a license key. 
	# REMINDER : DRM = all the things that restrict the use of proprietary hardware and copyrighted works.
	# ==> https://en.wikipedia.org/wiki/Digital_rights_management
	# ==> https://creativecommons.org/2017/07/09/terrible-horrible-no-good-bad-drm/
	# ==> https://fckdrm.com/
	# ==> http://www.info-mech.com/drm_flaws.html

	"com.lge.easyhome"
	# LG EasyHome
	# EasyHome is a more simplified version of the Home screen that you can choose to use on your phone.
	# It displays the Home screen like a remote control device. T
	# Source : https://www.lg.com/us/mobile-phones/VS985/Userguide/048.html

	"com.lge.eltest"
	# ELTest
	# Device hardware tests settings

	"com.lge.email"
	# LG Email app

	"com.lge.eula"
	# LG EULA (Terms of Use) accessible in the settings.
	# EULA = https://en.wikipedia.org/wiki/End-user_license_agreement

	"com.lge.eulaprovider"
	# License Provider
	# Needed by com.lge.eula.

	"com.lge.fmradio"
	# FM radio app

	"com.lge.friendsmanager"
	# LG Friends Manager (https://play.google.com/store/apps/details?id=com.lge.friendsmanager)
	# WTF ? Completly useless app.
	# Not sure but I think it enables you to download an app for a friend LG user.

	"com.lge.gallery.collagewallpaper"
	# LG Collage Wallpapers
	# Allows you to create patchwork wallpaper from several photos.
	# https://www.lg.com/uk/support/product-help/CT00008356-20150332136499-others

	"com.lge.gallery.vr.wallpaper"
	# LG 360 Image Wallpaper
	# Provied VR (360°) wallpapers.

	"com.lge.gcuv" # [MORE INFO NEEDED]
	# GCUV
	# Not 100% sure but @siraltus from XDA thinks it refers to "Gauce Components" which seems to be the LG's version of CSC 
	# (carrier sales code - automatic carrier-specific customization).
	# It gets run on first boot after factory reset, sets up the ROM features based on which carrier and country code is specified 
	# in the build.prop, and then gets frozen so it doesn't reconfigure things on subsequent boots.
	# It's basically the only person to mention "Gauce components" on the web (other than restricted LG webpages when using Google dorks).
	# https://forum.xda-developers.com/tmobile-lg-v10/development/rom-lg-v10-h901-10c-debranded-debloated-t3277305/page12/page12

	"com.lge.gestureanswering"
	# Answer me 2.0
	# Allows you to bring the phone to your ear to answer an incoming call automatically.
	# https://www.lg.com/us/mobile-phones/VS980/Userguide/109-1.html

	"com.lge.gnss.airtest"
	# GNSS Air Test
	# GNSS test, used to test... GNSS. Not needed, and GPNSS will continue to work.
	# NOTE : GNSS = Global Navigation Satellite System and is the standard generic term for satellite navigation systems.
	# This term includes e.g. the GPS, GLONASS, Galileo, Beidou and other regional systems.

	"com.lge.gnsslogcat"
	# GNSS Logcat
	# Used to dump GNSS logs.  

	"com.lge.gnsspostest"
	# GNSS Position test
	# GNSS test again.

	"com.lge.gnsstest"
	# GNSS Test
	# Woh ! Why does LG need so many GNSS test packages?! 

	"com.lge.hifirecorder"
	# LG Audio Recorder

	"com.lge.hotspotlauncher"
	# LG Mobile Hotspot
	# Provide hotsport feature enabling you to share the phone’s 4G data connection with any Wi-Fi capable devices.

	#"com.lge.ia.task.incalagent" # [MORE INFO NEEDED]
	# InCalAgent
	# Related to interface while you're in a call. Seemes also related to tasks list stuff.
	# Can someone tells me what happens when you delete it ? I think it is safe.

	"com.lge.ia.task.smartcare" # [MORE INFO NEEDED]
	# LIA SmartDoctor Engine
	# Needed by SmartDoctor (com.lge.phonemanagement) ?

	"com.lge.ia.task.smartsetting"
	# SmartSetting
	# Turns on/off or changes features, settings and more according to where you are or what you do.
	# https://www.lg.com/us/support/help-library/lg-android-smart-settings-CT10000025-20150103623722

	"com.lge.iftttmanager"
	# LG Smart settings
	# IFTTT = “if this, then that.”. Smart Settings can be seens as IFTTT.
	# Some events automatically triggers actions.

	#"com.lge.ime"
	# LG Stock Keyboard
	# Do not remove if you don't have an alternate keyboard available. Personally, I keep the stock keyboard just in case the keyboard app crash/fails (this happened to me once) locking me out of entering password.

	"com.lge.ime.solution.handwriting"
	# Handwriting feature on the LG keyboard

	"com.lge.ime.solution.text"
	# XT9 
	# Text predicting and correction for the LG keyboard.
	# For your culture (if you're young) : https://en.wikipedia.org/wiki/XT9

	"com.lge.launcher2.theme.optimus"
	# "Optimus" theme for the LG launcher (v2)

	"com.lge.leccp" # [MORE INFO NEEDED]
	# LG Connectivity Service
	# I didn't find any info about this package.	

	"com.lge.lgaccount"
	# LG Account
	# Enable you to create and manage you completly useless LG account.

	"com.lge.lgdrm.permission"
	# Handle permissions for LG DRM (com.lge.drmservice).
	# Why does LG need a whole package for this ? 

	"com.lge.lginstallservies"
	# LG Install Service
	# Used by LG to install some of its apps on the phone. Not needed unless you use the LG apps manager.

	"com.lge.lgmapui" # [MORE INFO NEEDED]
	# LGMapUI
	# User Interface (UI) for displaying location tracking reccord on the Health app (com.lge.lifetracker) ? 

	"com.lge.lgsetupview"
	"com.lge.LGSetupView"
	# Setup View
	# LG first setup (related to com.android.LGSetupWizard). 
	# The first time you turn your device on, a Welcome screen is displayed. It guides you through the basics of setting up your device.

	"com.lge.lgworld"
	# LG SmartWorld
	# LG Store. Enables you to nstall LG apps, theme, keyboard layout, fonts...

	"com.lge.lifetracker"
	# LG Health (https://play.google.com/store/apps/details?id=com.lge.lifetracker)
	# According to users reviews, it is a very bad activity tracking app. 
	# Privacy wise, you should never use this kind of thing obviously. 
	# https://www.lg.com/us/support/help-library/lg-android-lg-health-CT30013120-20150103629401

	"com.lge.mirrorlink"
	# MirrorLink
	# Enable you to connect your phone to a car to provide audio streaming, GPS navigation...
	# https://www.lg.com/ca_en/support/product-help/CT30014940-1440413573040-others
	# https://mirrorlink.com/

	"com.lge.mlt"
	# LG MLT
	# Run in background all the time and probably serves purpose to help LG remote support. The thing is this acts as a good spyware. 
	# It tries to track all your activity and logs GPS position together with the details gathered, and that includes calls, apps starting etc...
	# All data is collected and placed on /mpt partition, it seems not to be per reboot, but actually kept during flash and upgrades.
	#
	# https://forum.xda-developers.com/showthread.php?t=2187920

	"com.lge.mtalk.sf" # [MORE INFO NEEDED]
	# Voice Mate Speech Pack
	# Voice Mate (now Q Vocie) is the LG Personal assistant (https://en.wikipedia.org/wiki/Voice_Mate)
	# This package provides speech pack. Is it the main Q-voice package ? I don't think so but I need confirmation.

	"com.lge.musicshare"
	# LG Audio Share 
	# Enables you to connect two devices so that you can share the sound from music or video files with another LG devices.
	# https://www.lg.com/hk_en/support/product-help/CT30007700-20150123957406-others

	"com.lge.myplace"
	# My Place
	# Analyses the place you stay the most and recognises it as My Place (or your home) automatically.
	# https://www.lg.com/uk/support/product-help/CT00008356-1433767701724-setting

	"com.lge.myplace.engine"
	# My Places Engine
	# Needed by com.lge.myplace. See above.

	"com.lge.phonemanagement"
	# Smart Doctor App
	# Enables you to shut down idle apps and delete temporary files.
	# Lets you also see phone battery, mobile data, apps, network status and usage patterns.
	# On the paper it seems good but in practise, Android handle 8+ handles very well idles apps. 
	# https://www.lg.com/ca_en/support/product-help/CT20098088-20150129256824-others

	"com.lge.privacylock"
	# LG Content lock
	# You can lock the LG Gallery with a password or pattern. When connected to a PC, Content Lock prevents file previews.
	# https://www.lg.com/us/support/help-library/lg-g4-content-lock-CT10000027-1432774759427

	"com.lge.qhelp"
	# Quick Help
	# App who provide you support articles (FAQ section that walks you through most of the major features of the phone).
	# You can request support via email or request a call from LG.
	# https://www.lg.com/us/support/help-library/lg-android-quick-help-CT10000026-20150103624836

	"com.lge.qhelp.application" # [MORE INFO NEEDED]
	# Quick Help application
	# I think this package is the real Quick Help app. The package above only provides help contents IMO.

	"com.lge.qmemoplus"
	# LG QuickMemo+
	# Allows you to capture screen shots and use them to create memos. You can also insert a reminder, location information, image, video, and audio.
	# https://www.lg.com/us/support/help-library/lg-android-quickmemo-CT10000025-20150103629575

	"com.lge.remote.lgairdrive"
	# LG AirDrive
	# Lets you to control files in your device via a wireless connection. 
	# To use, you need to sign in to your LG account on both the PC and mobile device.
	# https://www.lg.com/africa/support/product-help/CT20080025-1436354408798-others
 
	"com.lge.remote.setting"
	# LG AirDrive settings
	# See package above.

	"com.lge.sizechangable.musicwidget.widget" # [MORE INFO NEEDED]
	# Music widget
	# Not sure if it only manages Music widget for the launcher or also for the lockscreen.
		
	"com.lge.sizechangable.weather"
	# Weather widget for the home screen.

	"com.lge.sizechangable.weather.platform"
	# Weather Service
	# Provide weather data for the weather app/widget.

	"com.lge.sizechangable.weather.theme.optimus"
	# "Optimus" theme for the weather app/widget.

	"com.lge.smartdoctor.webview"
	# Smart Doctor Webview
	# REMINDER : A WebView is acomponent that allows Android apps to display content from the web directly inside an application.

	"com.lge.smartshare"
	# SmartShare 
	# Feature that uses DLNA technology to stream multimedia contents between DLNA devices.
	# DLNA is a non-profit trade organisation which defines standards that enable devices to share stuff with each other.
	# Basically LG provides a way to stream multimedia contents from your phone to your smart TV (or via a DLNA plugin)
	# https://www.lg.com/ca_en/support/product-help/CT31903570-1428542236040-file-media-transfer-pictures-music-etc

	"com.lge.smartshare.provider"
	# Provider for Smart Share. 
	# Needed by com.lge.smartshare.
	# REMINDER : content providers help an application manage access to data stored by itself, stored by other apps, 
	# and provide a way to share data with other apps. They encapsulate the data, and provide mechanisms for defining data security

	"com.lge.smartsharepush" # [MORE INFO NEEDED]
	# Smart Share Push
	# Obviously related to Smart Share but I don't know its exact purpose. 

	"com.lge.snappage"
	# Snap Page
	# Part of the QuickMemo+ app, lets you capture the text/images/URL from a web page without grabbing ads.
	# It’s much like instapaper or pocket app, but it works locally, like reading mode on some browsers, saving only the body of the article. 
	# http://www.lg.com/us/mobile-phones/g4/display

	"com.lge.springcleaning"
	# Smart cleaning
	# Displays the space in use and free space in your phone and allows you to selectively clean up your files.
	# https://www.lg.com/us/mobile-phones/VS986/Userguide/339.html

	"com.lge.sync"
	# LG Bridge Service
	# Used to backup, restore, update your LG phone, and transfer files wirelessly between computer and LG phone. 
	# You will need to install LG Bridge software on your PC.
	# NOTE : Cause noticable battery drain.

	"com.lge.video.vr.wallpaper"
	# Video Wallpaper
	# LG 360° VR Wallpapers

	"com.lge.videoplayer"
	# LG Video Player
	# NB : This is a bad one. VLC is much better.

	"com.lge.videostudio"
	# Quick Video Editor
	# Allows you to create and edit video files using the videos (and photos) stored on the phone.
	# https://www.lg.com/us/mobile-phones/VS980/Userguide/281.html

	"com.lge.voicecare"
	# LG Voice care
	# Allows you to use your device if the touch screen or display is damaged. 
	# You must agree to location-based information use and personal information collection to use Voice Care. 
	# https://www.lg.com/hk_en/support/product-help/CT20136018-20150122834174-others

	"com.lge.vrplayer"
	# LG VR player
	# Enables you to watch 360° pictures/videos.

	"com.lge.wernicke"
	# QVoice Engine
	# Needed by Q-voice (the LG Q Voice voice assistant) to work.

	"com.lge.wernicke.nlp"
	# Natural-language processing for LG intelligent assistant.
	# Used to understand what says an human when he speaks.
	# Needed by QVoice

	"com.lge.wfd.spmirroring.source" # [MORE INFO NEEDED]
	# Provide wifi-direct feature
	# Note : Wifi-direct is Wi-Fi standard enabling devices to easily connect with each other without requiring a wireless access point.
	# It allows allows two devices to establish a direct Wi-Fi connection without requiring a wireless router
	# https://en.wikipedia.org/wiki/Wi-Fi_Direct
	# spmirroring = ??? screen p... mirroring ?

	"com.lge.wfds.service.v3"
	# Wifi-direct service (v3)
	# See above.

	"com.lge.wifi.p2p"
	# LG P2p Service 
	# Wifi-drect P2P allows the device to discover the services of nearby devices directly, without being connected to a network.
	# Needed for LG Wifi-direct feature.
	# https://developer.android.com/training/connect-devices-wirelessly/nsd-wifi-direct

	"com.lge.wifihotspotwidget"
	# Wifi Hotspot widget
	# REMINDER : Hotspot enable you to share the phone’s 4G data connection with any Wi-Fi capable devices.
	# It is not the Hotspot feature. Only the widget ! 
	
	"com.lge.appwidget.dualsimstatus"
	# Dual SIM status widget
	# Probably only present in dual sim LG phone variants. Does not remove the persistent notification or dual SIM functionality.
	
	"com.lge.hiddenpersomenu"
	"com.lge.hiddenmenu"
	"com.lge.operator.hiddenmenu"
	"com.lge.servicemenu"
	# Service menus. I believe if you remove the last one the secret code you can dial doesn't work anymore (who needs it anyway..?)
	
	"com.rsupport.rs.activity.lge.allinone"
	# LG support App remote access
	# You probably don't want that to happen
	
	"com.lge.laot"
	# LAOP test [MORE INFO NEEDED]
	# I don't know what LAOP is. I could not find information about it. It's a test so it's probably fine. I have removed it.
	
	"com.lge.lgfmservice"
	# Radio
	
	####################### UP TO YOU #######################

	"com.lge.bnr.launcher"
	# LG Mobile Switch Launcher
	# This doesn't remove the default launchers.
	# It is most likely to backup/restore the user's launcher configuration.

	#"com.lge.clock"
	# LG Clock app

	#"com.lge.filemanager"  
	# Stock file manager

	"com.lge.homeselector"
	# LG Home selector
	# This is the settings menu for the home launcher (present in the settings app as "Home launcher")
	# If you remove this app, the Home screen settings menu is gone from settings app. (not needed if you use external launcher)
    # You can still switch between installed launchers, the package name is a bit misleading.

	#"com.lge.launcher2"
	# LG Home (v2)
	# Stock launcher
	# It's basically the home screen, the way icons apps are organized and displayed.
	# DON'T REMOVE THIS IF YOU DIDN'T INSTALL ANOTHER LAUNCHER!
	# NOTE : Yeah there is another package described as "launcher". Normally, you only have one of them on your phone. 

	#"com.lge.launcher3"
	# LG Home (v3)
	# Stock launcher
	# It's basically the home screen, the way icons apps are organized and displayed.
	# DON'T REMOVE THIS IF YOU DIDN'T INSTALL ANOTHER LAUNCHER!
	# NOTE : Yeah there is 3 packages described as "launcher". Normally, you only have one of them on your phone. 

	"com.lge.music"
	# Stock music player

	"com.lge.floatingbar"  
	# LG Floating bar
	# Lets you put shortcuts to apps or features, as well as quick access to contacts and music player controls on a "floating bar" on the Home screen.
	# https://www.neowin.net/news/lg-v30-closer-look-floating-bar/

	#"com.lge.updatecenter" # [MORE INFO NEEDED]
	# LG Update Center
	# Provide Android upgrade and LG updates (Settings --> System --> Update Center)
	# I believe you won't receive any updates if this packages is deleted.
	
	#"com.lge.theme.black"
	#"com.lge.theme.white"
	#"com.lge.theme.titan" # This one is labelled 'Platinum' in themes app
    #"com.lge.theme.highcontrast"
	# Various themes. Safe to remove any you don't want/use. I could not find the package for the default (LG) theme.
	# Make sure you don't delete the package for the theme you're currently using. I don't know what will happen then.

    #"com.lge.signboard"
    # Always on display. Probably a battery killer if used with OLED devices.
    # You will lose the menu in the settings app that allows you to set the always on display. From the top of my head I remember the default ones as clock, kitkat easter egg.

    #"com.lge.equalizer" # [MORE INFO NEEDED]
    # Equalizer settings.
    
    #"com.lge.exchange" # [MORE INFO NEEDED]
    # It looks like the Microsoft outlook/email in the logo. Believe this is some sort of microsoft integration.
    # I don't 100% remember if I was able to add accounts to the phone still (eg. Nextcloud), I need to test that soon.
    
    #"com.lge.gametuner"
    # Settings/features for games, such as resolution and frame rate limiting.
    # A little side note, any games installed in the work profile can't use gametuner (maybe if you install this package into the work profile it'll work)
    
    #"com.lge.task"
    # Task storage
    # Probably related to the task app (another bloatware). I say its safe to remove if you don't use it.
    
    #"com.lge.touchcontrol"
    # I have never seen this menu in the settings app. I say its safe to remove. I can't think of any use case for this setting, it just allows you to change where you're allowed to touch the screen
    
    #"com.lge.wapservice"
    # Icon looks like email configuration. I say its safe to remove. Probably related to the stock email app.
    
    #"com.lge.rcs.sharedsketch" [MORE INFO NEEDED]
    # I have no idea what it is, maybe some drawing program related to rcs. I removed it.
    
    #"com.lge.faceglance.trustagent"
    #"com.lge.faceglance.enrollment"
    # Face Recognition
    # Remove if you don't need it. If you want security I don't think this is a good idea to use it.
    
    #"com.lge.app.floating.res"
    # Multitasking framework
    # Allows you to use multitasking features like multiple apps in one screen.
    # Does not remove screen pinning feature.
    # I don't know if this removes the floating windows feature that you have to enable with ADB (to make it look more like a desktop)
	)
