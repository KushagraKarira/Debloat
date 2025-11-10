#include <gtk/gtk.h>
#include <glib.h>
#include <gio/gio.h>

// --- STRUCTS AND DATA MANAGEMENT ---

// Structure to hold information about a single installed package
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
 * The action bar appears if at least one app is selected.
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

    // Toggle visibility based on selection count
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

    // Clear and load app list for the new device
    // In a real app, this would run "adb -s <id> shell pm list packages -3"
    // We will use a mock function for simplicity here.
    void update_app_list(AppState *state);
    update_app_list(state);
}


/**
 * @brief Handler for the Debloat button click.
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

    // 2. RUN ADB COMMANDS (synchronous execution for simplicity - use GSubprocess for real app)
    // NOTE: In a real GTK app, blocking the main thread with g_spawn_sync() is bad practice.
    // Use GSubprocess or g_spawn_async_with_pipes() and handle output/errors asynchronously.

    gtk_widget_set_sensitive(state->debloat_button, FALSE);
    gtk_label_set_text(GTK_LABEL(state->status_label), "Debloating... Please wait (UI will block momentarily).");
    g_usleep(250000); // Small visual delay

    gint success_count = 0;
    gint failure_count = 0;

    for (l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (item->is_selected) {
            gchar *stdout_buf = NULL;
            gchar *stderr_buf = NULL;
            gint status = -1;
            GError *error = NULL;

            // ADB command template (simulated or simplified for example)
            // Real Command: adb -s <device_id> shell pm uninstall -k --user 0 <package_name>
            gchar *argv[] = { 
                "adb", 
                // "-s", state->selected_device_id, // Uncomment for specific device targeting
                "shell", 
                "pm", 
                "uninstall", 
                "-k", 
                "--user", 
                "0", 
                item->package_name, 
                NULL 
            };

            // SIMULATED COMMAND SUCCESS/FAILURE
            // For a minimal working example, we'll simulate the result
            if (g_str_has_prefix(item->package_name, "com.mock.fail")) {
                failure_count++;
                g_print("Simulated Failure for: %s\n", item->package_name);
            } else {
                success_count++;
                g_print("Simulated Success for: %s\n", item->package_name);
            }
            
            // This is the actual synchronous execution call (commented out to avoid real adb dependency)
            /*
            if (g_spawn_sync(NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &stdout_buf, &stderr_buf, &status, &error)) {
                if (status == 0 && stdout_buf && g_str_has_prefix(stdout_buf, "Success")) {
                    success_count++;
                } else {
                    failure_count++;
                }
            } else {
                g_warning("Failed to run ADB for %s: %s\n", item->package_name, error->message);
                g_clear_error(&error);
                failure_count++;
            }
            g_free(stdout_buf);
            g_free(stderr_buf);
            */
            
            // Remove the package from the UI upon success (or simulated success)
            if (success_count > 0 && failure_count == 0) { // Simple check to remove immediately
                gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), item->row);
            }
        }
    }

    // 3. Update status and reset selection
    gchar *result_msg = g_strdup_printf(
        "Debloat process complete. Success: %d, Failures: %d.", 
        success_count, 
        failure_count
    );
    gtk_label_set_text(GTK_LABEL(state->status_label), result_msg);
    g_free(result_msg);
    g_free(selected_packages_str);

    // Clear the selection state in the remaining list items
    GList *current_l;
    for (current_l = state->app_list; current_l != NULL; current_l = current_l->next) {
        AppItem *item = (AppItem *)current_l->data;
        if (item->is_selected) {
            item->is_selected = FALSE;
        }
    }
    
    // Refresh UI state
    gtk_widget_set_sensitive(state->debloat_button, TRUE);
    update_action_bar_visibility(state);
}


// --- LIST POPULATION FUNCTIONS ---

/**
 * @brief Populates the Device List with mock data.
 * In a real app, this would run 'adb devices' and parse the output.
 */
