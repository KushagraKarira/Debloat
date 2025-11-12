#include <gtk/gtk.h>
#include <glib.h>
#include <gio/gio.h>

// --- STRUCTS AND DATA MANAGEMENT ---

// Structure for package definitions
typedef struct {
    const gchar *display_name;
    const gchar *package_name;
} PackageEntry;

// Structure to hold information about a single installed package instance
typedef struct {
    gchar *package_name;
    gchar *display_name;
    gboolean is_selected;
    GtkWidget *row; // Reference to the list row for easy update
} AppItem;

// Structure to hold global application state
typedef struct {
    GtkApplicationWindow *window;
    GtkWidget *device_list_box;
    GtkWidget *app_list_box;
    GtkWidget *action_bar;
    GtkWidget *debloat_button;
    GtkWidget *status_label;
    gchar *selected_device_id;
    GList *app_list; // List of AppItem*
} AppState;

// Global state instance
static AppState *app_state = NULL;

// --- FUNCTION PROTOTYPES ---
static void update_app_list(AppState *state);
static void update_action_bar_visibility(AppState *state);


// --- PACKAGE DATA ---

// Long list of Samsung bloatware packages
static const PackageEntry SAMSUNG_APPS[] = {
    {"Samsung Message", "com.samsung.android.messaging"},
    {"Windows Link", "com.microsoft.appmanager"},
    {"Samsung E-Commerce", "com.samsung.ecomm.global.in"},
    {"My Galaxy", "com.mygalaxy"},
    {"Smart Switch", "com.sec.android.easyMover"},
    {"OneDrive", "com.microsoft.skydrive"},
    {"Opera Max VPN", "com.opera.max.oem"},
    {"Samsung Apps Store", "com.sec.android.app.samsungapps"},
    {"Samsung Keyboard", "com.sec.android.inputmethod"},
    {"My Files", "com.sec.android.app.myfiles"},
    {"Samsung Browser", "com.sec.android.app.sbrowser"},
    {"Samsung Page", "com.samsung.android.app.spage"},
    {"Bixby Vision", "com.samsung.android.visionintelligence"},
    {"Bixby Agent", "com.samsung.android.bixby.agent"},
    {"Bixby Action", "com.samsung.android.bixby.es.globalaction"},
    {"Bixby Vision Framework", "com.samsung.android.bixbyvision.framework"},
    {"Bixby Dummy", "com.samsung.android.bixby.agent.dummy"},
    {"Vision Cloud Agent", "com.samsung.android.visioncloudagent"},
    {"Vision Provider", "com.samsung.visionprovider"},
    {"Bixby Wakeup", "com.samsung.android.bixby.wakeup"},
    {"Bixby Voice Input", "com.samsung.android.bixby.voiceinput"},
    {"AASA Service", "com.samsung.aasaservice"},
    {"Auto Preconfig", "com.sec.android.AutoPreconfig"},
    {"BBC Agent", "com.samsung.android.bbc.bbcagent"},
    {"Flipboard", "flipboard.boxer.app"},
    {"Drive Link Stub", "com.samsung.android.drivelink.stub"},
    {"Choco Cooky Font", "com.monotype.android.font.chococooky"},
    {"Partner Bookmarks", "com.android.providers.partnerbookmarks"},
    {"Shared Storage Backup", "com.android.sharedstoragebackup"},
    {"Wallpaper Cropper", "com.android.wallpapercropper"},
    {"Watch Manager Stub", "com.samsung.android.app.watchmanagerstub"},
    {"BC Service", "com.sec.bcservice"},
    {"T-Mobile Adapt", "com.tmobile.pr.adapt"},
    {"Cool Jazz Font", "com.monotype.android.font.cooljazz"},
    {"Factory App", "com.sec.factory"},
    {"Samsung Email", "com.samsung.android.email.provider"},
    {"EPDG Test App", "com.sec.epdgtestapp"},
    {"Facebook App Installer", "com.facebook.system"},
    {"Facebook App Manager", "com.facebook.appmanager"},
    {"Easy Mode Widget", "com.sec.android.widgetapp.easymodecontactswidget"},
    {"Samsung Apps Widget", "com.sec.android.widgetapp.samsungapps"},
    {"Samsung Health Service", "com.sec.android.service.health"},
    {"IMS Settings", "com.samsung.advp.imssettings"},
    {"Keyguard Wallpaper Updater", "com.samsung.android.keyguardwallpaperupdator"},
    {"Live Wallpaper Picker", "com.android.wallpaper.livepicker"},
    {"Magnifier", "com.sec.android.app.magnifier"},
    {"Media Share Service", "com.samsung.android.allshare.service.mediashare"},
    {"Photo Retouching", "com.sec.android.mimage.photoretouching"},
    {"Photo Table Dream", "com.android.dreams.phototable"},
    {"Print Spooler", "com.android.printspooler"},
    {"Rosemary Font", "com.monotype.android.font.rosemary"},
    {"Safety Information", "com.samsung.safetyinformation"},
    {"Samsung Billing", "com.sec.android.app.billing"},
    {"Mirror Link", "com.samsung.android.app.mirrorlink"},
    {"Setup Wizard", "com.sec.android.app.SecSetupWizard"},
    {"SMT", "com.samsung.SMT"},
    {"Policy DM", "com.policydm"},
    {"Security Log Agent", "com.samsung.android.securitylogagent"},
    {"Smartcard Manager", "com.sec.smartcard.manager"},
    {"Open Mobile API", "org.simalliance.openmobileapi.service"},
    {"Beacon Manager", "com.samsung.android.beaconmanager"},
    {"Trustonic TUI Service", "com.trustonic.tuiservice"},
    {"UI BC Virtual Softkey", "com.sec.android.uibcvirtualsoftkey"},
    {"TalkBack", "com.samsung.android.app.talkback"},
    {"S Voice Sync", "com.samsung.svoice.sync"},
    {"With TV", "com.samsung.android.app.withtv"},
    {"Yahoo Edge Finance", "com.samsung.android.widgetapp.yahooedge.finance"},
    {"Yahoo Edge Sport", "com.samsung.android.widgetapp.yahooedge.sport"},
    {"People Stripe", "com.samsung.android.service.peoplestripe"},
    {"Game Home", "com.samsung.android.game.gamehome"},
    {"Game Tools", "com.samsung.android.game.gametools"},
    {"Mobeam Barcode", "com.mobeam.barcodeService"},
    {"Location Service", "com.sec.location.nsflp2"},
    {"Easy Setup", "com.samsung.android.easysetup"},
    {"Easy One Hand", "com.sec.android.easyonehand"},
    {"Lookup Dictionary", "com.diotek.sec.lookup.dictionary"},
    {"Easy Mover Agent", "com.sec.android.easyMover.Agent"},
    {"ANT Service Socket", "com.dsi.ant.service.socket"},
    {"ANT Channel Sample", "com.dsi.ant.sample.acquirechannels"},
    {"ANT Server", "com.dsi.ant.server"},
    {"VR Service", "com.samsung.android.hmt.vrsvc"},
    {"Chrome", "com.android.chrome"},
    {"Google TTS", "com.google.android.tts"},
    {"Samsung Pass", "com.samsung.android.samsungpass"},
    {"Samsung Cloud", "com.samsung.android.scloud"},
    {"Samsung Pay Framework", "com.samsung.android.spayfw"},
    {"S Voice", "com.samsung.android.svoice"},
    {"Mate Agent", "com.samsung.android.mateagent"},
    {"Android Easter Egg", "com.android.egg"},
    {"S Voice IME", "com.samsung.android.svoiceime"},
    {"Theme Store", "com.samsung.android.themestore"},
    {"Simple Sharing", "com.samsung.android.app.simplesharing"},
    {"WSO MACP", "com.wsomacp"},
    {"LED Cover", "com.sec.android.cover.ledcover"},
    {"LED Cover Dream", "com.samsung.android.app.ledcoverdream"},
    {"Desktop Launcher (DeX)", "com.sec.android.app.desktoplauncher"},
    {"DeX UI Service", "com.sec.android.desktopmode.uiservice"},
    {"Secure Folder", "com.samsung.knox.securefolder"},
    {"Smart Mirroring", "com.samsung.android.smartmirroring"},
    {"Yelp Edge Panel", "com.yelp.android.samsungedge"},
    {"CNN Edge Panel", "com.cnn.mobile.android.phone.edgepanel"},
    {"Knox Container Agent", "com.samsung.android.knox.containeragent"},
    {"Samsung Pass Autofill", "com.samsung.android.samsungpassautofill"},
    {"DA Agent", "com.samsung.android.da.daagent"},
    {"Android Emergency", "com.android.emergency"},
    {"Emergency Mode Service", "com.sec.android.emergencymode.service"},
    {"Google VR VRCOR", "com.google.vr.vrcore"},
    {"Hancom Office", "com.hancom.office.editor.hidden"},
    {"Game Service", "com.enhance.gameservice"},
    {"ANT Plus Plugins", "com.dsi.ant.plugins.antplus"},
    {"T-Mobile Name ID", "com.tmobile.services.nameid"},
    {"T-Mobile VVM", "com.samsung.tmovvm"},
    {"Print Service Rec.", "com.google.android.printservice.recommendation"},
    {"Managed Provisioning", "com.android.managedprovisioning"},
    {"SPP Push", "com.sec.spp.push"},
    {"Camera Avatar Auth", "com.sec.android.app.camera.avatarauth"},
    {"Task Edge", "com.samsung.android.app.taskedge"},
    {"Clipboard Edge", "com.samsung.android.app.clipboardedge"},
    {"Assistant Menu", "com.samsung.android.app.assistantmenu"},
    {"Data Create", "com.sec.android.app.DataCreate"},
    {"Proxy Handler", "com.android.proxyhandler"},
    {"Qualcomm ATFWD", "com.qualcomm.atfwd"},
    {"Qualcomm Time Service", "com.qualcomm.timeservice"},
    {"Basic Dreams", "com.android.dreams.basic"},
    {"MDM", "com.samsung.android.mdm"},
    {"Quick Tool", "com.sec.android.app.quicktool"},
    {"Browser Edge", "com.samsung.android.app.sbrowseredge"},
    {"Emergency Mode Provider", "com.sec.android.provider.emergencymode"},
    {"MDM Sim Pin", "com.sec.enterprise.mdm.services.simpin"},
    {"MDM VPN", "com.sec.enterprise.mdm.vpn"},
    {"Bookmark Provider", "com.android.bookmarkprovider"},
    {"Wallpaper Cropper 2", "com.sec.android.wallpapercropper2"},
    {"Facebook Katana", "com.facebook.katana"},
    {"BBC File Provider", "com.samsung.android.bbc.fileprovider"},
    {"MMS Service", "com.android.mms.service"},
    {"App Push", "com.sec.app.push"},
    {"AllShare File Share", "com.samsung.android.allshare.servicefileshare"},
    {"Find My Mobile (FMM)", "com.samsung.android.fmm"},
    {"Professional Audio Utility", "com.samsung.android.sdk.professionalaudio.utility.jammonitor"},
    {"Samsung Fast", "com.samsung.android.fast"},
    {NULL, NULL} // End marker
};

