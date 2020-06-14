#!/bin/sh

# Being a developer, i understand the code that is made up of tears endless nights
# This script, as many people voted is created to remove Chinese origin apps from your smartphone, at current stage i'm working only on the top 25 apps
# The debloat project had the idea that removes apps because you paid for the hardware and should be in full control of it, even if it means enabeling sudo or selling hardware without warranty, but i also understand the company side of it.

# I belive all the below apps we're made with the one purpose, to make the world a better and easier place for all
# I also belive in Gandhi, and how he must have thought of the foriegn manufacturers when he declared The Disobey Movement
# Forgive me, In my heart i truly belive in the idea of "All is one, One is all" implying the purposse that is given to the one is to serve the humanity in it's entirity without excluding anyone.

echo Puriging app : Helo
adb shell pm uninstall --user 0 

echo Puriging app : SHAREit
adb shell pm uninstall --user 0 com.lenovo.anyshare.gps

echo Puriging app : TikTok
adb shell pm uninstall --user 0 com.ss.android.ugc.aweme
adb shell pm uninstall --user 0 com.zhiliaoapp.musically
adb shell pm uninstall --user 0 com.ss.android.ugc.trill.go

echo Puriging app : LIKE
adb shell pm uninstall --user 0 video.like

echo Puriging app : Kwai
adb shell pm uninstall --user 0 

echo Puriging app : UCBrowser 
adb shell pm uninstall --user 0 com.ucturbo
adb shell pm uninstall --user 0 com.UCMobile.intl


echo Puriging app : UCBrowser Mini
adb shell pm uninstall --user 0 com.uc.browser.en

echo Puriging app : LiveMe
adb shell pm uninstall --user 0 

echo Puriging app : Bigo Live
adb shell pm uninstall --user 0

echo Puriging app : Vigo Video
adb shell pm uninstall --user 0 com.ss.android.ugc.boomlite
adb shell pm uninstall --user 0 com.cheerfulinc.flipagram

echo Puriging app : BeautyPlus
adb shell pm uninstall --user 0

echo Puriging app : Xender 
adb shell pm uninstall --user 0 cn.xender

echo Puriging app : Cam Scanner
adb shell pm uninstall --user 0 com.intsig.camscanner

echo Puriging app : PUBG 
adb shell pm uninstall --user 0 com.tencent.ig
adb shell pm uninstall --user 0 com.tencent.iglite

echo Puriging app : Clash of Kings
adb shell pm uninstall --user 0

echo Puriging app : Mobile Legends
adb shell pm uninstall --user 0 com.mobile.legends

echo Puriging app : ClubFactory
adb shell pm uninstall --user 0

echo Puriging app : Shein
adb shell pm uninstall --user 0 

echo Puriging app : Romwe
adb shell pm uninstall --user 0

echo Puriging app : AppLock
adb shell pm uninstall --user 0 com.martianmode.applock

echo Puriging app : Club Factory
adb shell pm uninstall --user 0

echo Puriging app : VMate
adb shell pm uninstall --user 0 

echo Puriging app : Game of Sultans
adb shell pm uninstall --user 0

echo Puriging app : Mafia City
adb shell pm uninstall --user 0

echo Puriging app : Battle of Empires
adb shell pm uninstall --user 0
