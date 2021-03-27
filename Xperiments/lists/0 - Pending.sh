#!/usr/bin/env bash

### These packages will be added in the default selection when I will find time to document them.
# Uncommented packages should be safe to remove but you still need to be **VERY** careful. 

# REMINDER : "Safe" only means "does not cause bootloop". 
# These packages can definitely break useful features (but you can easily reinstall them with the script)

declare -a pending=(

	# These packages seems to be disguished Xiaomi apps and not part of AOSP. Can someone confirm/refute this?
	#"com.android.systemui.gesture.line.overlay"
	#"com.android.systemui.navigation.bar.overlay"

	############## AOSP ###############

	############# GOOGLE ##############

	############ QUALCOMM #############
	"com.qti.qualcomm.deviceinfo"
	"com.qualcomm.qti.cne"
	#"com.qualcomm.qti.ims"
	"com.qualcomm.qti.performancemode"
	"com.qualcomm.qti.poweroffalarm"
	"com.qualcomm.qti.qdma"
	#"com.qualcomm.qti.seccamservice"
	#"com.qualcomm.qti.services.systemhelper"
	#"com.qualcomm.qti.simcontacts"
	#"com.qualcomm.qti.uceShimService"
	#"com.qualcomm.qti.uimGbaApp"
	#"com.qualcomm.qti.workloadclassifier"
	"com.qualcomm.uimremoteclient"
	#"vendor.qti.hardware.cacert.server"
	#"vendor.qti.iwlan"

	########## THIRD PARTY ############
	"com.verizon.cloudsetupwizard"
	"com.jrd.verizonuriintentservice"
	"com.tcl.vzwintents"
	"com.tct.vzwwifioffload"
	"com.vzw.easvalidation"
	"com.ts.setupwizard.overlay.overlay"
	"com.jrdcom.Elabel.overlay"
	"com.hawk.android.browser"
	"com.hiya.axolotl.tcl"
	"com.jrdcom.Elabel"
	"com.jrdcom.Elabel.a_overlay"
	"com.jrdcom.filemanager"
	"com.jrdcom.filemanager.a_overlay"
	"com.vendor.frameworkresoverlay"

	############# SAMSUNG #############	

	############### SONY ##############	
	"com.sonymobile.devicesecurity.service"
	"com.sonymobile.home.product.res.overlay"
	"com.sonymobile.swiqisystemservice"

	############# HUAWAI ##############

	############## NOKIA ##############
	# Nokia users: Please give the apk. I can't find them on the web.
	"com.evenwell.whitebalance"
	"com.evenwell.whitebalance.overlay.base"
	"com.hmdglobal.enterprise.api"

	########### XIAOMI ##############

	############# TCL ###############
	"com.android.gallery3d.refocus"
	"android.autoinstalls.config.TCL.PAI"
	"com.android.providers.tctdatahubprovider"
	#"com.tcl.android.launcher"
	#"com.tcl.android.launcher.a_overlay"
	#"com.tcl.android.launchertheme.res"
	#"com.tcl.android.launchertheme.res.overlay"
	#"com.tcl.android.wallpaper.livepicker"
	"com.tcl.aota.a_overlay"
	#"com.tcl.camera"
	#"com.tcl.camera.a_overlay"
	"com.tcl.compass"
	"com.tcl.compass.a_overlay"
	"com.tcl.demopage"
	#"com.tcl.entitlement"
	#"com.tcl.faceunlock"
	"com.tcl.fmradio"
	"com.tcl.fmradio.a_overlay"
	#"com.tcl.fota.system"
	#"com.tcl.fota.system.a_overlay"
	"com.tcl.healthy"
	"com.tcl.logger.a_overlay"
	"com.tcl.mibc.tclplus"
	"com.tcl.mibc.tclplus.a_overlay"
	"com.tcl.nfc.gsma.usermenu"
	"com.tcl.screenrecorder"
	"com.tcl.screenrecorder.a_overlay"
	"com.tcl.screenshotex"
	"com.tcl.sos"
	"com.tcl.tclswitch.a_overlay"
	"com.tcl.token"
	"com.tcl.usercare"
	"com.tcl.usercare.a_overlay"
	#"com.tclhz.gallery"
	#"com.tclhz.gallery.a_overlay"
	#"com.tct"
	#"com.tct.aio"
	#"com.tct.android.secureinput"
	"com.tct.applock"
	"com.tct.batterywarning"
	"com.tct.calculator"
	"com.tct.calculator.a_overlay"
	"com.tct.cellular.arda"
	"com.tct.contacts.transfer"
	"com.tct.contacts.transfer.a_overlay"
	"com.tct.diagprotector"
	#"com.tct.dialer"
	#"com.tct.dialer.a_overlay"
	#"com.tct.faceunlock"
	"com.tct.gamemode"
	"com.tct.gdpr"
	#"com.tct.iris"
	"com.tct.music"
	"com.tct.onetouchbooster"
	"com.tct.onetouchbooster.a_overlay"
	"com.tct.privacymode"
	"com.tct.privatespace"
	"com.tct.retaildemo"
	"com.tct.retaildemo.a_overlay"
	"com.tct.setupwizard"
	"com.tct.simplelauncher"
	"com.tct.simplelauncher.a_overlay"
	#"com.tct.simsettings"
	"com.tct.smart.account"
	"com.tct.smart.aikey"
	"com.tct.smart.cloud"
	"com.tct.smart.drivemode"
	"com.tct.smart.notes"
	"com.tct.smart.switchphone"
	"com.tct.smart.switchphone.service"
	"com.tct.soundrecorder"
	#"com.tct.systemservice"
	"com.tct.video"
	"com.tct.weather"
	"com.tct.weather.a_overlay"
	#"com.tct.wfcwebiew"
	)
