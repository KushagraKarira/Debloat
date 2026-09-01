#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/glist.h>
#include <gio/gio.h>
#include <sys/wait.h> // Required for WIFEXITED, WEXITSTATUS, WTERMSIG

// --- STRUCTS AND DATA MANAGEMENT ---

/**
 * @brief Defines a single uninstallable package.
 */
typedef struct {
    const gchar *display_name; // User-friendly name (e.g., "Mi Browser")
    const gchar *package_name; // Technical name (e.g., "com.android.browser")
} PackageEntry;

/**
 * @brief Tracks the state of a package in the UI.
 */
typedef struct {
    gchar *package_name;
    gchar *display_name;
    gboolean is_selected;             // Is the checkbox ticked?
    GtkWidget *check_button;          // Reference to the check button
    GtkWidget *main_list_row;         // Reference to the row in the "Debloat" list
    GtkWidget *uninstalled_list_row; // Reference to the row in the "Undo" list
} AppItem;

/**
 * @brief Holds all global state for the application.
 */
typedef struct {
    AdwApplicationWindow *window; // The main application window
    GtkWidget *device_list_box;   // Left-side list of manufacturers
    GtkWidget *app_list_box;      // Right-side list of apps to debloat
    GtkWidget *action_bar;        // Bottom bar with "Remove" button
    GtkWidget *debloat_button;    // The "Remove" button
    GtkWidget *status_label;      // Label inside the action_bar
    GtkWidget *search_entry;      // Search entry for filtering apps

    // ADB Device selection
    GtkWidget *device_dropdown;     // Dropdown to select a connected device
    GtkStringList *device_model;    // Data model for the device_dropdown
    GtkWidget *device_status_label; // Label under the dropdown for feedback

    // State variables
    gchar *selected_device_id;        // The *real* serial of the selected device (e.g., "ABC12345")
    gchar *selected_manufacturer_id;  // The *category* selected (e.g., "Samsung")
    GList *app_list;                  // GList of AppItem* for the "Debloat" page
    
    // "Undo" page state
    GList *uninstalled_app_list;      // GList of AppItem* for the "Undo" page
    GtkWidget *uninstalled_list_box;  // UI list for the "Undo" page

    // "Debug" page widgets
    GtkWidget *debug_command_entry;   // Text entry for custom commands
    GtkTextBuffer *debug_output_buffer; // Text buffer for command output

} AppState;

// Global state instance, initialized in main()
static AppState *app_state = NULL;

// --- FUNCTION PROTOTYPES ---
static void update_app_list(AppState *state);
static void update_action_bar_visibility(AppState *state);
static void populate_adb_devices(AppState *state);
static void on_adb_device_selected(GtkDropDown *dropdown, GParamSpec *pspec, AppState *state);
static void on_reinstall_clicked(GtkButton *button, AppItem *item);
static void add_row_to_uninstalled_list(AppState *state, AppItem *item);
static void on_about_clicked(GtkButton *button, GtkWindow *parent);
static void show_welcome_dialog(GtkWindow *parent);
static void on_run_debug_command_clicked(GtkButton *button, AppState *state);
static void on_refresh_devices_clicked(GtkButton *button, AppState *state);
static void detect_and_select_manufacturer(AppState *state);


// --- PACKAGE DATA (Bloatware lists) ---

static const PackageEntry SAMSUNG_APPS[] = {
    {"Samsung Message", "com.samsung.android.messaging"},
    {"Windows Link", "com.microsoft.appmanager"},
    {"Samsung E-Commerce", "com.samsung.ecomm.global.in"},
    {"My Galaxy", "com.mygalaxy"},
    {"Smart Switch", "com.sec.android.easyMover"},
    {"OneDrive", "com.microsoft.skydrive"},
    {"Opera Max VPN", "com.opera.max.oem"},
    {"Samsung Apps Store", "com.sec.android.app.samsungapps"},
    {"Bixby Agent", "com.samsung.android.bixby.agent"},
    {"Facebook App Installer", "com.facebook.system"},
    {"Facebook App Manager", "com.facebook.appmanager"},
    {"Game Home", "com.samsung.android.game.gamehome"},
    {"S Voice", "com.samsung.android.svoice"},
    {"AppCloud GL", "com.aura.oobe.samsung.gl"},
    {"AppCloud", "com.aura.oobe.samsung "},
    {NULL, NULL} 
};

static const PackageEntry XIAOMI_APPS[] = {
    {"Mi Browser", "com.android.browser"},
    {"Mi Global Browser", "com.mi.globalbrowser"},
    {"Mi Wallet India", "com.mipay.wallet.in"},
    {"Mi Analytics", "com.miui.analytics"},
    {"Main System Advertising (MSA)", "com.miui.msa.global"},
    {"Mi Video", "com.miui.video"},
    {"Xiaomi Shop", "com.xiaomi.shop"},
    {"Xiaomi O2O", "com.xiaomi.o2o"},
    {"Xiaomi Pass", "com.xiaomi.pass"},
    {NULL, NULL} 
};

static const PackageEntry VIVO_APPS[] = {
    {"Vivo Browser", "com.vivo.browser"},
    {"Vivo App Store", "com.vivo.appstore"},
    {"BBK Cloud", "com.bbk.cloud"},
    {"Vivo Gallery", "com.vivo.gallery"},
    {"Vivo Weather", "com.vivo.weather"},
    {"Facebook System", "com.facebook.system"},
    {"Vivo Easy Share", "com.vivo.easyshare"},
    {"Vivo Hiboard", "com.vivo.hiboard"},
    {"MTK Logger", "com.mediatek.mtklogger"},
    {"Vivo Assistant", "com.vivo.assistant"},
    {NULL, NULL} 
};

static const PackageEntry TCL_APPS[] = {
    {"TCL Compass", "com.tcl.compass"},
    {"Demo Page", "com.tcl.demopage"},
    {"FM Radio", "com.tcl.fmradio"},
    {"Face Unlock", "com.tct.faceunlock"},
    {"Game Mode", "com.tct.gamemode"},
    {"TCL Music", "com.tct.music"},
    {"Privacy Mode", "com.tct.privacymode"},
    {"Retail Demo", "com.tct.retaildemo"},
    {"Smart Cloud", "com.tct.smart.cloud"},
    {"Smart Drive Mode", "com.tct.smart.drivemode"},
    {NULL, NULL} 
};