// Long list of Xiaomi bloatware packages
static const PackageEntry XIAOMI_APPS[] = {
    {"Mi Browser", "com.android.browser"},
    {"Mi Global Browser", "com.mi.globalbrowser"},
    {"Facemoji Lite", "com.facemoji.lite.xiaomi"},
    {"Calendar", "com.android.calendar"},
    {"Messages", "com.android.mms"},
    {"Mi Wallet India", "com.mipay.wallet.in"},
    {"AutoNavi Minimap", "com.autonavi.minimap"},
    {"Baidu Duersdk", "com.baidu.duersdk.opensdk"},
    {"Baidu Input", "com.baidu.input_mi"},
    {"Baidu Searchbox", "com.baidu.searchbox"},
    {"CAF FM Radio", "com.caf.fmradio"},
    {"Mi Remote Controller", "com.duokan.phone.remotecontroller"},
    {"Facebook App Manager", "com.facebook.appmanager"},
    {"Facebook Katana", "com.facebook.katana"},
    {"Facebook Services", "com.facebook.services"},
    {"Facebook App Installer", "com.facebook.system"},
    {"Mi Fashion Gallery", "com.mfashiongallery.emag"},
    {"Mi File Explorer", "com.mi.android.globalFileexplorer"},
    {"Mi Personal Assistant", "com.mi.android.globalpersonalassistant"},
    {"Mi Link Service", "com.milink.service"},
    {"Mi Live Assistant", "com.mi.liveassistant"},
    {"Mi Wallet", "com.mipay.wallet"},
    {"Mi Analytics", "com.miui.analytics"},
    {"Mi Fashion Gallery", "com.miui.android.fashiongallery"},
    {"Mi Antispam", "com.miui.antispam"},
    {"Mi Feedback/Bugreport", "com.miui.bugreport"},
    {"Mi Calculator", "com.miui.calculator"},
    {"Mi Clean Master", "com.miui.cleanmaster"},
    {"Mi Cloud Backup", "com.miui.cloudbackup"},
    {"Mi Cloud Service", "com.miui.cloudservice"},
    {"Mi Cloud Service Sysbase", "com.miui.cloudservice.sysbase"},
    {"Mi Compass", "com.miui.compass"},
    {"Mi Content Catcher", "com.miui.contentcatcher"},
    {"Mi Daemon", "com.miui.daemon"},
    {"Mi FM Radio", "com.miui.fm"},
    {"Mi Gallery", "com.miui.gallery"},
    {"Mi Home Launcher", "com.miui.home"},
    {"Mi Hybrid", "com.miui.hybrid"},
    {"KLO Bugreport", "com.miui.klo.bugreport"},
    {"Main System Advertising (MSA)", "com.miui.msa.global"},
    {"Mi Notes", "com.miui.notes"},
    {"Mi Personal Assistant", "com.miui.personalassistant"},
    {"Mi Music Player", "com.miui.player"},
    {"Mi Weather Provider", "com.miui.providers.weather"},
    {"Mi Screen Recorder", "com.miui.screenrecorder"},
    {"System Ad Solution", "com.miui.systemAdSolution"},
    {"Mi Kingsoft Translation", "com.miui.translation.kingsoft"},
    {"Mi Youdao Translation", "com.miui.translation.youdao"},
    {"Mi Video", "com.miui.video"},
    {"Mi Video Player", "com.miui.videoplayer"},
    {"Mi Video Player Overlay", "com.miui.videoplayer.overlay"},
    {"Mi Virtual SIM", "com.miui.virtualsim"},
    {"Mi VSIM Core", "com.miui.vsimcore"},
    {"Mi Weather", "com.miui.weather2"},
    {"Mi Yellow Page", "com.miui.yellowpage"},
    {"Mi Webkit Core", "com.mi.webkit.core"},
    {"Qiyi Video", "com.qiyi.video"},
    {"Sogou Input", "com.sohu.inputmethod.sogou.xiaomi"},
    {"Xiaomi AB", "com.xiaomi.ab"},
    // {"Mi Account", "com.xiaomi.account"}, // Skipped as commented out in source
    {"Xiaomi Channel", "com.xiaomi.channel"},
    {"Game Center SDK", "com.xiaomi.gamecenter.sdk.service"},
    {"Xiaomi Joyose", "com.xiaomi.joyose"},
    {"Xiaomi JR", "com.xiaomi.jr"},
    {"Xiaomi Lens", "com.xiaomi.lens"},
    {"Mi Drop", "com.xiaomi.midrop"},
    {"Mi Drop Overlay", "com.xiaomi.midrop.overlay"},
    {"Mi Apps Store", "com.xiaomi.mipicks"},
    {"Xiaomi O2O", "com.xiaomi.o2o"},
    {"Xiaomi Pass", "com.xiaomi.pass"},
    {"Xiaomi Payment", "com.xiaomi.payment"},
    {"Mi Scanner", "com.xiaomi.scanner"},
    {"Xiaomi Shop", "com.xiaomi.shop"},
    {"Xiaomi VIP Account", "com.xiaomi.vipaccount"},
    {NULL, NULL} // End marker
};

