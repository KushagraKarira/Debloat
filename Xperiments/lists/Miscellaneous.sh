#!/usr/bin/env bash

# FYI : Exodus (https://reports.exodus-privacy.eu.org/en/) lets you see all permissions and trackers in Google Play Store apps. 

# HELP ME : I got mad seeking for the meaning of VPL. A lot of people use it on Reddit (https://www.reddit.com/r/Bestbuy/search/?q=vpl&restrict_sr=1)
# and I'm sure it's the good abbreviation but I don't know what V.P.L means !! It seems to refer to a special job for vendors.

declare -a amazon=(
	"com.amazon.appmanager"
	# Mobile Device Information Provider
	# Seems related to Kindle

	"com.amazon.avod.thirdpartyclient"
	# Amazon Prime Video (https://play.google.com/store/apps/details?id=com.amazon.avod.thirdpartyclient)
	# VOD service from Amazon.
	# https://en.wikipedia.org/wiki/Prime_Video

	"com.amazon.mShop.android"
	# Amazon Shopping (https://play.google.com/store/apps/details?id=com.amazon.mShop.android.shopping)

	"com.amazon.fv"
	# Amazon App suite. Provides access to Amazon digital content

	"com.amazon.kindle"
	# Amazon Kindle (https://play.google.com/store/apps/details?id=com.amazon.kindle)

	"com.amazon.mp3"
	# Amazon Music (https://play.google.com/store/apps/details?id=com.amazon.mp3)
	# Amazon streaming app

	"com.amazon.venezia"
	# Amazon AppStore

	"com.amazon.aa" 
	# Amazon Assistant app. Nice spyware ! 
	# Show you recoomanded products available on Amazon and price compare as you shop across the web.
	# NOTE : https://www.gadgetguy.com.au/amazon-assistant-spies-on-you/

	"com.amazon.aa.attribution"
	# Amazon Assistant Attribution. A spyware again !
	# Tracking tool. Allows sellers to measure the impact of media channels **off Amazon** on sales.
	# https://www.repricerexpress.com/amazon-attribution/

	"com.amazon.mShop.android.shopping"
	"com.amazon.mShop.android.shopping.vpl" # for VPL phones
	# Amazon Shopping (https://play.google.com/store/apps/details?id=com.amazon.mShop.android.shopping)
	# Same package as com.amazon.mShop.android.

	"com.amazon.clouddrive.photos"
	# Amazon Photos (https://play.google.com/store/apps/details?id=com.amazon.clouddrive.photos)

	"in.amazon.mShop.android.shopping"
	# Amazon India (https://play.google.com/store/apps/details?id=in.amazon.mShop.android.shopping)

	)

declare -a facebook=(
	# Facebook packages always run in background and send data to Facebook even if you don't have a Facebook account.

	"com.facebook.katana"
	# Facebook app (https://play.google.com/store/apps/details?id=com.facebook.katana)

	"com.facebook.system" 
	# Facebook App Installer (empty shell app which incite you to install the Facebook app)

	"com.facebook.appmanager"
	# Facebook app manager handles Facebook apps updates.

	"com.facebook.services"
	# Facebook Services is a tool that lets you manage different Facebook services automatically using your Android device. 
	# In particular, the tool focuses on searching for nearby shops and establishments based on your interests.
	# I don't know if this a dependency for com.facebook.katana but nobody cares because we all want to delete all the Facebook stuff right ?!!

	"com.facebook.orca"
	# Facebook Messenger (https://play.google.com/store/apps/details?id=com.facebook.orca)

	"com.instagram.android"
	# Instagram  (https://play.google.com/store/apps/details?id=com.instagram.android)

	"com.whatsapp"
	# Whatsapp (https://play.google.com/store/apps/details?id=com.whatsapp)
	)

declare -a microsoft=(
	"com.microsoft.skydrive"
	# Microsoft OneDrive (Cloud) (https://play.google.com/store/apps/details?id=com.microsoft.skydrive)

	"com.skype.raider"
	# Skype (https://play.google.com/store/apps/details?id=com.skype.raider)

	"com.microsoft.office.excel" # Excel
	"com.microsoft.office.word" # Word
	"com.microsoft.office.outlook" # Outlook
	"com.microsoft.office.powerpoint" # Powerpoint

	"com.skype.m2"
	# Skype Lite (https://play.google.com/store/apps/details?id=com.skype.m2)

	"com.microsoft.office.officehubhl"
	# Office Mobile hub (on Samsung Phone)
	# Includes the complete Word, PowerPoint, and Excel apps to offer a convenient office experience on the go.

	"com.microsoft.office.officehub"
	# Microsoft Office Mobile
	# Includes the complete Word, PowerPoint, and Excel apps to offer a convenient office experience on the go. 

	"com.microsoft.office.officehubrow"
	# Microsoft Mobile Office Beta

	"com.microsoft.appmanager"
	# Your Phone Companion - Link to Windows (https://play.google.com/store/apps/details?id=com.microsoft.appmanager)
	# Microsoft app for synchronising your phone with a W10 PC.

	"com.microsoft.translator"
	# Microsoft Translator app (https://play.google.com/store/apps/details?id=com.microsoft.translator)
	)

