#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/glist.h>
#include <gio/gio.h>
#include <sys/wait.h> // Required for WEXITSTATUS

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

    // ADB Device selection
    GtkWidget *device_dropdown;     // Dropdown to select a connected device
    GtkStringList *device_model;    // Data model for the device_dropdown
    GtkWidget *device_status_label; // Label under the dropdown for feedback

    // State variables
    gchar *selected_device_id;        // The *real* serial of the selected device (e.g., "ABC12345")
    gchar *selected_manufacturer_id;  // The *category* selected (e.g., "Samsung")
    GList *app_list;                  // GList of AppItem* for the "Debloat" page
    
    // "Undo" page state
    GList *uninstalled_app_list; // GList of AppItem* for the "Undo" page
    GtkWidget *uninstalled_list_box; // UI list for the "Undo" page

    // "Debug" page widgets
    GtkWidget *debug_command_entry; // Text entry for custom commands
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


// --- PACKAGE DATA (Bloatware lists) ---
// ... (Lists are unchanged, snipped for brevity) ...

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
 * This is used as a GDestroyNotify function for g_list_free_full.
 */
static void app_item_free(AppItem *item) {
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

    // Count how many apps are currently checked
    for (l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (item->is_selected) {
            selected_count++;
        }
    }

    // Logic to show/hide the bar and update its contents
    if (selected_count > 0 && state->selected_device_id != NULL) {
        // Condition: Device selected AND apps checked
        // Action: Show bar, enable button, show "Ready" message
        gtk_widget_set_visible(state->action_bar, TRUE);
        gchar *label_text = g_strdup_printf("Ready to debloat %d app(s) on device '%s'.", selected_count, state->selected_device_id);
        gtk_label_set_text(GTK_LABEL(state->status_label), label_text);
        g_free(label_text);
        gtk_widget_set_sensitive(state->debloat_button, TRUE);
    } 
    else if (state->selected_device_id == NULL) {
        // Condition: No device selected
        // Action: Hide the bar. The device_status_label will show instructions.
        gtk_widget_set_visible(state->action_bar, FALSE);
    }
    else {
        // Condition: Device selected, but NO apps checked
        // Action: Show bar, disable button, show "Select apps" message
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
    // AdwAlertDialog is the modern dialog to use
    AdwDialog *dialog = adw_alert_dialog_new(
        "Welcome to Android Debloater!",
        NULL // No subtitle
    );

    // Add a single "Got it!" response button
    adw_alert_dialog_add_response(ADW_ALERT_DIALOG(dialog), "ok", "Got it!");
    adw_alert_dialog_set_default_response(ADW_ALERT_DIALOG(dialog), "ok");

    // Create a vertical box to hold the instruction text
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 12);
    gtk_widget_set_margin_bottom(box, 12);

    const gchar *instructions = 
        "To use this tool, you must enable **USB Debugging (ADB)** on your device.\n\n"
        "**Wired Connection:**\n"
        "1.  Go to **Settings** > **About Phone**.\n"
        "2.  Tap on **Build Number** 7 times until you see 'You are now a developer!'.\n"
        "3.  Go back to **Settings** > **System** > **Developer options**.\n"
        "4.  Find and turn on **USB debugging**.\n"
        "5.  Connect your phone to your computer via USB.\n"
        "6.  A prompt will appear on your phone: 'Allow USB debugging?'. Check 'Always allow' and tap **OK**.";

    // Create a label that understands Pango markup (for **bold**)
    GtkWidget *label = gtk_label_new(instructions);
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0); // Left-align
    gtk_box_append(GTK_BOX(box), label);

    // Add the instruction box to the dialog's "extra child" area
    adw_alert_dialog_set_extra_child(ADW_ALERT_DIALOG(dialog), box);

    // Show the dialog.
    // We cast `parent` to GtkWidget* as required by the `choose` function.
    adw_alert_dialog_choose(ADW_ALERT_DIALOG(dialog), GTK_WIDGET(parent), NULL, NULL, NULL);
}


/**
 * @brief Shows the "About" dialog.
 */
static void on_about_clicked(GtkButton *button, GtkWindow *parent) {
    // Create a new Libadwaita About Dialog
    AdwDialog *dialog = adw_about_dialog_new();
    
    // Set all the properties for the dialog
    adw_about_dialog_set_application_name(ADW_ABOUT_DIALOG(dialog), "Android Debloater");
    adw_about_dialog_set_version(ADW_ABOUT_DIALOG(dialog), "0.7 (Libadwaita)");
    adw_about_dialog_set_developer_name(ADW_ABOUT_DIALOG(dialog), "Kushagra Karira");
    adw_about_dialog_set_copyright(ADW_ABOUT_DIALOG(dialog), "© 2025 Kushagra Karira");
    adw_about_dialog_set_comments(ADW_ABOUT_DIALOG(dialog), "A GTK4/Libadwaita application for debloating Android devices using ADB.");
    adw_about_dialog_set_website(ADW_ABOUT_DIALOG(dialog), "https://kushagrakarira.com");
    adw_about_dialog_set_issue_url(ADW_ABOUT_DIALOG(dialog), "https://github.com/KushagraKarira/Debloat/issues");
    adw_about_dialog_add_link(ADW_ABOUT_DIALOG(dialog), "Project Link", "https.github.com/KushagraKarira/Debloat");
    
    // Present the dialog modally over the parent window
    adw_dialog_present(dialog, GTK_WIDGET(parent));
}