// --- HELPER FUNCTIONS ---

/**
 * @brief Frees memory allocated for an AppItem.
 */
static void app_item_free(AppItem *item) {
    g_free(item->package_name);
    g_free(item->display_name);
    g_free(item);
}

/**
 * @brief Updates the visibility and sensitivity of the bottom action bar.
 */
static void update_action_bar_visibility(AppState *state) {
    gint selected_count = 0;
    GList *l;

    for (l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (item->is_selected) {
            selected_count++;
        }
    }

    if (selected_count > 0) {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gchar *label_text = g_strdup_printf("Ready to debloat %d app(s).", selected_count);
        gtk_label_set_text(GTK_LABEL(state->status_label), label_text);
        g_free(label_text);
        gtk_widget_set_sensitive(state->debloat_button, TRUE);
    } else {
        gtk_widget_set_visible(state->action_bar, FALSE);
    }
}

// --- CALLBACKS ---

/**
 * @brief Handler for the About button.
 */
static void on_about_clicked(GtkButton *button, GtkApplicationWindow *parent) {
    GtkWidget *dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "ADB Debloat Manager");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "© 2025 Kushagra Karira");
    
    const gchar *comments[] = {
        "A GTK4 application for debloating Android devices using ADB.",
        NULL
    };
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), comments[0]);

    const gchar *authors[] = {
        "Kushagra Karira",
        NULL
    };
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), authors);

    // Set Website/Project Link
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://kushagrakarira.com");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "kushagrakarira.com");
    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), 
        "Project Link: https://github.com/KushagraKarira/Debloat");
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog), GTK_LICENSE_CUSTOM);


    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    
    // FIX: Replaced deprecated gtk_widget_show() with gtk_window_present()
    gtk_window_present(GTK_WINDOW(dialog));
}