static const PackageEntry SONY_APPS[] = {
    {"TV SideView Video", "com.sony.tvsideview.videoph"},
    {"Creative effect", "com.sonyericsson.android.addoncamera.artfilter"},
    {"Usage Stats", "com.sonyericsson.idd.agent"},
    {"Backup/Restore", "com.sonyericsson.mtp.extension.backuprestore"},
    {"Sony Music", "com.sonyericsson.music"},
    {"Sony Themes", "com.sonymobile.themes.xperialoops2"},
    {"Xperia Lounge", "com.sonymobile.xperialounge.services"},
    {"Xperia Wallpaper", "com.sonymobile.xperiaxlivewallpaper"},
    {"Xperia Transfer", "com.sonymobile.xperiatransfermobile"},
    {"Sony Weather", "com.sonymobile.xperiaweather"},
    {NULL, NULL} 
};

static const PackageEntry REALME_APPS[] = {
    {"Android Setup", "com.google.android.setupwizard"},
    {"DocVault", "com.os.docvault"},
    {"HeyTap Cloud", "com.heytap.cloud"},
    {"Music (Oppo)", "com.oppo.music"},
    {"Video (ColorOS)", "com.coloros.video"},
    {"Clone Phone", "com.coloros.backuprestore"},
    {"Feedback Toolkit", "com.oppo.logkit"},
    {"My Realme (User Center)", "com.heytap.usercenter"},
    {"Opera News", "com.opera.branding.news"},
    {"Play Movies & TV", "com.google.android.videos"},
    {NULL, NULL} 
};

static const PackageEntry OPPO_APPS[] = {
    {"Compass App", "com.coloros.compass2"},
    {"Video App", "com.coloros.video"},
    {"Photos App (Gallery)", "com.coloros.gallery3d"},
    {"Oppo Cloud", "com.coloros.cloud"},
    {"Find My Phone", "com.coloros.findmyphone"},
    {"Game Space", "com.coloros.gamespace"},
    {"Oppo App Market", "com.oppo.market"},
    {"Oppo Music", "com.oppo.music"},
    {"Oppo Wallet", "com.coloros.wallet"},
    {"Find My Phone Client", "com.realme.findphone.client2"},
    {NULL, NULL} 
};

static const PackageEntry ONEPLUS_APPS[] = {
    {"Shot On OnePlus", "cn.oneplus.photos"},
    {"OnePlus Switch (Backup)", "com.oneplus.backuprestore"},
    {"Zen Mode", "com.oneplus.brickmode"},
    {"Game Space", "com.oneplus.gamespace"},
    {"Health Check", "com.oneplus.healthcheck"},
    {"Community Forum", "net.oneplus.forums"},
    {"Cricket Scores", "net.oneplus.opsports"},
    {"OnePlus Weather", "net.oneplus.weather"},
    {"OnePlus Widget", "net.oneplus.widget"},
    {NULL, NULL} 
};

static const PackageEntry NOKIA_APPS[] = {
    {"Data Go", "com.hmdglobal.datago"},
    {"My Phone Support", "com.hmdglobal.support"},
    {"APR Upload Service", "com.evenwell.AprUploadService"},
    {"Power Saving", "com.evenwell.PowerSaving"},
    {"Smart Switch", "com.evenwell.smartswitch"},
    {"Traffic Monitor", "com.evenwell.trafficmonitor"},
    {"Weather App", "com.evenwell.Weather"},
    {"Weather Widget", "com.evenwell.weather.widget"},
    {NULL, NULL} 
};

static const PackageEntry MOTOROLA_APPS[] = {
    {"Lenovo ID", "com.lenovo.lsf.user"},
    {"Rescue Security", "com.lmi.motorola.rescuesecurity"},
    {"FM Radio", "com.motorola.android.fmradio"},
    {"OMA Provisioning", "com.motorola.android.provisioning"},
    {"Face Unlock Agent", "com.motorola.faceunlocktrustagent"},
    {"Invisible Net", "com.motorola.invisiblenet"},
    {"Motosignature", "com.motorola.motosignature.app"},
    {"Carrier Provisioning", "com.motorola.omadm.service"},
    {"System Package", "com.motorola.pgmsystem2"},
    {"System Server", "com.motorola.systemserver"},
    {NULL, NULL} 
};

static const PackageEntry ZTE_APPS[] = {
    {"ZTE Voice Assistant", "com.zte.assistant"},
    {"ZTE Weather", "com.zte.weather"},
    {NULL, NULL} 
};

static const PackageEntry JIO_APPS[] = {
    {"Remote Control Service", "com.communitake.remotecontrolservice"},
    {"Factory Monitor (STB)", "com.sdmc.factorymonitor"},
    {"PVOD Update Service", "com.iwedia.pvodupdateservice"},
    {"Remote Care RIL (STB)", "com.jio.stbremotecare.ril"},
    {"STB Ad Service", "com.jio.stbadservice"},
    {"Log Service (STB)", "com.rjil.jiostblogservice"},
    {"TIF Extn (STB)", "com.jio.stb.tifextn"},
    {"TR069 Client", "insight.tr069.client"},
    {"Managed Provisioning", "com.android.managedprovisioning"},
    {NULL, NULL} 
};


// --- HELPER FUNCTIONS ---

/**
 * @brief Frees all memory associated with an AppItem.
 */
static void app_item_free(AppItem *item) {
    if (!item) return;
    g_free(item->package_name);
    g_free(item->display_name);
    g_free(item);
}

/**
 * @brief Shows/hides the bottom action bar based on current state.
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

    if (selected_count > 0 && state->selected_device_id != NULL) {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gchar *label_text = g_strdup_printf("Ready to debloat %d app(s) on device '%s'.", selected_count, state->selected_device_id);
        gtk_label_set_text(GTK_LABEL(state->status_label), label_text);
        g_free(label_text);
        gtk_widget_set_sensitive(state->debloat_button, TRUE);
    } 
    else if (state->selected_device_id == NULL) {
        gtk_widget_set_visible(state->action_bar, FALSE);
    }
    else {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gtk_label_set_text(GTK_LABEL(state->status_label), "Select apps from the list to remove.");
        gtk_widget_set_sensitive(state->debloat_button, FALSE);
    }
}

// --- CALLBACKS ---

/**
 * @brief Shows a welcome dialog with instructions for enabling USB Debugging.
 */
static void show_welcome_dialog(GtkWindow *parent) {
    AdwDialog *dialog = adw_alert_dialog_new(
        "Welcome to Android Debloater!",
        NULL
    );

    adw_alert_dialog_add_response(ADW_ALERT_DIALOG(dialog), "ok", "Got it!");
    adw_alert_dialog_set_default_response(ADW_ALERT_DIALOG(dialog), "ok");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 12);
    gtk_widget_set_margin_bottom(box, 12);

    const gchar *instructions = 
        "To use this tool, you must enable USB Debugging (ADB) on your device.\n\n"
        "1.  Go to Settings > About Phone.\n"
        "2.  Tap on Build Number 7 times until you see 'You are now a developer!'.\n"
        "3.  Go back to Settings > System > Developer options.\n"
        "4.  Find and turn on USB debugging.\n"
        "5.  Connect your phone to your computer via USB.\n"
        "6.  A prompt will appear on your phone: 'Allow USB debugging?'. Check 'Always allow' and tap OK.";

    GtkWidget *label = gtk_label_new(instructions);
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    gtk_box_append(GTK_BOX(box), label);

    adw_alert_dialog_set_extra_child(ADW_ALERT_DIALOG(dialog), box);
    adw_alert_dialog_choose(ADW_ALERT_DIALOG(dialog), GTK_WIDGET(parent), NULL, NULL, NULL);
}

