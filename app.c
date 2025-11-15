#include <gtk/gtk.h>
#include <adwaita.h> // <-- ADD THIS
#include <glib/glist.h>
#include <gio/gio.h>
#include <sys/wait.h> // Required for WEXITSTATUS

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
    GtkWidget *main_list_row; // Reference to the row in the main app list
    GtkWidget *uninstalled_list_row; // Reference to the row in the uninstalled list
} AppItem;

// Structure to hold global application state
typedef struct {
    AdwApplicationWindow *window; // <-- CHANGED
    GtkWidget *device_list_box;
    GtkWidget *app_list_box;
    GtkWidget *action_bar;
    GtkWidget *debloat_button;
    GtkWidget *status_label;
    
    // New widgets for real ADB device selection
    GtkWidget *device_dropdown;
    GtkStringList *device_model;

    gchar *selected_device_id;        // The real device serial (e.g., "ABC12345")
    gchar *selected_manufacturer_id;  // The list to show (e.g., "Samsung")
    GList *app_list; // List of AppItem*
    
    GList *uninstalled_app_list; // List of uninstalled AppItem*
    GtkWidget *uninstalled_list_box; // The UI list for uninstalled apps
} AppState;

// Global state instance
static AppState *app_state = NULL;

// --- FUNCTION PROTOTYPES ---
static void update_app_list(AppState *state);
static void update_action_bar_visibility(AppState *state);
static void populate_adb_devices(AppState *state);
static void on_adb_device_selected(GtkDropDown *dropdown, GParamSpec *pspec, AppState *state);
static void on_reinstall_clicked(GtkButton *button, AppItem *item);
static void add_row_to_uninstalled_list(AppState *state, AppItem *item);
static void on_about_clicked(GtkButton *button, GtkWindow *parent); // <-- CHANGED


// --- PACKAGE DATA (Bloatware lists) ---
// (Lists are unchanged, snipped for brevity)
// ...
// ... (SAMSUNG_APPS, XIAOMI_APPS, VIVO_APPS, etc.)
// ...

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

    // New simplified logic: Do we have selected apps AND a real selected device?
    if (selected_count > 0 && state->selected_device_id != NULL) {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gchar *label_text = g_strdup_printf("Ready to debloat %d app(s) on device '%s'.", selected_count, state->selected_device_id);
        gtk_label_set_text(GTK_LABEL(state->status_label), label_text);
        g_free(label_text);
        gtk_widget_set_sensitive(state->debloat_button, TRUE);
    } 
    else {
        gtk_widget_set_visible(state->action_bar, FALSE);
    }
}

// --- CALLBACKS ---

/**
 * @brief Handler for the About button. (FIXED: Uses AdwAboutDialog correctly)
 */
static void on_about_clicked(GtkButton *button, GtkWindow *parent) {
    
    // adw_about_dialog_new() returns the AdwDialog parent type
    AdwDialog *dialog = adw_about_dialog_new();
    
    // Cast to AdwAboutDialog* to set its specific properties
    adw_about_dialog_set_application_name(ADW_ABOUT_DIALOG(dialog), "Android Debloater");
    adw_about_dialog_set_version(ADW_ABOUT_DIALOG(dialog), "0.7 (Libadwaita)");
    adw_about_dialog_set_developer_name(ADW_ABOUT_DIALOG(dialog), "Kushagra Karira");
    adw_about_dialog_set_copyright(ADW_ABOUT_DIALOG(dialog), "© 2025 Kushagra Karira");
    adw_about_dialog_set_comments(ADW_ABOUT_DIALOG(dialog), "A GTK4/Libadwaita application for debloating Android devices using ADB.");
    adw_about_dialog_set_website(ADW_ABOUT_DIALOG(dialog), "https://kushagrakarira.com");
    adw_about_dialog_set_issue_url(ADW_ABOUT_DIALOG(dialog), "https://github.com/KushagraKarira/Debloat/issues");
    adw_about_dialog_add_link(ADW_ABOUT_DIALOG(dialog), "Project Link", "https://github.com/KushagraKarira/Debloat");
    
    // This is the correct way to show the dialog.
    // It automatically handles modality and being "transient for" the parent.
    // Cast the parent GtkWindow to a GtkWidget
    adw_dialog_present(dialog, GTK_WIDGET(parent));
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
    const gchar *manufacturer_id = g_object_get_data(G_OBJECT(row), "device-id");
    if (!manufacturer_id) return;

    // Store the selected MANUFACTURER ID
    if (state->selected_manufacturer_id) g_free(state->selected_manufacturer_id);
    state->selected_manufacturer_id = g_strdup(manufacturer_id);
    
    g_print("Manufacturer list selected: %s\n", state->selected_manufacturer_id);
    
    // Disable the debloat button immediately until a package is selected
    gtk_widget_set_sensitive(state->debloat_button, FALSE);
    gtk_widget_set_visible(state->action_bar, FALSE);

    // Load app list for the new device category
    update_app_list(state);
}