declare -a qualcomm=(
	#################################
	# CAF (Code Aurora Forum) is a place where source code is released by Qualcomm. 
	# These are Qualcomm's reference sources for their platform. This is what they provide to OEMs
	# and what nearly all OEMs base their software off of. Usually when Google starts working on an upcoming Android version, 
	# they'll merge CAF repo.
	# Packages below are installed by OEM when they need device-specific patches not merged in AOSP yet. 

	"com.caf.fmradio"
	# https://source.codeaurora.org/quic/la/platform/vendor/qcom-opensource/fm/tree/fmapp2/src/com/caf/fmradio

	"org.codeaurora.gps.gpslogsave"
	# Saves GPS logs.

	#"org.codeaurora.bluetooth"
	# https://source.codeaurora.org/quic/la/platform/vendor/qcom-opensource/bluetooth

	#"org.codeaurora.ims"
	# IMS is an open industry standard for voice and multimedia communications over packet-based IP networks (Volte/VoIP/Wifi calling)
	# This his package is needed for the Volte/VoIP/Wifi calling provided by your carrier.
	# This is not needed by messaging apps (Signal, Telegram, WhatsApp...)

	#################################
	# QTI = Qualcomm Technologies Inc
	# quicinc = Qualcomm Innovation Center, Inc.
	# Qualcomm is an American multinational semiconductor and telecommunications equipment company 
	# that designs and markets wireless telecommunications products and services.
	# Qualcomm tracking : https://forum.fairphone.com/t/telemetry-spyware-list-of-privacy-threats-on-fp3-android-9/55179/20

	"com.qti.qualcomm.datastatusnotification" # [MORE INFO NEEDED]
	# Maybe it displays data status notification (network data usage)
	# Asks for sending SMS permission.

	"com.qti.service.colorservice" # [MORE INFO NEEDED]
	# Don't know why but it uses mobile data.
	# It most likely does something to colors on your display. Can someone see the difference ? Is it accessbility feature ?
	# Needed for Blue screen feature ?

	"com.qti.confuridialer"
	# Conference URI dialer
	# Lets you call someone using SIP-URI/IMS (https://en.wikipedia.org/wiki/SIP_URI_scheme)
	# It's Voice over IP but using SIP (https://en.wikipedia.org/wiki/Session_Initiation_Protocol)
	# I can assure you that all your messaging apps doesn't use this. It is used by carriers for their wifi-calling stuff. 
	# This package in particular could be used if you activate the SIP call option from your dialer. 

	"com.qti.snapdragon.qdcm_ff" # [MORE INFO NEEDED]
	# Qualcomm Display Color Management tool
	# Works in background and "enhance" the display’s appearance through an intelligent color adjustment and gamut-mapping system 
	# "to make colors look vibrant and true to life".
	# Not really convinced. Can someone see the difference ? 
	# https://www.qualcomm.com/news/onq/2016/05/02/qualcomm-trupalette-brings-your-phones-display-life
	#
	# ff = FinFet ? (https://en.wikipedia.org/wiki/FinFET)

	"com.qualcomm.atfwd"
	# Qualcomm's WiFi display. It allows you to mirror your devices display on a TV 
	# Seems to be used by Mircast. Need to be confirmed tho.

	#"com.qualcomm.cabl"
	# Content Adaptative Backlight Settings
	# CABL will try to adjust the image being displaye by changing the contrast/quality/image backlight depending on 
	# the content on the screen.
	# Downside to this is loss of dynamic range which results in some colors being washed out/clipped.
	# CABL != Auto brightness (which doesn't change the content of the screen, only the brightness)
	# NOTE: You may want to remove this. It does not work very well on many phones
	# https://mobileinternist.com/disable-adaptive-brightness-android

	"com.qualcomm.embms"
	# Run in background.
	# I guess it add support to LTE Broadcast or eMBMS (evolved Multimedia Broadcast Multicast Service) 
	# Enables carriers to sends stuff using multicast (same content to be delivered to a large number of users at the same time) instead of LTE.
	# It is a more efficient use of network resources when compared to each user receiving the same content individually.
	# Personally I don't want my carrier to send me stuff.
	#
	# FYI : https://en.wikipedia.org/wiki/Multimedia_Broadcast_Multicast_Service
	# 	https://www.one2many.eu/en/lte-broadcast/what-is-embms

	#"com.qualcomm.fastdormancy"
	# Provide Fast Dormancy feature/setting in the dialer (reduce battery consumption and network utilization during periods of data inactivity) 
	# https://en.wikipedia.org/wiki/Fast_Dormancy

	"com.qualcomm.location"
	# May enable your device to determine its location more quickly and accurately, even when your device is unable to get a strong GPS signal. 
	# **May** also help your device conserve battery power when you use applications or services requiring location data
	# It will periodically downloads data to your device regarding the locations of nearby cellular towers and WiFi access points
	#
	# Qualcomm Location periodically sends a unique software ID, the location of your device (longitude, latitude and altitude, and its uncertainty) 
	# and nearby cellular towers and Wi-Fi hotspots, signal strength, and time (collectively, “Location Data”) to Qualcomm servers. 
	# As with any Internet communication, they also receive the IP address your device uses. 
	# https://www.qualcomm.com/site/privacy/services

	#"com.qualcomm.qcrilmsgtunnel" # [MORE INFO NEEDED]
	# Tunnel between modem and android framework. Related to SMS ? 
	# FYI : ril = Radio Interface layer. It's the bridge between Android phone framework services and the hardware.
	# There is no noticeable immediate consequences after disabling it but it'd better to know more about.

	"com.qualcomm.simcontacts" # [MORE INFO NEEDED]
	# Sim Contacts
	# Safe to remove.
 	# I don't exactly know what's the purpose of this package. Import/Export tool?

	#"com.qualcomm.svi" # [MORE INFO NEEDED]
	# SVI Settings
	# Sunlight Visibility Improvement
	# I'm pretty sure it relies on com.qualcomm.cabl (can someone confirm?) to improve content visibility in sunlight.
	# You need to activate the Sunlight Mode in the quick settings menu

	#"com.qualcomm.uimremoteserver" # [MORE INFO NEEDED]
	# UIM Remote Server
	# UIM = User Identiy Module
	# Given its name I don't think it is a mandatory and pertinent feature. Can someone test?

	#"com.qualcomm.wfd.service" # [MORE INFO NEEDED]
	# Wifi Display
	# Provides a way to cast your screen to your TV (support for Miracast)
	# https://en.wikipedia.org/wiki/Miracast

	"com.qualcomm.qti.auth.fidocryptoservice"
	# Qualcomm FIDO implementation. 
	# FIDO : https://en.wikipedia.org/wiki/FIDO_Alliance
	# Fido is a set of open technical specifications for mechanisms of authenticating users to online services that do not depend on passwords.
	# https://fidoalliance.org/specs/u2f-specs-1.0-bt-nfc-id-amendment/fido-glossary.html
	# https://fidoalliance.org/specs/fido-v2.0-rd-20170927/fido-overview-v2.0-rd-20170927.html

	"com.qualcomm.qti.autoregistration"
	# Collect device activation data to remotely activate a phone’s warranty
	# FYI : In 2019 this package was sending private data (IMEI, CELLID , CCID) in CLEAR text to zzhc.vnet.cn (chinese server). 
	# According to HMD (Nokia) it was a mistake : 
	# https://www.androidauthority.com/nokia-7-plus-user-info-967901/

	#"com.qualcomm.qti.callenhancement"
	# Supposed to enhance call quality (I let you test if it really does)
	# FYI : This app can record all your phone call
	# A vulnerability was found in 2019, allowing unauthorized microphone audio recording by 3rd-party apps.
	# https://nvd.nist.gov/vuln/detail/CVE-2019-15472

	"com.qualcomm.qti.callfeaturessetting" # [MORE INFO NEEDED]
	# Not mandatory (according to XDA users)
	# Can someone explain what feature does this package add?

	"com.qualcomm.qti.confdialer" # [MORE INFO NEEDED]
	# ConfDialer
	# LTE Conferencing Service
	# How to use this feature? It is nowhere explained.

	"com.qti.dpmserviceapp" # [MORE INFO NEEDED]
	# Usually, for android DPM = Device Policy Manager
	# But I found someone in a russian forum saying it's a service for playing digital rights protected (DRM) Content.
	# I decompiled the app but that didn't help me to understand what this package really do. There isn't much code and it often
	# logs stuff.
	# https://ru.c.mi.com/forum.php?mod=viewthread&tid=1861371

	"com.qualcomm.qti.networksetting"
	# Network operators (hidden settings menu)
	# Lets you select network modes like GSM only, WCDMA only, LTE only etc, toggle VoLTE On/Off...

	"com.qualcomm.qti.optinoverlay" # [MORE INFO NEEDED]
	# Overlay for something but what... 
	# (nothing useful in any case)

	#"com.qualcomm.qti.qms.service.trustzoneaccess"
	# Handles access to the Qualcomm/ARM Trustzone?
	# You may not need Qualcomm Trustzone if you don't used OEM trusted apps.
	# See com.trustonic.tuiservice

	"com.qualcomm.qti.perfdump"
	# Performance dump (logging)
	# Enable a more accurate overview of the running services (and maybe how much power/RAM they take?)

	"com.qualcomm.qti.qms.service.connectionsecurity" # [MORE INFO NEEDED]
	# Telemetry service
	# qms = quality management service
	# Background-Connection to tls.telemetry.swe.quicinc.com (Host/Domain belongs to Qualcomm)

	"com.qualcomm.qti.qms.service.telemetry" 
	# Telemetry service.
	# Yeah obviously it phones to Qualcomm.

	"com.qualcomm.qti.qtisystemservice" # [MORE INFO NEEDED]
	# Seems to only log stuff related to telephony
	# A user removed this without noticing any issues

	"com.qualcomm.qti.roamingsettings" # [MORE INFO NEEDED]
	# Hidden settings menu
	# Lets to tweak roaming settings (How to access this settings?)

	"com.qualcomm.qti.rcsbootstraputil"
	"com.qualcomm.qti.rcsimsbootstraputil" # [MORE INFO NEEDED]
	# RCS Service 
	# RCS = Rich Communication Services. 
	# RCS is a communication protocol between mobile telephone carriers and between phone and carrier, aiming at replacing SMS.
	# https://en.wikipedia.org/wiki/Rich_Communication_Services
	# Note that it uses IP protocol so you need to connect to Wifi/3G/4G...  to take advantage of it.
	#
	# It's a hot mess right now. It aims at being universal but you still need to have a Samsung Messages 
	# or Google Message. 3-party apps can't support it because google hasn't released a public API yet.
	# 
	# In a lot of country messages go through Google's Jibe servers.
	# https://jibe.google.com/policies/terms/	
	#
	# https://pocketnow.com/why-you-should-probably-avoid-googles-rcs-text-messaging-chat-feature
	#
	# Does this packages is really needed for VolTE/VoWifi?

	#"com.qualcomm.qti.services.secureui" # [MORE INFO NEEDED]
	# Qualcomm Secure UI Service.
	# Uncertain role...

	"com.qualcomm.qti.uceshimservice"
	# UCE shim service 
	# UCE = Unified Communications for Enterprise ? (not sure)

	#"com.qualcomm.timeservice" # [MORE INFO NEEDED]
	# Qualcomm Time Service
	# It maybe keeps the real time clock in the Qualcomm processor synchronised with Android time.
	# Seems not safe to remove. 

	"com.qti.xdivert" # [MORE INFO NEEDED]
	# Smart-Divert
	# If enabled, diverts your calls to another number.
	# You can choose to divert all calls, divert on no reply or divert when the line is busy.
	# Where can you enable/disable this feature? 

	"com.quicinc.cne.CNEService"
	# Qualcomm service
	# Automatically switchs between Wifi/3G/4G depending on network performances. 
	# https://www.qualcomm.com/news/onq/2013/07/02/qualcomms-cne-bringing-smarts-3g4g-wi-fi-seamless-interworking

	"com.qti.qualcomm.datastatusnotification" # [MORE INFO NEEDED]
	# Can read/send SMS
	# Allows to cap data when you've reached the limit of your plan (not 100% sure)

	"com.qualcomm.qti.dynamicddsservice"
	# Dynamic DDS Service
	# DDS = Direct Digital Synthesizer
	# To make this very simple, it enables frequencies to be changed quickly without settling time.
	# It is very useful for testing, communications and frequency sweep applications. Not sure you need this in your phone.
	# https://www.qualcomm.com/news/releases/1996/05/07/qualcomm-introduces-new-high-speed-dual-direct-digital-synthesizer
	# If you want to know more about the use of a DDS : https://www.allaboutcircuits.com/technical-articles/direct-digital-synthesis/

	"com.qualcomm.qti.lpa"
	# Only useful if you use an esim (virtual sim)
	# lpa = Local Profile Assistants. It is a software that allows consumers to choose and change their subscription data when switching between 
	# network operators/carriers.
	# https://developer.qualcomm.com/blog/rise-esims-and-isims-and-their-impact-iot
	# https://source.android.com/devices/tech/connect/esim-overview

	"com.qualcomm.qti.remoteSimlockAuth" # [MORE INFO NEEDED]
	# Enable you to lock/unlock your eSIM remotely.
	# Seems more of a security risk to me than anything else.
	# Is it related to Safeswitch ? https://www.qualcomm.com/products/features/security/safeswitch

	#"com.qualcomm.qti.simsettings" # [MORE INFO NEEDED]
	# Obviously related to SIM settings

	#"com.qualcomm.qti.telephonyservice"
	# Sound processing during phonecalls
	# You absolutely need this package.

	#"com.qualcomm.qti.telephony.vodafonepack" # [MORE INFO NEEDED]
	# Related to Vodafone Prepaid Recharge Plan
	# If you're not a Vodafone client but still has this package on your phone you can delete it.
	# For Vodafon client, I don't know what this package does.
	# https://en.wikipedia.org/wiki/Vodafone

	"com.qualcomm.qti.uim"
	# Related to RUIM I guess. It is a kind of SIM card
	# https://en.wikipedia.org/wiki/Removable_User_Identity_Module
	# Still used in China it seems.

	"com.quicinc.fmradio"
	# quicinc = Qualcomm Innovation Center
	# FM Radio app by Qualcomm

	"com.qualcomm.qti.qmmi"
	# QMMI is a test app made by Qualcomm. It is used by service center to test the working of the various device components.
	# More info: https://community.phones.nokia.com/discussion/52566/android-10-on-nokia-8-1/p19
	# Useless for end-users.

	#"com.qualcomm.svi"
	# Sunlight Visibility Improvement
	# I've heard vaguely that some phones use it for the above purpose? On a LG Q6 there was no effect on functionality after removing.

	"com.qti.confuridialer"
	# Conference URI dialer. Also a conference call service, for digital signal only, as SIP / VoIP
	# https://devcondition.com/article/removing-unneeded-miui-applications/
	)