/**
 * @brief Shows the "About" dialog.
 */
static void on_about_clicked(GtkButton *button, GtkWindow *parent) {
    (void)button;
    AdwDialog *dialog = adw_about_dialog_new();
    
    adw_about_dialog_set_application_name(ADW_ABOUT_DIALOG(dialog), "Android Debloater");
    adw_about_dialog_set_version(ADW_ABOUT_DIALOG(dialog), "0.7.2 (Libadwaita)");
    adw_about_dialog_set_developer_name(ADW_ABOUT_DIALOG(dialog), "Kushagra Karira");
    adw_about_dialog_set_copyright(ADW_ABOUT_DIALOG(dialog), "GPL-3.0 2019-2026 Kushagra Karira");
    adw_about_dialog_set_comments(ADW_ABOUT_DIALOG(dialog), "A GTK4/Libadwaita application for debloating Android devices using ADB.");
    adw_about_dialog_set_website(ADW_ABOUT_DIALOG(dialog), "https://kushagrakarira.com");
    adw_about_dialog_set_issue_url(ADW_ABOUT_DIALOG(dialog), "https://github.com/KushagraKarira/Debloat/issues");
    adw_about_dialog_add_link(ADW_ABOUT_DIALOG(dialog), "Project Link", "https://github.com/KushagraKarira/Debloat");
    
    adw_dialog_present(dialog, GTK_WIDGET(parent));
}

/**
 * @brief Called when an app's checkbox is toggled.
 */
static void on_app_toggled(GtkCheckButton *check_button, AppItem *item) {
    item->is_selected = gtk_check_button_get_active(check_button);
    update_action_bar_visibility(app_state);
}

/**
 * @brief Select all visible apps in the current list.
 */
static void on_select_all_clicked(GtkButton *button, AppState *state) {
    (void)button;
    for (GList *l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (gtk_widget_get_visible(item->main_list_row)) {
            item->is_selected = TRUE;
            if (item->check_button) {
                gtk_check_button_set_active(GTK_CHECK_BUTTON(item->check_button), TRUE);
            }
        }
    }
    update_action_bar_visibility(state);
}

/**
 * @brief Deselect all apps in the current list.
 */
static void on_deselect_all_clicked(GtkButton *button, AppState *state) {
    (void)button;
    for (GList *l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        item->is_selected = FALSE;
        if (item->check_button) {
            gtk_check_button_set_active(GTK_CHECK_BUTTON(item->check_button), FALSE);
        }
    }
    update_action_bar_visibility(state);
}

/**
 * @brief Filter the app list in real time as the user types in the search bar.
 */
static void on_search_changed(GtkSearchEntry *entry, AppState *state) {
    const gchar *search_text = gtk_editable_get_text(GTK_EDITABLE(entry));
    gchar *lower_search = search_text ? g_utf8_strdown(search_text, -1) : g_strdup("");

    for (GList *l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (strlen(lower_search) == 0) {
            gtk_widget_set_visible(item->main_list_row, TRUE);
        } else {
            gchar *lower_display = g_utf8_strdown(item->display_name, -1);
            gchar *lower_pkg = g_utf8_strdown(item->package_name, -1);

            gboolean match = (strstr(lower_display, lower_search) != NULL) ||
                             (strstr(lower_pkg, lower_search) != NULL);
            gtk_widget_set_visible(item->main_list_row, match);

            g_free(lower_display);
            g_free(lower_pkg);
        }
    }

    g_free(lower_search);
    update_action_bar_visibility(state);
}

/**
 * @brief Called when a manufacturer (device category) is selected from the left list.
 */
static void on_device_selected(GtkListBox *list_box, GtkListBoxRow *row, AppState *state) {
    (void)list_box;
    if (!row) return;

    const gchar *manufacturer_id = (const gchar *)g_object_get_data(G_OBJECT(row), "device-id");
    if (!manufacturer_id) return;

    if (state->selected_manufacturer_id) {
        g_free(state->selected_manufacturer_id);
    }
    state->selected_manufacturer_id = g_strdup(manufacturer_id);
    
    gtk_widget_set_visible(state->action_bar, FALSE);
    update_app_list(state);
}

/**
 * @brief Auto-detect device manufacturer using ADB getprop and select matching list.
 */
static void detect_and_select_manufacturer(AppState *state) {
    if (!state->selected_device_id) return;

    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status;
    GError *error = NULL;

    gchar *cmd_args[] = {
        "adb", "-s", state->selected_device_id, "shell", "getprop", "ro.product.manufacturer", NULL
    };

    g_spawn_sync(
        NULL, cmd_args, NULL, G_SPAWN_SEARCH_PATH,
        NULL, NULL, &stdout_str, &stderr_str, &exit_status, &error
    );

    if (!error && stdout_str) {
        gchar *mfg = g_strstrip(g_ascii_strdown(stdout_str, -1));
        const gchar *matched_id = NULL;

        if (strstr(mfg, "samsung")) matched_id = "Samsung";
        else if (strstr(mfg, "xiaomi") || strstr(mfg, "redmi") || strstr(mfg, "poco")) matched_id = "Xiaomi";
        else if (strstr(mfg, "oneplus")) matched_id = "OnePlus";
        else if (strstr(mfg, "oppo")) matched_id = "Oppo";
        else if (strstr(mfg, "realme")) matched_id = "RealMe";
        else if (strstr(mfg, "vivo") || strstr(mfg, "iqoo")) matched_id = "Vivo";
        else if (strstr(mfg, "sony")) matched_id = "Sony";
        else if (strstr(mfg, "motorola") || strstr(mfg, "lenovo")) matched_id = "Motorola";
        else if (strstr(mfg, "nokia") || strstr(mfg, "hmd")) matched_id = "Nokia";
        else if (strstr(mfg, "tcl") || strstr(mfg, "alcatel")) matched_id = "TCL";
        else if (strstr(mfg, "zte") || strstr(mfg, "nubia")) matched_id = "ZTE";

        if (matched_id) {
            GtkListBoxRow *row = NULL;
            int idx = 0;
            while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->device_list_box), idx++))) {
                const gchar *row_id = (const gchar *)g_object_get_data(G_OBJECT(row), "device-id");
                if (row_id && g_strcmp0(row_id, matched_id) == 0) {
                    gtk_list_box_select_row(GTK_LIST_BOX(state->device_list_box), row);
                    on_device_selected(GTK_LIST_BOX(state->device_list_box), row, state);
                    break;
                }
            }
        }
        g_free(mfg);
    }

    if (error) g_error_free(error);
    g_free(stdout_str);
    g_free(stderr_str);
}