/**
 * @brief Runs 'adb devices' and populates the device_dropdown.
 */
static void populate_adb_devices(AppState *state) {
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status;
    GError *error = NULL;
    gchar *cmd_args[] = {"adb", "devices", NULL};

    g_print("Running: adb devices\n");

    // --- START FIX V3 ---
    // Clear the existing list, but keep the placeholder at index 0
    guint n_items = g_list_model_get_n_items(G_LIST_MODEL(state->device_model));
    // Remove items from index 1 (the one after the placeholder)
    while (n_items > 1) {
        gtk_string_list_remove(state->device_model, 1);
        n_items--;
    }
    // --- END FIX V3 ---
    
    // Set default selection to the placeholder
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
        g_printerr("Failed to execute 'adb': %s. Is it in your PATH?\n", error->message);
        gtk_label_set_text(GTK_LABEL(state->status_label), "Error: 'adb' command not found.");
        g_error_free(error);
        g_free(stdout_str);
        g_free(stderr_str);
        return;
    }

    if (stdout_str) {
        g_print("ADB Output:\n%s\n", stdout_str);
        
        // Split the output by lines
        gchar **lines = g_strsplit(stdout_str, "\n", -1);
        
        for (int i = 0; lines[i] != NULL; i++) {
            // Skip the first line ("List of devices attached")
            if (i == 0) continue;

            // Split line by tab
            gchar **parts = g_strsplit(lines[i], "\t", 2);
            
            if (g_strv_length(parts) == 2) {
                gchar *device_id = g_strstrip(parts[0]);
                gchar *device_state = g_strstrip(parts[1]);

                // Only add devices that are fully connected
                if (g_strcmp0(device_state, "device") == 0) {
                    g_print("Found device: %s\n", device_id);
                    gtk_string_list_append(state->device_model, device_id);
                }
            }
            g_strfreev(parts);
        }
        g_strfreev(lines);
    }
    
    g_free(stdout_str);
    g_free(stderr_str);
    update_action_bar_visibility(state);
}

/**
 * @brief Handler for when a device is selected from the ADB dropdown.
 */
static void on_adb_device_selected(GtkDropDown *dropdown, GParamSpec *pspec, AppState *state) {
    guint pos = gtk_drop_down_get_selected(dropdown);

    if (state->selected_device_id) {
        g_free(state->selected_device_id);
        state->selected_device_id = NULL;
    }

    // pos 0 is the "Select a connected device..." placeholder
    if (pos > 0) {
        const gchar *device_id = gtk_string_list_get_string(state->device_model, pos);
        state->selected_device_id = g_strdup(device_id);
        g_print("ADB device selected: %s\n", state->selected_device_id);
    } else {
        g_print("No ADB device selected.\n");
    }
    
    update_action_bar_visibility(state);
}

/**
 * @brief Simple callback to refresh the ADB device list.
 */
static void on_refresh_devices_clicked(GtkButton *button, AppState *state) {
    populate_adb_devices(state);
}