/**
 * @brief Handler for when an app's checkbox is toggled.
 */
static void on_app_toggled(GtkCheckButton *check_button, AppItem *item) {
    item->is_selected = gtk_check_button_get_active(check_button);
    update_action_bar_visibility(app_state);
}

/**
 * @brief Handler for when a device is selected in the left list.
 */
static void on_device_selected(GtkListBox *list_box, GtkListBoxRow *row, AppState *state) {
    if (!row) return;

    // Get the device ID stored in the row
    const gchar *device_id = g_object_get_data(G_OBJECT(row), "device-id");
    if (!device_id) return;

    if (state->selected_device_id) {
        g_free(state->selected_device_id);
    }
    state->selected_device_id = g_strdup(device_id);

    g_print("Device selected: %s\n", state->selected_device_id);

    // Load app list for the new device
    update_app_list(state);
}


/**
 * @brief Handler for the Debloat button click. (Simulated functionality)
 */
static void on_debloat_clicked(GtkButton *button, AppState *state) {
    gchar *selected_packages_str = g_strdup("");
    gint count = 0;
    GList *l;

    // 1. Collect selected packages
    for (l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (item->is_selected) {
            if (count > 0) {
                selected_packages_str = g_strconcat(selected_packages_str, ", ", item->package_name, NULL);
            } else {
                selected_packages_str = g_strconcat(selected_packages_str, item->package_name, NULL);
            }
            count++;
        }
    }

    if (count == 0) {
        g_free(selected_packages_str);
        gtk_label_set_text(GTK_LABEL(state->status_label), "No apps selected for debloat.");
        return;
    }

    g_print("Attempting to debloat %d apps on device %s: %s\n", count, state->selected_device_id, selected_packages_str);

    // SIMULATED ADB COMMANDS START
    gtk_widget_set_sensitive(state->debloat_button, FALSE);
    gtk_label_set_text(GTK_LABEL(state->status_label), "Debloating... Please wait (UI will block momentarily).");
    g_usleep(250000); 

    gint success_count = 0;
    gint failure_count = 0;
    GList *items_to_remove = NULL;


    GList *current_l = state->app_list;
    while (current_l != NULL) {
        AppItem *item = (AppItem *)current_l->data;
        GList *next_l = current_l->next; 

        if (item->is_selected) {
            // Simulated success unless it's a specific package name that we mock to fail
            if (g_str_has_prefix(item->package_name, "com.mock.fail")) {
                failure_count++;
                g_print("Simulated Failure for: %s\n", item->package_name);
            } else {
                success_count++;
                g_print("Simulated Success for: %s\n", item->package_name);

                items_to_remove = g_list_append(items_to_remove, item);
                gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), item->row);
            }
        }
        current_l = next_l; 
    }

    // Remove successful items from the internal app_list state
    GList *remove_l;
    for (remove_l = items_to_remove; remove_l != NULL; remove_l = remove_l->next) {
        AppItem *item = (AppItem *)remove_l->data;
        state->app_list = g_list_remove(state->app_list, item);
        app_item_free(item);
    }
    g_list_free(items_to_remove);


    // Update status and reset selection
    gchar *result_msg = g_strdup_printf(
        "Debloat process complete. Success: %d, Failures: %d.", 
        success_count, 
        failure_count
    );
    gtk_label_set_text(GTK_LABEL(state->status_label), result_msg);
    g_free(result_msg);
    g_free(selected_packages_str);

    gtk_widget_set_sensitive(state->debloat_button, TRUE);
    update_action_bar_visibility(state);
}