/**
 * @brief Runs `adb devices` to find connected devices and populates the dropdown.
 */
static void populate_adb_devices(AppState *state) {
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status;
    GError *error = NULL;
    gchar *cmd_args[] = {"adb", "devices", NULL};
    gint devices_found = 0;
    gint unauthorized_found = 0;

    gtk_label_set_text(GTK_LABEL(state->device_status_label), "Refreshing device list...");

    guint n_items = g_list_model_get_n_items(G_LIST_MODEL(state->device_model));
    if (n_items > 1) {
        gtk_string_list_splice(state->device_model, 1, n_items - 1, NULL);
    }
    
    gtk_drop_down_set_selected(GTK_DROP_DOWN(state->device_dropdown), 0);
    if (state->selected_device_id) {
        g_free(state->selected_device_id);
        state->selected_device_id = NULL;
    }

    g_spawn_sync(
        NULL, cmd_args, NULL, G_SPAWN_SEARCH_PATH,
        NULL, NULL, &stdout_str, &stderr_str, &exit_status, &error
    );

    if (error) {
        gtk_label_set_text(GTK_LABEL(state->device_status_label), "Error: 'adb' command not found in PATH.");
        g_error_free(error);
        g_free(stdout_str);
        g_free(stderr_str);
        return;
    }

    if (stdout_str) {
        gchar **lines = g_strsplit(stdout_str, "\n", -1);
        
        for (int i = 0; lines[i] != NULL; i++) {
            if (i == 0) continue;

            gchar **parts = g_strsplit(lines[i], "\t", 2);
            if (g_strv_length(parts) == 2) {
                gchar *device_id = g_strstrip(parts[0]);
                gchar *device_state = g_strstrip(parts[1]);

                if (g_strcmp0(device_state, "device") == 0) {
                    gtk_string_list_append(state->device_model, device_id);
                    devices_found++;
                } else if (g_strcmp0(device_state, "unauthorized") == 0) {
                    unauthorized_found++;
                }
            }
            g_strfreev(parts);
        }
        g_strfreev(lines);
    }
    
    gchar *status_msg;
    if (devices_found > 0) {
        status_msg = g_strdup_printf("Found %d authorized device(s). Select one to begin.", devices_found);
    } else if (unauthorized_found > 0) {
        status_msg = g_strdup_printf("Found %d unauthorized device(s). Please allow USB Debugging prompt on phone.", unauthorized_found);
    } else {
        status_msg = g_strdup("No devices found. Connect via USB and refresh.");
    }
    gtk_label_set_text(GTK_LABEL(state->device_status_label), status_msg);
    g_free(status_msg);

    g_free(stdout_str);
    g_free(stderr_str);
    
    update_action_bar_visibility(state);
}

/**
 * @brief Called when a device is selected from the dropdown.
 */
static void on_adb_device_selected(GtkDropDown *dropdown, GParamSpec *pspec, AppState *state) {
    (void)pspec;
    guint pos = gtk_drop_down_get_selected(dropdown);

    if (state->selected_device_id) {
        g_free(state->selected_device_id);
        state->selected_device_id = NULL;
    }

    if (pos > 0) {
        const gchar *device_id = gtk_string_list_get_string(state->device_model, pos);
        state->selected_device_id = g_strdup(device_id);
        gtk_label_set_text(GTK_LABEL(state->device_status_label), "");
        detect_and_select_manufacturer(state);
    }
    
    update_action_bar_visibility(state);
}

static void on_refresh_devices_clicked(GtkButton *button, AppState *state) {
    (void)button;
    populate_adb_devices(state);
}

/**
 * @brief Called when an "Undo" (reinstall) button is clicked.
 */
static void on_reinstall_clicked(GtkButton *button, AppItem *item) {
    if (!app_state || !app_state->selected_device_id) {
        return;
    }

    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);

    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status;
    GError *error = NULL;

    gchar *cmd_args[] = {
        "adb",
        "-s", app_state->selected_device_id,
        "shell", 
        "cmd", 
        "package", 
        "install-existing",
        item->package_name,
        NULL
    };

    g_spawn_sync(
        NULL, cmd_args, NULL, G_SPAWN_SEARCH_PATH,
        NULL, NULL, &stdout_str, &stderr_str, &exit_status, &error
    );

    gchar *stripped_output = stdout_str ? g_strstrip(stdout_str) : NULL;

    if (error) {
        g_error_free(error);
        gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE);
    } else if (stripped_output && g_str_has_prefix(stripped_output, "Package")) {
        gtk_list_box_remove(GTK_LIST_BOX(app_state->uninstalled_list_box), item->uninstalled_list_row);
        app_state->uninstalled_app_list = g_list_remove(app_state->uninstalled_app_list, item);
        app_item_free(item);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE);
    }

    g_free(stdout_str);
    g_free(stderr_str);
}

/**
 * @brief Creates a new UI row in the "Recently Uninstalled" list.
 */
static void add_row_to_uninstalled_list(AppState *state, AppItem *item) {
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_start(hbox, 12);
    gtk_widget_set_margin_end(hbox, 12);
    gtk_widget_set_margin_top(hbox, 8);
    gtk_widget_set_margin_bottom(hbox, 8);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    GtkWidget *display_label = gtk_label_new(item->display_name);
    GtkWidget *package_label = gtk_label_new(item->package_name);

    gtk_label_set_xalign(GTK_LABEL(display_label), 0.0);
    gtk_label_set_xalign(GTK_LABEL(package_label), 0.0);
    gtk_widget_add_css_class(package_label, "caption");
    gtk_widget_set_opacity(package_label, 0.7);

    gtk_box_append(GTK_BOX(vbox), display_label);
    gtk_box_append(GTK_BOX(vbox), package_label);
    gtk_widget_set_hexpand(vbox, TRUE);
    
    GtkWidget *reinstall_button = gtk_button_new_with_label("Restore");
    gtk_widget_add_css_class(reinstall_button, "suggested-action");
    g_signal_connect(reinstall_button, "clicked", G_CALLBACK(on_reinstall_clicked), item);

    gtk_box_append(GTK_BOX(hbox), vbox);
    gtk_box_append(GTK_BOX(hbox), reinstall_button);
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), hbox);
    
    item->uninstalled_list_row = row; 
    gtk_list_box_append(GTK_LIST_BOX(state->uninstalled_list_box), row);
}

