#!/usr/bin/env bash

declare -a us_carriers=(

   ######  You REALLY should delete as much carriers packages as possible. US mobile carriers are really sc*mbags. I think you already know this.
   # There are so many scandals that I can't mention all of them here but if you are not aware here is some : 
   #
   # 1) They have been discovered to sell the real-time location of your phone to shady third parties (like data-bounty-hunters)
   # https://www.zdnet.com/article/us-cell-carriers-selling-access-to-real-time-location-data/
   # https://www.theverge.com/2019/2/6/18214667/att-t-mobile-sprint-location-tracking-data-bounty-hunters

   # 2) In 2016, Verizon strucked a $1.35M settlement with the FCC for modifying wireless user data packets to covertly track users around the internet 
   # without telling them
   # https://www.theregister.co.uk/2016/12/20/turn_inc_verizon_supercookies/
   # Others stuff for Verizon : https://www.vice.com/en_ca/article/jgem57/verizon-launches-private-search-engine-after-years-of-violating-its-customers-privacy
   #
   # 3) Not a scandal but freaking scary :
   # https://www.theverge.com/2019/5/22/18635674/att-location-ad-tracking-data-collection-privacy-nightmare
   # 
   # 4) AT&T is selling your phone calls and text messages to marketers
   # https://news.ycombinator.com/item?id=24756042
   #
   # Wikipedia pages are worth reading : 
   # https://en.wikipedia.org/wiki/AT%26T#Criticism_and_controversies
   # https://en.wikipedia.org/wiki/Verizon_Communications#Criticism
   #
   #
   # TL;DR : Your mobile carriers spy at you, sell your personnal data (e.g location) to anyone (like data-bounty-hunters for instance) and do
   # everything they can (big lobbying) to lock the internet and make more money.


   ## Vocabulary ##
   # VPL 
   # Stands for 'Vendor Provided Labor' (now 'Certified Advisor'). 
   # They are basically ambassadors for a brand. Theorically the brand gives money to a store to hire an employee dedicated to better selling 
   # its brand. Some stores just use them as normal sales employees.
   # See https://old.reddit.com/r/Bestbuy/comments/keclql/what_is_a_vpl/
   # Theses ambassadors most likely have a phone with custom apps for demonstration purposes
   # https://gitlab.com/W1nst0n/universal-android-debloater/-/issues/40
   # Example of package: "com.asurion.android.mobilerecovery.sprint.vpl

   
   ########################  T-Mobile ########################
   # Mostly old bloatware/spyware.
   # Now T-mobile opt-out their bloatwares. That's good :
   # https://www.theverge.com/2018/3/20/17142246/t-mobile-bloatware-customer-apps-android

   "com.mobitv.client.tmobiletvhd"
   # T-Mobile TV (discontinued, replaced by nl.tmobiletv.vinson) provided by mobitv (https://en.wikipedia.org/wiki/MobiTV)
   # https://play.google.com/store/apps/details?id=nl.tmobiletv.vinson&hl=en

   "com.tmobile.pr.adapt"
   # Diagnostic tool. 
   # This app can see all your installed apps, that you have allowed unknown sources on, that your rooted, 
   # and will deny your warranty saying your rooted. It constantly runs in the background.

   "com.tmobile.pr.mytmobile"
   # T-mobile app (https://play.google.com/store/apps/details?id=com.tmobile.pr.mytmobile)

   "com.tmobile.services.nameid" 
   # Name ID T-Mobile (powered by Hiya or cequint if on Samsung devices)
   # NOTE : Never trust a company which promotes call ID/spam blocking features.
   # https://techcrunch.com/2019/08/09/many-robocall-blocking-apps-send-your-private-data-without-permission/

   "com.tmobile.simlock"
   # Device Unlock.
   # Allows you to request and apply a mobile device unlock directly from the device.
   # https://support.t-mobile.com/docs/DOC-14011

   "com.tmobile.vvm.application"
   # T-Mobile Visual VoiceMail (https://play.google.com/store/apps/details?id=com.tmobile.vvm.application)
   # Lets you use your voice mail and manage your inbox without dialing into your voicemail. 

   "com.whitepages.nameid.tmobile"
   # T-mobile NAME ID by WhitePages (https://www.whitepages.com/)
   # Discontinued. Replaced by com.tmobile.services.nameid
   # https://www.fiercewireless.com/wireless/t-mobile-to-offer-name-id-service-from-whitepages
   # https://www.geekwire.com/2016/whitepages-spins-caller-id-spam-blocking-app-startup-hiya/

   "us.com.dt.iq.appsource.tmobile"
   # App Source (discontinued)
   # This app aimed at organizing all of your existing apps on the phone by category and helping you discover 
   # new apps through search and recommendations.


   ########################  Verizon  ########################
   # Everything from them is bloatware or spyware. That's simple.

   "com.asurion.android.verizon.vms"
   # Verizon Digital Secure (https://play.google.com/store/apps/details?id=com.asurion.android.verizon.vms)
   # Note : This app is developped by Asurion, a US company whose business is to sell insurances.
   # All US carriers use Asurion for the phone insurances.

   "com.customermobile.preload.vzw"
   # Verizon Store/Retail Demo Mode

   "com.LogiaGroup.LogiaDeck"
   # Mobile Services Manager
   # Seems to be a spyware. 
   # Good explainantion from someone who worked for carrier : 
   # https://www.reddit.com/r/lgv20/comments/6u0wnf/what_is_mobile_services_manager_did_i_catch_a/

   "com.motricity.verizon.ssodownloadable"
   # Verizon Login by Motricity (now Voltari)
   # Voltari provides relevance-driven mobile advertising, mobile marketing, mobile merchandising, and predictive analytics solutions.
   # Needed for My Verizon.
   # https://en.wikipedia.org/wiki/Voltari
   # https://www.lightreading.com/motricity-holds-on-to-verizon-account/d/d-id/678478?

   "com.securityandprivacy.android.verizon.vms"
   # Digital Secure (https://play.google.com/store/apps/details?id=com.securityandprivacy.android.verizon.vms&hl=en)
   # I don't know why this apps is released twice on the Play store under 2 different package name.

   "com.telecomsys.directedsms.android.SCG"
   # Verizon Location Agent
   # Location tracking (does not impact GPS function if deleted, don't worry)

   "com.vcast.mediamanager" 
   # Verizon Cloud (https://play.google.com/store/apps/details?id=com.vcast.mediamanager)

   "com.verizon.llkagent"
   # Used for Verizon store demo mode.

   "com.verizon.loginengine.unbranded" # [MORE INFO NEEDED]
   # Carrier Login Engine
   # Needed for wifi-calling. (To be confirmed)

   "com.verizon.messaging.vzmsgs"
   # Verizon Messages (https://play.google.com/store/apps/details?id=com.verizon.messaging.vzmsgs)

   "com.verizon.mips.services"
   # My Verizon Services 
   # Related to My Verizon app.

   "com.verizon.obdm" # [MORE INFO NEEDED]
   "com.verizon.obdm_permissions"
   # D-MAT.
   # Has a LOT of permissions ! (https://knowledge.protektoid.com/apps/com.verizon.obdm/d48680955d8d58bade2e6620ccb1e30b9bf23cb8e50055e10de3466da558c0ee)
   # DMAT Account ? It is used to hold shares and securities in dematerialised/electronic format.
   # Seems weird that Verizon provide this so it's likely not this.

   "com.verizon.permissions.appdirectedsms" # [MORE INFO NEEDED]
   "com.verizon.permissions.vzwappapn"
   # Custom permissions for some verizon stuff ?  

   "com.verizon.vzwavs" # [MORE INFO NEEDED]
   # Has a scary list of permissions in any case.

   "com.verizontelematics.verizonhum"
   # Hum Family Locator (https://play.google.com/store/apps/details?id=com.verizontelematics.verizonhum)

   "com.vznavigator.Generic"
   # VZ Navigator (GPS app) (https://play.google.com/store/apps/details?id=com.vznavigator.Generic)

   "com.vzw.apnlib"
   # Kind of library for Verizon APN ?
   # REMINDER : https://developer.android.com/reference/android/telephony/data/ApnSetting

   "com.vzw.apnservice"
   # APN Services.
   # REMINDER : https://developer.android.com/reference/android/telephony/data/ApnSetting

   "com.vzw.ecid"
   # Verizon Call Filter (https://play.google.com/store/apps/details?id=com.vzw.ecid)
   # NOTE : Never trust a company which promotes call ID/spam blocking features.
 
   "com.vzw.hss.myverizon"
   # My Verizon (https://play.google.com/store/apps/details?id=com.vzw.hss.myverizon)

   "com.vzw.hss.widgets.infozone.large"
   # My InfoZone Widget
   # Gives weekly tips, access to device info and account information.
   # https://www.droid-life.com/2013/02/12/verizon-introduces-my-infozone-widget-allows-easy-access-to-tips-device-info-and-account-information/

   "com.vzw.qualitydatalog"
   # Logging stuff


   ########  Motorola Verizon blotwares ! ########

   "com.motorola.mot5gmod"
   "com.motorola.vzw.mot5gmod"
   # 5G Moto Mod (https://play.google.com/store/apps/details?id=com.motorola.mot5gmod)

   "com.motorola.vzw.pco.extensions.pcoreceiver"
   # Protocol Configuration Options.
   # Related to APN configuration. 
   # https://www.freshpatents.com/-dt20180607ptan20180159824.php
      
   "com.motorola.vzw.phone.extensions"
   # Free HD wallpaper from verizon

   #"com.motorola.service.vzw.entitlement" # [MORE INFO NEEDED]
   # Deleting this package whill disable Hotspot functionality if you're a Verizon client. 
   # What you can do is preventing the phone from notifying the carrier about when you use hotspot :
   # https://android.stackexchange.com/questions/226580/how-is-verizon-suddenly-tracking-my-hot-spot-usage-on-android-and-how-do-i-disab

   "com.motorola.vzw.cloudsetup"
   # Cloud setup 

   "com.motorola.ltebroadcastservices_vzw"
   # ????

   "com.motorola.vzw.loader"
   # ????

   "com.motorola.omadm.vzw"
   # OMA Device Management for Verizon 
   # Handles configuration of the device (including first time use), enabling and disabling features provided by carriers.
   # https://en.wikipedia.org/wiki/OMA_Device_Management
   # I believe it's only useful if you want to use a Verizon service with a non branded phone (not sure at all)
   # https://www.androidpolice.com/2015/03/10/android-5-1-includes-new-carrier-provisioning-api-allows-carriers-easier-methods-of-setting-up-services-on-devices-they-dont-control/
   # Displays annoying notifications if you unlocked your bootloader

   "com.motorola.vzw.provider"
   # ????

   "com.motorola.visualvoicemail"
   # Verizon Visual Voicemail (https://play.google.com/store/apps/details?id=com.motorola.visualvoicemail)
   # Lets you manage your voicemail messages without dialing. It shows all of your messages in a list and lets you manage them on your phone

   ########  Sprint  ########

   "com.android.sprint.hiddenmenuapp"
   # Lets you access hidden features tests/settings (you need to type a special code in the dialer)
   # https://bestcellular.com/dial-codes/

   "com.asurion.home.sprint.vpl"
   # Tech Expert (for VPL devices/employees) ?
   # Now "Sprint Complete" (see below).
   # Note : This app is developped by Asurion, a US company whose business is to sell insurances.
   # All US carriers use Asurion for the phone insurances.

   "com.asurion.android.mobilerecovery.sprint.vpl"
   "com.asurion.android.mobilerecovery.sprint"
   # Sprint Protect
   # Support app (see com.asurion.android.protech.att)
   
   "com.asurion.home.sprint"
   # Sprint Complete (https://play.google.com/store/apps/details?id=com.asurion.home.sprint)
   # Lets you call or chat with live tech experts ! Maybe you will find the love of your life ! 
   # Note : See note above.

   "com.hyperlync.Sprint.Backup"
   # Sprint Backup" (https://play.google.com/store/apps/details?id=com.hyperlync.Sprint.Backup)
   # Lets you backup your phone’s content to your Sprint Backup account.
   # FYI : This app is developped by Hyperlync Technologies an Israel-based company which provide cyber-security solutions. 
   # It is now owned by Edition Ltd a big Singapour based company (https://www.reuters.com/companies/EDITol.SI)

   "com.hyperlync.Sprint.CloudBinder"
   # Sprint Cloud Binder (https://play.google.com/store/apps/details?id=com.hyperlync.Sprint.CloudBinder)
   # Hub for all you cloud accounts.
   # See package above.

   "com.locationlabs.finder.sprint.vpl"
   "com.locationlabs.finder.sprint"
   # Sprint Family Locator (https://play.google.com/store/apps/details?id=com.locationlabs.finder.sprint)
   # Lets you locate any phone registered under the Sprint family plan
   # Location labs is owned by AGV which is owned by Avast.
   # You shouldn't trust Avast.
   # FYI : https://www.google.com/search?hl=en&q=avast+privacy+nightmare
   #     https://www.vice.com/en_us/article/qjdkq7/avast-antivirus-sells-user-browsing-data-investigation
   #     https://www.pcmag.com/news/the-cost-of-avasts-free-antivirus-companies-can-spy-on-your-clicks

   "com.mobitv.client.sprinttvng"
   # Sprint TV & Movies provided by mobitv (https://en.wikipedia.org/wiki/MobiTV)
   # Lets you watch live TV and VOD.

   "com.mobolize.sprint.securewifi"
   # Secure Wifi (https://play.google.com/store/apps/details?id=com.mobolize.sprint.securewifi)
   # Sprint VPN app provided by Mobolize. You need to pay for using it. 
   # You'd better use a reliable third-party VPN if you really need to use one.
   # This one runs in background all time and every time it sees a "unsecured network" it will popup to encourage you to pay for this VPN.
   # Best ressources I know for choosing a VPN : 
   # https://thatoneprivacysite.net/choosing-the-best-vpn-for-you/
   
   "com.motorola.omadm.sprint"
   # OMA Device Management for Sprint 
   # Handles configuration of the device (including first time use), enabling and disabling features provided by carriers.
   # I believe it's only useful if you want to use a Sprint service with a non branded phone (not sure at all)
   # https://www.androidpolice.com/2015/03/10/android-5-1-includes-new-carrier-provisioning-api-allows-carriers-easier-methods-of-setting-up-services-on-devices-they-dont-control/
   # Displays annoying notifications if you unlocked your bootloader

   #"com.motorola.sprintwfc"
   # Sprint Wifi Calling
   # Provides wifi calling to Sprint customers.

   #"com.sec.sprint.wfcstub" # [MORE INFO NEEDED]
   # Seems to be related to Wifi-Calling on Samsung phone.

   "com.sprint.android.musicplus2033"
   "com.sprint.android.musicplus2033.vpl" # for VPL phones
   # Sprint Music Plus (https://play.google.com/store/apps/details?id=com.sprint.android.musicplus2033)
   # Sprint’s official Music Store and player

   "com.sprint.ecid"
   # Enhanced Caller ID
   # Enable you to hide name and phone number when you make phone calls
   # https://www.sprint.com/en/support/solutions/services/restrict-your-caller-id-information.html

   "com.sprint.care"
   # My Sprint (https://play.google.com/store/apps/details?id=com.sprint.care)
   # Lets you manage your Sprint Account and pay your bill.

   "com.sprint.ce.updater"
   # Mobile Installer
   # Used by Sprint to (force) install/update Sprint apps.
   # https://community.sprint.com/t5/Samsung/How-to-stop-quot-Mobile-Installer-quot-from-pushing-apps-to/td-p/1036387

   "com.sprint.fng"
   # Sprint Spot (https://play.google.com/store/apps/details?id=com.sprint.fng)
   # Provides Sprint customers a way to discover and access apps, services, games, TV & video, music, and more.

   "com.sprint.international.message"
   # Sprint Worldwide 
   # Just an help page for international travelers.

   "com.sprint.psdg.sw"
   # Carrier Setup Wizard
   # The first time you turn your device on, a Welcome screen is displayed. It guides you through the basics of setting up your device.
   # Here it handles the setup of Sprint features/services.

   "com.sprint.ms.cdm"
   # Sprint Device Manager
   # Mobile Device Management (MDM) allows company’s IT department to reach inside your phone in the background, allowing them to ensure 
   # your device is secure, know where it is, and remotely erase your data if the phone is stolen.
   # You should NEVER install a MDM tool on your phone. Never.
   # https://onezero.medium.com/dont-put-your-work-email-on-your-personal-phone-ef7fef956c2f
   # https://blog.cdemi.io/never-accept-an-mdm-policy-on-your-personal-phone/

   "com.sprint.ms.cnap" # [MORE INFO NEEDED]
   # Caller ID
   # cnap = Caller Name Presentation (https://en.wikipedia.org/wiki/Calling_Name_Presentation)
   # Lets you change the name that is displayed on caller ID when making a call.
   # Strange is it the same thing than "com.sprint.ecid" ?

   #"com.sprint.ms.smf.services"
   # Sprint Hub (https://play.google.com/store/apps/details?id=com.sprint.ms.smf.services)
   # Enables Sprint features (including Wifi calling) and products for devices operating on the Sprint network.

   "com.sprint.safefound"
   "com.sprint.safefound.vpl" # for VPL mobiles/employees ? 
   # Safe & Found (https://play.google.com/store/apps/details?id=com.sprint.safefound)
   # Mobile safety and security application that helps protect and locate your "loved ones". 
   # You have the ability to track and manage smartphones, tablets and Tracker all in one app.
   # https://www.sprint.com/en/support/solutions/services/safe-and-found.html

   "com.sprint.topup" # [MORE INFO NEEDED]
   # Doesn't exist anymore ? Now Sprint Pay (https://play.google.com/store/apps/details?id=com.sprintpay)

   "com.sprint.w.installer"
   # Sprint ID
   # Provides mobile ID Packs featuring apps, ringers, wallpapers, widgets and more.
   # Can (and do) force install apps you disabled.

   "com.sprint.w.v8"
   # Old app Discover App (discontinued / new package name)
   # Lets you discover Sprint apps ? 


   ######## AT&T #######

   "com.aetherpal.attdh.se"
   # Device Help for AT&T Samsung device
   # Developed by Aetherpal, a company which sells smart remote controls tools (https://en.wikipedia.org/wiki/AetherPal)
   # I guess this app is used for tech support.

   "com.aetherpal.attdh.zte"
   # Device Help for AT&T ZTE devices.
   # Developed by Aetherpal, a company which sells smart remote controls tools (https://en.wikipedia.org/wiki/AetherPal)
   # I guess this app is used for tech support.

   "net.aetherpal.device"
   # AT&T Remote Support provided by aetherpal (was acquired by VMware)
   # Allows an AT&T Advanced Support representative to assist you by accessing your device remotely.

   "com.asurion.android.mobilerecovery.att"
   # AT&T Protect Plus (discontinued. Replaced by AT&T ProTech : com.asurion.android.protech.att)
   # Help and support app. Lets you call or chat with a live U.S.-based AT&T ProTech support expert
   # Note : This app is developped by Asurion, a US company whose business is to sell insurances.
   # All US carriers use Asurion for the phone insurances.

   "com.asurion.android.protech.att"
   # AT&T ProTech (https://play.google.com/store/apps/details?id=com.asurion.android.protech.att)
   # Help and support app. Lets you call or chat with a live U.S.-based AT&T ProTech support "expert".
   # Note : This app is developped by Asurion, a US company whose business is to sell insurances.
   # All US carriers use Asurion for the phone insurances.

   "com.att.android.attsmartwifi"
   # AT&T Smart Wi-Fi (https://play.google.com/store/apps/details?id=com.att.android.attsmartwifi)
   # Finds and auto-connects to available hotspots to minimize cellular data consumption.
   # Auto-connects is not a godd idea.
   # Worth reading : 
   # https://www.europol.europa.eu/activities-services/public-awareness-and-prevention-guides/risks-of-using-public-wi-fi
   # https://www.eff.org/deeplinks/2020/01/why-public-wi-fi-lot-safer-you-think
   # You are ok if you go on HTTPS websites.
   # Use a VPN if you want to hide the domain names you visit, avoid usage restriction (no P2P, blacklisted websites...) and encrypt HTTP traffic.
   # ==> https://thatoneprivacysite.net/choosing-the-best-vpn-for-you/

   "com.att.callprotect"
   # AT&T Call Protect (https://play.google.com/store/apps/details?id=com.att.callprotect)
   # Spam call blocking app provided by Hiya 
   # NOTE : You should never trust spam blocking apps (https://itmunch.com/robocall-caught-sending-customers-confidential-data-without-consent/)

   "com.att.dh"
   # Device Help (https://play.google.com/store/apps/details?id=com.att.dh)
   # Troubleshooting app.

   "com.att.dtv.shaderemote"
   # DIRECTV Remote App (https://play.google.com/store/apps/details?id=com.att.dtv.shaderemote)
   # Lets you control DIRECTV HD receivers in your home that are connected to Internet, from your phone.
   # FYI : DIRECTV is a susbsidiary of AT&T 
   # Worth reading : https://en.wikipedia.org/wiki/DirecTV#Consumer_protection_lawsuits_and_violations

   "com.att.iqi"
   # Carrier IQ / Device Health 
   # Gathers, stores and forwards diagnostic measurements on its behalf (see https://en.wikipedia.org/wiki/Carrier_IQ)
   # Great ! A rootkit : https://en.wikipedia.org/wiki/Carrier_IQ#Rootkit_discovery_and_media_attention

   "com.att.mobiletransfer"
   # AT&T Mobile Transfer
   # Lets you transfert user data from an older AT&T phone to a new one.

   "com.att.myWireless"
   # My AT&T (https://play.google.com/store/apps/details?id=com.att.myWireless)
   # Lets you manage your AT&T account.

   "com.att.mobilesecurity"
   # AT&T Mobile Security (https://play.google.com/store/apps/details?id=com.att.mobilesecurity)
   # AT&T android antivirus.

   "com.att.mobile.android.vvm"
   # AT&T Visual Voicemail (https://play.google.com/store/apps/details?id=com.att.mobile.android.vvm)
   # Lets you manage your voicemail directly from the app without the need to dial into your mailbox.

   "com.att.tv"
   # AT&T TV (https://play.google.com/store/apps/details?id=com.att.tv)
   # Lets you Stream TV live and on demand from your phone.

   "com.att.tv.watchtv"
   # AT&T WatchTV (https://play.google.com/store/apps/details?id=com.att.tv.watchtv)
   # Lets you stream TV live and VOD form your phone.
   # No it's not the same thing than AT&T TV. Yes it's a mess. 
   # Differences with AT&T TV : https://www.cordcuttersnews.com/att-tv-vs-att-tv-now-whats-the-difference/

   #"com.matchboxmobile.wisp"
   # AT&T Hotspot 
   # Provides Hotspot functionnality I guess
   # Note : MatchBoxMobile is a UK Software company.

   "com.dti.att" # [MORE INFO NEEDED]
   # AT&T App Select
   # I guess it lets you choose AT&T apps to install.
   # It has a LOT of permissions : https://knowledge.protektoid.com/apps/com.dti.att/7a36d4f5f00bae044566221400719c75ea2f4f33bc2578a7f8210f36d718a8d6
   # Someone knows what "dti" is/means ?

   "com.locationlabs.cni.att"
   # AT&T Smart Limits
   # Parental Control tool.
   # https://m.att.com/shopmobile/wireless/features/smart-limits.html

   "com.matchboxmobile.wisp" # [MORE INFO NEEDED]
   # AT&T Hot Spots
   # Run in background. Automatically connects you to a free AT&T wifi hotspot at one of their participating partner locations 
   # such as Starbucks.

   "com.motorola.att.phone.extensions" # [MORE INFO NEEDED]
   # Provide acess to AT&T extensions in you dialer. I'm not sure tho. It's only a supposition.
   # https://asecare.att.com/tutorials/adding-and-deleting-an-extension-on-your-officehand-mobile-app-2990/?product=AT&T%20Office@Hand

   "com.motorola.attvowifi"
   # AT&T Wifi-calling
   # https://www.att.com/shop/wireless/features/wifi-calling.html

   "com.wavemarket.waplauncher"
   # AT&T Secure Family (https://play.google.com/store/apps/details?id=com.wavemarket.waplauncher)
   # Parental control app.
   # 7 trackers + 16 permissions (https://reports.exodus-privacy.eu.org/en/reports/com.wavemarket.waplauncher/latest/)

   "com.samsung.attvvm"
   # Visual Voicemail
   # Simple GUI for voicemail

   "com.synchronoss.dcs.att.r2g"
   # Setup & Transfer
   # App for transferring "contacts, photos, videos, music, call logs, and documents" from another device
   )