static void update_device_list(AppState *state) {
    // Clear existing list
    GList *children, *l;
    children = gtk_widget_get_children(state->device_list_box);
    for (l = children; l != NULL; l = l->next) {
        gtk_list_box_remove(GTK_LIST_BOX(state->device_list_box), GTK_WIDGET(l->data));
    }
    g_list_free(children);

    // MOCK DEVICES - Replace with real ADB parsing logic
    const gchar *mock_devices[] = {
        "emulator-5554 (Pixel 4 XL)",
        "ZY322J5G5L (Motorola G8 Power)",
        "192.168.1.100:5555 (Remote Debugging)",
        NULL
    };

    const gchar *mock_ids[] = {
        "emulator-5554",
        "ZY322J5G5L",
        "192.168.1.100:5555",
        NULL
    };

    GtkListBoxRow *first_row = NULL;

    for (int i = 0; mock_devices[i] != NULL; i++) {
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new(mock_devices[i]);
        gtk_label_set_xalign(GTK_LABEL(label), 0.0); // Align left
        gtk_widget_set_margin_start(label, 10);
        gtk_widget_set_margin_end(label, 10);
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), label);

        // Store the device ID in the row's object data
        g_object_set_data(G_OBJECT(row), "device-id", g_strdup(mock_ids[i]));

        gtk_list_box_append(GTK_LIST_BOX(state->device_list_box), row);

        if (!first_row) {
            first_row = GTK_LIST_BOX_ROW(row);
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
 * In a real app, this would run 'adb shell pm list packages -3' and parse.
 */
static void update_app_list(AppState *state) {
    // Clear existing app list
    GList *children, *l;
    children = gtk_widget_get_children(state->app_list_box);
    for (l = children; l != NULL; l = l->next) {
        gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), GTK_WIDGET(l->data));
    }
    g_list_free(children);

    // Free internal state list
    g_list_free_full(state->app_list, (GDestroyNotify)app_item_free);
    state->app_list = NULL;

    // MOCK PACKAGES - Replace with real ADB parsing logic
    // Display Name, Package Name
    const gchar *mock_packages[][2] = {
        {"Chrome", "com.android.chrome"},
        {"Facebook Services", "com.facebook.services"},
        {"Google Maps", "com.google.android.apps.maps"},
        {"Carrier Services", "com.google.android.carriersetup"},
        {"Digital Wellbeing", "com.google.android.apps.wellbeing"},
        {"Mock Fail App", "com.mock.fail.app"},
        {"TikTok", "com.zhiliaoapp.musically"},
        {NULL, NULL}
    };

    for (int i = 0; mock_packages[i][0] != NULL; i++) {
        AppItem *item = g_new0(AppItem, 1);
        item->display_name = g_strdup(mock_packages[i][0]);
        item->package_name = g_strdup(mock_packages[i][1]);
        item->is_selected = FALSE;

        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

        // Checkbox
        GtkWidget *check = gtk_check_button_new();
        gtk_box_append(GTK_BOX(hbox), check);

        // Labels (Display Name and Package Name)
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        GtkWidget *display_label = gtk_label_new(item->display_name);
        GtkWidget *package_label = gtk_label_new(item->package_name);

        gtk_label_set_xalign(GTK_LABEL(display_label), 0.0);
        gtk_label_set_xalign(GTK_LABEL(package_label), 0.0);
        gtk_widget_set_opacity(package_label, 0.6); // Dim the package name

        gtk_box_append(GTK_BOX(vbox), display_label);
        gtk_box_append(GTK_BOX(vbox), package_label);
        gtk_box_append(GTK_BOX(hbox), vbox);

        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), hbox);
        gtk_list_box_append(GTK_LIST_BOX(state->app_list_box), row);

        // Store reference and connect signal
        item->row = row;
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

    // Main layout: Paned window for left menu (20%) and right content (80%)
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_window_set_child(GTK_WINDOW(state->window), paned);

    // --- LEFT PANE (Device Selection, 20%) ---
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *device_header = gtk_label_new("Connected Devices");
    gtk_widget_add_css_class(device_header, "title-4");
    gtk_box_append(GTK_BOX(left_box), device_header);

    state->device_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->device_list_box), GTK_SELECTION_SINGLE);
    g_signal_connect(state->device_list_box, "row-activated", G_CALLBACK(on_device_selected), state);

    GtkWidget *device_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(device_scrolled), state->device_list_box);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(device_scrolled), 200);

    gtk_widget_set_hexpand(state->device_list_box, TRUE);
    gtk_widget_set_vexpand(state->device_list_box, TRUE);

    gtk_paned_set_start_child(GTK_PANED(paned), left_box);
    gtk_paned_set_resize_start_child(GTK_PANED(paned), FALSE); // Device list width fixed to 20% by default
    gtk_paned_set_shrink_start_child(GTK_PANED(paned), FALSE);

    gtk_box_append(GTK_BOX(left_box), device_scrolled);


    // --- RIGHT PANE (App List, 80%) ---
    GtkWidget *right_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *app_header = gtk_label_new("Installed User/System Apps");
    gtk_widget_add_css_class(app_header, "title-4");
    gtk_box_append(GTK_BOX(right_vbox), app_header);
    
    state->app_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(state->app_list_box), GTK_SELECTION_NONE);

    GtkWidget *app_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(app_scrolled), state->app_list_box);
    gtk_box_append(GTK_BOX(right_vbox), app_scrolled);
    gtk_widget_set_vexpand(app_scrolled, TRUE);

    // --- BOTTOM ACTION BAR (Debloat) ---
    state->action_bar = gtk_action_bar_new();
    gtk_widget_set_visible(state->action_bar, FALSE); // Hidden by default

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

    // --- FINAL SETUP ---
    // Set initial paned position (approx 20% of 900px default width)
    gtk_paned_set_position(GTK_PANED(paned), 180); 

    // Load initial device list (which will trigger app list load)
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