/**
 * @brief Handler for the "Undo" (Reinstall) button click.
 */
static void on_reinstall_clicked(GtkButton *button, AppItem *item) {
    // Use the global app_state to get the selected device ID
    if (!app_state || !app_state->selected_device_id) {
        g_printerr("Cannot reinstall: No ADB device selected.\n");
        gtk_label_set_text(GTK_LABEL(app_state->status_label), "Error: Select ADB device to reinstall.");
        return;
    }

    g_print("Attempting to reinstall: %s\n", item->package_name);
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE); // Disable button during op

    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status;
    GError *error = NULL;

    // Use the 'cmd package install-existing' command
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
        g_printerr("Reinstall failed: %s\n", error->message);
        g_error_free(error);
    } else if (stripped_output && g_str_has_prefix(stripped_output, "Package")) {
        // Success output is usually "Package com.example.app installed..."
        g_print("Reinstall successful.\n");
        
        // Remove from UI
        gtk_list_box_remove(GTK_LIST_BOX(app_state->uninstalled_list_box), item->uninstalled_list_row);
        
        // Remove from data list
        app_state->uninstalled_app_list = g_list_remove(app_state->uninstalled_app_list, item);
        
        // Free the item data (it's no longer tracked)
        app_item_free(item);
        
        // We don't need to re-enable the button, it's being destroyed.
    } else {
        g_printerr("Reinstall failed. STDOUT: %s, STDERR: %s\n", 
            stdout_str ? stdout_str : "None", 
            stderr_str ? stderr_str : "None");
        gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE); // Re-enable on failure
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
    
    // Store a reference to the new row
    item->uninstalled_list_row = row; 

    gtk_list_box_append(GTK_LIST_BOX(state->uninstalled_list_box), row);
}


/**
 * @brief Handler for the Debloat button click. Executes the actual ADB uninstall command.
 */
