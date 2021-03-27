#!/usr/bin/env bash

declare -a xiaomi=(

	"android.autoinstalls.config.Xiaomi.cepheus"
	"android.autoinstalls.config.Xiaomi.dipper"
	"android.autoinstalls.config.Xiaomi.daisy"
	"android.autoinstalls.config.Xiaomi.cactus"
	# android.autoinstalls.config.Xiaomi.X where X is the phone's codename
	# Used to **auto** install stuff
	# IMO it's a similar feature than Play Auto Install (https://forum.xda-developers.com/xperia-z/help/how-stop-google-play-auto-install-t2590253)

	"android.romstats" # [MORE INFO NEEDED]
	# Misleading package name. This is a Xiaomi-only package.
	# Can someone provide the .apk?
	# Telemetry stuff

	"cn.wps.xiaomi.abroad.lite"
	# Mi Doc viewer
	# Documents (*.doc/docx, *.ppt/pptx, *.xls/xlsx, *.pdf, *.wps, and *.txt) viewer powered by WPS Office
	# FYI: WPS is a chinese closed-source software. It's as bad as Microsoft office (privacy-wise)
	# https://www.wps.com/privacy-policy

	"com.android.backup"
	# Xiaomi Backup and Restore feature (mislead package name).
	# This package was replaced by 'com.miui.backup' on newer models.

	"com.android.midrive"
	# Mi Drive 
	# Misleading package name. It is indeed a closed-source Xiaomi application.
	# Allow for cloud storage (on Mi Cloud) and syncing across multiple Android devices.

	"com.autonavi.minimap"
	# 高德地图 (Yeah no english translation) (https://play.google.com/store/apps/details?id=com.autonavi.minimap)
	# Xiaomi GPS

	"com.baidu.duersdk.opensdk"
	# Duer stuff from Baidu 
	# Duer is a virtual AI assistant.

	"com.baidu.input_mi"
	# Baidu IME (Baidu keyboard)
	# YOU SHOULD NEVER USE A CLOSED-SOURCE KEYBOARD ! 
	# https://www.techrepublic.com/blog/asian-technology/japanese-government-warns-baidu-ime-is-spying-on-users/
	# Archive : https://web.archive.org/save/https://www.techrepublic.com/blog/asian-technology/japanese-government-warns-baidu-ime-is-spying-on-users/

	"com.baidu.searchbox"
	# 百度 (https://play.google.com/store/apps/details?id=com.baidu.searchbox)
	# Baidu App search engine.

	"com.bsp.catchlog"
	# bsp = Board support package
	# Used to catch log files obviously.

	"com.duokan.phone.remotecontroller"
	# Mi Remote Controller (https://play.google.com/store/apps/details?id=com.duokan.phone.remotecontroller)
	# Control your electric appliances with your phone using Mi Remote.

	"com.duokan.phone.remotecontroller.peel.plugin"
	# Peel Mi Remote (https://play.google.com/store/apps/details?id=com.duokan.phone.remotecontroller.peel.plugin)
	# Peel Mi Remote is a TV guide extension for Xiaomi Mi Remote by "Peel Smart Remote".

	"com.facemoji.lite.xiaomi.gp"
	# Facemoji Keyboard Lite for Xiaomi - Emoji & Theme  (https://play.google.com/store/apps/details?id=com.facemoji.lite.xiaomi.gp)
	# Emoji keyboard

	"com.factory.mmigroup"
	# Hidden super-menu accessible by dialing *#*#64633#*#*
	# This menu lists all the others hidden test/debug apps.

	"com.fingerprints.sensortesttool"
	# Sensor Test Tool
	# Hidden test app used to test working of the fingerprint sensors.

	"com.huaqin.diaglogger"
	# Secret logging menu only accessible by dialing using a "secret code" (*#*#CODE#*#*)
	# You can use any of these code : "995995", "996996", "9434", "334334", "5959", "477477"
	# Used to log Bluetooth traffic and send them to com.miui.bugreport
	# Write logs to "/sdcard/diag_logs/" | "/sdcard/wlan_logs/" | "/sdcard/MIUI/debug_log/common/"
	#
	# FYI Huaqin is a Chinese mobile phone research and development company.

	"com.huaqin.factory"
	# Hidden test app (dial *#*#64663#*#*)
	# Used by technician in factory to test the hardware. Not intented to be run by end-users. 
	# Has a huge amount of permission.
	# A vulnerability was found in 2019 (CVE-2019-15340) allowing any app co-located on the device to 
	# programmatically disable and enable Wi-Fi, Bluetooth, and GPS silently (and without the corresponding access permission)
	# https://nvd.nist.gov/vuln/detail/CVE-2019-15340

	"com.huaqin.sar" # [MORE INFO NEEDED]
	# SetTransmitPower
	# I can't access the apk but I'm pretty sure it is another hidden test app not meant to be used by end-user
	# Given its name it could be used to adjust the transmit power of the cell phone antennas
	# SAR = Specific Absorption Rate (https://en.wikipedia.org/wiki/Specific_absorption_rate)
	# XDA users removed this without any issues. To be 100% sure it would be good to test the SAR without this package (just in case)

	"com.milink.service"
	# UniPlay Service
	# MIUI screen casting service. 
	# If removed, you'll have to use Android's native casting services which can be accessed through a 3rd party app.

	"com.mipay.wallet"
	"com.mipay.wallet.id"
	"com.mipay.wallet.in"
	# Mi Pay (https://play.google.com/store/apps/details?id=com.mipay.in.wallet)
	# Contactless NFC-based mobile payment system that supports credit, debit and public transportation cards in China.
	# https://www.mi-pay.com/
	#
	# .in = Mi Pay for India
	# .id = My Pay for Indonesia

	"com.miui.accessibility"
	# Mi Ditto
	# Accesibility feature. Dictation (TTS) and speech output, 
	# making mobile devices more convenient for people who have difficulties using conventionally designed smartphones. 

	"com.miui.audioeffect"
	# AudioEffect from Xiaomi (https://developer.android.com/reference/android/media/audiofx/AudioEffect)
	# Used by the equalizer (to be confirmed)

	"com.miui.cit"
	# Hardware tests
	# Secret codes (https://nitter.net/fs0c131y/status/933353182956326913#m) let you run hardware tests.
	# https://c.mi.com/thread-1744085-1-0.html

	"com.miui.cloudservice" 
	# Mi Cloud Service
	# NOTE : Settings will crash when pressing on any "Mi Cloud" button if this package is deleted.

	"com.miui.huanji"
	# Mi Mover (https://play.google.com/store/apps/details?id=com.miui.huanji)
	# Lets you transfer your contacts, messages, personal files, all the installed apps (but not their data) 
	# and all the settings (app + system) from an android phone to a Xiaomi phone.
	# The 2 phones will establish a direct wifi connection.

	"com.miui.enbbs" 
	# Xiaomi Forums old package.
	# Now com.mi.global.bbs.

	"com.miui.greenguard"
	# Security Guard Service
	# The app includes three different antivirus brands built in that the user can choose from to keep their phone protected: Avast, AVL and Tencent. 
	# Upon selecting the app, the user selects one of these providers as the default Anti-Virus engine to scan the device.
	# It the app that scan an app before installing it
	# NOTE : A vulnerability was found in 2019 : https://research.checkpoint.com/2019/vulnerability-in-xiaomi-pre-installed-security-app/

	"com.miui.hybrid"
	# Quick Apps
	# It's basically an app which shows you ads and tracks you...
	# Funny thing, Xiaomi's Quick Apps was reportedly being blocked by Google Play Protect.
	# https://www.androidpolice.com/2019/11/19/xiaomi-quick-apps-flagged-blocked-google-play-protect/
	#
	# Reverse engineering of the app : 
	# https://medium.com/@gags.gk/reverse-engineering-quick-apps-from-xiaomi-a1c9131ae0b7
	# Spoiler : you really should delete this package.

	"com.miui.hybrid.accessory"
	# Xiaomi Hybrid Accessory
	# Smartphone accessories support for Quick Apps (com.miui.hybrid)

	"com.miui.micloudsync"
	# Mi Cloud Sync
	# Needed for Cloud syncronisation

	"com.miui.miwallpaper"
	# Mi Wallpaper 

	"com.miui.nextpay" # [MORE INFO NEEDED]
	# Next Pay 
	# ???

	"com.miui.qr"
	# MUI Qr code scanner

	"com.miui.smsextra"
	# Dependency for MIUI Messaging (MIUI SMS app misleadingly called com.android.mms)
	# You can remove it if you don't use the default SMS app (and you should)
	# Run in background once the phone is booted, has access to internet and interact with Cloud Manager

	"com.miui.touchassistant" 
	# Quick Ball/Touch Assistant
	# Touch assistant with a combination of five unique shortcuts which aimed to give easy and quick access to functions and apps you use frequently.

	"com.miui.translation.xmcloud"
	# Translation stuff. Does not impact global translation for non-chinesse users.

	"com.miui.translationservice"
	# Translation stuff. Does not impact global translation for non-chinesse users.

	"com.miui.userguide"
	# Xiaomi User guide

	"com.miui.analytics" 
	# Xiaomi Analytics
	# This app is shady. According to a guy who try to reverse engineered the app, Xiaomi Analytics can replace any (signed?) package 
	# they want silently on your device within 24 hours. Maybe that no longer the case now but... you don't want analytics anyway.
	# Source : http://blog.thijsbroenink.com/2016/09/xiaomis-analytics-app-reverse-engineered/

	"com.miui.android.fashiongallery"
	# Mi Wallpaper Carousel (https://play.google.com/store/apps/details?id=com.miui.android.fashiongallery)
	# Display new photos on your lock screen every time you turn ON your screen.

	"com.miui.antispam" # [MORE INFO NEEDED]
	# MIUI Antispam 
	# spam phone numbers filter (blacklist).
	# Suspicious analytics inside and has access to internet. Cloud backup possible.
	# At quick glance it is not a private antispam app.
	# Can someone check what data are collected/transfered?

	#"com.miui.backup"
	# MIUI Backup
	# Local Backup/Restore feature (Settings > Additional Settings > Local backups)
	# It seems this app can communicate with Mi Drop
	# This app has 73 permissions and can obviously do everything it want.

	"com.miui.bugreport" 
	# Mi Feedback
	# Used to seend bug report to devs

	"com.miui.cleanmaster"
	# Mi Cleaner
	# Clean useless files

	"com.miui.cloudbackup"
	# Mi Cloud backup
	# Needed for Xiaomi cloud backup.

	"com.miui.cloudservice"
	# Mi Cloud Services needed for Mi Cloud

	"com.miui.cloudservice.sysbase"
	# Another Mi Cloud dependency 

	"com.miui.compass" 
	# Mi Compass
	# I think you understand its purpose...

	"com.miui.contentcatcher"
	# Application Extension Service	
	# I don't have a Xiaomi device so I can't test. A lot of people delete this package but I'd like to know its purpose.
	# IMO it's related to web browsing from a xiaomi app

	"com.miui.daemon"
	# MIUI daemon
	# Collect a lot of data and send them to China.
	# See : https://nitter.net/fs0c131y/status/938872347087564800?lang=en

	"com.miui.fm"
	# MIUI FM Radio app

	"com.miui.fmservice"
	# FM Radio Service
	# Needed by com.miui.fm to work correctly

	"com.miui.gallery"
	# MIUI Gallery app.
	# Simple Gallery is way better, ligther and open-source (https://f-droid.org/en/packages/com.simplemobiletools.gallery.pro/)

	"com.miui.klo.bugreport"
	# KLO Bugreport
	# This app registers system failures and Android applications errors and sends bugs to Xiaomi servers.

	"com.miui.miservice"
	# Services & feedback
	# Used to send feedbacks (and data) to Xiaomi. Integration in Wechat
	# Seems to be able to launch 'Baidu location service'
	# Has too much permisions, run in background all the time and can be removed without issue

	"com.miui.msa.global"
	# Main System Ads
	# Analyzation of user behaviors to show you ads. Yeah Xiaomi phones has ads...
	# https://www.theverge.com/2018/9/19/17877970/xiaomi-ads-settings-menu-android-phones

	"com.miui.notes" 
	# Mi Notes

	"com.miui.personalassistant"
	# Seems to be App Vault on some phones (https://play.google.com/store/apps/details?id=com.mi.android.globalpersonalassistant)
	# https://c.mi.com/thread-1017547-1-0.html

	"com.miui.phrase" # [MORE INFO NEEDED]
	# Frequent Phrases
	# Not sure to understand how exactly it can be used but it is supposed to predict phrases you'll want to write.
	# I don't know why it isn't handled in the keyboard app. This seems to be something else.
	# In any case it has access to internet, is linked to MiCloud and contains a weird CloudTelephonyManager java class in his code.

	"com.miui.player" 
	# Mi Music (https://play.google.com/store/apps/details?id=com.miui.player)

	"com.miui.providers.weather"
	# Xiaomi provider for MI Weather app (com.miui.weather)
	# REMINDER : Content providers helps an application manage access to data stored by itself, stored by other apps, 
	# and provide a way to share data with other apps. They encapsulate the data, and provide mechanisms for defining data security
	# Source: https://developer.android.com/guide/topics/providers/content-providers.html

	"com.miui.screenrecorder" 
	# Mi Screen Recorder

	"com.miui.spock"
	# Analytics app who constantly run in background.
	# Sends indentifiable data to Xiaomi servers
	# See https://www.virustotal.com/gui/file/70400d0055e1924966fb8367cafddc175dee914bbdc227342c9dd86fb3aa829f/details
	# It leaks system version, device model, exact firmware build + some few mysterious IDs

	"com.miui.systemAdSolution"
	# Spyware who analyse user behavior for targeted ads. Yeah Xiaomi phones has ads...
	# https://www.theverge.com/2018/9/19/17877970/xiaomi-ads-settings-menu-android-phones

	"com.miui.sysopt"
	# SysoptApplication
	# Strange app with no permission. By looking at the code it seems to be a kind of debug app.
	# The app doesn't seem to do intersting stuff.

	"com.miui.translation.kingsoft"
	# Translation stuff by Kingsoft (https://en.wikipedia.org/wiki/Kingsoft)

	"com.miui.translation.youdao"
	# Translation stufff by Youdao (https://en.wikipedia.org/wiki/Youdao)

	"com.miui.video"
	# IMO it's needed by com.miui.videoplayer (confirmation needed)

	"com.miui.videoplayer" 
	# Mi Video (https://play.google.com/store/apps/details?id=com.miui.videoplayer)

	"com.miui.videoplayer.overlay"
	# Mi Video overlay
	# REMINDER : A screen overlay in Android, also referred to as “Draw On Top”, allows an app to display content over another app

	"com.miui.virtualsim"
	# Mi Roaming
	# It enables users to connect to roaming data on-demand via virtual SIM technology.
	# https://alertify.eu/xiaomi-mi-roaming/

	"com.miui.vsimcore"
	# Virtual Sim core service

	"com.miui.miwallpaper.earth"
	"com.miui.miwallpaper.mars"
	# SuperWallpaperEARTH / SuperWallpaperMARS
	# Live/animated Xiaomi wallaper

	"com.miui.newmidrive"
	# Mi Drive (Chinese version)
	# Lets you upload and sync your files on the (Mi) Cloud.
	# Always run in background

	"com.wapi.wapicertmanager"
	# WAPI Certificates Manager
	# WAPI = WLAN Authentication Privacy Infrastructure (https://en.wikipedia.org/wiki/WLAN_Authentication_and_Privacy_Infrastructure
	# It was designed to replace WEP and become the new Standard but it was't rejected by the ISO (International Organization for Standardization)
	# It is currently only used in China
	# This app most likely manage certificates (they are used to make sure you're not connecting to a rogue Access Point)
	# Note: If you live in China, you most likely want to keep it.

	"com.miui.weather2"
	# Mi Weather app

	"com.miui.yellowpage"
	# Yellow Page from MIUI.
	# REMINDER : Yellow pages contain phone numbers of companies and services. They are provided by Xiaomi partners or businesses themselves.

	"com.mfashiongallery.emag"
	# Wallpapers by Xiaomi

	"com.mi.android.globalpersonalassistant" 
	# MI Vault aka the "assistant" you open swiping left from MI Home

	"com.mi.android.globalminusscreen"
	# App Vault (https://play.google.com/store/apps/details?id=com.mi.android.globalminusscreen)
	# Google Feed replica from Xiaomi
	# Completely useless app which displays all the trending stories from the web + a bunch of other stupid things.

	"com.mi.AutoTest"
	# Assemble test
	# Hidden app used by the manufacturer to test various hardware components

	"com.xiaomi.mi_connect_service"
	# MiConnectService
	# Handles connection to IoT stuff
	# Seems to be linked to Mi Home (com.xiaomi.smarthome)

	"com.mi.global.bbs"
	# Mi Community (https://play.google.com/store/apps/details?id=com.mi.global.bbs)
	# Xiaomi Forum app

	"com.mi.global.shop"
	# Mi Store (https://play.google.com/store/apps/details?id=com.mi.global.shop)
	# Xiaomi app store

	"com.mi.globalTrendNews"
	# Can't find info about this package
	# Probably used for displaying (useless) news

	"com.mi.health"
	# Mi Health
	# Pedometer, menstrual and sleep tracker
	# Your data are synchronized in the cloud. 
	# Do you really want Xiaomi to know you didn't slept much yesterday (your ovulation day btw...)

	"com.mi.liveassistant"
	# Mi Live Assistant
	# I don't really know what it is. Maybe an old name for "com.mi.android.globalpersonalassistant"

	"com.mi.setupwizardoverlay"
	# Weird package related to the SetupWizard (the menu which assists you to setup your phone for the first time)
	# A user said he needed to remove this package to be able to properly apply a dark theme to the Settings app.

	"com.mi.webkit.core"
	# MI WebView
	# Xiaomi alternative to Google WebView
	# REMINDER : It is a system component for the Android operating system that allows Android apps to display content 
	# from the web directly inside an application. It's based on Chrome.
	# On open-source privacy oriented Webview is Bromite (https://www.bromite.org/system_web_view)

	"com.qiyi.video"
	# IQIYI (https://play.google.com/store/apps/details?id=com.qiyi.video_US)
	# Online video platform from Baidu (https://en.wikipedia.org/wiki/IQiyi).
	# I didn't know this is currently one of the largest online video sites in the world, 
	# with nearly 6 billion hours spent on its service each month, and over 500 million monthly active users.

	"com.sohu.inputmethod.sogou.xiaomi"
	# Sogou keyboard for chinese only.

	"com.wt.secret_code_manager"
	# Hidden app which associates an action (display logging info) to a secret code.
	# This secret codes have to be dialed from the Xiaomi dialer.

	"com.xiaomi.ab"
	# MAB 
	# Has a LOT of permissions. If you try to desinstall it, Xiaomi will reinstall after reboot.
	# https://thoughtarama.wordpress.com/2017/05/09/mab-fucker-or-why-im-giving-up-my-xiaomi-redmi-note-3-phone/
	# Mab is a part of MIUI Analytics.

	"com.xiaomi.account" 
	# Mi Account

	"com.xiaomi.channel"
	# Mi Talk 
	# Mi instant messaging app that lets you do practically the same thing as Whatsapp. 
	# NOTE: You should use Signal or Wire instead Whatsapp/Mi Talk for more privacy.

	"com.xiaomi.gamecenter.sdk.service"
	# Game Service
	# Surely used to "improve" game performance

	"com.xiaomi.joyose"
	# Joyse Analytics and advertising
	# Run in background and can't be stopped. 

	"com.xiaomi.jr"
	# Help you getting loans when shopping.

	"com.xiaomi.lens"
	# Related to camera app ?
	# Safe to remove (according to a lot of users)
	# I'd like to have more info about it. Can a Xiaomi user help ? 

	"com.xiaomi.midrop" 
	# Share Me (Mi Drop) (https://play.google.com/store/apps/details?id=com.xiaomi.midrop)
	# P2P file transfer tool.

	"com.xiaomi.midrop.overlay"
	# Mi Drop overlay
	# REMINDER : A screen overlay in Android, also referred to as “Draw On Top”, allows an app to display content over another app

	"com.xiaomi.mipicks" 
	# Mi Picks (becomed Mi Apps Store and now Get Apps -- Xiaomi app store)
	# I believe this package is discontinued.
	# https://play.google.com/store/apps/details?id=com.mi.global.shop

	"com.xiaomi.o2o"
	# o2o = online-to-offline
	# ==> Describe systems enticing consumers within a digital environment to make purchases of goods or services from physical businesses.
	# https://en.wikipedia.org/wiki/Online_to_offline
	# NOTE : This package can make phone calls without user intervention.

	"com.xiaomi.pass"
	# Mi Pass is an App allows Xiaomi NFC phones to replace cards and keys in real life usage. 
	# Support NFC payment, bus card, key card, door and car lock features all together.

	"com.xiaomi.payment"
	# Old package name for Mi Credit (https://play.google.com/store/apps/details?id=com.micredit.in.gp)
	# Mi Credit is a personal loan platform from Xiaomi.

	"com.xiaomi.scanner" 
	# Mi Scanner
	# Lets you scan documents, crop, adjust grayscale and OCR.

	"com.xiaomi.shop"
	# Xiaomi app store (I thinks it's discontinued)
	# Now com.mi.global.shop (https://play.google.com/store/apps/details?id=com.mi.global.shop)

	"com.xiaomi.vipaccount"
	# Xiaomi VIP account
	# https://www.mi.com/in/service/privilegefaq/

	"com.xiaomi.glgm"
	# Xiaomi Games
	# Not sure if this app still exists.

	"com.xiaomi.micloud.sdk"
	# Mi Cloud sdk 
	# sdk = Software development kit
	# Seems to be a dependency for "com.miui.gallery" (the MIUI may not work if you remove this package)

	"com.xiaomi.mirecycle"
	# Mi Recycle 
	# Xiaomi has extended its partnership with Cashify to launch the 'Mi Recycle' feature through its MIUI Security app. 
	# It will let Xiaomi phone users check the health of their smartphone and get their resale value directly from Cashify, 
	# the online re-commerce company based out of New Delhi.
	# Source : https://gadgets.ndtv.com/mobiles/news/xiaomi-mi-recycle-cashify-miui-security-app-2018024

	"com.xiaomi.oversea.ecom"
	# Xiaomi ShopPlus.
	# Given its name I think this package is useless.

	"com.xiaomi.providers.appindex"
	# Provider for indexing app ? 
	# I believe it is a provider for the settings but I can't confirm (I don't have a Xiaomi device)
	# A lot of people debloat this but I'd like to know more about this one.

	"com.xiaomi.upnp"
	# UpnpService
	# UPnP = Universal Plug and Play
	# It’s a protocol that lets UPnP-enabled devices on your network automatically discover and communicate with each other
	# For exemple it works with the Xiaomi Network Speaker (and probably a lot more Xiaomi IoT stuff)
	# UPnP have a lot of security issue and you proably should disable it on your router.
	# https://nakedsecurity.sophos.com/2020/06/10/billions-of-devices-affected-by-upnp-vulnerability/
	# This package is the Xiaomi implementation on Android (no AOSP support)

	"com.xiaomi.simactivate.service"
	# Xiaomi SIM Activation Service
	# Only used to activate the Find My Device feature
	# For the activation to work you need to send a international SMS to China.
	# Your carrier may block this by default and/or you'll probably need to pay extra for this.

	"com.xiaomi.smarthome"
	# Mi Home (https://play.google.com/store/apps/details?id=com.xiaomi.smarthome)
	# IoT. Lets you control with Xiaomi Smart Home Suite devices.

	"com.xiaomi.xmsfkeeper"
	# Xiaomi Service Framework Keeper
	# Logger service for 'com.xiaomi.xmsf'

	##################################  ADVANCED DEBLOAT  ##################################

	#"android.ui.overlay.ct" # [MORE INFO NEEDED]
	#"android.telephony.overlay.cmcc" 
	#"com.android.mms.overlay.cmcc"
	#"com.android.settings.overlay.cmcc"
	#"com.android.systemui.overlay.cmcc"
	#"com.android.networksettings.overlay.ct"
	#"com.android.systemui.overlay.ct"
	# Very likely to be a bunch of overlay theme from the China Mobile Communications Corporation (CMCC) / China Telecom (CT)
	# Can someone remove them and see what exactly happens?

	#"com.miui.wmsvc" # [MORE INFO NEEDED]
	# WMService
	# Run at boot, has access to internet + GPS
	# I quickly looked at the decompiled code and I saw some unsanitized SQL inputs which is BAD! (vulnerable to SQL injection)
	# Try to get your android unique Google advertising ID from Google Play Services.
	# Feed and launch the spying/analytics app "com.miui.hybrid"
	# This app doesn't seems to do essential things except for tracking.
	# WARNING: Some people said removing this app causes bootloop, others said it's not. 
	# I'd like someone to check this. I think it should be okay if you remove all other linked Xiaomi crapwares (like the script does)

	#"com.xiaomi.xmsf" # [MORE INFO NEEDED]
	# Xiaomi Service Framework
	# Set of API's that Xiaomi apps can used (to put it simply a lot of Xiaomi apps used the same functions which are centralized here)
	# I first thought removing this will absolutely break everything but it seems not.
	# I don't know the situation now but in 2016 this app constantly tried to do tcp connections in background
	# Removing this big boy will definitively break Mi Cloud and Mi account (and all features needing these 2 things) but you should
	# be okay if you don't use most of Xiaomi apps (what's probably the case if you use this script)
	# 
	# Can someone try to remove this and give feedback? 
	# Check if alarms (from Xiaomi Clock & 3-party apps) still work if the phone is in sleep-mode.

	"com.wingtech.standard" # [MORE INFO NEEDED]
	# WTStandardTest
	# Wingtech is a chinese Original Design Manufacturer (ODM) involved in the manufacturing of Xiaomi devices.
	# There is very high chances this app is only a hardware conformance test app used during production process
	# you don't need as an end-user.
	# Can someone share the apk just to be 100% sure?

	#"com.xiaomi.location.fused" # [MORE INFO NEEDED]
	# FusedLocationProvider
	# It uses a combination of a device’s GPS, Wi-Fi and internal sensors to improve geolocation performance.
	# The thing is there is also a Fused Location Provider embeded in 'com.google.android.gms'
	# This Xiaomi location provider obviously has as much tracking as the Google one but if you can remove one tracking source
	# it's still better than nothing.
	# Can someone try to remove this package and give feedback please?

	#"com.android.browser"
	# Mi Browser
	# You really should use something else.
	# FYI https://www.xda-developers.com/xiaomi-mi-web-browser-pro-mint-collecting-browsing-data-incognito-mode/

	#"com.android.camera"
	#"com.android.camera2" 
	# Xiaomi Camera (I don't know why they kept this package name. It's really confusing.)
	# It's a proprietary app based on the AOSP sources:
	# https://android.googlesource.com/platform/packages/apps/Camera2/+/master/src/com/android/camera

	#"com.android.contacts" 
	# MIUI Contacts and dialer
	# It's the contacts app but you can access the dialer from this app.

	#"com.android.email" 
	# Xiaomi closed-source email app based on the AOSP version. Really confusing package name.

	#"com.android.fileexplorer"
	# Xiaomi/Mi File Explorer (Again it's a really poor choice for a package name considering it is not the AOSP File explorer)
	# It's a Closed-source app based on the AOSP version.

	#"com.android.gallery3d" 
	# Xiaomi Gallery app (I'll stop repeating this is a really poor choice for the package name...)

	#"com.android.globalFileexplorer" 
	# Misleading package name. It's the Xiaomi Files Manager on older phones

	#"com.android.incallui"
	# Xiaomi Phone (Here we go again! Another confusing package name)
	# Closed-source app built on top of the AOSP package.
	# The name is doubly misleading because this package is the whole dialer. It does not only provide the 'in call' screen.

	#"com.android.thememanager" # [MORE INFO NEEDED]
	# MIUI Themes (manager)
	# Xiaomi seems to love confusing package name
	# This package lets you select and apply themes provided by Xiaomi. 
	# There is a strong likelihood that removing this package will disable the ability to change wallpapers. 
	# Can someone test?

	#"com.android.thememanager.module" # [MORE INFO NEEDED]
	# I don't have the .apk but it is obviously related to "com.android.thememanager"
	# Can someone test with this package too?

	#"com.fido.asm"
	# FIDO UAF Autenthicator-Specific Module.
	# See 'com.huawei.fido.uafclient' for FIDO explaination.
	# The UAF Authenticator-Specific Module (ASM) is a software interface on top of UAF authenticators which gives a standardized way for FIDO UAF clients 
	# to detect and access the functionality of UAF authenticators and hides internal communication complexity from FIDO UAF Client.
	# Source: https://fidoalliance.org/specs/fido-uaf-v1.0-ps-20141208/fido-uaf-asm-api-v1.0-ps-20141208.html
	
	#"com.fido.xiaomi.uafclient"
	# UAF client for FIDO.
	# Fido is a set of open technical specifications for mechanisms of authenticating users to online services that do not depend on passwords.
	# https://fidoalliance.org/specs/u2f-specs-1.0-bt-nfc-id-amendment/fido-glossary.html
	# https://fidoalliance.org/specs/fido-v2.0-rd-20170927/fido-overview-v2.0-rd-20170927.html
	#
	# The UAF protocol is designed to enable online services to offer passwordless and multi-factor security by allowing users to register their device 
	# to the online service and using a local authentication mechanism such as iris or fingerprint recognition. .
	# https://developers.google.com/identity/fido/android/native-apps
	# Safe to remove if you don't use password-less authentification to access online servics.

	#"com.miui.audiomonitor" # [MORE INFO NEEDED]
	# My guess is this is a feature allowing to control the sound of multiples apps.
	# It's just a guess based on existing Xiaomi devices features. Can someone check this? 

	#"com.miui.calculator" 
	# MIUI Calculator (https://play.google.com/store/apps/details?id=com.miui.calculator)

	#"com.miui.face"
	# MIUI Biometric
	# Face Unlock feature

	#"com.miui.freeform"
	# Floating window
	# I think the name app is pretty straightforward
	# You can make apps appear above other applications
	# https://forum.xda-developers.com/android/miui/floating-windows-miui-12-t4125661

	#"com.miui.home"
	# MIUI System Launcher
	# It's basically the home screen, the way icons apps are organized and displayed.
	# DON'T REMOVE THIS IF YOU DIDN'T INSTALL ANOTHER LAUNCHER !

	#"com.mi.globallayout" # [MORE INFO NEEDED]
	# Home Layout
	# It most likely handles the main screen layout (grid size, apps placement...)
	#
	# Some people removed this without issue. Can someone try and give feedback?

	#"com.miui.mishare.connectivity"
	# Mi Share
	# Unified file sharing service between Xiaomi, Oppo, Realme and Vivo devices using Wifi-direct
	# Settings -> Connection & sharing -> Mi Share
	# FYI : Wifi direct allows 2 devices to establish a direct Wi-Fi connection without requiring a wireless router.

	#"com.miui.misound"
	# Earphones (it's the name of the app)
	# Provides the sound's section in Settings and is needed for the equalizing
	# Some people removed this package but I personaly don't think it's worth it. This package isn't really an issue
	# (no dangerous permissions and does not run in background all the time)
	# You can still remove it. You'll be just fine if you really don't need it.

	#"com.miui.notification" # [MORE INFO NEEDED]
	# Handles notifications in MIUI (badly according to reviews). Does it only handles notifications for Xiaomi apps?
	# Embeds a tracking statistics service
	# (usage tracking : `id`,`pkgName`,`latestSentTime`,`sentCount`,`avgSentDaily`,`avgSentWeekly)
	# Can someone remove this package just to see if it breaks all notifications for all apps? It doesn't seem so.
	# If you try don't forget to check if the settings app still works fine.

	#"com.miui.powerkeeper" # [MORE INFO NEEDED]
	# Battery and Performance
	# (aggressive) MIUI power management (https://dontkillmyapp.com/xiaomi)
	# That's a weird app that also contains a DRM Manager and a service related to Cloud Backup
	# Has obviously a lot of dangerous permissions.
	# I guess removing this package will decrease the battery performance. Is it that noticeable? Can someone try?

	#"com.miui.zman" # [MORE INFO NEEDED]
	# Mi Secure sharing
	# Provides an option in the settings of the Xiaomi Gallery to automatically remove location and metadata from images 
	# you want to share. This do not remove metadata of the picture in the gallery but only the shared copy.
	# There's also a "Secure sharing" watermark that shows up when you share photos on WeChat without metadata.
	# The question is does this really remove all EXIF tags? Can someone test?
	# This is a useful app anyway but do not forget that all your photos/vidoes taken with the Xiaomi camera are still geo-tagged 
	# (+ all others exif tags) by default. 
	# What you can do is at least revoke the GPS permission to the camera.
	# FOSS alternative to this app : 
	# https://f-droid.org/fr/packages/com.jarsilio.android.scrambledeggsif/
	# https://f-droid.org/fr/packages/de.kaffeemitkoffein.imagepipe/

	#"com.mi.android.globalFileexplorer"
	# Xiaomi Files Manager (https://play.google.com/store/apps/details?id=com.mi.android.globalFileexplorer)

	#"com.xiaomi.bluetooth"
	#"com.xiaomi.bluetooth.overlay"
	# MIUI Bluetooth Bluetooth Control. 
	# You need to keep this if you want the bluetooth to work 	

	#"com.xiaomi.bsp.gps.nps"
	# GPS location
	# I think bsp = board system package (https://en.wikipedia.org/wiki/Board_support_package)
	# Not sure about nps (It might be Non-Permanent GPS station)
	# It's a small package which seems to display a notification when an app is using GPS.
	# More precisely, there is a receiver (GnssEventReceiver) which listen to com.xiaomi.bsp.gps.nps.GetEvent 
	# This event most likely happen when an app use the GPS and refers to the state of the communication with the GNSS:
	# FIX, LOSE, RECOVER, START, STOP
	# It's safe to remove if you really want to.

	#"com.miui.core" # [MORE INFO NEEDED]
	# MIUI SDK
	# It is obiously needed for MIUI to work correctly. FYI, it manages the MIUI Analytics service.
	# Will cause bootloop if removed.
	# I read you can freeze it without issue. I'm... a bit dubious about this.
	# If someone want to try et report the result:
	# adb shell am force-stop com.miui.core && adb shell pm disable-user com.miui.core && adb shell pm clear com.miui.core

	#"com.miui.guardprovider"
	# Guard Provider security app
	# The app includes 3 different antivirus brands built in that the user can choose (Avast, AVL and Tencent). 
	# This app notably perform a virus scan of any apps you want to install. 
	# A serious vulnerability was found in 2019
	# Worth reading : https://research.checkpoint.com/2019/vulnerability-in-xiaomi-pre-installed-security-app/
	# You may want to remove this app from a privacy stance.

	#"com.miui.systemui.carriers.overlay"
	# MIUI User interface for MCC/MNC configuration

	#"com.miui.systemui.devices.overlay"
	#"com.miui.systemui.overlay.devices.android"
	# MIUI User interface for 'device' settings?

	#"com.xiaomi.discover"
	# System Apps Updater
	# WARNING : Disable System app updates (but not firmware updates)

	#"com.xiaomi.powerchecker"
	# Power Detector 
	# Security> Battery> Activity Control. 
	# Detects abnormal power usage by apps (not all. Some Xiaomi apps are whitelisted)
	# Needed for 'com.miui.powerkeeper' to work.

	#"com.xiaomi.miplay_client" # [MORE INFO NEEDED]
	# MiPlay Client
	# Provides support for Miracast?
	# https://en.wikipedia.org/wiki/Miracast
	# My guess is it provides the Wireless Display feature (Settings - Connection & sharing - Cast)
	# Can someone confirm?
	)