/**
 * @brief Called when the "Remove" button is clicked. Runs ADB uninstall commands.
 */
static void on_debloat_clicked(GtkButton *button, AppState *state) {
    (void)button;
    gint success_count = 0;
    gint failure_count = 0;
    GList *items_to_remove = NULL;
    gboolean error_in_adb_path = FALSE;

    gtk_widget_set_sensitive(state->debloat_button, FALSE);
    gtk_label_set_text(GTK_LABEL(state->status_label), "Executing ADB debloat process...");

    GList *current_l = state->app_list;
    while (current_l != NULL) {
        AppItem *item = (AppItem *)current_l->data;
        GList *next_l = current_l->next;

        if (item->is_selected) {
            gchar *stdout_str = NULL;
            gchar *stderr_str = NULL;
            gint exit_status;
            gboolean spawn_success;
            GError *error = NULL;
            gboolean uninstall_successful = FALSE;

            gchar *cmd_args[] = {
                "adb",
                "-s", state->selected_device_id,
                "shell", 
                "pm", 
                "uninstall", 
                "--user", 
                "0",
                item->package_name,
                NULL
            };

            spawn_success = g_spawn_sync(
                NULL, cmd_args, NULL, G_SPAWN_SEARCH_PATH,
                NULL, NULL, &stdout_str, &stderr_str, &exit_status, &error
            );

            if (error) {
                g_error_free(error);
                failure_count++;
                error_in_adb_path = TRUE;
            } else if (!spawn_success) {
                 failure_count++;
            } else {
                gchar *stripped_output = stdout_str ? g_strstrip(stdout_str) : NULL;
                
                if (stripped_output && g_str_has_prefix(stripped_output, "Success")) {
                    uninstall_successful = TRUE;
                }
                
                if (uninstall_successful) {
                    success_count++;
                    items_to_remove = g_list_append(items_to_remove, item);
                    gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), item->main_list_row);
                } else {
                    failure_count++;
                }
            }

            g_free(stdout_str);
            g_free(stderr_str);
            
            if (error_in_adb_path) break;
        }
        current_l = next_l;
    }

    for (GList *remove_l = items_to_remove; remove_l != NULL; remove_l = remove_l->next) {
        AppItem *item = (AppItem *)remove_l->data;
        state->app_list = g_list_remove(state->app_list, item);
        state->uninstalled_app_list = g_list_append(state->uninstalled_app_list, item);
        add_row_to_uninstalled_list(state, item);
        item->is_selected = FALSE;
    }
    g_list_free(items_to_remove);

    gchar *result_msg;
    if (error_in_adb_path) {
        result_msg = g_strdup("FATAL ERROR: Could not execute 'adb'. Check your PATH.");
    } else {
        result_msg = g_strdup_printf("Debloat complete: %d succeeded, %d failed.", success_count, failure_count);
    }

    gtk_label_set_text(GTK_LABEL(state->status_label), result_msg);
    g_free(result_msg);

    gtk_widget_set_sensitive(state->debloat_button, TRUE);
    update_action_bar_visibility(state);
}

/**
 * @brief Runs a custom command from the "Debug" page using safe shell argument parsing.
 */
static void on_run_debug_command_clicked(GtkButton *button, AppState *state) {
    (void)button;
    if (!state->selected_device_id) {
        gtk_text_buffer_set_text(state->debug_output_buffer, "Error: Please select a connected device from the 'Debloat' page first.", -1);
        return;
    }

    const gchar *command_str = gtk_editable_get_text(GTK_EDITABLE(state->debug_command_entry));
    if (!command_str || *command_str == '\0') {
        gtk_text_buffer_set_text(state->debug_output_buffer, "Error: No command entered.", -1);
        return;
    }

    gint user_cmd_len = 0;
    gchar **user_cmd_parts = NULL;
    GError *parse_error = NULL;

    if (!g_shell_parse_argv(command_str, &user_cmd_len, &user_cmd_parts, &parse_error)) {
        gchar *err_msg = g_strdup_printf("Error parsing command string: %s", parse_error->message);
        gtk_text_buffer_set_text(state->debug_output_buffer, err_msg, -1);
        g_free(err_msg);
        g_error_free(parse_error);
        return;
    }

    gchar **cmd_args = g_new(gchar*, 5 + user_cmd_len);
    cmd_args[0] = "adb";
    cmd_args[1] = "-s";
    cmd_args[2] = state->selected_device_id;
    cmd_args[3] = "shell";
    
    for (int i = 0; i < user_cmd_len; i++) {
        cmd_args[4 + i] = user_cmd_parts[i];
    }
    cmd_args[4 + user_cmd_len] = NULL;

    gtk_text_buffer_set_text(state->debug_output_buffer, "", -1);
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(state->debug_output_buffer, &iter);
    
    gchar *full_cmd_display = g_strjoinv(" ", cmd_args);
    gtk_text_buffer_insert(state->debug_output_buffer, &iter, "Running command:\n", -1);
    gtk_text_buffer_insert(state->debug_output_buffer, &iter, full_cmd_display, -1);
    gtk_text_buffer_insert(state->debug_output_buffer, &iter, "\n\n", -1);
    g_free(full_cmd_display);

    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status = 0;
    GError *error = NULL;

    g_spawn_sync(
        NULL, cmd_args, NULL, G_SPAWN_SEARCH_PATH,
        NULL, NULL, &stdout_str, &stderr_str, &exit_status, &error
    );

    if (error) {
        gchar *err_msg = g_strdup_printf("--- EXECUTION FAILED ---\n%s\n", error->message);
        gtk_text_buffer_insert(state->debug_output_buffer, &iter, err_msg, -1);
        g_free(err_msg);
        g_error_free(error);
    } else {
        if (stdout_str && *stdout_str != '\0') {
            gtk_text_buffer_insert(state->debug_output_buffer, &iter, "--- STDOUT ---\n", -1);
            gtk_text_buffer_insert(state->debug_output_buffer, &iter, stdout_str, -1);
            gtk_text_buffer_insert(state->debug_output_buffer, &iter, "\n", -1);
        }
        if (stderr_str && *stderr_str != '\0') {
            gtk_text_buffer_insert(state->debug_output_buffer, &iter, "--- STDERR ---\n", -1);
            gtk_text_buffer_insert(state->debug_output_buffer, &iter, stderr_str, -1);
            gtk_text_buffer_insert(state->debug_output_buffer, &iter, "\n", -1);
        }
        if ((!stdout_str || *stdout_str == '\0') && (!stderr_str || *stderr_str == '\0')) {
             gtk_text_buffer_insert(state->debug_output_buffer, &iter, "--- COMMAND RAN WITH NO OUTPUT ---\n", -1);
        }
        
        if (WIFEXITED(exit_status)) {
            gchar *exit_msg = g_strdup_printf("\n--- Exit Code: %d ---\n", WEXITSTATUS(exit_status));
            gtk_text_buffer_insert(state->debug_output_buffer, &iter, exit_msg, -1);
            g_free(exit_msg);
        } else if (WIFSIGNALED(exit_status)) {
            gchar *exit_msg = g_strdup_printf("\n--- Terminated by Signal: %d ---\n", WTERMSIG(exit_status));
            gtk_text_buffer_insert(state->debug_output_buffer, &iter, exit_msg, -1);
            g_free(exit_msg);
        }
    }

    g_free(stdout_str);
    g_free(stderr_str);
    g_strfreev(user_cmd_parts);
    g_free(cmd_args);
}