static void on_debloat_clicked(GtkButton *button, AppState *state) {
    gint count = 0;
    gint success_count = 0;
    gint failure_count = 0;
    GList *items_to_remove = NULL;
    gboolean error_in_adb_path = FALSE;

    gtk_widget_set_sensitive(state->debloat_button, FALSE);
    gtk_label_set_text(GTK_LABEL(state->status_label), "Starting real ADB debloat process...");

    GList *current_l = state->app_list;
    while (current_l != NULL) {
        AppItem *item = (AppItem *)current_l->data;
        GList *next_l = current_l->next; 

        if (item->is_selected) {
            count++;

            gchar *stdout_str = NULL;
            gchar *stderr_str = NULL;
            gint exit_status;
            gboolean spawn_success;
            GError *error = NULL;
            gboolean uninstall_successful = FALSE;

            // Construct the command arguments array
            gchar *cmd_args[] = {
                "adb",
                "-s", state->selected_device_id, // Use the device ID
                "shell", 
                "pm", 
                "uninstall", 
                "--user", 
                "0", 
                item->package_name,
                NULL
            };

            g_print("\n--- Running Uninstall Command ---\n");
            g_print("Executing: adb -s %s shell pm uninstall --user 0 %s\n", 
                        state->selected_device_id, item->package_name);
            
            // Execute the command using g_spawn_sync
            spawn_success = g_spawn_sync(
                NULL,            // working_directory
                cmd_args,        // argv
                NULL,            // envp
                G_SPAWN_SEARCH_PATH, // flags: search for 'adb' in PATH
                NULL,            // child_setup
                NULL,            // user_data
                &stdout_str,     // standard output
                &stderr_str,     // standard error
                &exit_status,    // exit status
                &error           // error
            );

            if (error) {
                // This usually means ADB could not be found or executed (e.g., path issue, permissions)
                g_printerr("ADB Execution Error for package %s: %s\n", item->package_name, error->message);
                g_error_free(error);
                failure_count++;
                error_in_adb_path = TRUE;
            } else if (!spawn_success) {
                 // Should be covered by the error block, but included for completeness
                 g_printerr("ADB Command failed to start for package %s.\n", item->package_name);
                 failure_count++;
            } else {
                // Command ran successfully, check the output for ADB result
                // FIX: Use g_strstrip (lowercase s) to clean output
                gchar *stripped_output = stdout_str ? g_strstrip(stdout_str) : NULL;
                
                if (stripped_output && g_str_has_prefix(stripped_output, "Success")) {
                    uninstall_successful = TRUE;
                    g_print("ADB Output: Success\n");
                } else if (stripped_output && g_str_has_prefix(stripped_output, "Failure")) {
                    // ADB reports Failure for various reasons (e.g., package not found, protected package)
                    g_print("ADB Output: Failure (Stdout: %s)\n", stripped_output);
                } else if (WEXITSTATUS(exit_status) != 0) {
                    // Non-zero exit status (e.g., device offline)
                    g_print("ADB Command Failed (Exit: %d). Stderr: %s\n", WEXITSTATUS(exit_status), stderr_str ? stderr_str : "None");
                }
                
                if (uninstall_successful) {
                    success_count++;
                    // Mark for UI removal and cleanup
                    items_to_remove = g_list_append(items_to_remove, item);
                    gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), item->main_list_row);
                } else {
                    failure_count++;
                }
            }

            g_free(stdout_str);
            g_free(stderr_str);
            g_print("---------------------------------\n");
            if (error_in_adb_path) break; // Stop if ADB execution failed globally
        }
        current_l = next_l; 
    }

    // Move successful items from the main list to the uninstalled list
    GList *remove_l;
    for (remove_l = items_to_remove; remove_l != NULL; remove_l = remove_l->next) {
        AppItem *item = (AppItem *)remove_l->data;
        
        // 1. Remove from main app list
        state->app_list = g_list_remove(state->app_list, item);
        
        // 2. Add to uninstalled data list
        state->uninstalled_app_list = g_list_append(state->uninstalled_app_list, item);
        
        // 3. Add to uninstalled UI list
        add_row_to_uninstalled_list(state, item);
        
        // 4. We DO NOT free the item
    }
    g_list_free(items_to_remove);


    // Update status and reset selection
    gchar *result_msg;
    if (error_in_adb_path) {
        result_msg = g_strdup_printf(
            "FATAL ERROR: Could not execute 'adb'. Check your system path and ensure ADB is installed and authorized."
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

    gtk_widget_set_sensitive(state->debloat_button, TRUE);
    update_action_bar_visibility(state);
}


// --- LIST POPULATION FUNCTIONS ---

/**
 * @brief Populates the Device List with options.
 */
static void update_device_list(AppState *state) {
    // Clear existing list
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->device_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->device_list_box), GTK_WIDGET(row));
    }

    // Device display names and unique IDs (These are category IDs)
    const gchar *mock_devices[] = {
        "Samsung (Galaxy Devices)",
        "Xiaomi (Mi/Redmi Devices)",
        "OnePlus (Oxygen OS)",
        "Oppo (ColorOS)",
        "RealMe (ColorOS/RealMe UI)",
        "Vivo (Funtouch OS)",
        "Sony (Xperia Devices)",
        "Motorola (Moto Devices)",
        "Nokia (HMD Devices)",
        "TCL (Alcatel/TCL Devices)",
        "ZTE (Nubia/Axon Devices)",
        "Jio (STB/Phone)",
        NULL
    };

    const gchar *mock_ids[] = {
        "Samsung",
        "Xiaomi",
        "OnePlus",
        "Oppo",
        "RealMe",
        "Vivo",
        "Sony",
        "Motorola",
        "Nokia",
        "TCL",
        "ZTE",
        "Jio",
        NULL
    };

    GtkListBoxRow *initial_row = NULL;

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

        // Select the row corresponding to the initial state->selected_manufacturer_id
        if (g_strcmp0(mock_ids[i], state->selected_manufacturer_id) == 0) {
            initial_row = GTK_LIST_BOX_ROW(row_widget);
        }
    }

    // Automatically select the initial device
    if (initial_row) {
        gtk_list_box_select_row(GTK_LIST_BOX(state->device_list_box), initial_row);
        // Manually trigger the selection handler to load apps
        on_device_selected(GTK_LIST_BOX(state->device_list_box), initial_row, state);
    }
}

