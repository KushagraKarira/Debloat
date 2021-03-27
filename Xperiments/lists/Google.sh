#!/usr/bin/env bash

declare -a google=(

	"com.android.hotwordenrollment.okgoogle"
	"com.android.hotwordenrollment.xgoogle"
	# "OK Google" detection service.

	"com.android.partnerbrowsercustomizations.chromeHomepage" 
	# Horrible stuff for Google Chrome. This package bypass your DNS settings (for letting pass Google ads)

	"com.android.chrome" 
	# Google Chrome app (https://play.google.com/store/apps/details?id=com.android.chrome)

	"com.chrome.beta" 
	# Google Chrome Beta (https://play.google.com/store/apps/details?id=com.chrome.beta)

	"com.chrome.canary" 
	# Google Chrome Canary (Nightly build) (https://play.google.com/store/apps/details?id=com.chrome.canary)

	"com.chrome.dev" 
	# Google Chrome (developer)	(https://play.google.com/store/apps/details?id=com.chrome.dev)

	"com.google.android.apps.access.wifi.consumer" 
	# Google Wifi app (https://play.google.com/store/apps/details?id=com.google.android.apps.access.wifi.consumer)

	"com.google.android.apps.adm" 
	# Google Find my device app (https://play.google.com/store/apps/details?id=com.google.android.apps.adm)

	"com.google.android.apps.ads.publisher" 
	# Google Adsense app (https://play.google.com/store/apps/details?id=com.google.android.apps.ads.publisher) 

	"com.google.android.apps.adwords" 
	# Google Ads app (https://play.google.com/store/apps/details?id=com.google.android.apps.adwords)

	"com.google.android.apps.authenticator2" 
	# Google authentificator app (https://play.google.com/store/apps/details?id=com.google.android.apps.authenticator2)

	"com.google.android.apps.blogger" 
	# Google blogger app (https://play.google.com/store/apps/details?id=com.google.android.apps.blogger)

	"com.google.android.apps.books" 
	# Google Play Books (https://play.google.com/store/apps/details?id=com.google.android.apps.books)

	"com.google.android.apps.chromecast.app" 
	# Google Home (https://play.google.com/store/apps/details?id=com.google.android.apps.chromecast.app_US)

	"com.google.android.apps.cloudprint" 
	# Cloud print (https://play.google.com/store/apps/details?id=com.google.android.apps.cloudprint)

	"com.google.android.apps.cultural" 
	# Google Arts & Culture (https://play.google.com/store/apps/details?id=com.google.android.apps.cultural_US)

	"com.google.android.apps.currents" 
	# Google Currents (discontinued)

	"com.google.android.apps.docs"
	# Google Drive (https://play.google.com/store/apps/details?id=com.google.android.apps.docs_US)

	"com.google.android.apps.docs.editors.docs" 
	# Google Docs (https://play.google.com/store/apps/details?id=com.google.android.apps.docs.editors.docs)

	"com.google.android.apps.docs.editors.sheets" 
	# Google sheets

	"com.google.android.apps.docs.editors.slides" 
	# Google slides (for presentation)

	"com.google.android.apps.dynamite" 
	# Hangout chat (https://play.google.com/store/apps/details?id=com.google.android.apps.dynamite)

	"com.google.android.apps.enterprise.cpanel" 
	# Google Admin (https://play.google.com/store/apps/details?id=com.google.android.apps.enterprise.cpanel)

	"com.google.android.apps.enterprise.dmagent" 
	# Google apps device policy (https://play.google.com/store/apps/details?id=com.google.android.apps.enterprise.dmagent)

	"com.google.android.apps.fireball" 
	# Google Allo (discontinued)

	"com.google.android.apps.fitness" 
	# Google Fit (https://play.google.com/store/apps/details?id=com.google.android.apps.fitness)

	"com.google.android.apps.freighter" 
	# Google Datally (discontinued)

	"com.google.android.apps.giant" 
	# Google Analytics (https://play.google.com/store/apps/details?id=com.google.android.apps.giant)

	"com.google.android.apps.googleassistant" 
	# Google Assistant (https://play.google.com/store/apps/details?id=com.google.android.apps.googleassistant_US)

	"com.google.android.apps.handwriting.ime" 
	# Google Handwriting Input (https://play.google.com/store/apps/details?id=com.google.android.apps.handwriting.ime)

	"com.google.android.apps.hangoutsdialer" 
	# Google Hangout Dialer (https://play.google.com/store/apps/details?id=com.google.android.apps.hangoutsdialer)

	"com.google.android.apps.inbox" 
	# Inbox by Gmail (Discontinued)
	"com.google.android.apps.kids.familylink" 
	# Google Family Link (https://play.google.com/store/apps/details?id=com.google.android.apps.kids.familylink)

	"com.google.android.apps.kids.familylinkhelper" 
	# Google Family Link for children & teens (https://play.google.com/store/apps/details?id=com.google.android.apps.kids.familylinkhelper)

	"com.google.android.apps.m4b" 
	# Google My Maps (https://play.google.com/store/apps/details?id=com.google.android.apps.m4b)
	# It is NOT Google Maps

	"com.google.android.apps.magazines" 
	# Google News (https://play.google.com/store/apps/details?id=com.google.android.apps.magazines)

	"com.google.android.apps.mapslite" 
	# Google Maps Go (lite web app of Maps) (https://play.google.com/store/apps/details?id=com.google.android.apps.mapslite)

	"com.google.android.apps.meetings" 
	# Hangout Meet (https://play.google.com/store/apps/details?id=com.google.android.apps.meetings)

	"com.google.android.apps.messaging" 
	# Google Messaging (SMS) (https://play.google.com/store/apps/details?id=com.google.android.apps.messaging)

	"com.google.android.apps.navlite" 
	# Google Maps GPS (https://play.google.com/store/apps/details?id=com.google.android.apps.navlite)
	"com.google.android.apps.nbu.files" 
	# File Management (https://play.google.com/store/apps/details?id=com.google.android.apps.nbu.files)

	"com.google.android.apps.paidtasks" 
	# Google Opinion Rewards (https://play.google.com/store/apps/details?id=com.google.android.apps.paidtasks)

	"com.google.android.apps.pdfviewer" 
	# Google PDF Viewer (https://play.google.com/store/apps/details?id=com.google.android.apps.pdfviewer)

	"com.google.android.apps.photos" 
	# Google Photos (https://play.google.com/store/apps/details?id=com.google.android.apps.photos_US)

	"com.google.android.apps.photos.scanner" 
	# PhotoScan app (https://play.google.com/store/apps/details?id=com.google.android.apps.photos.scanner)

	"com.google.android.apps.plus" 
	# Google+ (https://play.google.com/store/apps/details?id=com.google.android.apps.plus_US)

	"com.google.android.apps.podcasts"
	# Google Podcasts (https://play.google.com/store/apps/details?id=com.google.android.apps.podcasts)

	"com.google.android.apps.restore"
	# This is the backup restore wizard used for pulling Android system backups from your Google account. 
	# You only need this if you factory restore the phone, in which case it’s automatically reinstalled for you.

	"com.google.android.apps.recorder"
	# Google (audio) recorder (https://play.google.com/store/apps/details?id=com.google.android.apps.recorder)

	"com.google.android.apps.setupwizard.searchselector"
	# Most likely add a search bar to the setupwizard (com.google.android.setupwizard)

	"com.google.android.apps.santatracker" 
	# Google Santa Tracker WTF ??? (https://play.google.com/store/apps/details?id=com.google.android.apps.santatracker)

	"com.google.android.apps.subscriptions.red" 
	# Google One (https://play.google.com/store/apps/details?id=com.google.android.apps.subscriptions.red_US)

	"com.google.android.apps.tachyon" 
	# Google Duo (Video Calls) (https://play.google.com/store/apps/details?id=com.google.android.apps.tachyon)

	"com.google.android.apps.tasks" 
	# Google Task (TODO list) (https://play.google.com/store/apps/details?id=com.google.android.apps.tasks)

	"com.google.android.apps.translate" 
	# Google Translate (https://play.google.com/store/apps/details?id=com.google.android.apps.translate)

	"com.google.android.apps.travel.onthego"	
	# Google Trip (discontinued)

	"com.google.android.apps.uploader"
	# Picasa Uploader (discontinued)

	"com.google.android.apps.vega" 
	# Google My Business (https://play.google.com/store/apps/details?id=com.google.android.apps.vega)

	"com.google.android.apps.walletnfcrel" 
	# Google Pay (https://play.google.com/store/apps/details?id=com.google.android.apps.walletnfcrel)

	"com.google.android.apps.wallpaper" 
	# Google Wallpapers (https://play.google.com/store/apps/details?id=com.google.android.apps.wallpaper)

	"com.google.android.apps.wellbeing" 
	# Digital Wellbeing (habits tracking tool) (https://play.google.com/store/apps/details?id=com.google.android.apps.wellbeing)

	"com.google.android.apps.youtube.creator" 
	# Youtube Studio (https://play.google.com/store/apps/details?id=com.google.android.apps.youtube.creator)

	"com.google.android.apps.youtube.gaming" 
	# Youtube Gaming -(discontinued in March 2019, features integrated in main youtube app)

	"com.google.android.apps.youtube.kids" 
	# Youtube Kid (https://play.google.com/store/apps/details?id=com.google.android.apps.youtube.kids)

	"com.google.android.apps.youtube.music" 
	# Youtube Music (https://play.google.com/store/apps/details?id=com.google.android.apps.youtube.music)

	"com.google.android.apps.youtube.vr" 
	# Youtube VR (https://play.google.com/store/apps/details?id=com.google.android.apps.youtube.vr)

	"com.google.android.backup" # On Android 4.2
	"com.google.android.backuptransport" 
	# Allows Android apps to back up their data on Google servers.

	"com.google.android.calculator" 
	# Google Calculator (https://play.google.com/store/apps/details?id=com.google.android.calculator)

	"com.google.android.calendar" 
	# Google Calendar (https://play.google.com/store/apps/details?id=com.google.android.calendar)

	"com.google.android.configupdater" 
	# Related to carrier config
	# Auto updates certificates for TLS connection, firewall configuration, time zone info...
	# See : https://android.googlesource.com/platform/frameworks/base/+/master/core/java/android/os/ConfigUpdate.java
	# This is configuration stuff for Google services only.

	#"com.google.android.deskclock" 
	# Google clock app (https://play.google.com/store/apps/details?id=com.google.android.deskclock)

	"com.google.android.feedback" 
	# When an app crashes, this is the app that briefly asks you if you want to feedback the crash on the market, Google Play Store.

	"com.google.android.googlequicksearchbox" 
	# Google Search box (https://play.google.com/store/apps/details?id=com.google.android.googlequicksearchbox)

	"com.google.android.instantapps.supervisor"
	# Lets you try new games directly on Google Play.
	# https://www.zdnet.com/article/googles-instant-apps-goes-live-now-you-can-try-android-apps-before-installing-them/

	"com.google.android.keep" 
	# Google Keep (https://play.google.com/store/apps/details?id=com.google.android.keep)

	"com.google.android.markup" 
	# Google Markup app made for modifying pictures, ships by default on every Pie+ device.

	"com.google.android.marvin.talkback" 
	# Android Accessibility Suite (https://play.google.com/store/apps/details?id=com.google.android.marvin.talkback)
	# Helps blind and vision-impaired users.

	"com.google.android.music" 
	# Google Play Music (https://play.google.com/store/apps/details?id=com.google.android.music)

	"com.google.android.onetimeinitializer" 
	# Provides first time setup, safe to remove.

	"com.google.android.play.games" 
	# Google Play Games (https://play.google.com/store/apps/details?id=com.google.android.play.games)

	"com.google.android.printservice.recommendation"
	# Print recommendation service. 
	# Not clear, seems to help you to find printers but it's not mandatory. 
	# Safe to remove

	"com.google.android.projection.gearhead" 
	# Android auto (https://play.google.com/store/apps/details?id=com.google.android.projection.gearhead)

	"com.google.android.setupwizard"
	"com.google.android.setupwizard.a_overlay"
	"com.google.android.pixel.setupwizard"
	# It's the basic configuration setup guides you through the basics of setting up Google features on your device.
	# The second package is only present on Pixel phones.

	"com.google.android.soundpicker" 
	# Google Sounds. Removable if you already have another media select service.

	"com.google.android.street" 
	# Google Street View (https://play.google.com/store/apps/details?id=com.google.android.street)

	"com.google.android.syncadapters.bookmarks"
	# Synchronisation for Google Chrome bookmarks

	"com.google.android.syncadapters.calendar" 
	# Synchronisation for Google Calendar.

	"com.google.android.syncadapters.contacts" 
	# Synchronisation for Google Contacts.

	"com.google.android.talk" 
	# Google Hangouts (https://play.google.com/store/apps/details?id=com.google.android.talk)

	"com.google.android.tts" 
	# Text-to-speech (https://play.google.com/store/apps/details?id=com.google.android.tts)
	# Powers apps to read text on your scream aloud, in many languages

	"com.google.android.tv.remote" 
	# Android TV remote control (https://play.google.com/store/apps/details?id=com.google.android.tv.remote)

	"com.google.android.videoeditor"
	# Google Movie Studio (discontinued)

	"com.google.android.videos" 
	# Google Play Movies & TV (https://play.google.com/store/apps/details?id=com.google.android.videos)

	"com.google.android.voicesearch"
	# Google Voice Search (Speech-To-Text)

	"com.google.android.vr.home" 
	# Daydream (VR stuff) (https://play.google.com/store/apps/details?id=com.google.android.vr.home)

	"com.google.android.vr.inputmethod" 
	# Daydream virtual keyboard (VR stuff) (https://play.google.com/store/apps/details?id=com.google.android.vr.inputmethod)

	"com.google.android.wearable.app" 
	# Wear OS Smartwatch (https://play.google.com/store/apps/details?id=com.google.android.wearable.app)

	"com.google.android.youtube" 
	# YouTube app (https://play.google.com/store/apps/details?id=com.google.android.youtube)
	"com.google.ar.core" 
	# Google Play Services for AR (Augmented Reality) (https://play.google.com/store/apps/details?id=com.google.ar.core)

	"com.google.ar.lens" 
	# Google Lens (for AR too) (https://play.google.com/store/apps/details?id=com.google.ar.lens)

	"com.google.chromeremotedesktop" 
	# Chrome Remote Desktop (https://play.google.com/store/apps/details?id=com.google.chromeremotedesktop)

	"com.google.earth" 
	# Google Earth (https://play.google.com/store/apps/details?id=com.google.earth)

	"com.google.marvin.talkback" 
	# Android Accessibility Suite (https://play.google.com/store/apps/details?id=com.google.android.marvin.talkback)

	"com.google.samples.apps.cardboarddemo" 
	# Google Cardboard (VR stuff) (https://play.google.com/store/apps/details?id=com.google.samples.apps.cardboarddemo)

	"com.google.tango.measure" 
	# Google Measure (https://play.google.com/store/apps/details?id=com.google.tango.measure)

	"com.google.vr.cyclops" 
	# Google Cardboard Camera (VR stuff) (https://play.google.com/store/apps/details?id=com.google.vr.cyclops)

	"com.google.vr.expeditions" 
	# Google Expedition (VR stuff) (https://play.google.com/store/apps/details?id=com.google.vr.expeditions)

	"com.google.vr.vrcore" 
	# Google VR services (https://play.google.com/store/apps/details?id=com.google.vr.vrcore)

	"com.google.zxing.client.android" 
	# Google Barcode Scanner (Discontinued) (https://play.google.com/store/apps/details?id=com.google.zxing.client.android)


	####################################  ADVANCED DEBLOAT  ####################################

	#"com.google.android.apps.maps" 
	# Google maps (https://play.google.com/store/apps/details?id=com.google.android.apps.maps)

	#"com.google.android.apps.nexuslauncher"
	# Nexus Launcher (https://play.google.com/store/apps/details?id=com.google.android.apps.nexuslauncher)
	# It's basically the home screen, the way icons apps are organized and displayed.
	# DON'T REMOVE THIS IF YOU DIDN'T INSTALL ANOTHER LAUNCHER ! 

	#"com.google.android.apps.photos" 
	# Google photos (https://play.google.com/store/apps/details?id=com.google.android.apps.photos)

	#"com.google.android.apps.turbo"
	# Device Health Services (discontinued ?)
	# Calculates your remaining battery percentage based on your usage
	# Reviews for this app were... funny (https://www.reddit.com/r/google/comments/ajnbmh/the_reviews_for_device_health_services_are_quite/)
	# Note: this app needs com.google.android.gms

	"com.google.android.apps.work.oobconfig" # [MORE INFO NEEDED]
	# Needs internet to fetchs enterprise and carrier lock config via internet.
	# Run in background and is triggered when a SIM card is added/removed/replaced. 
	# It also has Gcm receiver (Google Cloud Messaging receiver)
	# Has a lot of permissions (16).
	# Talks to websites providing SSL certificates.
	# Needs Google Play Services to work.
	# https://www.hybrid-analysis.com/sample/71bcaf2e71d78665fc5bc53db39df5309f24dd4ecab6402cf6ca20027dc6ecad?environmentId=200

	#"com.google.android.carrierconfig" # [MORE INFO NEEDED]
	# Provides network overrides for carrier configuration.
	# The configuration available through CarrierConfigManager is a combination of default values, default network overrides, 
	# and carrier overrides. The default network overrides are provided by this service.
	# What's the difference between com.android.carrierconfig and this package?

	#"com.google.android.contacts" 
	# Google Contacts (https://play.google.com/store/apps/details?id=com.google.android.contacts)

	#"com.google.android.dialer"
	# Google Dialer (https://play.google.com/store/apps/details?id=com.google.android.dialer)
	# It's not default but seriously, don't use the Google dialer there are Google Analytics embedded inside
	# https://www.virustotal.com/gui/file/a978d90f27d5947dca33ed59b73bd8efbac67253f9ef7a343beb9197c8913d1a/details

	#"com.google.android.documentsui"
	# Google File manager

	"com.google.android.email"
	# AOSP Mail client
	# Does no longer exist. AOSP Mail is now com.android.email and Gmail is com.google.android.gm
	
	#"com.google.android.ext.shared"
	# Google shared library (used to share common code between apps)
	# For now the library (android.ext.shared is empty). So this apk is useless. 
	# This apk has the same role than the one above.

	"com.google.android.tag"
	# Support for NFC tags interactions (5 permissions : Contacts/Phone On by default).
	# NFC Tags are for instance used in bus to let you validate your transport card with your phone 
	# Other exemple : https://en.wikipedia.org/wiki/TecTile
	# You will still be able to connect to a NFC device (e.g a speaker) if removed.
	#"com.google.android.GoogleCamera" 
	# Google Camera (https://play.google.com/store/apps/details?id=com.google.android.GoogleCamera)

	#"com.google.android.gms" 
	# Google Play Services
	# gms = Google Mobile Services
	# It is a layer that sits on top of the OS and provide a bunch of Google APIs, giving developers access to various Google Services.
	# If you remove it all the apps relying on Google Play Services whill either : 
	# - detect the lack of play services and refuse to run
	# - detect the lack of play services but allow you to run (not properly) by dismissing a annoying popup
	# With some phones, removing Google Play Services bootloop the device so be careful.
	# NOTE : Deleting this package will improve a LOT your battery life !
	#
	# IMPORTANT : You need to uncheck Find My Device from the "Device admin apps" settings panel to be able to uninstall this package
	# Search "admin" in the settings search bar.

	#"com.google.android.overlay.gmsconfig" # [MORE INFO NEEDED]
	#"com.google.android.overlay.gmsgsaconfig"
	# Probably RROs (https://source.android.com/devices/architecture/rros?hl=en) tied to "com.google.android.gms" (Google Play Services)
	# If you remove com.google.android.gms you can remove those 2 packages as well.

	"com.google.android.gms.location.history"
	# Google Location history
	# https://support.google.com/accounts/answer/3118687?hl=en

	"com.google.android.gms.policy_sidecar_aps" # [MORE INFO NEEDED]
	# Talks to Gmail.com and Google.com.
	# Needs a Google Account and Google Play Services to work correctly.
	# I don't know much more but it's sufficient to know you can debloat it.
	# Given its name maybe it is related to Android auto? 
	# https://www.hybrid-analysis.com/sample/c710b66d043026007666966d933e3a1ed29720c5009764c01b5f056232a3518a?environmentId=200

	#"com.google.android.gsf"
	# Google Services Framework
	# Supports the Play Services application in a variety of ways from application updates, user authentication, location services, user searches & more 
	# https://android.stackexchange.com/questions/216176/what-is-the-exact-functionality-of-google-play-services-google-services-framew
	# https://stackoverflow.com/questions/37337448/what-is-the-difference-between-google-service-frameworkgsfgoogle-mobile-servi
	# Same recommendation than com.google.android.gms except that I've never seen a bootloop because of deleting this package.

	#"com.google.android.gsf.login" 
	# Support for managing Google accounts
	# Safe to remove if you don't use a Google account.

	#"com.google.android.location"
	# Handles location services on older devices. On newer ones Google location services is part of Google Play Services and
	# Android location service is provided by com.android.location.fused or com.android.location.

	"com.google.android.partnersetup"
	# Enables applications to perform functionality that requires access to your Google account information
	# Safe to remove if you don't have a Google account

	#"com.google.android.webview"
	# Android WebView is a system component for the Android operating system (OS) that allows Android apps to display content 
	# from the web directly inside an application. It's based on Chrome.
	# On open-source privacy oriented Webview is Bromite (https://www.bromite.org/system_web_view)
	# https://play.google.com/store/apps/details?id=com.google.android.webview

	#"com.google.android.launcher" 
	# Google Now Launcher (https://play.google.com/store/apps/details?id=com.google.android.launcher)
	# DO NOT REMOVE THIS IF YOU DON'T HAVE ANOTHER LAUNCHER INSTALLED.

	#"com.google.android.gm" # Gmail (https://play.google.com/store/apps/details?id=com.google.android.gm)

	#"com.google.android.ims" 
	# Carrier Services (for Google phones) (https://play.google.com/store/apps/details?id=com.google.android.ims)
	# IMS is an open industry standard for voice and multimedia communications over packet-based IP networks (Volte/VoIP/Wifi calling).

	#"com.android.vending" 
	# Google Play Store app.
	
	########  GOOGLE KEYBOARD  ########
	# I'm not sure if you can delete Google keyboard on Google phone without having issue to unlock your phone on boot.
	# If someone can test it, that will be great ! :D

	#"com.google.android.inputmethod.latin" 
	# Google Keyboard (https://play.google.com/store/apps/details?id=com.google.android.inputmethod.latin)

	#"com.google.android.apps.inputmethod.hindi" 
	# Google Keyboard + Hinndi characters (https://play.google.com/store/apps/details?id=com.google.android.apps.inputmethod.hindi)

	#"com.google.android.inputmethod.japanese"
	# Google Keyboard + Japanese characters (https://play.google.com/store/apps/details?id=com.google.android.inputmethod.japanese)

	#"com.google.android.inputmethod.korean"
	# Google Keyboard + Korean characters (https://play.google.com/store/apps/details?id=com.google.android.inputmethod.korean)

	#"com.google.android.inputmethod.pinyin"
	# Google Keyboard + Pinyin (chinese) characters (https://play.google.com/store/apps/details?id=com.google.android.inputmethod.pinyin)
	)