/**
 * @brief Called when an app's checkbox is toggled.
 */
static void on_app_toggled(GtkCheckButton *check_button, AppItem *item) {
    // Update the state of the app item
    item->is_selected = gtk_check_button_get_active(check_button);
    // Refresh the action bar (which might show/hide it)
    update_action_bar_visibility(app_state);
}

/**
 * @brief Called when a manufacturer (device category) is selected from the left list.
 */
static void on_device_selected(GtkListBox *list_box, GtkListBoxRow *row, AppState *state) {
    if (!row) return;

    // Get the unique ID (e.g., "Samsung") stored in the row
    const gchar *manufacturer_id = g_object_get_data(G_OBJECT(row), "device-id");
    if (!manufacturer_id) return;

    // Update the global state
    if (state->selected_manufacturer_id) g_free(state->selected_manufacturer_id);
    state->selected_manufacturer_id = g_strdup(manufacturer_id);
    
    g_print("Manufacturer list selected: %s\n", state->selected_manufacturer_id);
    
    // Hide the action bar until apps are loaded and selected
    gtk_widget_set_visible(state->action_bar, FALSE);

    // Reload the app list on the right side
    update_app_list(state);
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

    g_print("Running: adb devices\n");
    gtk_label_set_text(GTK_LABEL(state->device_status_label), "Refreshing device list...");

    // Clear all items *except* the placeholder at index 0
    guint n_items = g_list_model_get_n_items(G_LIST_MODEL(state->device_model));
    if (n_items > 1) {
        // Remove n_items - 1, starting from position 1
        gtk_string_list_splice(state->device_model, 1, n_items - 1, NULL);
    }
    
    // Reset dropdown to the placeholder
    gtk_drop_down_set_selected(GTK_DROP_DOWN(state->device_dropdown), 0);
    if (state->selected_device_id) {
        g_free(state->selected_device_id);
        state->selected_device_id = NULL;
    }

    // Run the 'adb devices' command synchronously
    g_spawn_sync(
        NULL,        // working directory
        cmd_args,    // command and arguments
        NULL,        // environment
        G_SPAWN_SEARCH_PATH, // flags (find 'adb' in PATH)
        NULL, NULL,  // child setup, user data
        &stdout_str, // standard output
        &stderr_str, // standard error
        &exit_status, // exit status
        &error       // GError
    );

    // Handle command execution errors (e.g., 'adb' not found)
    if (error) {
        g_printerr("Failed to execute 'adb': %s. Is it in your PATH?\n", error->message);
        gtk_label_set_text(GTK_LABEL(state->device_status_label), "Error: 'adb' command not found.");
        g_error_free(error);
        g_free(stdout_str);
        g_free(stderr_str);
        return;
    }

    // Parse the command's output
    if (stdout_str) {
        g_print("ADB Output:\n%s\n", stdout_str);
        
        gchar **lines = g_strsplit(stdout_str, "\n", -1);
        
        // Iterate over each line of the output
        for (int i = 0; lines[i] != NULL; i++) {
            // Skip the first line ("List of devices attached")
            if (i == 0) continue;

            // Split the line by tabs (e.g., "ABC12345\tdevice")
            gchar **parts = g_strsplit(lines[i], "\t", 2);
            
            // A valid line has two parts
            if (g_strv_length(parts) == 2) {
                gchar *device_id = g_strstrip(parts[0]);
                gchar *device_state = g_strstrip(parts[1]);

                // We only care about fully authorized devices
                if (g_strcmp0(device_state, "device") == 0) {
                    g_print("Found device: %s\n", device_id);
                    gtk_string_list_append(state->device_model, device_id);
                    devices_found++;
                }
            }
            g_strfreev(parts);
        }
        g_strfreev(lines);
    }
    
    // Update the status label with the result
    gchar *status_msg;
    if (devices_found > 0) {
        status_msg = g_strdup_printf("Found %d device(s). Select one to begin.", devices_found);
    } else {
        status_msg = g_strdup("No devices found. Connect one and refresh.");
    }
    gtk_label_set_text(GTK_LABEL(state->device_status_label), status_msg);
    g_free(status_msg);

    g_free(stdout_str);
    g_free(stderr_str);
    
    // Refresh the action bar (it will be hidden)
    update_action_bar_visibility(state);
}

/**
 * @brief Called when a *real* device is selected from the dropdown.
 */