// --- LIST POPULATION FUNCTIONS ---

/**
 * @brief Populates the Device List with Samsung and Xiaomi options.
 */
static void update_device_list(AppState *state) {
    // Clear existing list (GTK4 method)
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->device_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->device_list_box), GTK_WIDGET(row));
    }

    // Device display names and unique IDs
    const gchar *mock_devices[] = {
        "Samsung (Galaxy Device)",
        "Xiaomi (Mi/Redmi Device)",
        NULL
    };

    const gchar *mock_ids[] = {
        "Samsung",
        "Xiaomi",
        NULL
    };

    GtkListBoxRow *first_row = NULL;

    for (int i = 0; mock_devices[i] != NULL; i++) {
        GtkWidget *row_widget = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

        // Device Icon
        GtkWidget *icon = gtk_image_new_from_icon_name("phone-symbolic");
        gtk_box_append(GTK_BOX(hbox), icon);
        
        // Device Label
        GtkWidget *label = gtk_label_new(mock_devices[i]);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_box_append(GTK_BOX(hbox), label);
        
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_widget), hbox);

        // Store the device ID
        g_object_set_data(G_OBJECT(row_widget), "device-id", g_strdup(mock_ids[i]));

        gtk_list_box_append(GTK_LIST_BOX(state->device_list_box), row_widget);

        if (!first_row) {
            first_row = GTK_LIST_BOX_ROW(row_widget);
        }
    }

    // Automatically select the first device
    if (first_row) {
        gtk_list_box_select_row(GTK_LIST_BOX(state->device_list_box), first_row);
        // Manually trigger the selection handler to load apps
        on_device_selected(GTK_LIST_BOX(state->device_list_box), first_row, state);
    }
}