declare -a misc=(

	"cci.usage"
	# My Consumer Cellular (https://play.google.com/store/apps/details?id=cci.usage)
	# Lets you manage your Consumer Cellular account, track your usage, pay your bill.
	# Consumer Cellular is an American postpaid mobile virtual network operator
	# https://en.wikipedia.org/wiki/Consumer_Cellular

	"com.aaa.android.discounts"
	"com.aaa.android.discounts.vpl"
	# AAA Mobile (https://play.google.com/store/apps/details?id=com.aaa.android.discounts)
	# Kind of GPS that helps you find Point of interest (POI) like hotels, restaurants, and car repair facilities from the AAA databases.
	# NOTE : You’ll have to sign up for an AAA membership to enjoy all of the features and functionality of the Android app.
	# AAA = American Automobile Association
	
	"com.aspiro.tidal.vpl" # for VPL mobiles/employees ? 
	"com.aspiro.tidal"
	# Tidal Music (https://play.google.com/store/apps/details?id=com.aspiro.tidal)
	# Music streaming app

	"com.audible.application"
	# Cover Audible Audiobooks (https://play.google.com/store/apps/details?id=com.audible.application)

	"com.bleacherreport.android.teamstream"
	# Bleacher Report (https://play.google.com/store/apps/details?id=com.bleacherreport.android.teamstream)
	# Sports news site

	"com.blurb.checkout" 
	# Blurb Checkout 
	# Provides book purchase and checkout for Samsung’s Story Album app (discontinued)

	"com.booking"
	# Booking.com app (https://play.google.com/store/apps/details?id=com.booking)
	# Seriously, you shouldn't use it !
	# https://en.wikipedia.org/wiki/Booking.com
	# https://blog.usejournal.com/why-i-would-never-trust-booking-com-again-so-you-should-too-a2ab535ed915?gi=7ebe86eaa880
	# https://ro-che.info/articles/2017-09-17-booking-com-manipulation

	"com.cequint.ecid"
	# Caller ID from Cequint (https://www.cequint.com/)
	# https://www.fiercewireless.com/wireless/t-mobile-to-launch-caller-id-service-from-cequint
	# NOTE : Never trust a company which promotes call ID/spam blocking features.
	# https://itmunch.com/robocall-caught-sending-customers-confidential-data-without-consent/
	#
	# Cequint was acquired by TNS (https://tnsi.com/)
	# That was not a good thing : https://www.geekwire.com/2013/earnouts-bad-cequint-execs-sue-parent-company/

	"com.cnn.mobile.android.phone"
	# CNN Breaking US & World News app (https://play.google.com/store/apps/details?id=com.cnn.mobile.android.phone)

	"com.contextlogic.wish"
	# Wish Shopping (https://play.google.com/store/apps/details?id=com.contextlogic.wish)
	# You should not buy with Wish if you want my opinion.

	"com.cootek.smartinputv5.language.englishgb"
	"com.cootek.smartinputv5.language.spanishus"
	# TouchPal Keyboard by Cootek a chinese company.
	# Adware (lots lots of ads)
	# Worth reading : https://www.buzzfeednews.com/article/craigsilverman/google-banned-cootek-adware

	"com.crowdcare.agent.k"
	# Crowdcare is now Wysdom.AI (https://nitter.42l.fr/wysdomai)
	# From their Twitter description : The easiest way for businesses to improve customer satisfaction, contain costs, 
	# and generate revenue by using #AI to power customer experiences.
	# Wysdom.AI has joined the Microsoft Partner Network in 2018
	# https://wysdom.ai/privacy-policy/ (not good)

	"com.devhd.feedly"
	# Feedly (https://play.google.com/store/apps/details?id=com.devhd.feedly)
	# News aggregator application (RSS)
	# RSS aggregator are great but Feedly is really bad, especially privacy-wise.
	# https://feedly.com/i/legal/privacy
	# Better Alternative : https://github.com/FreshRSS/FreshRSS

	"com.diotek.sec.lookup.dictionary"
	# Samsung dictionary from Diotek (Korean company)
	# http://en.diotek.com/

	"com.directv.dvrscheduler"
	# DIRECTV (https://play.google.com/store/apps/details?id=com.directv.dvrscheduler)
	# Offical app from DIRECTV (subsidiary of AT&T)
	# Lets you watch Live TV, recorded shows, VODs and schedule recordings on your DVR
	# Worth reading : https://en.wikipedia.org/wiki/DirecTV#Consumer_protection_lawsuits_and_violations

	"com.discoveryscreen"
	# Appflash (https://play.google.com/store/apps/details?id=com.discoveryscreen)
	# Verizon Spyware.
	# https://www.eff.org/deeplinks/2017/04/update-verizons-appflash-pre-installed-spyware-still-spyware

	"com.dna.solitaireapp"
	# Solitaire Game app from DNA company ? 

	"com.draftkings.dknativermgGP.vpl"
	# DraftKings - Daily Fantasy Sports for Cash
	# App has been removed from the Playstore.
	# Worth reading : https://en.wikipedia.org/wiki/DraftKings
	
	"com.drivemode"
	# Drivemode (https://play.google.com/store/apps/details?id=com.drivemode.android)
	# Simplifies how you manage calls and messages while driving.
	# https://drivemode.com/privacy-2/

	"com.ebay.mobile"
	# Ebay app (https://play.google.com/store/apps/details?id=com.ebay.mobile)

	"com.ebay.carrier"
	# Kind of weird ebay apps preinstalled by carriers.

	"com.ehernandez.radiolainolvidable"
	# Radio La Inolvidable Peru (no longer exist)
	# Spanish Radio app.

	"com.emoji.keyboard.touchpal"
	# TouchPal Emoji Keyboard by Cootek a chinese company
	# Adware (lots lots of ads)
	# Worth reading : https://www.buzzfeednews.com/article/craigsilverman/google-banned-cootek-adware

	"com.eterno"
	# Daily hunts News. (https://play.google.com/store/apps/details?id=com.eterno&hl=en)

	"com.evernote"
	# Evernote app (https://play.google.com/store/apps/details?id=com.evernote)

	"com.galaxyfirsatlari"
	# Galaxy Fırsatları (https://play.google.com/store/apps/details?id=com.galaxyfirsatlari)
	# Samsung-only app for Turkish people
	# Recommands you stuff to buy. You are supposed to save money but we all know this kind of apps
	# encourages consumption.
	# Exodus found 10 trackers and 17 permissions : https://reports.exodus-privacy.eu.org/fr/reports/143830/ 

	"com.generalmobi.go2pay"
	# Go2Pay (https://play.google.com/store/apps/details?id=com.generalmobi.go2pay)
	# Payment app that offers mobile pre-paid recharges and post-paid bill payment, data card recharges and bill payment, 
	# DTH recharges through cashless transactions.
	# DTH = Direct To Home Television 

	"com.gotv.nflgamecenter.us.lite" 
	# Football NFL (https://play.google.com/store/apps/details?id=com.gotv.nflgamecenter.us.lite)

	"com.groupon"
	# Groupon (https://play.google.com/store/apps/details?id=com.groupon)
	# Online shopping deals and coupons.
	# Worth reading : https://en.wikipedia.org/wiki/Groupon#Reception

	"com.hancom.office.editor.hidden"
	# Legacy Hancom Office Editor (Korean alternative to Microsoft Office). Featured in Samsung and LG phones

	"com.handmark.expressweather"
	"com.handmark.expressweather.vpl"
	# 1Weather (https://play.google.com/store/apps/details?id=com.handmark.expressweather)
	# Forecasts alerts app (contain ads)

	"com.hulu.plus"
	# Hulu (https://play.google.com/store/apps/details?id=com.hulu.plus&hl)
	# Netflix competitor.
	# FYI : Hulu is owned by Disney.
	# https://en.wikipedia.org/wiki/Hulu
	# https://www.digitaltrends.com/home-theater/hulu-vs-disney-plus/

	"com.ideashower.readitlater.pro"
	# Pocket (https://play.google.com/store/apps/details?id=com.ideashower.readitlater.pro)
	# Allows you to save an article or web page to remote servers for later reading
	# Was purchased by Mozilla in 2017 but is still close source for now.
	# https://getpocket.com/privacy
	# Open-source alternative : https://wallabag.org/

	"com.imdb.mobile"
	# IMDb mobile app (https://play.google.com/store/apps/details?id=com.imdb.mobile)

	"com.infraware.polarisoffice5"
	# Polaris Office from US Infraware Inc company (Microsoft Office like)
	# https://en.wikipedia.org/wiki/Polaris_Office
	# https://play.google.com/store/apps/details?id=com.infraware.office.link

	"com.ironsource.appcloud.oobe"
	# AppCloud (discontinued) from ironSource, an advertising company.
	# Worth reading : https://en.wikipedia.org/wiki/IronSource.

	"com.ironsource.appcloud.oobe.huawei"
	# Essentials apps
	# App who promotes some other apps (and encourages you to install them)
	# This is developped by IronSource, a "next-generation advertising company" 
	# https://aura.ironsrc.com/ (app) | https://company.ironsrc.com/ (company)
	# When you try to read their privacy policy you arrive to an outstanding blank PDF file!
	# (http://www.ironsrc.com/wp-content/uploads/2019/03/ironSource-Privacy-Policy.pdf)

	"com.king.candycrush4"
	# Candy Crush Friends Saga (https://play.google.com/store/apps/details?id=com.king.candycrush4)

	"com.king.candycrushsodasaga"
	# Candy Crush Soda Saga (https://play.google.com/store/apps/details?id=com.king.candycrushsodasaga)

	"com.king.candycrushsaga"
	# Candy Crush Saga (https://play.google.com/store/apps/details?id=com.king.candycrushsaga)
	# I don't understand why this game is so popular (I guess the fact it is preinstalled in a lot of phone helps)

	"com.linkedin.android"
	# Linkedin app (https://play.google.com/store/apps/details?id=com.linkedin.android)

	"com.lookout" 
	# Lookout Security & Antivirus (https://play.google.com/store/apps/details?id=com.lookout)

	"com.micredit.in"
	# Mi Credit. Personal Loan app (https://play.google.com/store/apps/details?id=com.micredit.in.gp)

	"com.netflix.mediaclient"
	# Netflix app (https://play.google.com/store/apps/details?id=com.netflix.mediaclient)

	"com.netflix.partner.activation"
	# Netflix activation via partner manufacturer ? 

	"com.niksoftware.snapseed"
	# Snapseed (https://play.google.com/store/apps/details?id=com.niksoftware.snapseed)

	"com.nuance.swype.input"
	# Swype keyboard by Nuance
	# https://www.nuance.com/mobile/mobile-applications/swype/android.html
	# https://en.wikipedia.org/wiki/Swype

	"com.opera.branding"
	# Opera Branding Provider
	# Don't know what it really does.

	"com.opera.branding.news"
	# Opera News Branding Provider
	# Don't know what it really does.

	"com.opera.mini.native"
	# Opera Mini web browser (https://play.google.com/store/apps/details?id=com.opera.mini.native)

	"com.opera.preinstall"
	# Opera Preinstall Data
	# Generate utm tracking stuff

	"com.opera.max.oem"
	"com.opera.max.preinstall"
	# Opera Max (discontinued)
	# System-wide data-saving proxy that funnell all app data through Opera’s servers to compress images and videos 

	"com.particlenews.newsbreak"
	# News Break: Local & Breaking (https://play.google.com/store/apps/details?id=com.particlenews.newsbreak)
	# News provided by NewsBreak (https://www.newsbreak.com/)
	#
	# FYI : https://reports.exodus-privacy.eu.org/en/reports/com.particlenews.newsbreak/latest/

	"com.phonepe.app"
	# PhonePe (https://play.google.com/store/apps/details?id=com.phonepe.app)
	# PhonePe is a payments app that allows indian users to use BHIM UPI, your credit card and debit card or wallet to recharge your mobile phone, 
	# pay your utility bills and also make instant payments at offline and online stores.
	# PhonePe is an indian company (https://en.wikipedia.org/wiki/PhonePe)
	# BHIM = Bharat Interface for Money : https://en.wikipedia.org/wiki/BHIM
	# UPI = Unified Payement Interface : https://en.wikipedia.org/wiki/Unified_Payments_Interface

	"com.pinsight.v1"
	# App Spotlight
	# Makes you discover new apps from the Google Play store. The selection criteria are unkown.

	"com.playphone.gamestore"
	# Playphone Gamestore (https://www.playphone.com/)
	# "Helps" you discover the "best" Android games and connects you to a global gaming community. Sounds Amazing !

	"com.playphone.gamestore.loot"
	# Loot 
	# Premium service from playphone ? 

	"com.pure.indosat.care"
	# myIM3 (https://play.google.com/store/apps/details?id=com.pure.indosat.care)
	# App provided by Indosat Ooredoo, an Internet provider from Indonesia. 
	# Enables Indosat users to manage prepaid and postpaid numbers and check their credit and payments, purchase data packs, calls, SMS...

	"com.huaqin.FM"   
	# Radio app from huaqin a chinese company
	# NOTE : Transistor [https://f-droid.org/en/packages/org.y20k.transistor/] is much better

	"com.nextradioapp.nextradio"
	# NextRadio (https://play.google.com/store/apps/details?id=com.nextradioapp.nextradio)
	# 3-party app which lets you experience live and local FM radio on your smartphone.
	# https://nextradioapp.com/

	"com.pinsight.dw"
	# App Stack 
	# Force-installed app by Sprint. Pinsight is an advertising company (https://pinsightmedia.com/)
	# Note : Sprint sold Pinsight to InMobi in 2018.
	# https://www.fiercewireless.com/wireless/sprint-sells-mobile-ad-biz-pinsight-media-to-inmobi

	"com.realvnc.android.remote"
	# Remote controle service by Realvnc (https://en.wikipedia.org/wiki/RealVNC)
	# https://www.realvnc.com/en/legal/#privacy
	# Not sure having a remote control app installed as a system app is a good idea
	# This application is no longer maintained, besides.

	"com.remotefairy4"
	# AnyMote Universal Remote + Wifi Smart Home Control (https://play.google.com/store/apps/details?id=com.remotefairy4)
	# IR Remote control app 
	# Lots of trackers and permissions : https://reports.exodus-privacy.eu.org/en/reports/com.remotefairy4/latest/

	"com.republicwireless.tel"
	# Republic (https://play.google.com/store/apps/details?id=com.republicwireless.tel&hl)
	# Lets you manage your Republic wireless account.
	# Republic Wireless is an american mobile virtual network operator (https://en.wikipedia.org/wiki/Republic_Wireless)

	"com.rhapsody"
	"com.rhapsody.vpl"
	# Napster Music (https://play.google.com/store/apps/details?id=com.rhapsody)
	# Napster streaming app
	# https://en.wikipedia.org/wiki/Napster

	"com.roaming.android.gsimbase" # [MORE INFO NEEDED]
	"com.roaming.android.gsimcontentprovider"
	# GSIM = Generic Statistical Information Model ? I don't think so but I can't find anything.

	"com.sem.factoryapp"
	# SEMFactoryApp
	# Call home (172.217.168.14 --> Google IP). Need NFC permission.
	# This package is maybe used to test NFC.

	"com.servicemagic.consumer"
	# HomeAdvisor: Contractors for Home Improvement (https://play.google.com/store/apps/details?id=com.servicemagic.consumer)
	# Help you find local contractors from the service Home Advisor network
	# HomeAdvisor collects users data when a request is made and then sells that data to local contractors in exchange for money.
	# Worth Reading : https://en.wikipedia.org/wiki/HomeAdvisor#Critism

	"com.setk.widget"
	# Galaxy Bizz (https://play.google.com/store/apps/details?id=com.setk.widget)
	# Useless app that recommands you stuff to do/buy nearby your area

	"com.sharecare.askmd"
	# AskMD (discontinued) provided by Sharecare
	# Symptom checker app. Lets you see what might be causing your symptoms and helps you find a nearby physician 
	# Worth Reading : https://en.wikipedia.org/wiki/Sharecare#Criticisms

	"com.slacker.radio" 
	# LiveXLive - Streaming Music and Live Events (https://play.google.com/store/apps/details?id=com.slacker.radio)

	"com.shopee.id"
	# Shopee 2.2 (https://play.google.com/store/apps/details?id=com.shopee.id)
	# official app from Shopee, an e-commerce online shopping platform in Southeast Asian.

	"com.smithmicro.netwise.director.comcast.oem"
	# XFINITY Wifi settings (https://play.google.com/store/apps/details?id=com.smithmicro.netwise.director.comcast.oem)
	# Auto-connects you to XFINITY WiFi hotspot.
	# XFINITY is a subsidiary of the Comcast Corporation (https://en.wikipedia.org/wiki/Xfinity)

	"com.spotify.music"
	# Spotify app (https://play.google.com/store/apps/details?id=com.spotify.music)

	"com.swiftkey.swiftkeyconfigurator"
	# SwiftKey factory settings
	# Used by commercial swiftkey partners to configure the SwiftKey app.
	# Swiftkey is a keyboard developed by TouchType, a Microsoft subsidiary (https://en.wikipedia.org/wiki/SwiftKey)

	"com.synchronoss.dcs.att.r2g"
	# AT&T Ready2Go (discontinued)
	# Its purpose was to help you migrating your data to your new Android device.

	"com.s.antivirus"
	# AVG Antivirus (https://play.google.com/store/apps/details?id=com.s.antivirus) for Sony Xperia.

	"com.telenav.app.android.cingular"
	# AT&T Navigator (https://play.google.com/store/apps/details?id=com.telenav.app.android.cingular)
	# Crappy GPS app provided by Telenav and rebranded by AT&T.
	# Worth reading : https://www.telenav.com/legal/policies-privacy-policy

	"com.telenav.app.android.scout_us"
	# Scout GPS Navigation & Meet Up (https://play.google.com/store/apps/details?id=com.telenav.app.android.scout_us)
	# Bad GPS with bad chat features on top of that. 
	# https://www.scoutgps.com/

	"com.til.timesnews"
	# India News (https://play.google.com/store/apps/details?id=com.til.timesnews)
	
	#"com.touchtype.swiftkey"
	#"com.touchtype.swiftkey.res.overlay"
	# Swiftkey Keyboard (https://play.google.com/store/apps/details?id=com.touchtype.swiftkey)
	# Keyboard app developed by TouchType, a Microsoft subsidiary (https://en.wikipedia.org/wiki/SwiftKey)
	# Sends "anonymous" to Microsoft.
	# 4 Trackers + 11 Permissions : https://reports.exodus-privacy.eu.org/en/reports/com.touchtype.swiftkey/latest/
	# NOTE : default keyboard on some Nokia and Huawei phones

	"com.tracker.t"
	# WTF is this ?? Given its name I think you can take the risk to delete it.

	"com.turner.cnvideoapp" 
	# Cartoon network App (https://play.google.com/store/apps/details?id=com.turner.cnvideoapp)

	"com.tripadvisor.tripadvisor"
	# Trip advisor (https://play.google.com/store/apps/details?id=com.tripadvisor.tripadvisor)
	# You should never trust and use trip advisor
	# https://en.wikipedia.org/wiki/TripAdvisor (see 'Controversy and fraudulent reviews' section)
	# https://nypost.com/2016/03/01/why-you-should-never-ever-trust-tripadvisor/

	"com.ubercab"
	"com.ubercab.driver"
	"com.ubercab.eats"
	# Uber (https://play.google.com/store/apps/details?id=com.ubercab)
	# Uber Driver (https://play.google.com/store/apps/details?id=com.ubercab.driver)
	# Uber Eats (https://play.google.com/store/apps/details?id=com.ubercab.eats)
	# Uber does not protect personal user data and has a questionable ethic.
	# Worth reading : https://en.wikipedia.org/wiki/Uber#Criticism

	"com.UCMobile.intl"
	# UC Browser by Alibaba (https://play.google.com/store/apps/details?id=com.UCMobile.intl)
	# !! Unsecure chinese web browser !!
	# https://www.quora.com/Whats-wrong-with-UC-Browser
	# https://www.digitalinformationworld.com/2019/05/url-spoofing-uc-browser-android.html

	"com.ume.browser.northamerica"
	# UME Web Browser (https://play.google.com/store/apps/details?id=com.ume.browser.northamerica)
	# Trackers and a LOT of permissions (https://reports.exodus-privacy.eu.org/en/reports/com.ume.browser.cust/latest/)

	"com.vlingo.midas"
	# Speech recognition app forthe personal assistant by Vlingo 
	# Vlingo : https://en.wikipedia.org/wiki/Vlingo
	# FYI : In January 2012 AndroidPit discovered that Vlingo sended packets of information containing the users GPS co-ordinates,
	# IMEI (unique device identifier), contact list and the title of every song stored on the device back to Nuance without.
	# Source : https://www.androidpit.com/Vlingo-Privacy-Breach

	"com.wb.goog.got.conquest"
	# Game of Thrones: Conquest (https://play.google.com/store/apps/details?id=com.wb.goog.got.conquest)
	# Mobile game

	"com.yahoo.mobile.client.android.liveweather" 
	# Yahoo Weather (https://play.google.com/store/apps/details?id=com.yahoo.mobile.client.android.weather)
	# Please boycott Yahoo ! (all of their services are crappy so it's not so difficult)
	# If you're not aware : https://en.wikipedia.org/wiki/Criticism_of_Yahoo!

	"com.yellowpages.android.ypmobile"
	# Yellow Pages (https://play.google.com/store/apps/details?id=com.yellowpages.android.ypmobile)

	"com.yelp.android"
	# Yelp (https://play.google.com/store/apps/details?id=com.yelp.android)
	# Yelp lets users post reviews and rate businesses. 
	# Worth reading : https://en.wikipedia.org/wiki/Yelp#Controversy_and_litigation
	
	"com.zhiliaoapp.musically"
	# TikTok App 
	# Worth reading : https://en.wikipedia.org/wiki/TikTok#Privacy,_cyberbullying_and_addiction_concerns

	"de.axelspringer.yana.zeropage"
	# Upday news for Samsung (https://play.google.com/store/apps/details?id=de.axelspringer.yana)

	"flipboard.app" 
	# Flipboard News App (https://play.google.com/store/apps/details?id=flipboard.app)

	"flipboard.boxer.app" 
	# Briefing app (https://play.google.com/store/apps/details?id=flipboard.boxer.app)

	"id.co.babe"                                 
	# BaBe (https://play.google.com/store/apps/details?id=id.co.babe)
	# Indonesian news app.

	"in.mohalla.sharechat" 
	# ShareChat (https://play.google.com/store/apps/details?id=in.mohalla.sharechat)

	"in.playsimple.wordtrip"
	# World Trip (https://play.google.com/store/apps/details?id=in.playsimple.wordtrip)
	# Word Count & word streak puzzle game
	# 19 trackers + 11 permissions (https://reports.exodus-privacy.eu.org/en/reports/in.playsimple.wordtrip/latest/)

	"jp.co.omronsoft.openwnn"
	# Japanese keyboard/IME (Don't know why it's preinstalled on US/european devices)
	# Note : IME = input method editor 

	"jp.gocro.smartnews.android"
	# SmartNews: Local Breaking News (https://play.google.com/store/apps/details?id=jp.gocro.smartnews.android)
	# Delivers the top trending news stories from others publishers (NBC News, The Verges etc...)
	# 7 Trackers + 10 permissions (https://reports.exodus-privacy.eu.org/en/reports/jp.gocro.smartnews.android/latest/)

	"msgplus.jibe.sca.vpl"
	# Messaging Plus. Messings using the RCS protocol (https://en.wikipedia.org/wiki/Rich_Communication_Services)
 	# Related to Google Jibe (https://jibe.google.com/)

	"net.sharewire.parkmobilev2" 
	# ParkMobile - Find Parking (https://play.google.com/store/apps/details?id=net.sharewire.parkmobilev2)

	"pl.zdunex25.updater"
	# Updater for the zdnex25's theme
	# https://www.deviantart.com/zdunex25/gallery/26889741/themes

	#"se.dirac.acs"
	# Earphone audio quality improvement from the Swedish company Dirac.
	# The technology relies on impulse and magnitude frequency response correction to deliver a more dynamic soundstage, 
	# even when connected to budget headphones. 
	# The goal is to improve overall sound clarity and bass fidelity while correcting the frequency response so as to deliver a flat curve.
	# Can be disabled in Settings/Additional settings/Headphone and audio effects (to try to hear the difference)
	# https://www.androidcentral.com/heres-how-dirac-enabling-xiaomi-create-better-audio-products

	"tv.fubo.mobile.vpl"
	# fuboTV (https://play.google.com/store/apps/details?id=tv.fubo.mobile)	
	# Lets you Watch live Sports, TV Shows, Movies & News

	"tv.peel.app"
	# Peel Universal Smart TV Remote Control (discontinued)
	# Lets you remotely control devices like your TV, DVD or Blu-ray player.

	"zpub.res" # [MORE INFO NEEDED]
	# 3-party app preinstalled in ZTE phone.

	#### FONTS ####
	"com.monotype.android.font.chococooky"
	"com.monotype.android.font.cooljazz"
	"com.monotype.android.font.foundation"
	"com.monotype.android.font.rosemary"

	#### ANT(+) ####
	# ANT+ (Adaptive Network Topology) is a proprietary multicast wireless sensor network technology. 
	# It's like Bluetooth low energy, but oriented towards usage with sensors. ANT+ is mostly used for sport tracking devices 
	# https://www.thisisant.com/directory
	# Somes stuff are open-source : https://github.com/ant-wireless

	"com.dsi.ant.plugins.antplus" 
	# ANT+ plugin service (https://play.google.com/store/apps/details?id=com.dsi.ant.plugins.antplus)
	
	"com.dsi.ant.sample.acquirechannels"
	# I don't know why there is "sample" in the name. Is this package really useful to find ANT channels ? 

	"com.dsi.ant.server"
	# ANT HAL(Hardware Abstraction Layer) Server

	"com.dsi.ant.service.socket"
	# ANT Radio Service (https://play.google.com/store/apps/details?id=com.dsi.ant.service.socket)
	# it is NOT related to Radio FM !

	"co.sitic.pp"
	# Designed to remotely lock the phone (by sending a simple SMS) in case you don't pay your bill 
	# https://www.reddit.com/r/Android/comments/fde3l6/3rd_party_telemetry_found_in_nokia_smartphones/fjh4zbx/?context=3
	# This app was preinstalled on phone not served by that carrier (América Móvil) from South America. 
	# Normally you should not have this app anymore because it was removed by Nokia during an Android 10 update.

	##### TEE #####
	
	# TODO: Could removing these packages have an impact on Password/fingerprint/face/iris security? 

	#"com.gd.mobicore.pa"
	# Mobicore is now Trustonic
	# Trustonic is a small OS running on the CPU providing a TEE, an isolated environment that runs in parallel 
	# with the operating system, guaranteeing code and data loaded inside to be protected.
	# That's sound great but it's closed source and "normal" devs can't use it for their apps.
	# See "com.trustonic.tuiservice"

	#"com.trustonic.teeservice"
	# TEE = Trusted Execution Environment
	# See below

	#"com.trustonic.tuiservice"
	# The tuiService (Trusted User Interface) is a new security layer implemented by Trustonic.
	# Allows a Trusted Application to interact directly with the user via a common display and touch screen, completely isolated from the main device OS.
	# Seems like a good idea but it's closed source and "normal" devs can't use it for their apps. 
	# https://stackoverflow.com/questions/16909576/how-to-make-use-of-arm-trust-zone-in-android-application
	# It is basically only used by manufacturer apps like Samsung Pay and for DRM stuff.
	# Google implemented their own TUI in Android Pie : https://android-developers.googleblog.com/search/label/Trusted%20User%20Interface
	# 
	# https://www.trustonic.com/news/blog/benefits-trusted-user-interface/
	# https://en.wikipedia.org/wiki/Trusted_execution_environment
	#
	# If you're wondering, deleting theses packages will not cause security issues. It will break Trustonic TEE for sure
	# but if you don't use Trusted Apps. You won't need this ! 
	# Deleting this **may** reduce attack surface if your phone still has Trusted apps... because yeah, Trustonic TEE isn't foolproof (as it was claimed)
	# https://en.wikipedia.org/wiki/ARM_architecture#Security_extensions
	# https://googleprojectzero.blogspot.com/2017/07/trust-issues-exploiting-trustzone-tees.html
	# https://www.synacktiv.com/posts/exploit/kinibi-tee-trusted-application-exploitation.html
	# https://blog.quarkslab.com/introduction-to-trusted-execution-environment-arms-trustzone.html
	#
	# Good ressources : 
	# https://medium.com/@nimronagy/arm-trustzone-on-android-975bfe7497d2
	# https://www.gsd.inesc-id.pt/~nsantos/papers/pinto_acsur19.pdf
	# https://blog.quarkslab.com/introduction-to-trusted-execution-environment-arms-trustzone.html
	# https://medium.com/taszksec/unbox-your-phone-part-i-331bbf44c30c
	#
	# NOTE : Trustonic TEE (called Kinibi) is used in Samsung, Vivo, Oppo, Xiaomi, Meizu and LG devices.

	##### SIMALLIANCE (now Trusted Connectivity Alliance) #####
	# Non-profit trade association that aims at creating secure, open and interoperable mobile services.
	# https://trustedconnectivityalliance.org/introduction/

	#"org.simalliance.openmobileapi.uicc1terminal"
	#"org.simalliance.openmobileapi.uicc2terminal"
	# Open Mobile API ("interface") to access UICC secure elements 
	# UICC stands for Universal Integrated Circuit Card. 
	# It is a the physical and logical platform for the USIM and may contain additional USIMs and other applications.
	# (U)SIM is an application on the UICC.
	# https://bluesecblog.wordpress.com/2016/11/18/uicc-sim-usim/
	# Good read: https://arxiv.org/ftp/arxiv/papers/1601/1601.03027.pdf
	# Note2: The term SIM is widely used in the industry and especially with consumers to mean both SIMs and UICCs.
	# https://www.justaskgemalto.com/us/what-uicc-and-how-it-different-sim-card/

	#"org.simalliance.openmobileapi.service"
	# Smart Card Service
	# 
	# The SmartCard API is a reference implementation of the SIMalliance Open Mobile API specification that enables Android applications 
	# to communicate with Secure Elements, (SIM card, embedded Secure Elements, Mobile Security Card or others)
	# https://github.com/seek-for-android/pool/wiki/SmartcardAPI
	# Safe to remove if you think you don't need this

	)