static void on_adb_device_selected(GtkDropDown *dropdown, GParamSpec *pspec, AppState *state) {
    guint pos = gtk_drop_down_get_selected(dropdown);

    // Free the previously selected device ID, if any
    if (state->selected_device_id) {
        g_free(state->selected_device_id);
        state->selected_device_id = NULL;
    }

    // Position 0 is the "Select a connected device..." placeholder
    if (pos > 0) {
        // Get the device serial from the list model and store it
        const gchar *device_id = gtk_string_list_get_string(state->device_model, pos);
        state->selected_device_id = g_strdup(device_id);
        g_print("ADB device selected: %s\n", state->selected_device_id);
        gtk_label_set_text(GTK_LABEL(state->device_status_label), ""); // Clear status
    } else {
        g_print("No ADB device selected.\n");
    }
    
    // Refresh the action bar state
    update_action_bar_visibility(state);
}


/**
 * @brief Called when the "Refresh" button next to the device dropdown is clicked.
 */
static void on_refresh_devices_clicked(GtkButton *button, AppState *state) {
    populate_adb_devices(state);
}


/**
 * @brief Called when an "Undo" (reinstall) button is clicked.
 */
static void on_reinstall_clicked(GtkButton *button, AppItem *item) {
    // Check if a device is selected (must be the global state)
    if (!app_state || !app_state->selected_device_id) {
        g_printerr("Cannot reinstall: No ADB device selected.\n");
        // Note: status_label is on a different page, this message may not be seen.
        gtk_label_set_text(GTK_LABEL(app_state->status_label), "Error: Select ADB device to reinstall.");
        return;
    }

    g_print("Attempting to reinstall: %s\n", item->package_name);
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE); // Disable button during op

    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status;
    GError *error = NULL;

    // Command: "adb -s <device> shell cmd package install-existing <package>"
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

    // Run the reinstall command
    g_spawn_sync(
        NULL, cmd_args, NULL, G_SPAWN_SEARCH_PATH,
        NULL, NULL, &stdout_str, &stderr_str, &exit_status, &error
    );

    gchar *stripped_output = stdout_str ? g_strstrip(stdout_str) : NULL;

    if (error) {
        g_printerr("Reinstall failed (spawn error): %s\n", error->message);
        g_error_free(error);
    } else if (stripped_output && g_str_has_prefix(stripped_output, "Package")) {
        // Success output is usually "Package com.example.app installed..."
        g_print("Reinstall successful.\n");
        
        // Remove the row from the "Undo" list UI
        gtk_list_box_remove(GTK_LIST_BOX(app_state->uninstalled_list_box), item->uninstalled_list_row);
        
        // Remove the item from the "Undo" data list
        app_state->uninstalled_app_list = g_list_remove(app_state->uninstalled_app_list, item);
        
        // The item is no longer tracked anywhere, so free it
        app_item_free(item);
        
        // We don't need to re-enable the button, it is being destroyed with the row.
    } else {
        // ADB command failed
        g_printerr("Reinstall failed. STDOUT: %s, STDERR: %s\n", 
            stdout_str ? stdout_str : "None", 
            stderr_str ? stderr_str : "None");
        gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE); // Re-enable button on failure
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
    
    GtkWidget *label = gtk_label_new(item->display_name);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    gtk_widget_set_hexpand(label, TRUE);
    
    GtkWidget *reinstall_button = gtk_button_new_with_label("Undo");
    gtk_widget_add_css_class(reinstall_button, "suggested-action");
    g_signal_connect(reinstall_button, "clicked", G_CALLBACK(on_reinstall_clicked), item);

    gtk_box_append(GTK_BOX(hbox), label);
    gtk_box_append(GTK_BOX(hbox), reinstall_button);
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), hbox);
    
    // Store a reference to the new row in the item
    item->uninstalled_list_row = row; 

    gtk_list_box_append(GTK_LIST_BOX(state->uninstalled_list_box), row);
}


/**
 * @brief Called when the "Remove" button is clicked. Runs ADB uninstall commands.
 */
