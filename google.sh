#!/bin/sh
# Please read the documentation before any progres
# This script has been optimized for my Android One 10 - Mi A2, for the apps that were brought in with the update
adb shell pm uninstall --user 0 com.google.android.apps.docs #Google Drive
adb shell pm uninstall --user 0 com.google.android.apps.maps #Google Maps 
adb shell pm uninstall --user 0 com.google.android.apps.photos #Google Photos
adb shell pm uninstall --user 0 com.google.android.apps.tachyon #Google Duo
adb shell pm uninstall --user 0 com.google.android.googlequicksearchbox #Google App
adb shell pm uninstall --user 0 com.google.android.marvin.talkback #Talkback
adb shell pm uninstall --user 0 com.google.android.music #Google Music
adb shell pm uninstall --user 0 com.google.android.talk #Hangouts
# adb shell pm uninstall --user 0 com.google.android.tts #Text to spech
adb shell pm uninstall --user 0 com.google.android.videos #Play Movies
adb shell pm uninstall --user 0 com.google.android.youtube #Youtube
adb shell pm uninstall --user 0 com.google.android.apps.youtube.music #Youtube Music
adb shell pm uninstall --user 0 com.google.android.gm #Gmail
adb shell pm uninstall --user 0 com.google.android.apps.wellbeing #Digital Wellbeing
adb shell pm uninstall --user 0 com.android.chrome #GoogleChrome
adb shell pm uninstall --user 0 com.google.vr.vrcore
adb shell pm uninstall --user 0 com.google.android.youtube #YouTube
adb shell pm uninstall --user 0 com.google.android.apps.books #Google Play Books
adb shell pm uninstall --user 0 com.google.android.apps.magazines #Google News
adb shell pm uninstall --user 0 com.google.android.docs.editors.slides #Google Slides
adb shell pm uninstall --user 0 com.google.android.docs.editors.sheets #Google Sheets
adb shell pm uninstall --user 0 com.google.android.docs.editors.docs #Google Docs
adb shell pm uninstall --user 0 com.google.android.apps.nbu.paisa.user #Google Pay (TEZ) India
adb shell pm uninstall --user 0 com.google.android.apps.nbu.files #Files
# adb shell pm uninstall --user 0 com.google.android.apps.plus #Google+ #Removed from official google repo

# Use the restore script to restore all factory apps from /system