/**
 * @brief Populates the App List with packages for the selected device.
 */
static void update_app_list(AppState *state) {
    // Clear existing app list and free internal state
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->app_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), GTK_WIDGET(row));
    }
    g_list_free_full(state->app_list, (GDestroyNotify)app_item_free);
    state->app_list = NULL;

    // Determine which package list to load
    const PackageEntry *packages_to_load = NULL;

    if (state->selected_device_id && g_strcmp0(state->selected_device_id, "Samsung") == 0) {
        packages_to_load = SAMSUNG_APPS;
    } else if (state->selected_device_id && g_strcmp0(state->selected_device_id, "Xiaomi") == 0) {
        packages_to_load = XIAOMI_APPS;
    } else {
        return;
    }

    // Populate list box with the selected packages
    for (int i = 0; packages_to_load[i].display_name != NULL; i++) {
        AppItem *item = g_new0(AppItem, 1);
        item->display_name = g_strdup(packages_to_load[i].display_name);
        item->package_name = g_strdup(packages_to_load[i].package_name);
        item->is_selected = FALSE;

        GtkWidget *row_widget = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15); 

        // 1. Checkbox
        GtkWidget *check = gtk_check_button_new();
        gtk_box_append(GTK_BOX(hbox), check);

        // 2. App Icon
        GtkWidget *icon = gtk_image_new_from_icon_name("package-x-generic-symbolic"); 
        // FIX: Corrected function call to use gtk_image_set_pixel_size
        gtk_image_set_pixel_size(GTK_IMAGE(icon), 16); 
        gtk_box_append(GTK_BOX(hbox), icon);
        
        // 3. Labels (Display Name and Package Name)
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        GtkWidget *display_label = gtk_label_new(item->display_name);
        GtkWidget *package_label = gtk_label_new(item->package_name);

        gtk_label_set_xalign(GTK_LABEL(display_label), 0.0);
        gtk_label_set_xalign(GTK_LABEL(package_label), 0.0);
        gtk_widget_set_opacity(package_label, 0.6); 
        gtk_widget_set_margin_bottom(package_label, 3); 

        gtk_box_append(GTK_BOX(vbox), display_label);
        gtk_box_append(GTK_BOX(vbox), package_label);
        gtk_box_append(GTK_BOX(hbox), vbox);

        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_widget), hbox);
        gtk_list_box_append(GTK_LIST_BOX(state->app_list_box), row_widget);

        // Store reference and connect signal
        item->row = row_widget;
        g_signal_connect(check, "toggled", G_CALLBACK(on_app_toggled), item);

        state->app_list = g_list_append(state->app_list, item);
    }

    // Ensure the action bar is hidden initially
    update_action_bar_visibility(state);
}


// --- UI SETUP ---

/**
 * @brief Creates the main application window and UI elements.
 */
