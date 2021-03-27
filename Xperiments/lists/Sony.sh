#!/usr/bin/env bash

declare -a sony=(
	# I NEVER HAD A SONY DEVICE ON HAND. 
	# I did some intensive searches on the web to find a list and I try my best to document it but I need Nokia users to really improve it.
	# I use [MORE INFO NEEDED] tag as a marker.

	"com.sony.tvsideview.videoph"
	# Video & TV SideView (replaced by https://play.google.com/store/apps/details?id=com.sony.tvsideview.phone)
	# Lets you use your smartphone or tablet as a TV remote control for the home. 

	"com.sonyericsson.android.addoncamera.artfilter" # [MORE INFO NEEDED]
	# Sony Creative effect
	# Gives options for various photographic toning effects in the Sony camera app.
	# I'm not 100% sure for this one. Can someone confirm ? 

	"com.sonyericsson.android.omacp"
	# omacp = OMA Client Provisioning. It is a protocol specified by the Open Mobile Alliance (OMA).
	# It is used by carrier to send "configuration SMS" which can setup network settings (such as APN).
	# In my case, it was automatic and I never needed configuration messages. I'm pretty sure that in France this package is useless.
	# Maybe it's useful if carriers change their APN... but you still can change it manually, it's not difficult.
	# These special "confirguration SMS" can be abused : 
	# https://www.zdnet.fr/actualites/les-smartphones-samsung-huawei-lg-et-sony-vulnerables-a-des-attaques-par-provisioning-39890045.htm
	# https://www.csoonline.com/article/3435729/sms-based-provisioning-messages-enable-advanced-phishing-on-android-phones.html

	"com.sonyericsson.conversations.res.overlay_305" # [MORE INFO NEEDED]
	"com.sonyericsson.conversations.res.overlay"
	# Used to display a overlay notification (= on top of others app) when you receive a SMS with Sony SMS app ? 

	"com.sonyericsson.idd.agent"
	# Anonymous Usage Stats
	# Used to send "anonymous" information about how you use your Sony Smartphone to Sony servers.
	# Nobody knows how these info are anonymized...

	#"com.sonyericsson.mtp" # [MORE INFO NEEDED]
	# MTP extension service
	# I think it's needed transfert data from you phone to a PC when using the Media Transfert Protocol (MTP).
	# Does MTP still works if you delete this package ? 

	"com.sonyericsson.mtp.extension.backuprestore"
	# Backup/Restore Sony feature.
	# It enable you to backup (Contacts, call logs, text messages, calendar, settings, bookmarks & media files)
	# NOTE : I don't think this feature can backup your messages or calendars for instance if you don't use the Sony stock app.
	# https://support.sonymobile.com/global-en/xperiaz2/userguide/backing-up-and-restoring-content-on-a-device/

	"com.sonyericsson.mtp.extension.update" 
	# Update service for MTP Extension.
	# I don't know what it really does. Update drivers ? 

	"com.sonyericsson.music"
	# Sony music player (https://play.google.com/store/apps/details?id=com.sonyericsson.music)

	"com.sonyericsson.settings.res.overlay_305" # [MORE INFO NEEDED]
	# Some people removed this package. I guess it's only an (useless) overlay the the settings ? 

	"com.sonyericsson.startupflagservice" # [MORE INFO NEEDED]
	# Startup Flag Service
	# Used during the production of the phone to verify that the touch input works. 
	# It can be triggered when a specific TA-parameter is not set. This should never be triggered and if it does well it doesn't have any use for you.
	# 
	# TA means Timing Advance and its value correspond to the length of time a signal takes to reach the base station from a mobile phone.
	# https://www.telecomhall.net/t/parameter-timing-advance-ta/6390

	"com.sonyericsson.textinput.chinese"
	# Sony chinese keyboard

	"com.sonyericsson.trackid.res.overlay"
	"com.sonyericsson.trackid.res.overlay_305"
	# Overlay for Sony trackid
	# Trackid was a mobile music and audio search engine (like Shazam). It has been discontinued by Sony.
	# REMINDER : An overlay allows apps to display content on top of other apps.

	"com.sonyericsson.unsupportedheadsetnotifier"
	# Given its name, it think it diplays a notification when a insert a headset not compatible with you phone.

	"com.sonyericsson.wappush"
	# WAP Push
	# Used to display annoying WAP push.
	# WAP push is a type of text message that contains a direct link to a particular Web page. 
	# When a user is sent a WAP-push message, he receives an alert, once clicked, directs him to the Web page via his browser.
	# Personally, I don't like this. URLs are now recognized by the SMS instant messaging apps and you just have to click on it.

	"com.sonyericsson.warrantytime"
	# Lets you see some info about your warranty and how long it will last.
	
	"com.sonyericsson.xhs"
	# Sony Xperia Lounge (discontinued by Sony on August 2019)
	# The Xperia Lounge app was meant to provide loyal fans with various rewards for their Xperia smartphones, 
	# such as exclusive Xperia Themes and wallpapers, as well as competitions.

	"com.sonymobile.advancedlogging" 
	# Advanced Logging
	# Sends logs to Sony Mobile. These logs contain a wide range of personal information such as unique device IDs, your location, 
	# details regarding running applications, and events/input leading up to a crash.
	# Logging is only active for a short time and automatically disabled once logging has been completed. 
	# Logs are uploaded when connected to Wi-Fi and automatically deleted when the upload is complete.

	"com.sonymobile.advancedwidget.topcontacts"
	# Top Contacts widget
	# It will show pictures of your most frequently used contacts right on your home screen.
	# REMINDER : Widgets are small applications that you can use directly on the window screen. They also function as shortcuts

	"com.sonymobile.android.addoncamera.soundphoto" 
	# Sony Sound Photo
	# Lets you record a background sound and take a photo at the same time with the Sound Photo app.

	"com.sonymobile.androidapp.cameraaddon.areffect"
	# Old package for AR Effect (https://play.google.com/store/apps/details?id=com.sonymobile.androidapp.cameraaddon.areffect)
	# Lets you add AR (Augmented Reality) effects to your pictures and videos.

	"com.sonymobile.android.externalkeyboard"
	# International keyboard layouts
	# Useless if you use latin keyboard

	"com.sonymobile.android.contacts.res.overlay_305"
	# Overlay for Sony contacts.
	# REMINDER : An overlay allows apps to display content on top of other apps.

	"com.sonymobile.android.externalkeyboardjp"
	# Japanese layout for Sony keyboard.

	"com.sonymobile.anondata"
	# Anonymous Usage Stats (yes just as com.sonyericsson.idd.agent but it's for other phones)
	# Used to send "anonymous" information about how you use your Sony Smartphone to Sony servers.
	# Nobody knows how these info are anonymized...

	#"com.sonymobile.apnupdater" # [MORE INFO NEEDED]
	# I guess it automatically updates your APN settings if your carrier changes it ?
	# I thought it was the role of com.android.carrierconfig.
	# NOTE : The probability that your carrier's APN change is very low.
	# APN : https://tamingthedroid.com/what-apn-settings-mean

	#"com.sonymobile.apnupdater.res.overlay_305" # [MORE INFO NEEDED]
	# Overlay for APN Updater. Useful ? 

	"com.sonymobile.aptx.notifier"
	# Aptx Notifier
	# aptX (formerly apt-X) is a family of proprietary audio codec compression algorithms owned by Qualcomm.
	# If you don't mind closed source codec, aptX has lower latency and is less of a drain on your battery than default codec (AAC)
	# This package is used to display a notification when a device using aptX (bluetooth headphone typically) is connected.
	# Its only use is to tell you that you use aptX bluetooth with the connected device.

	"com.sonymobile.assist"
	# Xperia Assist (https://play.google.com/store/apps/details?id=com.sonymobile.assist)
	# Learns how you use your phone.

	"com.sonymobile.assist.persistent" # [MORE INFO NEEDED]
	# Related to Xperia Assist (see just above) but I don't know its purpose.

	"com.sonymobile.cameracommon.wearablebridge"
	# Camera Wearable bridge
	# Lets you take pictures with your phone by using Sony SmartWatch.

	#"com.sonymobile.cellbroadcast.notification" # [MORE INFO NEEDED]
	# Cell information
	# Cell broadcast is a method of sending messages to multiple mobile telephone users in a defined area at the same time.
	# It is often used for regional emergency alerts.
	# I think this package only handles notifications for broadcast cell, not the implementation.
	# It seems to me that broadcast SMS use normal notifications so there is chances that this package provide special notification for Sony SMS app ? 

	"com.sonymobile.coverapp2"
	# Style Cover
	# Themes for lockscreen.

	"com.sonymobile.demoappchecker"
	# Demo app checker
	# Lets you enter/exit (in) the demonstration mode.
	# https://en.wikipedia.org/wiki/Demo_mode

	"com.sonymobile.deviceconfigtool" # [MORE INFO NEEDED]
	# Configuration agent
	# Seems to do things related cloud but it's unclear.
	# https://knowledge.protektoid.com/apps/com.sonymobile.deviceconfigtool/91e44f1e19b364411776d758ff3b27f703bd4b60c9399c43c124f37d0c30df27

	"com.sonymobile.dualshockmanager"
	# DUALSHOCK
	# Provide PlayStation DualShock controller support for Android (Settings > Device connection > Dualshock)

	"com.sonymobile.email"
	# Sony Email app

	"com.sonymobile.entrance"
	# What's New (discontinued in 2014)
	# Provided news from Sony products through extremely annoying automated notifications.

	"com.sonymobile.getmore.client"
	# Xperia Tips (https://play.google.com/store/apps/details?id=com.sonymobile.getmore.client)
	# Gives you tips for your Xperia device.

	"com.sonymobile.getset"
	# Xperia Actions (discontinued)
	# Lets you automate some actions (only a few) 
	# https://support.sonymobile.com/global-en/xperiaxz/userguide/xperia-actions/

	"com.sonymobile.getset.priv"
	# Xperia Actions System
	# Same thing as the package above.

	"com.sonymobile.gettoknowit"
	# Introduction to Xperia (discontinued)
	# Introduces you the features of your phone.

	"com.sonymobile.glovemode"
	# Sony Glove mode
	# Lets you use your smart phone and touch the screen while wearing regular gloves.

	"com.sonymobile.googleanalyticsproxy"
	# Google Analytics Proxy
	# Allows you to publicly share your Google Analytics reporting data
	# https://developers.google.com/analytics/solutions/google-analytics-super-proxy

	"com.sonymobile.intelligent.backlight"
	# Smart backlight control
	# Keeps the screen on as long as the device is held in your hand. Once you put down the device, the screen turns off according to your sleep setting.

	"com.sonymobile.indeviceintelligence"
	# Xperia Intelligence Engine
	# This app is supposed to understand how you use the phone, the apps you prefer, and will suggest tips 
	# and options based on app usage, how often you use an app, what time of day...
	# For me this just looks like a AI bullshit app who has a huge list of permissions and launch in background at boot
	# This app performs geofencing (check if your are located in a certain perimeter, near your home for instance) 
	# and this doesn't looks great privacy-wise (https://en.wikipedia.org/wiki/Geo-fence)

	"com.sonymobile.intelligent.gesture"
	# Smart call handling
	# Lets you handle incoming calls without touching the screen.
	# https://support.sonymobile.com/global-en/xperiaxz/userguide/smart-call-handling/

	"com.sonymobile.intelligent.iengine" # [MORE INFO NEEDED]
	# According to a sony user it is part of Smart Screen rotation (auto screen rotation based on the gyroscope).
	# Seems not reliable.
	# Does it break the screen-rotation if removed?

	"com.sonymobile.intelligent.observer" # [MORE INFO NEEDED]
	# IntelligentObserver
	# ???? (but intelligent stuff are safe to remove)

	"com.sonymobile.lifelog"
	# Lifelog (https://play.google.com/store/apps/details?id=com.sonymobile.lifelog)
	# Another activity tracker app.

	"com.sonymobile.moviecreator.rmm"
	# Movie Creator (https://play.google.com/store/apps/details?id=com.sonymobile.moviecreator.rmm)
	# Automatically creates short movies using your photos and videos.

	"com.sonymobile.mtp.extension.fotaupdate" # [MORE INFO NEEDED]
	# fota update service
	# FOTA = Firmware Over-The-Air 
	# FOTA allows manufacturers to remotely install new software updates, features and services.
	# Given there is "mtp.extension" in the package name, I think it lets you update your phone via your PC.
	# What's weird is that it should be called SEUS then (https://www.mobilefun.co.uk/blog/2008/06/software-updates-sony-ericsson/)

	"com.sonymobile.music.googlelyricsplugin"
	# Google lyrics plugin
	# Provides lycris from Google in the sony music app

	"com.sonymobile.music.wikipediaplugin"
	# Wikipedia plugin for sony music app

	"com.sonymobile.music.youtubekaraokeplugin"
	# Youtube karaoke plugin for sony music app

	"com.sonymobile.music.youtubeplugin"
	# Youtube plugin for sony music app

	"com.sonymobile.pip" # [MORE INFO NEEDED]
	# Sony pip (Picture in Picture)
	# Allows videos to shrink down to a small resizable window.
	# Only useful bere Android Oreo which provide native support for Pip ?
	# https://developer.android.com/guide/topics/ui/picture-in-picture
	# https://support.sonymobile.com/global-en/xperiaxz1compact/faq/apps-&-settings/8019307455ff6184015e92f63324005926/

	"com.sonymobile.pobox"
	# Xperia Japanese keyboard
	# Does someone know what "popox" means ? 

	"com.sonymobile.prediction" # [MORE INFO NEEDED]
	# Sony text prediction (for Sony keyboard) 
	# It's only a supposition. Can someone confirm ?

	"com.sonymobile.retaildemo"
	# Retail Demo
	# Retail demonstration mode.
	# https://en.wikipedia.org/wiki/Demo_mode

	"com.sonymobile.scan3d"
	# Sony 3D Creator (https://play.google.com/store/apps/details?id=com.sonymobile.scan3d)
	# Lets you capture your stuff in 3D, from your smartphone, and turn people and objects into high-resolution 3D avatars.
	# https://www.sonymobile.com/global-en/apps-services/3d-creator/

	"com.sonymobile.simlockunlockapp" # [MORE INFO NEEDED]
	# Sim Lock
	# Provide menu (type *#*#7378423#*#* in dialer) to see if your device is locked to a network carrier
	# It need confirmation because it also could be related to SIM network unlock code.

	"com.sonymobile.smartcharger"
	# Battery Care
	# Detects your charging patterns and estimates the start and end time of your regular charging period. 
	# The rate of charging is controlled so that your battery reaches 100% just before you disconnect the charger.
	# https://support.sonymobile.com/gb/xperiaxz/userguide/battery-and-power-management/

	"com.sonymobile.support"
	# Sony Support (https://play.google.com/store/apps/details?id=com.sonymobile.support)
	# Useless support app. 

	"com.sonymobile.synchub" # [MORE INFO NEEDED]
	# Sony Backup & restore feature to/from Google drive ? 
	# Can someone confirm ? Does it impact com.sonyericsson.mtp.extension.backuprestore ?
	# https://support.sonymobile.com/global-en/xperia10/faq/apps-&-settings/801930747866b72a016b307df3b6007faf/

	"com.sonymobile.themes.sou.cid18.black"
	"com.sonymobile.themes.sou.cid19.silver"
	"com.sonymobile.themes.sou.cid20.blue"
	"com.sonymobile.themes.sou.cid21.pink"
	"com.sonymobile.themes.xperialoops2"
	# Sony themes

	"com.sonymobile.xperialounge.services"
	# Xperia™ Lounge Pass service (discontinued)
	# The Xperia Lounge app was meant to provide loyal fans with various rewards for their Xperia smartphones, 
	# such as exclusive Xperia Themes and wallpapers, as well as competitions.
	# https://www.phonearena.com/news/Sony-Xperia-Lounge-shutting-down_id118252

	"com.sonymobile.xperiaxlivewallpaper"
	"com.sonymobile.xperiaxlivewallpaper.product.res.overlay"
	# Xperia Loops
	# Useless and ugly live wallaper from Sony.

	"com.sonymobile.xperiaservices" # [MORE INFO NEEDED]
	# Xperia services
	# I guess it provides things for Sony apps but I don't know what.
	# Safe to remove but it'd be to know what Sony apps work without it.

	"com.sonymobile.xperiatransfermobile"
	# Xperia Transfer Mobile (https://play.google.com/store/apps/details?id=com.sonymobile.xperiatransfermobile)
	# Helps you move your contacts, messages, photos, and much more from your old Android, iOS or Windows Phone device to your new Xperia from Sony.

	"com.sonymobile.xperiaweather"
	# Sony weather app (https://play.google.com/store/apps/details?id=com.sonymobile.xperiaweather)
	# Note : Not all location are supported.

	##############################  ADVANCED DEBLOAT ##############################

	#"com.sonyericsson.album"
	# Sony gallery app (https://play.google.com/store/apps/details?id=com.sonyericsson.album)

	#"com.sonyericsson.android.camera3d" # on older phones
	#"com.sonymobile.camera"
	# Sony camera app

	#"com.sonymobile.android.contacts"
	# Sony contacts

	# "com.sonymobile.home"
	# "com.sonymobile.launcher"
	# Sony launcher
	# It's basically the home screen, the way icons apps are organized and displayed.
	# DON'T REMOVE THIS IF YOU DIDN'T INSTALL ANOTHER LAUNCHER !
	# You normally only have one of these two in your phone

	)



