#include <gtk/gtk.h>
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
    GtkWidget *adb_debug_button; 
    gchar *selected_device_id;
    GList *app_list; // List of AppItem*
} AppState;

// Global state instance
static AppState *app_state = NULL;

// --- FUNCTION PROTOTYPES ---
static void update_app_list(AppState *state);
static void update_action_bar_visibility(AppState *state);


// --- PACKAGE DATA (Bloatware lists) ---
// Note: Mock Fail entries have been removed as we are now using real command execution.

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

    if (selected_count > 0 && state->selected_device_id && g_strcmp0(state->selected_device_id, "ADB_DEBUG") != 0) {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gchar *label_text = g_strdup_printf("Ready to debloat %d app(s) on device category '%s'.", selected_count, state->selected_device_id);
        gtk_label_set_text(GTK_LABEL(state->status_label), label_text);
        g_free(label_text);
        gtk_widget_set_sensitive(state->debloat_button, TRUE);
    } else if (selected_count > 0 && g_strcmp0(state->selected_device_id, "ADB_DEBUG") == 0) {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gchar *label_text = g_strdup_printf("Ready to debloat %d app(s) on connected device '%s'.", selected_count, state->selected_device_id);
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
 * @brief Handler for the About button.
 */
static void on_about_clicked(GtkButton *button, GtkApplicationWindow *parent) {
    GtkWidget *dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Android Debloater");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.6");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "© 2025 Kushagra Karira");
    
    const gchar *comments[] = {
        "A GTK4 application for debloating Android devices using ADB. Requires ADB installed and a connected device.",
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
    const gchar *device_id_data = g_object_get_data(G_OBJECT(row), "device-id");
    if (!device_id_data) return;

    // Only update selected_device_id if it's the ADB_DEBUG option
    if (g_strcmp0(device_id_data, "ADB_DEBUG") == 0) {
        if (state->selected_device_id) g_free(state->selected_device_id);
        state->selected_device_id = g_strdup("ADB_DEBUG");
    } else {
        // For manufacturer selection, we use the manufacturer name as the device ID mock
        if (state->selected_device_id) g_free(state->selected_device_id);
        state->selected_device_id = g_strdup(device_id_data);
    }

    g_print("Device category selected: %s\n", state->selected_device_id);
    
    // Disable the debloat button immediately until a package is selected
    gtk_widget_set_sensitive(state->debloat_button, FALSE);
    gtk_widget_set_visible(state->action_bar, FALSE);

    // Load app list for the new device category
    update_app_list(state);
}

/**
 * @brief Handler for the ADB Debug button click. Simulates finding and connecting to a device.
 */