static void on_debloat_clicked(GtkButton *button, AppState *state) {
    gint count = 0;
    gint success_count = 0;
    gint failure_count = 0;
    GList *items_to_remove = NULL;
    gboolean error_in_adb_path = FALSE; // Flag for catastrophic failure

    // Disable button and update status
    gtk_widget_set_sensitive(state->debloat_button, FALSE);
    gtk_label_set_text(GTK_LABEL(state->status_label), "Starting real ADB debloat process...");

    // Iterate through the app list
    GList *current_l = state->app_list;
    while (current_l != NULL) {
        AppItem *item = (AppItem *)current_l->data;
        GList *next_l = current_l->next; // Store next item in case current is removed

        if (item->is_selected) {
            count++;

            gchar *stdout_str = NULL;
            gchar *stderr_str = NULL;
            gint exit_status;
            gboolean spawn_success;
            GError *error = NULL;
            gboolean uninstall_successful = FALSE;

            // Construct the command:
            // "adb -s <device> shell pm uninstall --user 0 <package>"
            gchar *cmd_args[] = {
                "adb",
                "-s", state->selected_device_id, // Use the selected device
                "shell", 
                "pm", 
                "uninstall", 
                "--user", 
                "0", // For the current user
                item->package_name,
                NULL
            };

            g_print("\n--- Running Uninstall Command ---\n");
            g_print("Executing: adb -s %s shell pm uninstall --user 0 %s\n", 
                        state->selected_device_id, item->package_name);
            
            // Execute the uninstall command
            spawn_success = g_spawn_sync(
                NULL, cmd_args, NULL, G_SPAWN_SEARCH_PATH,
                NULL, NULL, &stdout_str, &stderr_str, &exit_status, &error
            );

            // --- Check results ---
            if (error) {
                // Failure to *execute* 'adb' (e.g., not in PATH)
                g_printerr("ADB Execution Error for package %s: %s\n", item->package_name, error->message);
                g_error_free(error);
                failure_count++;
                error_in_adb_path = TRUE; // Mark as catastrophic
            } else if (!spawn_success) {
                 // Should be covered by `error`, but good to check
                 g_printerr("ADB Command failed to start for package %s.\n", item->package_name);
                 failure_count++;
            } else {
                // Command *ran*, now check ADB's output
                gchar *stripped_output = stdout_str ? g_strstrip(stdout_str) : NULL;
                
                if (stripped_output && g_str_has_prefix(stripped_output, "Success")) {
                    // ADB reported success
                    uninstall_successful = TRUE;
                    g_print("ADB Output: Success\n");
                } else {
                    // ADB reported failure (e.g., "Failure [not installed for user 0]")
                    g_print("ADB Output: Failure (Stdout: %s, Stderr: %s)\n", 
                        stripped_output ? stripped_output : "None",
                        stderr_str ? stderr_str : "None"
                    );
                }
                
                if (uninstall_successful) {
                    success_count++;
                    // Add to a temporary list for removal from the UI
                    items_to_remove = g_list_append(items_to_remove, item);
                    // Remove from the "Debloat" list UI immediately
                    gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), item->main_list_row);
                } else {
                    failure_count++;
                }
            }

            g_free(stdout_str);
            g_free(stderr_str);
            g_print("---------------------------------\n");
            
            // If 'adb' itself failed, don't try any more packages
            if (error_in_adb_path) break;
        }
        current_l = next_l; // Move to the next item
    }

    // --- Post-loop cleanup ---
    // Move all successfully removed items to the "Undo" list
    GList *remove_l;
    for (remove_l = items_to_remove; remove_l != NULL; remove_l = remove_l->next) {
        AppItem *item = (AppItem *)remove_l->data;
        
        // 1. Remove from "Debloat" data list
        state->app_list = g_list_remove(state->app_list, item);
        
        // 2. Add to "Undo" data list
        state->uninstalled_app_list = g_list_append(state->uninstalled_app_list, item);
        
        // 3. Add to "Undo" UI list
        add_row_to_uninstalled_list(state, item);
        
        // 4. Reset selection state (no longer selected)
        item->is_selected = FALSE;
    }
    g_list_free(items_to_remove); // Free the temporary list


    // --- Final Status Update ---
    gchar *result_msg;
    if (error_in_adb_path) {
        result_msg = g_strdup_printf(
            "FATAL ERROR: Could not execute 'adb'. Check your PATH."
        );
    } else {
        result_msg = g_strdup_printf(
            "Debloat complete: Success: %d, Failures: %d.", 
            success_count, 
            failure_count
        );
    }

    gtk_label_set_text(GTK_LABEL(state->status_label), result_msg);
    g_free(result_msg);

    // Re-enable the button and update the action bar
    gtk_widget_set_sensitive(state->debloat_button, TRUE);
    update_action_bar_visibility(state);
}


/**
 * @brief Runs a custom command from the "Debug" page.
 */