// --- LIST POPULATION FUNCTIONS ---

/**
 * @brief Populates the left-side manufacturer list with automatic memory cleanup handlers.
 */
static void update_device_list(AppState *state) {
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->device_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->device_list_box), GTK_WIDGET(row));
    }

    const gchar *mock_devices[] = {
        "Samsung (Galaxy Devices)", "Xiaomi (Mi/Redmi Devices)", "OnePlus (Oxygen OS)",
        "Oppo (ColorOS)", "RealMe (ColorOS/RealMe UI)", "Vivo (Funtouch OS)",
        "Sony (Xperia Devices)", "Motorola (Moto Devices)", "Nokia (HMD Devices)",
        "TCL (Alcatel/TCL Devices)", "ZTE (Nubia/Axon Devices)", "Jio (STB/Phone)",
        NULL
    };

    const gchar *mock_ids[] = {
        "Samsung", "Xiaomi", "OnePlus", "Oppo", "RealMe", "Vivo",
        "Sony", "Motorola", "Nokia", "TCL", "ZTE", "Jio",
        NULL
    };

    GtkListBoxRow *initial_row = NULL;

    for (int i = 0; mock_devices[i] != NULL; i++) {
        GtkWidget *row_widget = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_widget_set_margin_start(hbox, 10);
        gtk_widget_set_margin_end(hbox, 10);
        gtk_widget_set_margin_top(hbox, 8);
        gtk_widget_set_margin_bottom(hbox, 8);

        GtkWidget *icon = gtk_image_new_from_icon_name("phone-symbolic");
        gtk_image_set_pixel_size(GTK_IMAGE(icon), 20);
        gtk_box_append(GTK_BOX(hbox), icon);
        
        GtkWidget *label = gtk_label_new(mock_devices[i]);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_box_append(GTK_BOX(hbox), label);
        
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_widget), hbox);

        g_object_set_data_full(G_OBJECT(row_widget), "device-id", g_strdup(mock_ids[i]), g_free);

        gtk_list_box_append(GTK_LIST_BOX(state->device_list_box), row_widget);

        if (g_strcmp0(mock_ids[i], state->selected_manufacturer_id) == 0) {
            initial_row = GTK_LIST_BOX_ROW(row_widget);
        }
    }

    if (initial_row) {
        gtk_list_box_select_row(GTK_LIST_BOX(state->device_list_box), initial_row);
        on_device_selected(GTK_LIST_BOX(state->device_list_box), initial_row, state);
    }
}

/**
 * @brief Populates the right-side app list based on the selected manufacturer.
 */
static void update_app_list(AppState *state) {
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->app_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), GTK_WIDGET(row));
    }
    
    g_list_free_full(state->app_list, (GDestroyNotify)app_item_free);
    state->app_list = NULL;

    const PackageEntry *packages_to_load = NULL;
    const gchar *category_id = state->selected_manufacturer_id;

    if (category_id) {
        if (g_strcmp0(category_id, "Samsung") == 0) packages_to_load = SAMSUNG_APPS;
        else if (g_strcmp0(category_id, "Xiaomi") == 0) packages_to_load = XIAOMI_APPS;
        else if (g_strcmp0(category_id, "Vivo") == 0) packages_to_load = VIVO_APPS;
        else if (g_strcmp0(category_id, "TCL") == 0) packages_to_load = TCL_APPS;
        else if (g_strcmp0(category_id, "Sony") == 0) packages_to_load = SONY_APPS;
        else if (g_strcmp0(category_id, "RealMe") == 0) packages_to_load = REALME_APPS;
        else if (g_strcmp0(category_id, "Oppo") == 0) packages_to_load = OPPO_APPS;
        else if (g_strcmp0(category_id, "OnePlus") == 0) packages_to_load = ONEPLUS_APPS;
        else if (g_strcmp0(category_id, "Nokia") == 0) packages_to_load = NOKIA_APPS;
        else if (g_strcmp0(category_id, "Motorola") == 0) packages_to_load = MOTOROLA_APPS;
        else if (g_strcmp0(category_id, "ZTE") == 0) packages_to_load = ZTE_APPS;
        else if (g_strcmp0(category_id, "Jio") == 0) packages_to_load = JIO_APPS;
    }
    
    if (!packages_to_load) {
        gtk_label_set_text(GTK_LABEL(state->status_label), "Select a device category to view available bloatware.");
        return;
    }

    for (int i = 0; packages_to_load[i].display_name != NULL; i++) {
        AppItem *item = g_new0(AppItem, 1);
        item->display_name = g_strdup(packages_to_load[i].display_name);
        item->package_name = g_strdup(packages_to_load[i].package_name);
        item->is_selected = FALSE;

        GtkWidget *row_widget = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12); 
        gtk_widget_set_margin_start(hbox, 12);
        gtk_widget_set_margin_end(hbox, 12);
        gtk_widget_set_margin_top(hbox, 6);
        gtk_widget_set_margin_bottom(hbox, 6);

        GtkWidget *check = gtk_check_button_new();
        item->check_button = check;
        gtk_box_append(GTK_BOX(hbox), check);

        GtkWidget *icon = gtk_image_new_from_icon_name("package-x-generic-symbolic"); 
        gtk_image_set_pixel_size(GTK_IMAGE(icon), 18); 
        gtk_box_append(GTK_BOX(hbox), icon);
        
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
        GtkWidget *display_label = gtk_label_new(item->display_name);
        GtkWidget *package_label = gtk_label_new(item->package_name);

        gtk_label_set_xalign(GTK_LABEL(display_label), 0.0);
        gtk_label_set_xalign(GTK_LABEL(package_label), 0.0);
        gtk_widget_add_css_class(package_label, "caption");
        gtk_widget_set_opacity(package_label, 0.7); 

        gtk_box_append(GTK_BOX(vbox), display_label);
        gtk_box_append(GTK_BOX(vbox), package_label);
        gtk_box_append(GTK_BOX(hbox), vbox);

        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_widget), hbox);
        gtk_list_box_append(GTK_LIST_BOX(state->app_list_box), row_widget);

        item->main_list_row = row_widget;
        g_signal_connect(check, "toggled", G_CALLBACK(on_app_toggled), item);

        state->app_list = g_list_append(state->app_list, item);
    }

    if (state->search_entry) {
        on_search_changed(GTK_SEARCH_ENTRY(state->search_entry), state);
    }

    update_action_bar_visibility(state);
}


