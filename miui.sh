#!/bin/sh
adb shell pm uninstall --user 0 com.android.browser #Mi Browser
adb shell pm uninstall --user 0 com.android.calendar #Calendar
# adb shell pm uninstall --user 0 com.android.deskclock #Clock
adb shell pm uninstall --user 0 com.android.mms #Messages
adb shell pm uninstall --user 0 com.facebook.appmanager #Facebook App Manager
adb shell pm uninstall --user 0 com.facebook.services #Facebook Services
adb shell pm uninstall --user 0 com.facebook.system #Facebook App Installer
adb shell pm uninstall --user 0 com.mi.android.globalFileexplorer #Administrador
adb shell pm uninstall --user 0 com.miui.analytics #Analytics
adb shell pm uninstall --user 0 com.miui.bugreport #Mi Feedback
adb shell pm uninstall --user 0 com.miui.calculator #Calculator
adb shell pm uninstall --user 0 com.miui.compass #Mi Compass
adb shell pm uninstall --user 0 com.miui.msa.global #Main System Advertising
adb shell pm uninstall --user 0 com.miui.notes #Mi Notes
adb shell pm uninstall --user 0 com.miui.player #Mi Music
adb shell pm uninstall --user 0 com.miui.screenrecorder #Mi Screen Recorder
adb shell pm uninstall --user 0 com.miui.videoplayer #Mi Video
adb shell pm uninstall --user 0 com.xiaomi.midrop #Mi Drop
adb shell pm uninstall --user 0 com.xiaomi.mipicks #Mi Apps
adb shell pm uninstall --user 0 com.xiaomi.scanner #Mi Scanner

# Google bloatware :
adb shell pm uninstall --user 0 com.google.android.apps.docs #Google Drive
adb shell pm uninstall --user 0 com.google.android.apps.maps #Google Maps 
adb shell pm uninstall --user 0 com.google.android.apps.photos #Google Photos
adb shell pm uninstall --user 0 com.google.android.apps.tachyon #Google Duo
adb shell pm uninstall --user 0 com.google.android.googlequicksearchbox #Google App
adb shell pm uninstall --user 0 com.google.android.inputmethod.latin #Gboard
adb shell pm uninstall --user 0 com.google.android.music #Google Music
adb shell pm uninstall --user 0 com.google.android.videos #Play Movies
adb shell pm uninstall --user 0 com.google.android.youtube #Youtube

echo Mail me the complete list of Xiaomi apps with device model to speed up the progress.
echo Mail id : festophobia@gmail.com
echo or directly contribute to https://github.com/KushagraKarira/Unbloat