static void on_run_debug_command_clicked(GtkButton *button, AppState *state) {
    // 1. Check if a device is selected
    if (!state->selected_device_id) {
        gtk_text_buffer_set_text(state->debug_output_buffer, "Error: Please select a connected device from the 'Debloat' page first.", -1);
        return;
    }

    // 2. Get the command from the entry
    const gchar *command_str = gtk_editable_get_text(GTK_EDITABLE(state->debug_command_entry));
    if (!command_str || *command_str == '\0') {
        gtk_text_buffer_set_text(state->debug_output_buffer, "Error: No command entered.", -1);
        return;
    }

    // 3. Construct the full 'argv' array for g_spawn_sync
    // We need to run: "adb" "-s" "<device>" "shell" <user_command_parts...>
    
    // Split the user's command string by spaces
    gchar **user_cmd_parts = g_strsplit(command_str, " ", -1);
    gint user_cmd_len = g_strv_length(user_cmd_parts);
    
    // Create a new array large enough for:
    // "adb", "-s", device_id, "shell", (user parts), NULL
    gchar **cmd_args = g_new(gchar*, 5 + user_cmd_len);
    cmd_args[0] = "adb";
    cmd_args[1] = "-s";
    cmd_args[2] = state->selected_device_id;
    cmd_args[3] = "shell";
    
    // Copy the user's command parts into the main array
    for (int i = 0; i < user_cmd_len; i++) {
        cmd_args[4 + i] = user_cmd_parts[i];
    }
    cmd_args[4 + user_cmd_len] = NULL; // Must be NULL-terminated

    // 4. Clear the output buffer and show "Running..."
    gtk_text_buffer_set_text(state->debug_output_buffer, "", -1);
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(state->debug_output_buffer, &iter);
    
    gchar *full_cmd_display = g_strjoinv(" ", cmd_args);
    gtk_text_buffer_insert(state->debug_output_buffer, &iter, "Running command:\n", -1);
    gtk_text_buffer_insert(state->debug_output_buffer, &iter, full_cmd_display, -1);
    gtk_text_buffer_insert(state->debug_output_buffer, &iter, "\n\n", -1);
    g_free(full_cmd_display);

    // 5. Execute the command
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status;
    GError *error = NULL;

    g_spawn_sync(
        NULL, cmd_args, NULL, G_SPAWN_SEARCH_PATH,
        NULL, NULL, &stdout_str, &stderr_str, &exit_status, &error
    );

    // 6. Print all results (stdout, stderr, exit code) to the text buffer
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
        
        gchar *exit_msg = g_strdup_printf("\n--- Exit Status: %d ---\n", WEXITSTATUS(exit_status));
        gtk_text_buffer_insert(state->debug_output_buffer, &iter, exit_msg, -1);
        g_free(exit_msg);
    }

    // 7. Clean up memory
    g_free(stdout_str);
    g_free(stderr_str);
    g_strfreev(user_cmd_parts);
    g_free(cmd_args);
}


// --- LIST POPULATION FUNCTIONS ---

/**
 * @brief Populates the left-side manufacturer (category) list.
 */
static void update_device_list(AppState *state) {
    // Clear any existing rows
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->device_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->device_list_box), GTK_WIDGET(row));
    }

    // Device category names
    const gchar *mock_devices[] = {
        "Samsung (Galaxy Devices)", "Xiaomi (Mi/Redmi Devices)", "OnePlus (Oxygen OS)",
        "Oppo (ColorOS)", "RealMe (ColorOS/RealMe UI)", "Vivo (Funtouch OS)",
        "Sony (Xperia Devices)", "Motorola (Moto Devices)", "Nokia (HMD Devices)",
        "TCL (Alcatel/TCL Devices)", "ZTE (Nubia/Axon Devices)", "Jio (STB/Phone)",
        NULL
    };

    // Unique IDs for each category
    const gchar *mock_ids[] = {
        "Samsung", "Xiaomi", "OnePlus", "Oppo", "RealMe", "Vivo",
        "Sony", "Motorola", "Nokia", "TCL", "ZTE", "Jio",
        NULL
    };

    GtkListBoxRow *initial_row = NULL;

    // Create a row for each manufacturer
    for (int i = 0; mock_devices[i] != NULL; i++) {
        GtkWidget *row_widget = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

        GtkWidget *icon = gtk_image_new_from_icon_name("phone-symbolic");
        gtk_image_set_pixel_size(GTK_IMAGE(icon), 24); // Make icon larger
        gtk_box_append(GTK_BOX(hbox), icon);
        
        GtkWidget *label = gtk_label_new(mock_devices[i]);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0);
        gtk_box_append(GTK_BOX(hbox), label);
        
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_widget), hbox);

        // Store the category ID (e.g., "Samsung") as data on the row
        g_object_set_data(G_OBJECT(row_widget), "device-id", g_strdup(mock_ids[i]));

        gtk_list_box_append(GTK_LIST_BOX(state->device_list_box), row_widget);

        // Check if this is the one we should select by default
        if (g_strcmp0(mock_ids[i], state->selected_manufacturer_id) == 0) {
            initial_row = GTK_LIST_BOX_ROW(row_widget);
        }
    }

    // Automatically select the default manufacturer
    if (initial_row) {
        gtk_list_box_select_row(GTK_LIST_BOX(state->device_list_box), initial_row);
        // Manually trigger the selection handler to load apps for the first time
        on_device_selected(GTK_LIST_BOX(state->device_list_box), initial_row, state);
    }
}

/**
 * @brief Populates the right-side app list based on the selected manufacturer.
 */