static void activate(GtkApplication *app, AppState *state) {
    // Initialize application state
    state->selected_device_id = NULL;
    state->app_list = NULL;

    // Create main window
    state->window = GTK_APPLICATION_WINDOW(gtk_application_window_new(app));
    gtk_window_set_title(GTK_WINDOW(state->window), "Android Debloat Manager (GTK4/C)");
    gtk_window_set_default_size(GTK_WINDOW(state->window), 900, 600);

    // --- HEADER BAR with About Button ---
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(state->window), header_bar);

    GtkWidget *about_button = gtk_button_new_from_icon_name("help-about-symbolic");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), about_button);
    g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_clicked), state->window);

    // Main layout: Paned window
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_window_set_child(GTK_WINDOW(state->window), paned);

    // --- LEFT PANE (Device Selection, 20%) ---
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *device_header = gtk_label_new("Connected Devices");
    gtk_widget_add_css_class(device_header, "title-4");
    gtk_box_append(GTK_BOX(left_box), device_header);
    gtk_widget_set_margin_top(device_header, 10);
    gtk_widget_set_margin_bottom(device_header, 5);

    state->device_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->device_list_box), GTK_SELECTION_SINGLE);
    g_signal_connect(state->device_list_box, "row-activated", G_CALLBACK(on_device_selected), state);

    GtkWidget *device_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(device_scrolled), state->device_list_box);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(device_scrolled), 200);

    gtk_widget_set_hexpand(state->device_list_box, TRUE);
    gtk_widget_set_vexpand(state->device_list_box, TRUE);

    gtk_paned_set_start_child(GTK_PANED(paned), left_box);
    gtk_paned_set_resize_start_child(GTK_PANED(paned), FALSE); 
    gtk_paned_set_shrink_start_child(GTK_PANED(paned), FALSE);

    gtk_box_append(GTK_BOX(left_box), device_scrolled);


    // --- RIGHT PANE (App List, 80%) ---
    GtkWidget *right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *app_header = gtk_label_new("Installed User/System Apps (Select to Debloat)");
    gtk_widget_add_css_class(app_header, "title-4");
    gtk_box_append(GTK_BOX(right_vbox), app_header);
    gtk_widget_set_margin_top(app_header, 10);
    gtk_widget_set_margin_bottom(app_header, 5);
    
    state->app_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->app_list_box), GTK_SELECTION_NONE);

    GtkWidget *app_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(app_scrolled), state->app_list_box);
    gtk_box_append(GTK_BOX(right_vbox), app_scrolled);
    gtk_widget_set_vexpand(app_scrolled, TRUE);

    // --- BOTTOM ACTION BAR (Debloat) ---
    state->action_bar = gtk_action_bar_new();
    gtk_widget_set_visible(state->action_bar, FALSE); 

    state->status_label = gtk_label_new("Select apps to debloat.");
    gtk_action_bar_pack_start(GTK_ACTION_BAR(state->action_bar), state->status_label);

    state->debloat_button = gtk_button_new_with_label("DEBLOAT SELECTED APPS");
    gtk_widget_add_css_class(state->debloat_button, "suggested-action");
    gtk_action_bar_pack_end(GTK_ACTION_BAR(state->action_bar), state->debloat_button);
    g_signal_connect(state->debloat_button, "clicked", G_CALLBACK(on_debloat_clicked), state);

    // Combine right content and action bar
    GtkWidget *right_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(right_container), right_vbox);
    gtk_box_append(GTK_BOX(right_container), state->action_bar);

    gtk_paned_set_end_child(GTK_PANED(paned), right_container);

    // Set initial paned position 
    gtk_paned_set_position(GTK_PANED(paned), 180); 

    // Load initial device list (which auto-selects and loads the app list)
    update_device_list(state);

    gtk_window_present(GTK_WINDOW(state->window));
}

/**
 * @brief Frees the global state resources on application shutdown.
 */
static void on_shutdown(GtkApplication *app, gpointer user_data) {
    if (app_state->selected_device_id) {
        g_free(app_state->selected_device_id);
    }
    if (app_state->app_list) {
        g_list_free_full(app_state->app_list, (GDestroyNotify)app_item_free);
    }
    g_free(app_state);
}

// --- MAIN FUNCTION ---

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Allocate and initialize global state
    app_state = g_new0(AppState, 1);

    app = gtk_application_new("com.gemini.debloater", G_APPLICATION_DEFAULT_FLAGS);
    
    // Connect signals
    g_signal_connect(app, "activate", G_CALLBACK(activate), app_state);
    g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