declare -a mediatek=(
	#"com.mediatek" # [MORE INFO NEEDED]
	# Mediatek is a Taiwanese chipset manufacturer.
	# Can someone share the apk? This package name is really weird.
	# It is most likely a set of general APIs for accessing general mediatek functionalities.
	# Can someone share the apk?

	"com.mediatek.atmwifimeta"
	# wifi data logger you don't want.

	#"com.mediatek.callrecorder" # [MORE INFO NEEDED]
	# This is not the kind of feature expected from a Soc company.
	# If you remove this I guess you will not be able to record your calls from the stock dialer
	# Can someone share the apk and verify this?

	"com.mediatek.engineermode"
	# Enigneer mode you can access by dialing a secret code (*#*#3646633#*#* on some Xiaomi phones for instance)
	# It enables you to access to debug/logged data and some hidden firmware settings. 

	#"com.mediatek.gpslocationupdate" # [MORE INFO NEEDED]
	# I wonder if it is really only a logging app. 
	# Can someone try to remove it and use a GPS app to see it still works?
	# Can someone share the apk? (from a Xiaomi/Huawei phone)

	#"com.mediatek.location.lppe.main"
	# LPPE = LTE Positioning Protocol enhancements/extensions (LTE = "4G")
	# Positioning and assistance protocol between E-SMLC (mobile location center) and UE (User Equipement = phone)
	# https://www.gpsworld.com/wirelessexpert-advice-positioning-protocol-next-gen-cell-phones-11125/
	# I don't know the app has the permission to read SMS

	#"com.mediatek.ims" # [MORE INFO NEEDED]
	# Mediatek's implementation of IMS
	# https://www.programmersought.com/article/50164530665/
	# Note: IMS is an open industry standard for voice and multimedia communications over packet-based IP networks (Volte/VoIP/Wifi calling).
	# Unless you use VolTE (wifi calling) or RCS from Google or your carrier you don't need IMS.
	# I'm also not sure to understand the purpose of this package because there already is "com.google.android.ims" on the phone.
	# Can someone remove this package and test if IMS still works?

	"com.mediatek.mdmconfig" # [MORE INFO NEEDED]
	# Mobile Device Management (MDM) allows company’s IT department to reach inside your phone in the background, allowing them to ensure 
	# your device is secure, know where it is, and remotely erase your data if the phone is stolen.
	# It's a way to ensure employees stay productive and do not breach corporate policies
	# You should NEVER have a MDM tool on your personal phone. Never.
	# https://blog.cdemi.io/never-accept-an-mdm-policy-on-your-personal-phone/
	# This package probably isn't a MDM tool on its own but you definitively don't need it on your phone.
	# Can someone share the apk?

	"com.mediatek.mtklogger"
	# Logs debug data. Has a lot of permissions and run in background all the time.
	# Don't keep useless apps: reduce the attack surface
	# Vulnerability found in this app in 2016: https://nvd.nist.gov/vuln/detail/CVE-2016-10135

	#"com.mediatek.nlpservice" # [MORE INFO NEEDED]
	# Mediatek Network Location Provider
	# Provides periodic reports on the geographical location of the device. Each provider has a set of criteria under which it may be used.
	# For example, some providers require GPS hardware and visibility to a number of satellites others require the use of 
	# the cellular radio, or access to a specific carrier's network, or to the internet. 
	# They may also have different battery consumption characteristics or monetary costs to the user.
	# 
	# I don't really understand why you would need this as there is already one in 'com.google.android.gms'
	# I wonder if NLP can be replaced by https://github.com/microg/UnifiedNlp
	# I suggest to test if you get a better signal/battery performance with Mediatek NLP, if not you can get rid of it.

	#"com.mediatek.omacp"
	# omacp = OMA Client Provisioning. It is a protocol specified by the Open Mobile Alliance (OMA).
	# Configuration messages parser. Used for provisioning APN settings to devices via SMS 
	# In my case, it was automatic and I never needed configuration messages.
	# Maybe it's useful if carriers change their APN. But you still can change the config manually, it's not difficult.
	# Dunno why Mediatek handles this kind of things. Safe to remove. At worst, you'll need to manually config your APN.
	# Note: OMACP can be abused. Be careful:
	# https://research.checkpoint.com/2019/advanced-sms-phishing-attacks-against-modern-android-based-smartphones/
	# https://www.zdnet.com/article/samsung-huawei-lg-and-sony-phones-vulnerable-to-rogue-provisioning-messages/

	"com.mediatek.providers.drm"
	# DRM provider (actually Beep Science is the MediaTek’s default DRM vendor)
	# You probably need this if you want to watch Netflix & others stuff in high-res 
	# REMINDER : DRM = all the things that restrict the use of proprietary hardware and copyrighted works.
	# ==> https://en.wikipedia.org/wiki/Digital_rights_management
	# ==> https://creativecommons.org/2017/07/09/terrible-horrible-no-good-bad-drm/
	# ==> https://fckdrm.com/
	# ==> http://www.info-mech.com/drm_flaws.html

	"com.mediatek.wfo.impl"
	# According to olorin (https://www.olorin.me/2019/09/08/debloating-the-umidigi-f1-play/)
	# it's the MediaTek’s default fingerprint app (and he removed it)
	# Can someone confirm what does this package exactly do? 
	# Remember that any preinstalled apps you don't actually need just increase the surface attack.
	# Vulnerability found in 2019: https://nvd.nist.gov/vuln/detail/CVE-2019-15368
	# Any app co-located on the device could modify a system property through an exported interface without proper authorization.
	)