static void update_app_list(AppState *state) {
    // Clear existing app list UI
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->app_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), GTK_WIDGET(row));
    }
    // Free all AppItem structs from the previous list
    g_list_free_full(state->app_list, (GDestroyNotify)app_item_free);
    state->app_list = NULL;

    // This large if/else block maps the category ID to the correct static array.
    // A GHashTable could also be used, but this is simple and clear.
    const PackageEntry *packages_to_load = NULL;
    const gchar *category_id = state->selected_manufacturer_id;

    if (category_id) {
        if (g_strcmp0(category_id, "Samsung") == 0) {
            packages_to_load = SAMSUNG_APPS;
        } else if (g_strcmp0(category_id, "Xiaomi") == 0) {
            packages_to_load = XIAOMI_APPS;
        } else if (g_strcmp0(category_id, "Vivo") == 0) {
            packages_to_load = VIVO_APPS;
        } else if (g_strcmp0(category_id, "TCL") == 0) {
            packages_to_load = TCL_APPS;
        } else if (g_strcmp0(category_id, "Sony") == 0) {
            packages_to_load = SONY_APPS;
        } else if (g_strcmp0(category_id, "RealMe") == 0) {
            packages_to_load = REALME_APPS;
        } else if (g_strcmp0(category_id, "Oppo") == 0) {
            packages_to_load = OPPO_APPS;
        } else if (g_strcmp0(category_id, "OnePlus") == 0) {
            packages_to_load = ONEPLUS_APPS;
        } else if (g_strcmp0(category_id, "Nokia") == 0) {
            packages_to_load = NOKIA_APPS;
        } else if (g_strcmp0(category_id, "Motorola") == 0) {
            packages_to_load = MOTOROLA_APPS;
        } else if (g_strcmp0(category_id, "ZTE") == 0) {
            packages_to_load = ZTE_APPS;
        } else if (g_strcmp0(category_id, "Jio") == 0) {
            packages_to_load = JIO_APPS;
        }
    }
    
    if (!packages_to_load) {
        gtk_label_set_text(GTK_LABEL(state->status_label), "Select a device category to view available bloatware.");
        return;
    }

    // Populate the list box with the selected packages
    for (int i = 0; packages_to_load[i].display_name != NULL; i++) {
        // Create a new AppItem to track this app's state
        AppItem *item = g_new0(AppItem, 1);
        item->display_name = g_strdup(packages_to_load[i].display_name);
        item->package_name = g_strdup(packages_to_load[i].package_name);
        item->is_selected = FALSE;

        // --- Create the UI row ---
        GtkWidget *row_widget = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15); 

        GtkWidget *check = gtk_check_button_new();
        gtk_box_append(GTK_BOX(hbox), check);

        GtkWidget *icon = gtk_image_new_from_icon_name("package-x-generic-symbolic"); 
        gtk_image_set_pixel_size(GTK_IMAGE(icon), 16); 
        gtk_box_append(GTK_BOX(hbox), icon);
        
        // VBox for Display Name + Package Name
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        GtkWidget *display_label = gtk_label_new(item->display_name);
        GtkWidget *package_label = gtk_label_new(item->package_name);

        gtk_label_set_xalign(GTK_LABEL(display_label), 0.0);
        gtk_label_set_xalign(GTK_LABEL(package_label), 0.0);
        gtk_widget_add_css_class(package_label, "caption"); // Use caption style for smaller text
        gtk_widget_set_opacity(package_label, 0.7); 

        gtk_box_append(GTK_BOX(vbox), display_label);
        gtk_box_append(GTK_BOX(vbox), package_label);
        gtk_box_append(GTK_BOX(hbox), vbox);
        // --- End UI row ---

        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_widget), hbox);
        gtk_list_box_append(GTK_LIST_BOX(state->app_list_box), row_widget);

        // Store reference to the row for later removal
        item->main_list_row = row_widget;
        
        // Connect the checkbox to its callback, passing the 'item'
        g_signal_connect(check, "toggled", G_CALLBACK(on_app_toggled), item);

        // Add the new item to our data list
        state->app_list = g_list_append(state->app_list, item);
    }

    // Refresh the action bar (it will be hidden)
    update_action_bar_visibility(state);
}


// --- UI SETUP ---

/**
 * @brief Creates all UI widgets and connects signals.
 */