declare -a french_carriers=(

   ########################  Bouygues  ###########################

   "fr.bouyguestelecom.ecm.android"
   # Espace Client (https://play.google.com/store/apps/details?id=fr.bouyguestelecom.ecm.android)
   # Lets you manage your Bouygues account.

   "fr.bouyguestelecom.tv.android"
   # B.tv (https://play.google.com/store/apps/details?id=fr.bouyguestelecom.tv.android)
   # Lets you watch TV from your phone.

   "fr.bouyguestelecom.vvmandroid"
   # Messagerie vocale visuelle (https://play.google.com/store/apps/details?id=fr.bouyguestelecom.vvmandroid)
   

   ############################ Free #############################

   # You say to me.

   ########################   Orange/Sosh  ########################

   "com.orange.aura.oobe" # [MORE INFO NEEDED]
   # oobe = out-of-box-experience
   # https://en.wikipedia.org/wiki/Out-of-box_experience
   # ???

   "com.orange.mail.fr"
   # Mail Orange (https://play.google.com/store/apps/details?id=com.orange.mail.fr)

   "com.orange.miorange"
   # Lets you access to your Orange account

   "com.orange.mylivebox.fr"
   # Ma Livebox (https://play.google.com/store/apps/details?id=com.orange.mylivebox.fr)
   # Lets you manage your livebox from your phone.

   "com.orange.mysosh"
   # My Sosh (https://play.google.com/store/apps/details?id=com.orange.mysosh)
   # Lets you access to your Sosh account

   "com.orange.orangeetmoi"
   # Orange Et Moi (https://play.google.com/store/apps/details?id=com.orange.orangeetmoi)
   # Orange customer space

   "com.orange.owtv"
   # TV d'Orange (https://play.google.com/store/apps/details?id=com.orange.owtv)
   # Lets you watch TV/VOD on your phone.

   "com.orange.tdd"
   # Transfert de données (https://play.google.com/store/apps/details?id=com.orange.tdd)
   # Lets you transfer wirelessly: contacts, SMS, call log, calendar, photos, videos, audio files, etc., all from your old Android

   "com.orange.update"
   # Handles Orange apps updates.

   "com.orange.update.OrangeUpdateApplication" # [MORE INFO NEEDED]
   # Obviously related to update...

   "com.orange.vvm"
   # Messagerie vocale visuelle (https://play.google.com/store/apps/details?id=com.orange.vvm)
   # Lets you manage your voicemail with an app.

   "com.orange.wifiorange"
   # Mon Reseau (https://play.google.com/store/apps/details?id=com.orange.wifiorange)
   # Lets you measure your speed connection and find better Orange wifi hotspots.
   # Informs you also about near network incidents.

   "fr.orange.cineday"
   # Orange cineday (https://play.google.com/store/apps/details?id=fr.orange.cineday)
   # Useless app but cineday is pretty nice. 
   # Every tuesday you can invite the person of your choice in movies (within the limit of available seats).
   # You can just use https://cineday.orange.fr/cineday/

   #############################  SFR  ############################

   "com.sfr.android.moncompte"
   # SFR & Moi (https://play.google.com/store/apps/details?id=com.sfr.android.moncompte)
   # Lets you manage your SFR account

   "com.sfr.android.sfrcloud"
   # SFR Cloud (https://play.google.com/store/apps/details?id=com.sfr.android.sfrcloud)
   # Cloud provided by SFR

   "com.sfr.android.sfrmail"
   # SFR Mail (https://play.google.com/store/apps/details?id=com.sfr.android.sfrmail)

   "com.sfr.android.sfrplay"
   # SFR Play (https://play.google.com/store/apps/details?id=com.sfr.android.sfrplay)
   # VOD streaming from SFR.

   "com.sfr.android.vvm"
   # SFR Répondeur + (https://play.google.com/store/apps/details?id=com.sfr.android.vvm)
   # Lets you use your voice mail and manage your inbox without dialing into your voicemail.

   )

declare -a german_carriers=(

   ####### TELEKOM #######

   "de.telekom.tsc"
   # AppEnabler
   # tsc = Telecom Service Center
   # Used to display ads in notifications panel.
   )