##################### YOU SHOULDN'T MESS WITH THEM (core packages and may cause bootloop)  #####################

#"com.google.android.captiveportallogin"
# Support for captive portal : https://en.wikipedia.org/wiki/Captive_portal
# A captive portal login is a web page where the users have to input their login information or accept the displayed terms of use. 
# Some networks (typically public wifi network) use the captive portal login to block access until the user inputs 
# some necessary information
# NOTE : This package is a now a mandatory mainline module (https://www.xda-developers.com/android-project-mainline-modules-explanation/)

#"com.google.android.modulemetadata"
# Module that contains ... metadata about the list of modules on the device. And that’s about it.
# I wouldn't advise you to mess with it as it could break the proper working of other important modules (see #37)	
# Good explanation of what android modules are : https://www.xda-developers.com/android-project-mainline-modules-explanation/

#"com.google.android.ext.services"
#"com.google.android.overlay.modules.ext.services"
# Android Services Library that contains an "Android Notification Ranking Service." 
# It sorts notifications by "importance" based on things like freshness, app type (IM apps come first), and by contact. 
# The library android.ext.services is open-source. Google probably uses it to update its API without having to rely to the OEM
# It is a mainline module and is needed to boot since Android 11! NO NOT REMOVE IT!
# https://source.android.com/devices/architecture/modular-system/extservices
# https://arstechnica.com/gadgets/2016/11/android-extensions-could-be-googles-plan-to-make-android-updates-suck-less/

#"com.google.android.networkstack"
# Network Stack Components
# https://source.android.com/devices/architecture/modular-system/networking
# Provides common IP services, network connectivity monitoring, and captive login portal detection.

#"com.google.android.networkstack.permissionconfig"
# Network Stack Permission Configuration
# Defines a permission that enables modules to perform network-related tasks.
# https://source.android.com/devices/architecture/modular-system/networking

#"com.google.android.packageinstaller"
#"com.google.android.packageinstaller.a_overlay"
# Gives ability to install, update or remove applications on the device.
# If you delete this package, your phone will probably bootloop.

#"com.google.android.permissioncontroller"
#"com.google.android.overlay.modules.permissioncontroller"
#"com.google.android.overlay.modules.permissioncontroller.forframework"
# The PermissionController module enables updatable privacy policies and UI elements.
# For example, the policies and UI around granting and managing permissions.
# https://source.android.com/devices/architecture/modular-system/permissioncontroller