static void on_adb_debug_clicked(GtkButton *button, AppState *state) {
    // 1. Simulate ADB command execution to find a device
    g_print("Executing: adb devices\n");

    // 2. Simulate finding a device ID
    // In a real app, this would be retrieved from the adb output. Here, we use a placeholder.
    const gchar *real_device_id = "REAL_DEVICE_ID"; 
    g_print("ADB Result: Device found: %s\n", real_device_id);

    // 3. Update the selected device ID to the "real" device ID
    if (state->selected_device_id) {
        g_free(state->selected_device_id);
    }
    state->selected_device_id = g_strdup(real_device_id);
    
    // 4. Update status message
    gchar *status_msg = g_strdup_printf("ADB Debug: Found and connected to device ID: %s. Selecting Samsung's bloatware list to debloat on this device.", real_device_id);
    gtk_label_set_text(GTK_LABEL(state->status_label), status_msg);
    g_free(status_msg);

    // 5. Select the Samsung mock row and reload apps
    GtkListBoxRow *samsung_row = NULL;
    GtkListBox *list_box = GTK_LIST_BOX(state->device_list_box);
    int i = 0;
    // Iterate through list box rows using gtk_list_box_get_row_at_index
    while (TRUE) {
        GtkListBoxRow *row = gtk_list_box_get_row_at_index(list_box, i++);
        if (!row) break; 
        
        const gchar *device_id_data = g_object_get_data(G_OBJECT(row), "device-id");
        if (device_id_data && g_strcmp0(device_id_data, "Samsung") == 0) {
            samsung_row = row;
            break;
        }
    }

    if (samsung_row) {
        gtk_list_box_select_row(GTK_LIST_BOX(state->device_list_box), samsung_row);
        // Manually call the selection handler to load the packages for 'Samsung'
        on_device_selected(GTK_LIST_BOX(state->device_list_box), samsung_row, state);
    }
    
    // Ensure the action bar visibility is correct based on new state
    update_action_bar_visibility(state);
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
                    gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), item->row);
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

    // Remove successful items from the internal app_list state
    GList *remove_l;
    for (remove_l = items_to_remove; remove_l != NULL; remove_l = remove_l->next) {
        AppItem *item = (AppItem *)remove_l->data;
        state->app_list = g_list_remove(state->app_list, item);
        app_item_free(item);
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

        // Select the row corresponding to the initial state->selected_device_id
        if (g_strcmp0(mock_ids[i], state->selected_device_id) == 0) {
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
    GtkListBoxRow *selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(state->device_list_box));
    const gchar *category_id = selected_row ? g_object_get_data(G_OBJECT(selected_row), "device-id") : NULL;

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
        item->row = row_widget;
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
    state->selected_device_id = g_strdup("Oppo"); 
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
    GtkWidget *device_header = gtk_label_new("Select Device Category");
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

    gtk_box_append(GTK_BOX(left_box), device_scrolled);

    // ADB Debug section
    GtkWidget *adb_frame = gtk_frame_new("ADB Connection");
    GtkWidget *adb_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    gtk_widget_set_margin_start(adb_vbox, 10);
    gtk_widget_set_margin_end(adb_vbox, 10);
    gtk_widget_set_margin_top(adb_vbox, 10);
    gtk_widget_set_margin_bottom(adb_vbox, 10);
    
    gtk_frame_set_child(GTK_FRAME(adb_frame), adb_vbox);
    gtk_box_append(GTK_BOX(left_box), adb_frame);

    gtk_widget_set_margin_start(adb_frame, 10);
    gtk_widget_set_margin_end(adb_frame, 10);
    gtk_widget_set_margin_top(adb_frame, 10);
    gtk_widget_set_margin_bottom(adb_frame, 10);

    state->adb_debug_button = gtk_button_new_with_label("Connect Real ADB Device");
    gtk_widget_add_css_class(state->adb_debug_button, "suggested-action");
    gtk_box_append(GTK_BOX(adb_vbox), state->adb_debug_button);
    g_signal_connect(state->adb_debug_button, "clicked", G_CALLBACK(on_adb_debug_clicked), state);


    gtk_paned_set_start_child(GTK_PANED(paned), left_box);
    gtk_paned_set_resize_start_child(GTK_PANED(paned), FALSE); 
    gtk_paned_set_shrink_start_child(GTK_PANED(paned), FALSE);


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

    state->status_label = gtk_label_new("Select a device and then select apps to debloat.");
    gtk_action_bar_pack_start(GTK_ACTION_BAR(state->action_bar), state->status_label);

    state->debloat_button = gtk_button_new_with_label("DEBLOAT SELECTED APPS");
    gtk_widget_add_css_class(state->debloat_button, "destructive-action");
    gtk_action_bar_pack_end(GTK_ACTION_BAR(state->action_bar), state->debloat_button);
    g_signal_connect(state->debloat_button, "clicked", G_CALLBACK(on_debloat_clicked), state);

    // Combine right content and action bar
    GtkWidget *right_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(right_container), right_vbox);
    gtk_box_append(GTK_BOX(right_container), state->action_bar);

    gtk_paned_set_end_child(GTK_PANED(paned), right_container);

    // Set initial paned position 
    gtk_paned_set_position(GTK_PANED(paned), 280); 

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