// --- UI SETUP ---

/**
 * @brief Creates all UI widgets and connects signals.
 */
static void activate(GtkApplication *app, AppState *state) {
    state->selected_device_id = NULL;
    state->selected_manufacturer_id = g_strdup("Samsung");
    state->app_list = NULL;
    state->uninstalled_app_list = NULL; 

    // --- Window ---
    state->window = ADW_APPLICATION_WINDOW(adw_application_window_new(GTK_APPLICATION(app)));
    gtk_window_set_title(GTK_WINDOW(state->window), "Android Debloater");
    gtk_window_set_default_size(GTK_WINDOW(state->window), 960, 640);

    GtkWidget *main_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    adw_application_window_set_content(state->window, main_content_box);

    // --- Header Bar ---
    GtkWidget *header_bar = adw_header_bar_new();
    gtk_box_append(GTK_BOX(main_content_box), header_bar);

    GtkWidget *stack_switcher = gtk_stack_switcher_new();
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(header_bar), stack_switcher);

    GtkWidget *about_button = gtk_button_new_from_icon_name("help-about-symbolic");
    gtk_widget_set_tooltip_text(about_button, "About Android Debloater");
    adw_header_bar_pack_end(ADW_HEADER_BAR(header_bar), about_button);
    g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_clicked), GTK_WINDOW(state->window));

    // --- Main Stack (Pages) ---
    GtkWidget *stack = gtk_stack_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stack_switcher), GTK_STACK(stack));
    gtk_box_append(GTK_BOX(main_content_box), stack);


    // --- PAGE 1: DEBLOAT ---
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_stack_add_titled(GTK_STACK(stack), paned, "debloat", "Debloat");

    // --- Left Pane (Device Selection) ---
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_start(left_box, 12);
    gtk_widget_set_margin_end(left_box, 12);
    gtk_widget_set_margin_top(left_box, 12);
    gtk_widget_set_margin_bottom(left_box, 12);
    gtk_paned_set_start_child(GTK_PANED(paned), left_box);
    gtk_paned_set_resize_start_child(GTK_PANED(paned), FALSE); 
    gtk_paned_set_shrink_start_child(GTK_PANED(paned), FALSE);

    GtkWidget *adb_header = gtk_label_new("Connected Device");
    gtk_widget_add_css_class(adb_header, "heading");
    gtk_label_set_xalign(GTK_LABEL(adb_header), 0.0);
    gtk_box_append(GTK_BOX(left_box), adb_header);

    GtkWidget *adb_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(left_box), adb_hbox);

    state->device_model = gtk_string_list_new(NULL);
    gtk_string_list_append(state->device_model, "Select connected device...");
    state->device_dropdown = gtk_drop_down_new(G_LIST_MODEL(state->device_model), NULL);
    gtk_widget_set_hexpand(state->device_dropdown, TRUE);
    g_signal_connect(state->device_dropdown, "notify::selected", G_CALLBACK(on_adb_device_selected), state);
    gtk_box_append(GTK_BOX(adb_hbox), state->device_dropdown);

    GtkWidget *refresh_button = gtk_button_new_from_icon_name("view-refresh-symbolic");
    gtk_widget_set_tooltip_text(refresh_button, "Refresh ADB Device List");
    g_signal_connect(refresh_button, "clicked", G_CALLBACK(on_refresh_devices_clicked), state);
    gtk_box_append(GTK_BOX(adb_hbox), refresh_button);

    state->device_status_label = gtk_label_new("Welcome! Connect a device.");
    gtk_widget_set_margin_top(state->device_status_label, 4);
    gtk_label_set_xalign(GTK_LABEL(state->device_status_label), 0.0);
    gtk_label_set_wrap(GTK_LABEL(state->device_status_label), TRUE);
    gtk_widget_add_css_class(state->device_status_label, "caption");
    gtk_box_append(GTK_BOX(left_box), state->device_status_label);

    GtkWidget *device_header = gtk_label_new("Manufacturers");
    gtk_widget_add_css_class(device_header, "heading");
    gtk_label_set_xalign(GTK_LABEL(device_header), 0.0);
    gtk_widget_set_margin_top(device_header, 12);
    gtk_box_append(GTK_BOX(left_box), device_header);

    state->device_list_box = gtk_list_box_new();
    gtk_widget_add_css_class(state->device_list_box, "boxed-list");
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->device_list_box), GTK_SELECTION_SINGLE);
    g_signal_connect(state->device_list_box, "row-activated", G_CALLBACK(on_device_selected), state);

    GtkWidget *device_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(device_scrolled), state->device_list_box);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(device_scrolled), 220);
    gtk_widget_set_vexpand(device_scrolled, TRUE);
    gtk_box_append(GTK_BOX(left_box), device_scrolled);

    // --- Right Pane (App List, Search & Action Bar) ---
    GtkWidget *right_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_paned_set_end_child(GTK_PANED(paned), right_container);

    GtkWidget *right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_start(right_vbox, 12);
    gtk_widget_set_margin_end(right_vbox, 12);
    gtk_widget_set_margin_top(right_vbox, 12);
    gtk_widget_set_vexpand(right_vbox, TRUE);
    gtk_box_append(GTK_BOX(right_container), right_vbox);

    GtkWidget *controls_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_append(GTK_BOX(right_vbox), controls_hbox);

    state->search_entry = gtk_search_entry_new();
    gtk_search_entry_set_key_capture_widget(GTK_SEARCH_ENTRY(state->search_entry), GTK_WIDGET(state->window));
    gtk_widget_set_hexpand(state->search_entry, TRUE);
    g_signal_connect(state->search_entry, "search-changed", G_CALLBACK(on_search_changed), state);
    gtk_box_append(GTK_BOX(controls_hbox), state->search_entry);

    GtkWidget *select_all_btn = gtk_button_new_with_label("Select All");
    g_signal_connect(select_all_btn, "clicked", G_CALLBACK(on_select_all_clicked), state);
    gtk_box_append(GTK_BOX(controls_hbox), select_all_btn);

    GtkWidget *deselect_all_btn = gtk_button_new_with_label("Clear");
    g_signal_connect(deselect_all_btn, "clicked", G_CALLBACK(on_deselect_all_clicked), state);
    gtk_box_append(GTK_BOX(controls_hbox), deselect_all_btn);
    
    state->app_list_box = gtk_list_box_new();
    gtk_widget_add_css_class(state->app_list_box, "boxed-list");
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->app_list_box), GTK_SELECTION_NONE);

    GtkWidget *app_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(app_scrolled), state->app_list_box);
    gtk_widget_set_vexpand(app_scrolled, TRUE);
    gtk_box_append(GTK_BOX(right_vbox), app_scrolled);

    // Bottom Action Bar
    state->action_bar = gtk_action_bar_new();
    gtk_widget_set_visible(state->action_bar, FALSE);
    gtk_box_append(GTK_BOX(right_container), state->action_bar);

    state->status_label = gtk_label_new("Select a device and then select apps to debloat.");
    gtk_action_bar_pack_start(GTK_ACTION_BAR(state->action_bar), state->status_label);

    state->debloat_button = gtk_button_new_with_label("Remove Selected");
    gtk_widget_set_tooltip_text(state->debloat_button, "Uninstall selected apps from connected device");
    gtk_widget_add_css_class(state->debloat_button, "destructive-action");
    gtk_action_bar_pack_end(GTK_ACTION_BAR(state->action_bar), state->debloat_button);
    g_signal_connect(state->debloat_button, "clicked", G_CALLBACK(on_debloat_clicked), state);
    
    gtk_paned_set_position(GTK_PANED(paned), 300); 


    // --- PAGE 2: UNDO ---
    GtkWidget *undo_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(undo_page_box, 16);
    gtk_widget_set_margin_end(undo_page_box, 16);
    gtk_widget_set_margin_top(undo_page_box, 16);
    gtk_widget_set_margin_bottom(undo_page_box, 16);
    gtk_stack_add_titled(GTK_STACK(stack), undo_page_box, "undo", "Undo");

    GtkWidget *uninstalled_header = gtk_label_new("Recently Uninstalled (Restore)");
    gtk_widget_add_css_class(uninstalled_header, "heading");
    gtk_label_set_xalign(GTK_LABEL(uninstalled_header), 0.0);
    gtk_box_append(GTK_BOX(undo_page_box), uninstalled_header);

    state->uninstalled_list_box = gtk_list_box_new();
    gtk_widget_add_css_class(state->uninstalled_list_box, "boxed-list");
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->uninstalled_list_box), GTK_SELECTION_NONE);

    GtkWidget *uninstalled_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(uninstalled_scrolled), state->uninstalled_list_box);
    gtk_widget_set_vexpand(uninstalled_scrolled, TRUE);
    gtk_box_append(GTK_BOX(undo_page_box), uninstalled_scrolled);


    // --- PAGE 3: DEBUG ---
    GtkWidget *debug_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_start(debug_page_box, 16);
    gtk_widget_set_margin_end(debug_page_box, 16);
    gtk_widget_set_margin_top(debug_page_box, 16);
    gtk_widget_set_margin_bottom(debug_page_box, 16);
    gtk_stack_add_titled(GTK_STACK(stack), debug_page_box, "debug", "Debug");

    GtkWidget *debug_header = gtk_label_new("Run Custom ADB Shell Command");
    gtk_widget_add_css_class(debug_header, "heading");
    gtk_label_set_xalign(GTK_LABEL(debug_header), 0.0);
    gtk_box_append(GTK_BOX(debug_page_box), debug_header);

    GtkWidget *debug_info_label = gtk_label_new(
        "Enter a shell command (e.g. 'pm list packages -e'). Prefix 'adb -s <serial> shell' is handled automatically."
    );
    gtk_label_set_xalign(GTK_LABEL(debug_info_label), 0.0);
    gtk_widget_add_css_class(debug_info_label, "caption");
    gtk_box_append(GTK_BOX(debug_page_box), debug_info_label);

    GtkWidget *debug_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_append(GTK_BOX(debug_page_box), debug_hbox);
    
    state->debug_command_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(state->debug_command_entry), "pm list packages -f");
    gtk_widget_set_hexpand(state->debug_command_entry, TRUE);
    g_signal_connect(state->debug_command_entry, "activate", G_CALLBACK(on_run_debug_command_clicked), state);
    gtk_box_append(GTK_BOX(debug_hbox), state->debug_command_entry);

    GtkWidget *debug_run_button = gtk_button_new_with_label("Execute");
    gtk_widget_add_css_class(debug_run_button, "suggested-action");
    g_signal_connect(debug_run_button, "clicked", G_CALLBACK(on_run_debug_command_clicked), state);
    gtk_box_append(GTK_BOX(debug_hbox), debug_run_button);

    GtkWidget *output_header = gtk_label_new("Command Output");
    gtk_widget_add_css_class(output_header, "heading");
    gtk_label_set_xalign(GTK_LABEL(output_header), 0.0);
    gtk_widget_set_margin_top(output_header, 8);
    gtk_box_append(GTK_BOX(debug_page_box), output_header);

    GtkWidget *debug_scrolled = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(debug_scrolled, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(debug_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_append(GTK_BOX(debug_page_box), debug_scrolled);

    GtkWidget *debug_output_view = gtk_text_view_new();
    gtk_widget_add_css_class(debug_output_view, "monospace");
    gtk_text_view_set_editable(GTK_TEXT_VIEW(debug_output_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(debug_output_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(debug_output_view), GTK_WRAP_WORD_CHAR);
    state->debug_output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(debug_output_view));
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(debug_scrolled), debug_output_view);
    
    // --- FINAL SETUP ---
    update_device_list(state);
    populate_adb_devices(state);

    gtk_window_present(GTK_WINDOW(state->window));
    show_welcome_dialog(GTK_WINDOW(state->window));
}

/**
 * @brief Frees all global state resources on application shutdown.
 */
static void on_shutdown(GtkApplication *app, gpointer user_data) {
    (void)app;
    (void)user_data;
    if (!app_state) return;

    if (app_state->selected_device_id) {
        g_free(app_state->selected_device_id);
    }
    if (app_state->selected_manufacturer_id) {
        g_free(app_state->selected_manufacturer_id);
    }
    if (app_state->device_model) {
        g_object_unref(app_state->device_model);
    }
    if (app_state->uninstalled_app_list) {
        g_list_free_full(app_state->uninstalled_app_list, (GDestroyNotify)app_item_free);
    }
    if (app_state->app_list) {
        g_list_free_full(app_state->app_list, (GDestroyNotify)app_item_free);
    }
    g_free(app_state);
    app_state = NULL;
}

// --- MAIN FUNCTION ---

int main(int argc, char **argv) {
    AdwApplication *app;
    int status;

    app_state = g_new0(AppState, 1);
    app = adw_application_new("com.gemini.debloater", G_APPLICATION_DEFAULT_FLAGS);
    
    g_signal_connect(app, "activate", G_CALLBACK(activate), app_state);
    g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}