static void activate(GtkApplication *app, AppState *state) {
    // Initialize application state
    state->selected_device_id = NULL;
    state->selected_manufacturer_id = g_strdup("Samsung"); // Default category to show
    state->app_list = NULL;
    state->uninstalled_app_list = NULL; 

    // --- Window ---
    state->window = ADW_APPLICATION_WINDOW(adw_application_window_new(GTK_APPLICATION(app)));
    gtk_window_set_title(GTK_WINDOW(state->window), "Android Debloater");
    gtk_window_set_default_size(GTK_WINDOW(state->window), 900, 600);

    GtkWidget *main_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    adw_application_window_set_content(state->window, main_content_box);

    // --- Header Bar ---
    GtkWidget *header_bar = adw_header_bar_new();
    gtk_box_append(GTK_BOX(main_content_box), header_bar);

    // Stack switcher will sit in the middle of the header bar
    GtkWidget *stack_switcher = gtk_stack_switcher_new();
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(header_bar), stack_switcher);

    // "About" button on the right
    GtkWidget *about_button = gtk_button_new_from_icon_name("help-about-symbolic");
    adw_header_bar_pack_end(ADW_HEADER_BAR(header_bar), about_button);
    g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_clicked), GTK_WINDOW(state->window));

    // --- Main Stack (for Pages) ---
    GtkWidget *stack = gtk_stack_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stack_switcher), GTK_STACK(stack));
    gtk_box_append(GTK_BOX(main_content_box), stack);


    // --- PAGE 1: DEBLOAT ---
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_stack_add_titled(GTK_STACK(stack), paned, "debloat", "Debloat");

    // --- Left Pane (Device Selection) ---
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(left_box, 10);
    gtk_widget_set_margin_end(left_box, 10);
    gtk_widget_set_margin_top(left_box, 10);
    gtk_widget_set_margin_bottom(left_box, 10);
    gtk_paned_set_start_child(GTK_PANED(paned), left_box);
    gtk_paned_set_resize_start_child(GTK_PANED(paned), FALSE); 
    gtk_paned_set_shrink_start_child(GTK_PANED(paned), FALSE);

    // ADB Device Dropdown
    GtkWidget *adb_header = gtk_label_new("Select Connected Device");
    gtk_widget_add_css_class(adb_header, "title-4");
    gtk_label_set_xalign(GTK_LABEL(adb_header), 0.0);
    gtk_box_append(GTK_BOX(left_box), adb_header);

    GtkWidget *adb_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(left_box), adb_hbox);

    state->device_model = gtk_string_list_new(NULL);
    gtk_string_list_append(state->device_model, "Select a connected device...");
    state->device_dropdown = gtk_drop_down_new(G_LIST_MODEL(state->device_model), NULL);
    gtk_widget_set_hexpand(state->device_dropdown, TRUE);
    g_signal_connect(state->device_dropdown, "notify::selected", G_CALLBACK(on_adb_device_selected), state);
    gtk_box_append(GTK_BOX(adb_hbox), state->device_dropdown);

    GtkWidget *refresh_button = gtk_button_new_from_icon_name("view-refresh-symbolic");
    gtk_widget_set_tooltip_text(refresh_button, "Refresh ADB Device List");
    g_signal_connect(refresh_button, "clicked", G_CALLBACK(on_refresh_devices_clicked), state);
    gtk_box_append(GTK_BOX(adb_hbox), refresh_button);

    // Device Status Label
    state->device_status_label = gtk_label_new("Welcome! Connect a device.");
    gtk_widget_set_margin_top(state->device_status_label, 5);
    gtk_label_set_xalign(GTK_LABEL(state->device_status_label), 0.0);
    gtk_label_set_wrap(GTK_LABEL(state->device_status_label), TRUE);
    gtk_box_append(GTK_BOX(left_box), state->device_status_label);

    // Manufacturer List
    GtkWidget *device_header = gtk_label_new("Select Manufacturer List");
    gtk_widget_add_css_class(device_header, "title-4");
    gtk_label_set_xalign(GTK_LABEL(device_header), 0.0);
    gtk_widget_set_margin_top(device_header, 10);
    gtk_box_append(GTK_BOX(left_box), device_header);

    state->device_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->device_list_box), GTK_SELECTION_SINGLE);
    g_signal_connect(state->device_list_box, "row-activated", G_CALLBACK(on_device_selected), state);

    GtkWidget *device_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(device_scrolled), state->device_list_box);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(device_scrolled), 200);
    gtk_widget_set_vexpand(device_scrolled, TRUE);
    gtk_box_append(GTK_BOX(left_box), device_scrolled);

    // --- Right Pane (App List & Action Bar) ---
    GtkWidget *right_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_paned_set_end_child(GTK_PANED(paned), right_container);

    GtkWidget *right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(right_vbox, TRUE);
    gtk_box_append(GTK_BOX(right_container), right_vbox);

    GtkWidget *app_header = gtk_label_new("Installed User/System Apps (Select to Debloat)");
    gtk_widget_add_css_class(app_header, "title-4");
    gtk_widget_set_margin_top(app_header, 10);
    gtk_widget_set_margin_bottom(app_header, 5);
    gtk_box_append(GTK_BOX(right_vbox), app_header);
    
    state->app_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->app_list_box), GTK_SELECTION_NONE);

    GtkWidget *app_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(app_scrolled), state->app_list_box);
    gtk_widget_set_vexpand(app_scrolled, TRUE);
    gtk_box_append(GTK_BOX(right_vbox), app_scrolled);

    // Bottom Action Bar
    state->action_bar = gtk_action_bar_new();
    gtk_widget_set_visible(state->action_bar, FALSE); // Hide by default
    gtk_box_append(GTK_BOX(right_container), state->action_bar);

    state->status_label = gtk_label_new("Select a device and then select apps to debloat.");
    gtk_action_bar_pack_start(GTK_ACTION_BAR(state->action_bar), state->status_label);

    state->debloat_button = gtk_button_new_with_label("Remove");
    gtk_widget_set_tooltip_text(state->debloat_button, "Uninstall selected apps from the device");
    gtk_widget_add_css_class(state->debloat_button, "destructive-action");
    gtk_action_bar_pack_end(GTK_ACTION_BAR(state->action_bar), state->debloat_button);
    g_signal_connect(state->debloat_button, "clicked", G_CALLBACK(on_debloat_clicked), state);
    
    gtk_paned_set_position(GTK_PANED(paned), 320); 


    // --- PAGE 2: UNDO ---
    GtkWidget *undo_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(undo_page_box, 10);
    gtk_widget_set_margin_end(undo_page_box, 10);
    gtk_widget_set_margin_top(undo_page_box, 10);
    gtk_widget_set_margin_bottom(undo_page_box, 10);
    gtk_stack_add_titled(GTK_STACK(stack), undo_page_box, "undo", "Undo");

    GtkWidget *uninstalled_header = gtk_label_new("Recently Uninstalled (Undo)");
    gtk_widget_add_css_class(uninstalled_header, "title-4");
    gtk_label_set_xalign(GTK_LABEL(uninstalled_header), 0.0);
    gtk_widget_set_margin_top(uninstalled_header, 10);
    gtk_box_append(GTK_BOX(undo_page_box), uninstalled_header);

    state->uninstalled_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->uninstalled_list_box), GTK_SELECTION_NONE);

    GtkWidget *uninstalled_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(uninstalled_scrolled), state->uninstalled_list_box);
    gtk_widget_set_vexpand(uninstalled_scrolled, TRUE);
    gtk_box_append(GTK_BOX(undo_page_box), uninstalled_scrolled);


    // --- PAGE 3: DEBUG ---
    GtkWidget *debug_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(debug_page_box, 10);
    gtk_widget_set_margin_end(debug_page_box, 10);
    gtk_widget_set_margin_top(debug_page_box, 10);
    gtk_widget_set_margin_bottom(debug_page_box, 10);
    gtk_stack_add_titled(GTK_STACK(stack), debug_page_box, "debug", "Debug");

    // Header
    GtkWidget *debug_header = gtk_label_new("Run Custom ADB Shell Command");
    gtk_widget_add_css_class(debug_header, "title-4");
    gtk_label_set_xalign(GTK_LABEL(debug_header), 0.0);
    gtk_widget_set_margin_top(debug_header, 10);
    gtk_box_append(GTK_BOX(debug_page_box), debug_header);

    GtkWidget *debug_info_label = gtk_label_new(
        "Enter a shell command (e.g., 'pm list packages'). 'adb -s <device_id> shell' is prefixed automatically."
    );
    gtk_label_set_xalign(GTK_LABEL(debug_info_label), 0.0);
    gtk_widget_add_css_class(debug_info_label, "caption");
    gtk_box_append(GTK_BOX(debug_page_box), debug_info_label);

    // Command Entry Box
    GtkWidget *debug_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_margin_top(debug_hbox, 5);
    gtk_box_append(GTK_BOX(debug_page_box), debug_hbox);
    
    state->debug_command_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(state->debug_command_entry), "pm list packages -f");
    gtk_widget_set_hexpand(state->debug_command_entry, TRUE);
    gtk_box_append(GTK_BOX(debug_hbox), state->debug_command_entry);

    GtkWidget *debug_run_button = gtk_button_new_with_label("Run");
    gtk_widget_add_css_class(debug_run_button, "suggested-action");
    g_signal_connect(debug_run_button, "clicked", G_CALLBACK(on_run_debug_command_clicked), state);
    gtk_box_append(GTK_BOX(debug_hbox), debug_run_button);

    // Output Text View
    GtkWidget *output_header = gtk_label_new("Command Output");
    gtk_widget_add_css_class(output_header, "title-4");
    gtk_label_set_xalign(GTK_LABEL(output_header), 0.0);
    gtk_widget_set_margin_top(output_header, 10);
    gtk_box_append(GTK_BOX(debug_page_box), output_header);

    GtkWidget *debug_scrolled = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(debug_scrolled, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(debug_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_append(GTK_BOX(debug_page_box), debug_scrolled);

    GtkWidget *debug_output_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(debug_output_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(debug_output_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(debug_output_view), GTK_WRAP_WORD_CHAR);
    state->debug_output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(debug_output_view));
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(debug_scrolled), debug_output_view);
    
    // --- FINAL SETUP ---

    // Load initial manufacturer list (which auto-selects and loads the app list)
    update_device_list(state);
    
    // Populate ADB devices at startup
    populate_adb_devices(state);

    // Show the main window
    gtk_window_present(GTK_WINDOW(state->window));

    // Show the welcome guide dialog *after* the main window is presented
    show_welcome_dialog(GTK_WINDOW(state->window));
}

/**
 * @brief Frees all global state resources on application shutdown.
 */
static void on_shutdown(GtkApplication *app, gpointer user_data) {
    // Free all memory allocated and stored in the global state
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
    // Finally, free the state struct itself
    g_free(app_state);
}

// --- MAIN FUNCTION ---

int main(int argc, char **argv) {
    AdwApplication *app;
    int status;

    // Allocate and zero-initialize global state
    app_state = g_new0(AppState, 1);

    // Create a new Libadwaita application
    app = adw_application_new("com.gemini.debloater", G_APPLICATION_DEFAULT_FLAGS);
    
    // Connect signals
    g_signal_connect(app, "activate", G_CALLBACK(activate), app_state); // Pass state
    g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