##################### YOU SHOULDN'T MESS WITH THEM (core packages and may cause bootloop)  #####################

#"com.android.updater"
# Mi Updater
# Provide system updates
# REMOVING THIS WILL BOOTLOOP YOUR DEVICE!

#"com.lbe.security.miui"
# Permission manager
# Lets you monitor apps permission requests.

#"com.xiaomi.finddevice"
# Find My Device feature (in the Settings)
# Enables you to locate your lost phone and erase your data remotely. 
# Your phone needs to be connected to internet (Wifi/mobile data) for this feature to work. 
# REMOVING THIS PACKAGE WILL BOOTLOOP YOUR DEVICE!
# 
# NOTE : You cannot disable it via adb
# According some sources, disabling MIUI optimizations in the Developer
# settings and removing the apk file in a custom recovery does not cause a
# bootloop, but I didn't test this.

#"com.miui.global.packageinstaller"
# The security check / virus scan which runs after a package installation
# Uninstalling it does not cause a bootloop
# Package installation still works fine

#"com.miui.securitycenter"
# MIUI Security app
# Provides "protection and optimization tools" 
# App lock, Data usage, Security scan, Cleaner, Battery saver, Blocklist and other features.
# This package is mostly the front-end (UI).
# REMOVING THIS WILL BOOTLOOP YOUR DEVICE!
# 
# NOTE : I don't have a Xiaomi phone on hand anymore but maybe only disabling it will work : adb shell 'pm disable-user com.miui.securitycenter'
# Can someone try?

#"com.miui.securitycore"
# Core features of the "com.miui.securitycenter"
# Removing com.miui.securitycenter will cause your device to bootlop so I guess you should not remove this package neither.
# (Can someone try just in case?)

#"com.miui.system"
# Called 'MIUI System Launcher' but it's not the launcher itself (com.miui.home is)
# This package is another core MIUI app you can't remove. It centralize a lot of default configuration values

#"com.miui.rom"
# Core package of MIUI
# DO NOT REMOVE THIS

#"com.miui.securityadd"
# Related to the MIUI Security app
# REMOVING THIS WILL BOOTLOOP YOUR DEVICE!
# 
# NOTE : I don't have a Xiaomi phone on hand anymore but maybe only disabling it will work : adb shell 'pm disable-user com.miui.securityadd'
# Can someone try ?

#"com.xiaomi.misettings"
# Xiaomi Settings app