/**
 * @brief Populates the App List with packages for the selected device category.
 */
static void update_app_list(AppState *state) {
    // Clear existing app list and free internal state
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->app_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), GTK_WIDGET(row));
    }
    g_list_free_full(state->app_list, (GDestroyNotify)app_item_free);
    state->app_list = NULL;

    // Determine which package list to load based on the category ID
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
        item->main_list_row = row_widget;
        g_signal_connect(check, "toggled", G_CALLBACK(on_app_toggled), item);

        state->app_list = g_list_append(state->app_list, item);
    }

    // Ensure the action bar is hidden/updated
    update_action_bar_visibility(state);
}


// --- UI SETUP ---

/**
 * @brief Creates the main application window and UI elements.
 */
static void activate(GtkApplication *app, AppState *state) {
    // Initialize application state
    state->selected_device_id = NULL; // No device selected at start
    state->selected_manufacturer_id = g_strdup("Oppo"); // Default list to show
    state->app_list = NULL;
    state->uninstalled_app_list = NULL; 

    // Create main window
    state->window = ADW_APPLICATION_WINDOW(adw_application_window_new(GTK_APPLICATION(app)));
    gtk_window_set_title(GTK_WINDOW(state->window), "Android Debloater");
    gtk_window_set_default_size(GTK_WINDOW(state->window), 900, 600);

    // --- Main Vertical Box (Header + Stack) ---
    GtkWidget *main_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    adw_application_window_set_content(state->window, main_content_box);

    // --- HEADER BAR with StackSwitcher and About Button ---
    GtkWidget *header_bar = adw_header_bar_new();
    gtk_box_append(GTK_BOX(main_content_box), header_bar);

    GtkWidget *stack_switcher = gtk_stack_switcher_new();
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(header_bar), stack_switcher);

    GtkWidget *about_button = gtk_button_new_from_icon_name("help-about-symbolic");
    adw_header_bar_pack_end(ADW_HEADER_BAR(header_bar), about_button);
    g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_clicked), GTK_WINDOW(state->window));

    // --- Main Stack (Debloat Page, Undo Page) ---
    GtkWidget *stack = gtk_stack_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stack_switcher), GTK_STACK(stack));
    gtk_box_append(GTK_BOX(main_content_box), stack);


    // --- PAGE 1: DEBLOAT (Paned Layout) ---
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_stack_add_titled(GTK_STACK(stack), paned, "debloat", "Debloat");

    // --- LEFT PANE (Device Selection) ---
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(left_box, 10);
    gtk_widget_set_margin_end(left_box, 10);
    gtk_widget_set_margin_top(left_box, 10);
    gtk_widget_set_margin_bottom(left_box, 10);

    // --- ADB Device Selector ---
    GtkWidget *adb_header = gtk_label_new("Select Connected Device");
    gtk_widget_add_css_class(adb_header, "title-4");
    gtk_label_set_xalign(GTK_LABEL(adb_header), 0.0);
    gtk_box_append(GTK_BOX(left_box), adb_header); // <-- TYPO FIX

    GtkWidget *adb_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_append(GTK_BOX(left_box), adb_hbox);

    // The Dropdown Menu
    state->device_model = gtk_string_list_new(NULL);
    // Add placeholder *immediately*
    gtk_string_list_append(state->device_model, "Select a connected device...");
    state->device_dropdown = gtk_drop_down_new(G_LIST_MODEL(state->device_model), NULL);
    gtk_widget_set_hexpand(state->device_dropdown, TRUE);
    g_signal_connect(state->device_dropdown, "notify::selected", G_CALLBACK(on_adb_device_selected), state);
    gtk_box_append(GTK_BOX(adb_hbox), state->device_dropdown);

    // The Refresh Button
    GtkWidget *refresh_button = gtk_button_new_from_icon_name("view-refresh-symbolic");
    gtk_widget_set_tooltip_text(refresh_button, "Refresh ADB Device List");
    g_signal_connect(refresh_button, "clicked", G_CALLBACK(on_refresh_devices_clicked), state);
    gtk_box_append(GTK_BOX(adb_hbox), refresh_button);

    // --- Manufacturer List ---
    GtkWidget *device_header = gtk_label_new("Select Manufacturer List"); // Title changed
    gtk_widget_add_css_class(device_header, "title-4");
    gtk_label_set_xalign(GTK_LABEL(device_header), 0.0);
    gtk_box_append(GTK_BOX(left_box), device_header);
    gtk_widget_set_margin_top(device_header, 10);

    state->device_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->device_list_box), GTK_SELECTION_SINGLE);
    g_signal_connect(state->device_list_box, "row-activated", G_CALLBACK(on_device_selected), state);

    GtkWidget *device_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(device_scrolled), state->device_list_box);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(device_scrolled), 200);

    gtk_widget_set_hexpand(state->device_list_box, TRUE);
    gtk_widget_set_vexpand(state->device_list_box, TRUE);

    gtk_box_append(GTK_BOX(left_box), device_scrolled);
    
    // Add left_box to paned
    gtk_paned_set_start_child(GTK_PANED(paned), left_box);
    gtk_paned_set_resize_start_child(GTK_PANED(paned), FALSE); 
    gtk_paned_set_shrink_start_child(GTK_PANED(paned), FALSE);

    // --- RIGHT PANE (App List) ---
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

    state->status_label = gtk_label_new("Select a device and then select apps to debloat.");
    gtk_action_bar_pack_start(GTK_ACTION_BAR(state->action_bar), state->status_label);

    state->debloat_button = gtk_button_new_with_label("Remove"); // <-- CHANGED
    gtk_widget_set_tooltip_text(state->debloat_button, "Uninstall selected apps from the device");
    gtk_widget_add_css_class(state->debloat_button, "destructive-action");
    gtk_action_bar_pack_end(GTK_ACTION_BAR(state->action_bar), state->debloat_button);
    g_signal_connect(state->debloat_button, "clicked", G_CALLBACK(on_debloat_clicked), state);

    // Combine right content and action bar
    GtkWidget *right_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(right_container), right_vbox);
    gtk_box_append(GTK_BOX(right_container), state->action_bar);

    gtk_paned_set_end_child(GTK_PANED(paned), right_container);

    // Set initial paned position 
    gtk_paned_set_position(GTK_PANED(paned), 320); 


    // --- PAGE 2: UNDO ---
    GtkWidget *undo_page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(undo_page_box, 10);
    gtk_widget_set_margin_end(undo_page_box, 10);
    gtk_widget_set_margin_top(undo_page_box, 10);
    gtk_widget_set_margin_bottom(undo_page_box, 10);
    
    // Add "Undo" page to the stack
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


    // --- FINAL SETUP ---

    // Load initial device list (which auto-selects and loads the app list)
    update_device_list(state);
    
    // Populate ADB devices at startup
    populate_adb_devices(state);

    gtk_window_present(GTK_WINDOW(state->window));
}

/**
 * @brief Frees the global state resources on application shutdown.
 */
static void on_shutdown(GtkApplication *app, gpointer user_data) {
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
}

// --- MAIN FUNCTION ---

int main(int argc, char **argv) {
    AdwApplication *app; // <-- CHANGED
    int status;

    // Allocate and initialize global state
    app_state = g_new0(AppState, 1);

    app = adw_application_new("com.gemini.debloater", G_APPLICATION_DEFAULT_FLAGS); // <-- CHANGED
    
    // Connect signals
    g_signal_connect(app, "activate", G_CALLBACK(activate), app_state);
    g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
