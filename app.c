#include <gtk/gtk.h>
#include <adwaita.h>
#include <glib/glist.h>
#include <gio/gio.h>
#include <sys/wait.h>

// --- STRUCTS AND DATA MANAGEMENT ---

typedef enum {
    RISK_SAFE = 0,
    RISK_CAUTION = 1
} RiskLevel;

typedef struct {
    const gchar *display_name;
    const gchar *package_name;
    RiskLevel risk;
} PackageEntry;

typedef struct {
    gchar *package_name;
    gchar *display_name;
    RiskLevel risk;
    gboolean is_selected;
    gboolean is_installed_on_device;
    gchar *action_taken; // "uninstalled" or "disabled" when in Undo list
    GtkWidget *check_button;
    GtkWidget *install_badge;
    GtkWidget *main_list_row;
    GtkWidget *uninstalled_list_row;
} AppItem;

typedef struct {
    AdwApplicationWindow *window;
    GtkWidget *device_list_box;
    GtkWidget *app_list_box;
    GtkWidget *action_bar;
    GtkWidget *debloat_button;
    GtkWidget *status_label;
    GtkWidget *search_entry;
    GtkWidget *installed_only_toggle;

    // ADB Device selection
    GtkWidget *device_dropdown;
    GtkStringList *device_model;
    GtkWidget *device_status_label;

    // State variables
    gchar *selected_device_id;
    gchar *selected_manufacturer_id;
    GList *app_list;
    GHashTable *installed_packages_set; // gchar* package_name -> GINT_TO_POINTER(1)

    // "Undo" page state
    GList *uninstalled_app_list;
    GtkWidget *uninstalled_list_box;

    // "Debug" page widgets
    GtkWidget *debug_command_entry;
    GtkWidget *debug_run_button;
    GtkTextBuffer *debug_output_buffer;

    // Async batch debloat state
    gboolean is_debloating;
    GList *pending_debloat_queue;
    gint debloat_total;
    gint debloat_done;
    gint debloat_success;
    gint debloat_failed;
} AppState;

static AppState *app_state = NULL;

// --- FUNCTION PROTOTYPES ---
static void update_app_list(AppState *state);
static void update_action_bar_visibility(AppState *state);
static void populate_adb_devices_async(AppState *state);
static void on_adb_device_selected(GtkDropDown *dropdown, GParamSpec *pspec, AppState *state);
static void on_reinstall_clicked(GtkButton *button, AppItem *item);
static void add_row_to_uninstalled_list(AppState *state, AppItem *item);
static void on_about_clicked(GtkButton *button, GtkWindow *parent);
static void show_welcome_dialog(GtkWindow *parent);
static void on_run_debug_command_clicked(GtkButton *button, AppState *state);
static void on_refresh_devices_clicked(GtkButton *button, AppState *state);
static void detect_and_select_manufacturer_async(AppState *state);
static void query_installed_packages_async(AppState *state);
static void save_undo_state_for_device(AppState *state);
static void load_undo_state_for_device(AppState *state);
static void refresh_app_install_badges(AppState *state);
static void on_search_changed(GtkSearchEntry *entry, AppState *state);
static void start_next_debloat_item(AppState *state);

// --- PACKAGE DATA (Bloatware lists) ---

static const PackageEntry SAMSUNG_APPS[] = {
    {"Samsung Message", "com.samsung.android.messaging", RISK_SAFE},
    {"Windows Link", "com.microsoft.appmanager", RISK_SAFE},
    {"Samsung E-Commerce", "com.samsung.ecomm.global.in", RISK_SAFE},
    {"My Galaxy", "com.mygalaxy", RISK_SAFE},
    {"Smart Switch", "com.sec.android.easyMover", RISK_SAFE},
    {"OneDrive", "com.microsoft.skydrive", RISK_SAFE},
    {"Opera Max VPN", "com.opera.max.oem", RISK_SAFE},
    {"Samsung Apps Store", "com.sec.android.app.samsungapps", RISK_SAFE},
    {"Bixby Agent", "com.samsung.android.bixby.agent", RISK_SAFE},
    {"Facebook App Installer", "com.facebook.system", RISK_SAFE},
    {"Facebook App Manager", "com.facebook.appmanager", RISK_SAFE},
    {"Game Home", "com.samsung.android.game.gamehome", RISK_SAFE},
    {"S Voice", "com.samsung.android.svoice", RISK_SAFE},
    {"AppCloud GL", "com.aura.oobe.samsung.gl", RISK_SAFE},
    {"AppCloud", "com.aura.oobe.samsung", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry XIAOMI_APPS[] = {
    {"Mi Browser", "com.android.browser", RISK_SAFE},
    {"Mi Global Browser", "com.mi.globalbrowser", RISK_SAFE},
    {"Mi Wallet India", "com.mipay.wallet.in", RISK_SAFE},
    {"Mi Analytics", "com.miui.analytics", RISK_SAFE},
    {"Main System Advertising (MSA)", "com.miui.msa.global", RISK_SAFE},
    {"Mi Video", "com.miui.video", RISK_SAFE},
    {"Xiaomi Shop", "com.xiaomi.shop", RISK_SAFE},
    {"Xiaomi O2O", "com.xiaomi.o2o", RISK_SAFE},
    {"Xiaomi Pass", "com.xiaomi.pass", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry VIVO_APPS[] = {
    {"Vivo Browser", "com.vivo.browser", RISK_SAFE},
    {"Vivo App Store", "com.vivo.appstore", RISK_SAFE},
    {"BBK Cloud", "com.bbk.cloud", RISK_SAFE},
    {"Vivo Gallery", "com.vivo.gallery", RISK_SAFE},
    {"Vivo Weather", "com.vivo.weather", RISK_SAFE},
    {"Facebook System", "com.facebook.system", RISK_SAFE},
    {"Vivo Easy Share", "com.vivo.easyshare", RISK_SAFE},
    {"Vivo Hiboard", "com.vivo.hiboard", RISK_SAFE},
    {"MTK Logger", "com.mediatek.mtklogger", RISK_SAFE},
    {"Vivo Assistant", "com.vivo.assistant", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry TCL_APPS[] = {
    {"TCL Compass", "com.tcl.compass", RISK_SAFE},
    {"Demo Page", "com.tcl.demopage", RISK_SAFE},
    {"FM Radio", "com.tcl.fmradio", RISK_SAFE},
    {"Face Unlock", "com.tct.faceunlock", RISK_SAFE},
    {"Game Mode", "com.tct.gamemode", RISK_SAFE},
    {"TCL Music", "com.tct.music", RISK_SAFE},
    {"Privacy Mode", "com.tct.privacymode", RISK_SAFE},
    {"Retail Demo", "com.tct.retaildemo", RISK_SAFE},
    {"Smart Cloud", "com.tct.smart.cloud", RISK_SAFE},
    {"Smart Drive Mode", "com.tct.smart.drivemode", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry SONY_APPS[] = {
    {"TV SideView Video", "com.sony.tvsideview.videoph", RISK_SAFE},
    {"Creative effect", "com.sonyericsson.android.addoncamera.artfilter", RISK_SAFE},
    {"Usage Stats", "com.sonyericsson.idd.agent", RISK_SAFE},
    {"Backup/Restore", "com.sonyericsson.mtp.extension.backuprestore", RISK_SAFE},
    {"Sony Music", "com.sonyericsson.music", RISK_SAFE},
    {"Sony Themes", "com.sonymobile.themes.xperialoops2", RISK_SAFE},
    {"Xperia Lounge", "com.sonymobile.xperialounge.services", RISK_SAFE},
    {"Xperia Wallpaper", "com.sonymobile.xperiaxlivewallpaper", RISK_SAFE},
    {"Xperia Transfer", "com.sonymobile.xperiatransfermobile", RISK_SAFE},
    {"Sony Weather", "com.sonymobile.xperiaweather", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry REALME_APPS[] = {
    {"Android Setup", "com.google.android.setupwizard", RISK_CAUTION},
    {"DocVault", "com.os.docvault", RISK_SAFE},
    {"HeyTap Cloud", "com.heytap.cloud", RISK_SAFE},
    {"Music (Oppo)", "com.oppo.music", RISK_SAFE},
    {"Video (ColorOS)", "com.coloros.video", RISK_SAFE},
    {"Clone Phone", "com.coloros.backuprestore", RISK_SAFE},
    {"Feedback Toolkit", "com.oppo.logkit", RISK_SAFE},
    {"My Realme (User Center)", "com.heytap.usercenter", RISK_SAFE},
    {"Opera News", "com.opera.branding.news", RISK_SAFE},
    {"Play Movies & TV", "com.google.android.videos", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry OPPO_APPS[] = {
    {"Compass App", "com.coloros.compass2", RISK_SAFE},
    {"Video App", "com.coloros.video", RISK_SAFE},
    {"Photos App (Gallery)", "com.coloros.gallery3d", RISK_SAFE},
    {"Oppo Cloud", "com.coloros.cloud", RISK_SAFE},
    {"Find My Phone", "com.coloros.findmyphone", RISK_CAUTION},
    {"Game Space", "com.coloros.gamespace", RISK_SAFE},
    {"Oppo App Market", "com.oppo.market", RISK_SAFE},
    {"Oppo Music", "com.oppo.music", RISK_SAFE},
    {"Oppo Wallet", "com.coloros.wallet", RISK_SAFE},
    {"Find My Phone Client", "com.realme.findphone.client2", RISK_CAUTION},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry ONEPLUS_APPS[] = {
    {"Shot On OnePlus", "cn.oneplus.photos", RISK_SAFE},
    {"OnePlus Switch (Backup)", "com.oneplus.backuprestore", RISK_SAFE},
    {"Zen Mode", "com.oneplus.brickmode", RISK_SAFE},
    {"Game Space", "com.oneplus.gamespace", RISK_SAFE},
    {"Health Check", "com.oneplus.healthcheck", RISK_SAFE},
    {"Community Forum", "net.oneplus.forums", RISK_SAFE},
    {"Cricket Scores", "net.oneplus.opsports", RISK_SAFE},
    {"OnePlus Weather", "net.oneplus.weather", RISK_SAFE},
    {"OnePlus Widget", "net.oneplus.widget", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry NOKIA_APPS[] = {
    {"Data Go", "com.hmdglobal.datago", RISK_SAFE},
    {"My Phone Support", "com.hmdglobal.support", RISK_SAFE},
    {"APR Upload Service", "com.evenwell.AprUploadService", RISK_SAFE},
    {"Power Saving", "com.evenwell.PowerSaving", RISK_CAUTION},
    {"Smart Switch", "com.evenwell.smartswitch", RISK_SAFE},
    {"Traffic Monitor", "com.evenwell.trafficmonitor", RISK_SAFE},
    {"Weather App", "com.evenwell.Weather", RISK_SAFE},
    {"Weather Widget", "com.evenwell.weather.widget", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry MOTOROLA_APPS[] = {
    {"Lenovo ID", "com.lenovo.lsf.user", RISK_SAFE},
    {"Rescue Security", "com.lmi.motorola.rescuesecurity", RISK_SAFE},
    {"FM Radio", "com.motorola.android.fmradio", RISK_SAFE},
    {"OMA Provisioning", "com.motorola.android.provisioning", RISK_CAUTION},
    {"Face Unlock Agent", "com.motorola.faceunlocktrustagent", RISK_SAFE},
    {"Invisible Net", "com.motorola.invisiblenet", RISK_SAFE},
    {"Motosignature", "com.motorola.motosignature.app", RISK_SAFE},
    {"Carrier Provisioning", "com.motorola.omadm.service", RISK_CAUTION},
    {"System Package", "com.motorola.pgmsystem2", RISK_CAUTION},
    {"System Server", "com.motorola.systemserver", RISK_CAUTION},
    {"Moto Care", "com.motorola.motocare", RISK_SAFE},
    {"Moto Notification CCC", "com.motorola.ccc.notification", RISK_SAFE},
    {"Moto DIMO Telemetry", "com.motorola.dimo", RISK_SAFE},
    {"Moto Spaces", "com.motorola.spaces", RISK_SAFE},
    {"Moto AI Services", "com.motorola.aiservices", RISK_SAFE},
    {"Moto Security Hub", "com.motorola.securityhub", RISK_SAFE},
    {"Moto Browser/Partner Apps", "com.motorola.brapps", RISK_SAFE},
    {"Moto Game Mode", "com.motorola.gamemode", RISK_SAFE},
    {"InMobi Weather", "com.inmobi.weather", RISK_SAFE},
    {"Moto Hub (Gestures/Display)", "com.motorola.moto", RISK_CAUTION},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry LENOVO_APPS[] = {
    {"Lenovo ID", "com.lenovo.lsf.user", RISK_SAFE},
    {"Lenovo AnyShare", "com.lenovo.anyshare.gps", RISK_SAFE},
    {"Lenovo Launcher", "com.lenovo.launcher", RISK_CAUTION},
    {"Lenovo Tab/Device Center", "com.lenovo.tab.devicecenter", RISK_SAFE},
    {"Lenovo Smart Connect / Project Unity", "com.motorola.cn.projectunity", RISK_SAFE},
    {"Lenovo Bug Report", "com.lenovo.leos.appstore.bugreport", RISK_SAFE},
    {"Lenovo Feedback", "com.lenovo.feedback", RISK_SAFE},
    {"Lenovo Partner/Browser", "com.lenovo.browser.hd", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry ZTE_APPS[] = {
    {"ZTE Voice Assistant", "com.zte.assistant", RISK_SAFE},
    {"ZTE Weather", "com.zte.weather", RISK_SAFE},
    {NULL, NULL, RISK_SAFE}
};

static const PackageEntry JIO_APPS[] = {
    {"Remote Control Service", "com.communitake.remotecontrolservice", RISK_SAFE},
    {"Factory Monitor (STB)", "com.sdmc.factorymonitor", RISK_CAUTION},
    {"PVOD Update Service", "com.iwedia.pvodupdateservice", RISK_SAFE},
    {"Remote Care RIL (STB)", "com.jio.stbremotecare.ril", RISK_CAUTION},
    {"STB Ad Service", "com.jio.stbadservice", RISK_SAFE},
    {"Log Service (STB)", "com.rjil.jiostblogservice", RISK_SAFE},
    {"TIF Extn (STB)", "com.jio.stb.tifextn", RISK_CAUTION},
    {"TR069 Client", "insight.tr069.client", RISK_CAUTION},
    {"Managed Provisioning", "com.android.managedprovisioning", RISK_CAUTION},
    {NULL, NULL, RISK_SAFE}
};

// --- HELPER FUNCTIONS ---

static void app_item_free(AppItem *item) {
    if (!item) return;
    g_free(item->package_name);
    g_free(item->display_name);
    g_free(item->action_taken);
    g_free(item);
}

static gchar *get_device_undo_file_path(const gchar *device_id) {
    if (!device_id || *device_id == '\0') return NULL;
    gchar *safe_id = g_uri_escape_string(device_id, NULL, TRUE);
    gchar *dir_path = g_build_filename(g_get_user_data_dir(), "android-debloater", NULL);
    g_mkdir_with_parents(dir_path, 0700);
    gchar *filename = g_strdup_printf("undo_%s.ini", safe_id);
    gchar *full_path = g_build_filename(dir_path, filename, NULL);
    g_free(safe_id);
    g_free(dir_path);
    g_free(filename);
    return full_path;
}

static void save_undo_state_for_device(AppState *state) {
    if (!state->selected_device_id) return;
    gchar *path = get_device_undo_file_path(state->selected_device_id);
    if (!path) return;

    GKeyFile *kf = g_key_file_new();
    gint idx = 0;
    for (GList *l = state->uninstalled_app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        gchar *group = g_strdup_printf("item_%d", idx++);
        g_key_file_set_string(kf, group, "display_name", item->display_name ? item->display_name : "");
        g_key_file_set_string(kf, group, "package_name", item->package_name ? item->package_name : "");
        g_key_file_set_string(kf, group, "action_taken", item->action_taken ? item->action_taken : "uninstalled");
        g_free(group);
    }

    GError *error = NULL;
    if (!g_key_file_save_to_file(kf, path, &error)) {
        g_warning("Failed to save undo state: %s", error->message);
        g_error_free(error);
    }
    g_key_file_free(kf);
    g_free(path);
}

static void clear_uninstalled_list_ui_and_data(AppState *state) {
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->uninstalled_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->uninstalled_list_box), GTK_WIDGET(row));
    }
    g_list_free_full(state->uninstalled_app_list, (GDestroyNotify)app_item_free);
    state->uninstalled_app_list = NULL;
}

static void load_undo_state_for_device(AppState *state) {
    clear_uninstalled_list_ui_and_data(state);
    if (!state->selected_device_id) return;

    gchar *path = get_device_undo_file_path(state->selected_device_id);
    if (!path || !g_file_test(path, G_FILE_TEST_EXISTS)) {
        g_free(path);
        return;
    }

    GKeyFile *kf = g_key_file_new();
    GError *error = NULL;
    if (g_key_file_load_from_file(kf, path, G_KEY_FILE_NONE, &error)) {
        gsize len = 0;
        gchar **groups = g_key_file_get_groups(kf, &len);
        for (gsize i = 0; i < len; i++) {
            gchar *pkg = g_key_file_get_string(kf, groups[i], "package_name", NULL);
            gchar *disp = g_key_file_get_string(kf, groups[i], "display_name", NULL);
            gchar *act = g_key_file_get_string(kf, groups[i], "action_taken", NULL);
            if (pkg && *pkg != '\0') {
                AppItem *item = g_new0(AppItem, 1);
                item->package_name = pkg;
                item->display_name = disp ? disp : g_strdup(pkg);
                item->action_taken = act ? act : g_strdup("uninstalled");
                state->uninstalled_app_list = g_list_append(state->uninstalled_app_list, item);
                add_row_to_uninstalled_list(state, item);
            } else {
                g_free(pkg);
                g_free(disp);
                g_free(act);
            }
        }
        g_strfreev(groups);
    } else if (error) {
        g_error_free(error);
    }

    g_key_file_free(kf);
    g_free(path);
}

static void update_action_bar_visibility(AppState *state) {
    if (state->is_debloating) {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gtk_widget_set_sensitive(state->debloat_button, FALSE);
        return;
    }

    gint selected_count = 0;
    for (GList *l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (item->is_selected) {
            selected_count++;
        }
    }

    if (selected_count > 0 && state->selected_device_id != NULL) {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gchar *label_text = g_strdup_printf(
            "Ready to remove %d app(s) on '%s'.",
            selected_count,
            state->selected_device_id
        );
        gtk_label_set_text(GTK_LABEL(state->status_label), label_text);
        g_free(label_text);
        gtk_widget_set_sensitive(state->debloat_button, TRUE);
    } else if (state->selected_device_id == NULL) {
        gtk_widget_set_visible(state->action_bar, FALSE);
    } else {
        gtk_widget_set_visible(state->action_bar, TRUE);
        gtk_label_set_text(GTK_LABEL(state->status_label), "Select apps from the list to remove.");
        gtk_widget_set_sensitive(state->debloat_button, FALSE);
    }
}

// --- ASYNC ADB RUNNER HELPER ---

typedef struct {
    GTask *task;
    GSubprocess *proc;
} SubprocessRunCtx;

static void on_subprocess_communicated(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GSubprocess *proc = G_SUBPROCESS(source_object);
    SubprocessRunCtx *ctx = (SubprocessRunCtx *)user_data;
    gchar *stdout_buf = NULL;
    gchar *stderr_buf = NULL;
    GError *error = NULL;

    g_subprocess_communicate_utf8_finish(proc, res, &stdout_buf, &stderr_buf, &error);
    if (error) {
        g_task_return_error(ctx->task, error);
    } else {
        gchar *combined = g_strdup_printf(
            "%d\n%s\x1e%s",
            g_subprocess_get_exit_status(proc),
            stdout_buf ? stdout_buf : "",
            stderr_buf ? stderr_buf : ""
        );
        g_task_return_pointer(ctx->task, combined, g_free);
    }

    g_free(stdout_buf);
    g_free(stderr_buf);
    g_object_unref(ctx->task);
    g_object_unref(proc);
    g_free(ctx);
}

static void run_adb_async(
    const gchar *const *argv,
    GAsyncReadyCallback callback,
    gpointer user_data
) {
    GTask *task = g_task_new(NULL, NULL, callback, user_data);
    GError *error = NULL;
    GSubprocess *proc = g_subprocess_newv(
        argv,
        G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE,
        &error
    );
    if (error) {
        g_task_return_error(task, error);
        g_object_unref(task);
        return;
    }

    SubprocessRunCtx *ctx = g_new0(SubprocessRunCtx, 1);
    ctx->task = task;
    ctx->proc = proc;
    g_subprocess_communicate_utf8_async(proc, NULL, NULL, on_subprocess_communicated, ctx);
}

static gboolean parse_adb_result(
    GObject *source_object,
    GAsyncResult *res,
    gint *out_exit_status,
    gchar **out_stdout,
    gchar **out_stderr,
    GError **error
) {
    (void)source_object;
    gchar *packed = g_task_propagate_pointer(G_TASK(res), error);
    if (!packed) return FALSE;

    gchar *sep = strchr(packed, '\x1e');
    gchar *nl = strchr(packed, '\n');
    if (out_exit_status && nl) {
        *out_exit_status = atoi(packed);
    }
    if (sep && nl && sep > nl) {
        *sep = '\0';
        if (out_stdout) *out_stdout = g_strdup(nl + 1);
        if (out_stderr) *out_stderr = g_strdup(sep + 1);
    } else {
        if (out_stdout) *out_stdout = g_strdup(packed);
        if (out_stderr) *out_stderr = g_strdup("");
    }
    g_free(packed);
    return TRUE;
}

// --- CALLBACKS ---

static void show_welcome_dialog(GtkWindow *parent) {
    AdwDialog *dialog = adw_alert_dialog_new("Welcome to Android Debloater!", NULL);
    adw_alert_dialog_add_response(ADW_ALERT_DIALOG(dialog), "ok", "Got it!");
    adw_alert_dialog_set_default_response(ADW_ALERT_DIALOG(dialog), "ok");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 12);
    gtk_widget_set_margin_bottom(box, 12);

    const gchar *instructions =
        "To use this tool, you must enable USB Debugging (ADB) on your device.\n\n"
        "1. Go to Settings > About Phone.\n"
        "2. Tap on Build Number 7 times until you see 'You are now a developer!'.\n"
        "3. Go back to Settings > System > Developer options.\n"
        "4. Find and turn on USB debugging.\n"
        "5. Connect your phone to your computer via USB.\n"
        "6. Allow 'USB debugging?' when prompted on your phone.";

    GtkWidget *label = gtk_label_new(instructions);
    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    gtk_box_append(GTK_BOX(box), label);

    adw_alert_dialog_set_extra_child(ADW_ALERT_DIALOG(dialog), box);
    adw_alert_dialog_choose(ADW_ALERT_DIALOG(dialog), GTK_WIDGET(parent), NULL, NULL, NULL);
}

static void on_about_clicked(GtkButton *button, GtkWindow *parent) {
    (void)button;
    AdwDialog *dialog = adw_about_dialog_new();
    adw_about_dialog_set_application_name(ADW_ABOUT_DIALOG(dialog), "Android Debloater");
    adw_about_dialog_set_version(ADW_ABOUT_DIALOG(dialog), "0.8.0 (Libadwaita)");
    adw_about_dialog_set_developer_name(ADW_ABOUT_DIALOG(dialog), "Kushagra Karira");
    adw_about_dialog_set_copyright(ADW_ABOUT_DIALOG(dialog), "GPL-3.0 2019-2026 Kushagra Karira");
    adw_about_dialog_set_comments(ADW_ABOUT_DIALOG(dialog), "A GTK4/Libadwaita application for debloating Android devices using ADB.");
    adw_about_dialog_set_website(ADW_ABOUT_DIALOG(dialog), "https://kushagrakarira.com");
    adw_about_dialog_set_issue_url(ADW_ABOUT_DIALOG(dialog), "https://github.com/KushagraKarira/Debloat/issues");
    adw_about_dialog_add_link(ADW_ABOUT_DIALOG(dialog), "Project Link", "https://github.com/KushagraKarira/Debloat");
    adw_dialog_present(dialog, GTK_WIDGET(parent));
}

static void on_app_toggled(GtkCheckButton *check_button, AppItem *item) {
    item->is_selected = gtk_check_button_get_active(check_button);
    update_action_bar_visibility(app_state);
}

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

static void on_installed_only_toggled(GtkToggleButton *toggle, AppState *state) {
    (void)toggle;
    on_search_changed(GTK_SEARCH_ENTRY(state->search_entry), state);
}

static void on_search_changed(GtkSearchEntry *entry, AppState *state) {
    const gchar *search_text = entry ? gtk_editable_get_text(GTK_EDITABLE(entry)) : "";
    gchar *lower_search = search_text ? g_utf8_strdown(search_text, -1) : g_strdup("");
    gboolean installed_only = state->installed_only_toggle &&
                              gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(state->installed_only_toggle)) &&
                              state->installed_packages_set != NULL;

    for (GList *l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        gboolean visible = TRUE;

        if (installed_only && !item->is_installed_on_device) {
            visible = FALSE;
        }

        if (visible && strlen(lower_search) > 0) {
            gchar *lower_display = g_utf8_strdown(item->display_name, -1);
            gchar *lower_pkg = g_utf8_strdown(item->package_name, -1);
            visible = (strstr(lower_display, lower_search) != NULL) ||
                      (strstr(lower_pkg, lower_search) != NULL);
            g_free(lower_display);
            g_free(lower_pkg);
        }

        gtk_widget_set_visible(item->main_list_row, visible);
    }

    g_free(lower_search);
    update_action_bar_visibility(state);
}

static void on_device_selected(GtkListBox *list_box, GtkListBoxRow *row, AppState *state) {
    (void)list_box;
    if (!row) return;

    const gchar *manufacturer_id = (const gchar *)g_object_get_data(G_OBJECT(row), "device-id");
    if (!manufacturer_id) return;

    if (g_strcmp0(state->selected_manufacturer_id, manufacturer_id) == 0) {
        return;
    }

    g_free(state->selected_manufacturer_id);
    state->selected_manufacturer_id = g_strdup(manufacturer_id);

    gtk_widget_set_visible(state->action_bar, FALSE);
    update_app_list(state);
}

static void on_manufacturer_detected_done(GObject *source, GAsyncResult *res, gpointer user_data) {
    AppState *state = (AppState *)user_data;
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    GError *error = NULL;

    if (parse_adb_result(source, res, NULL, &stdout_str, &stderr_str, &error) && stdout_str) {
        gchar *mfg = g_strstrip(g_ascii_strdown(stdout_str, -1));
        const gchar *matched_id = NULL;

        if (strstr(mfg, "samsung")) matched_id = "Samsung";
        else if (strstr(mfg, "xiaomi") || strstr(mfg, "redmi") || strstr(mfg, "poco")) matched_id = "Xiaomi";
        else if (strstr(mfg, "oneplus")) matched_id = "OnePlus";
        else if (strstr(mfg, "oppo")) matched_id = "Oppo";
        else if (strstr(mfg, "realme")) matched_id = "RealMe";
        else if (strstr(mfg, "vivo") || strstr(mfg, "iqoo")) matched_id = "Vivo";
        else if (strstr(mfg, "sony")) matched_id = "Sony";
        else if (strstr(mfg, "motorola")) matched_id = "Motorola";
        else if (strstr(mfg, "lenovo")) matched_id = "Lenovo";
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

static void detect_and_select_manufacturer_async(AppState *state) {
    if (!state->selected_device_id) return;
    const gchar *argv[] = {
        "adb", "-s", state->selected_device_id, "shell", "getprop", "ro.product.manufacturer", NULL
    };
    run_adb_async(argv, on_manufacturer_detected_done, state);
}

static void refresh_app_install_badges(AppState *state) {
    for (GList *l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (state->installed_packages_set) {
            item->is_installed_on_device = g_hash_table_contains(
                state->installed_packages_set, item->package_name
            );
            if (item->install_badge) {
                gtk_label_set_text(
                    GTK_LABEL(item->install_badge),
                    item->is_installed_on_device ? "Installed" : "Not found"
                );
                gtk_widget_set_visible(item->install_badge, TRUE);
            }
        } else {
            item->is_installed_on_device = FALSE;
            if (item->install_badge) {
                gtk_widget_set_visible(item->install_badge, FALSE);
            }
        }
    }
    on_search_changed(GTK_SEARCH_ENTRY(state->search_entry), state);
}

static void on_installed_packages_queried(GObject *source, GAsyncResult *res, gpointer user_data) {
    AppState *state = (AppState *)user_data;
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    GError *error = NULL;

    if (parse_adb_result(source, res, NULL, &stdout_str, &stderr_str, &error) && stdout_str) {
        if (state->installed_packages_set) {
            g_hash_table_destroy(state->installed_packages_set);
        }
        state->installed_packages_set = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

        gchar **lines = g_strsplit(stdout_str, "\n", -1);
        for (int i = 0; lines[i] != NULL; i++) {
            gchar *line = g_strstrip(lines[i]);
            if (g_str_has_prefix(line, "package:")) {
                const gchar *pkg = line + strlen("package:");
                if (*pkg != '\0') {
                    g_hash_table_add(state->installed_packages_set, g_strdup(pkg));
                }
            }
        }
        g_strfreev(lines);
        refresh_app_install_badges(state);
    }

    if (error) g_error_free(error);
    g_free(stdout_str);
    g_free(stderr_str);
}

static void query_installed_packages_async(AppState *state) {
    if (!state->selected_device_id) {
        if (state->installed_packages_set) {
            g_hash_table_destroy(state->installed_packages_set);
            state->installed_packages_set = NULL;
        }
        refresh_app_install_badges(state);
        return;
    }

    const gchar *argv[] = {
        "adb", "-s", state->selected_device_id, "shell", "pm", "list", "packages", "--user", "0", NULL
    };
    run_adb_async(argv, on_installed_packages_queried, state);
}

static void on_adb_devices_populated(GObject *source, GAsyncResult *res, gpointer user_data) {
    AppState *state = (AppState *)user_data;
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    GError *error = NULL;
    gint devices_found = 0;
    gint unauthorized_found = 0;

    if (!parse_adb_result(source, res, NULL, &stdout_str, &stderr_str, &error)) {
        gtk_label_set_text(GTK_LABEL(state->device_status_label), "Error: 'adb' command not found in PATH.");
        if (error) g_error_free(error);
        g_free(stdout_str);
        g_free(stderr_str);
        return;
    }

    if (stdout_str) {
        gchar **lines = g_strsplit(stdout_str, "\n", -1);
        for (int i = 1; lines[i] != NULL; i++) {
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
        status_msg = g_strdup_printf("Found %d unauthorized device(s). Allow USB Debugging on phone.", unauthorized_found);
    } else {
        status_msg = g_strdup("No devices found. Connect via USB and refresh.");
    }
    gtk_label_set_text(GTK_LABEL(state->device_status_label), status_msg);
    g_free(status_msg);

    g_free(stdout_str);
    g_free(stderr_str);
    update_action_bar_visibility(state);
}

static void populate_adb_devices_async(AppState *state) {
    gtk_label_set_text(GTK_LABEL(state->device_status_label), "Refreshing device list...");

    guint n_items = g_list_model_get_n_items(G_LIST_MODEL(state->device_model));
    if (n_items > 1) {
        gtk_string_list_splice(state->device_model, 1, n_items - 1, NULL);
    }

    gtk_drop_down_set_selected(GTK_DROP_DOWN(state->device_dropdown), 0);
    g_free(state->selected_device_id);
    state->selected_device_id = NULL;

    if (state->installed_packages_set) {
        g_hash_table_destroy(state->installed_packages_set);
        state->installed_packages_set = NULL;
    }
    refresh_app_install_badges(state);
    clear_uninstalled_list_ui_and_data(state);

    const gchar *argv[] = {"adb", "devices", NULL};
    run_adb_async(argv, on_adb_devices_populated, state);
}

static void on_adb_device_selected(GtkDropDown *dropdown, GParamSpec *pspec, AppState *state) {
    (void)pspec;
    guint pos = gtk_drop_down_get_selected(dropdown);

    g_free(state->selected_device_id);
    state->selected_device_id = NULL;

    if (pos > 0) {
        const gchar *device_id = gtk_string_list_get_string(state->device_model, pos);
        state->selected_device_id = g_strdup(device_id);
        gtk_label_set_text(GTK_LABEL(state->device_status_label), "");
        detect_and_select_manufacturer_async(state);
        query_installed_packages_async(state);
        load_undo_state_for_device(state);
    } else {
        gtk_label_set_text(GTK_LABEL(state->device_status_label), "No device selected.");
        if (state->installed_packages_set) {
            g_hash_table_destroy(state->installed_packages_set);
            state->installed_packages_set = NULL;
        }
        refresh_app_install_badges(state);
        clear_uninstalled_list_ui_and_data(state);
    }

    update_action_bar_visibility(state);
}

static void on_refresh_devices_clicked(GtkButton *button, AppState *state) {
    (void)button;
    populate_adb_devices_async(state);
}

typedef struct {
    GtkButton *button;
    AppItem *item;
    AppState *state;
} ReinstallCtx;

static void on_restore_enable_done(GObject *source, GAsyncResult *res, gpointer user_data) {
    ReinstallCtx *ctx = (ReinstallCtx *)user_data;
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    GError *error = NULL;
    gboolean ok = FALSE;

    if (parse_adb_result(source, res, NULL, &stdout_str, &stderr_str, &error) && stdout_str) {
        gchar *s = g_strstrip(stdout_str);
        if (g_str_has_prefix(s, "Package") || g_str_has_prefix(s, "Enabled")) {
            ok = TRUE;
        }
    }

    if (ok) {
        gtk_list_box_remove(GTK_LIST_BOX(ctx->state->uninstalled_list_box), ctx->item->uninstalled_list_row);
        ctx->state->uninstalled_app_list = g_list_remove(ctx->state->uninstalled_app_list, ctx->item);
        save_undo_state_for_device(ctx->state);
        query_installed_packages_async(ctx->state);
        app_item_free(ctx->item);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(ctx->button), TRUE);
    }

    if (error) g_error_free(error);
    g_free(stdout_str);
    g_free(stderr_str);
    g_free(ctx);
}

static void on_reinstall_clicked(GtkButton *button, AppItem *item) {
    if (!app_state || !app_state->selected_device_id) return;
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);

    ReinstallCtx *ctx = g_new0(ReinstallCtx, 1);
    ctx->button = button;
    ctx->item = item;
    ctx->state = app_state;

    if (g_strcmp0(item->action_taken, "disabled") == 0) {
        const gchar *argv[] = {
            "adb", "-s", app_state->selected_device_id,
            "shell", "pm", "enable", "--user", "0", item->package_name, NULL
        };
        run_adb_async(argv, on_restore_enable_done, ctx);
    } else {
        const gchar *argv[] = {
            "adb", "-s", app_state->selected_device_id,
            "shell", "cmd", "package", "install-existing", item->package_name, NULL
        };
        run_adb_async(argv, on_restore_enable_done, ctx);
    }
}

static void add_row_to_uninstalled_list(AppState *state, AppItem *item) {
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_start(hbox, 12);
    gtk_widget_set_margin_end(hbox, 12);
    gtk_widget_set_margin_top(hbox, 8);
    gtk_widget_set_margin_bottom(hbox, 8);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    GtkWidget *display_label = gtk_label_new(item->display_name);
    gchar *sub = g_strdup_printf("%s  •  %s",
        item->package_name,
        g_strcmp0(item->action_taken, "disabled") == 0 ? "Disabled" : "Uninstalled"
    );
    GtkWidget *package_label = gtk_label_new(sub);
    g_free(sub);

    gtk_label_set_xalign(GTK_LABEL(display_label), 0.0);
    gtk_label_set_xalign(GTK_LABEL(package_label), 0.0);
    gtk_widget_add_css_class(package_label, "caption");
    gtk_widget_set_opacity(package_label, 0.75);

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

typedef struct {
    AppState *state;
    AppItem *item;
} DebloatStepCtx;

static void finish_debloat_item_success(AppState *state, AppItem *item, const gchar *mode) {
    state->debloat_success++;
    state->app_list = g_list_remove(state->app_list, item);
    gtk_list_box_remove(GTK_LIST_BOX(state->app_list_box), item->main_list_row);

    item->is_selected = FALSE;
    item->action_taken = g_strdup(mode);
    state->uninstalled_app_list = g_list_append(state->uninstalled_app_list, item);
    add_row_to_uninstalled_list(state, item);
    save_undo_state_for_device(state);

    if (state->installed_packages_set) {
        g_hash_table_remove(state->installed_packages_set, item->package_name);
    }
}

static void on_disable_fallback_done(GObject *source, GAsyncResult *res, gpointer user_data) {
    DebloatStepCtx *ctx = (DebloatStepCtx *)user_data;
    AppState *state = ctx->state;
    AppItem *item = ctx->item;
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    GError *error = NULL;
    gboolean disabled_ok = FALSE;

    if (parse_adb_result(source, res, NULL, &stdout_str, &stderr_str, &error) && stdout_str) {
        gchar *s = g_ascii_strdown(g_strstrip(stdout_str), -1);
        if (strstr(s, "disabled") != NULL) {
            disabled_ok = TRUE;
        }
        g_free(s);
    }

    if (disabled_ok) {
        finish_debloat_item_success(state, item, "disabled");
    } else {
        state->debloat_failed++;
    }

    if (error) g_error_free(error);
    g_free(stdout_str);
    g_free(stderr_str);
    g_free(ctx);

    state->debloat_done++;
    start_next_debloat_item(state);
}

static void on_uninstall_step_done(GObject *source, GAsyncResult *res, gpointer user_data) {
    DebloatStepCtx *ctx = (DebloatStepCtx *)user_data;
    AppState *state = ctx->state;
    AppItem *item = ctx->item;
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    GError *error = NULL;
    gboolean uninstalled_ok = FALSE;

    if (parse_adb_result(source, res, NULL, &stdout_str, &stderr_str, &error) && stdout_str) {
        gchar *s = g_strstrip(stdout_str);
        if (g_str_has_prefix(s, "Success")) {
            uninstalled_ok = TRUE;
        }
    }

    if (error) {
        g_error_free(error);
        g_free(stdout_str);
        g_free(stderr_str);
        state->debloat_failed++;
        state->debloat_done++;
        g_free(ctx);
        start_next_debloat_item(state);
        return;
    }

    g_free(stdout_str);
    g_free(stderr_str);

    if (uninstalled_ok) {
        finish_debloat_item_success(state, item, "uninstalled");
        state->debloat_done++;
        g_free(ctx);
        start_next_debloat_item(state);
    } else {
        // Fallback to pm disable-user --user 0
        const gchar *argv[] = {
            "adb", "-s", state->selected_device_id,
            "shell", "pm", "disable-user", "--user", "0", item->package_name, NULL
        };
        run_adb_async(argv, on_disable_fallback_done, ctx);
    }
}

static void start_next_debloat_item(AppState *state) {
    if (!state->pending_debloat_queue || !state->selected_device_id) {
        g_list_free(state->pending_debloat_queue);
        state->pending_debloat_queue = NULL;
        state->is_debloating = FALSE;

        gchar *done_msg = g_strdup_printf(
            "Done: %d removed/disabled, %d failed.",
            state->debloat_success,
            state->debloat_failed
        );
        gtk_label_set_text(GTK_LABEL(state->status_label), done_msg);
        g_free(done_msg);
        update_action_bar_visibility(state);
        return;
    }

    AppItem *item = (AppItem *)state->pending_debloat_queue->data;
    GList *next = state->pending_debloat_queue->next;
    g_list_free_1(state->pending_debloat_queue);
    state->pending_debloat_queue = next;

    gchar *prog = g_strdup_printf(
        "Removing %d/%d: %s...",
        state->debloat_done + 1,
        state->debloat_total,
        item->display_name
    );
    gtk_label_set_text(GTK_LABEL(state->status_label), prog);
    g_free(prog);

    DebloatStepCtx *ctx = g_new0(DebloatStepCtx, 1);
    ctx->state = state;
    ctx->item = item;

    const gchar *argv[] = {
        "adb", "-s", state->selected_device_id,
        "shell", "pm", "uninstall", "--user", "0", item->package_name, NULL
    };
    run_adb_async(argv, on_uninstall_step_done, ctx);
}

static void begin_confirmed_debloat(AppState *state) {
    if (state->is_debloating || !state->selected_device_id) return;

    GList *queue = NULL;
    gint caution_count = 0;
    for (GList *l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (item->is_selected) {
            queue = g_list_append(queue, item);
            if (item->risk == RISK_CAUTION) caution_count++;
        }
    }

    if (!queue) return;
    (void)caution_count;

    state->is_debloating = TRUE;
    state->pending_debloat_queue = queue;
    state->debloat_total = (gint)g_list_length(queue);
    state->debloat_done = 0;
    state->debloat_success = 0;
    state->debloat_failed = 0;

    gtk_widget_set_visible(state->action_bar, TRUE);
    gtk_widget_set_sensitive(state->debloat_button, FALSE);
    start_next_debloat_item(state);
}

static void on_debloat_confirm_response(GObject *source, GAsyncResult *res, gpointer user_data) {
    AdwAlertDialog *dialog = ADW_ALERT_DIALOG(source);
    AppState *state = (AppState *)user_data;
    const gchar *resp = adw_alert_dialog_choose_finish(dialog, res);
    if (g_strcmp0(resp, "confirm") == 0) {
        begin_confirmed_debloat(state);
    }
}

static void on_debloat_clicked(GtkButton *button, AppState *state) {
    (void)button;
    if (state->is_debloating || !state->selected_device_id) return;

    gint count = 0;
    gint caution_count = 0;
    for (GList *l = state->app_list; l != NULL; l = l->next) {
        AppItem *item = (AppItem *)l->data;
        if (item->is_selected) {
            count++;
            if (item->risk == RISK_CAUTION) caution_count++;
        }
    }
    if (count == 0) return;

    gchar *body = caution_count > 0
        ? g_strdup_printf(
            "Remove %d selected package(s) on '%s' for user 0?\n\n"
            "Warning: %d package(s) are marked 'Caution' (launcher, provisioning, or hub components). "
            "Protected packages will fall back to 'pm disable-user --user 0'.",
            count, state->selected_device_id, caution_count
          )
        : g_strdup_printf(
            "Remove %d selected package(s) on '%s' for user 0?\n\n"
            "Protected packages will fall back to 'pm disable-user --user 0', and you can restore them from the Undo tab.",
            count, state->selected_device_id
          );

    AdwDialog *dialog = adw_alert_dialog_new("Confirm Debloat?", body);
    g_free(body);

    adw_alert_dialog_add_response(ADW_ALERT_DIALOG(dialog), "cancel", "Cancel");
    adw_alert_dialog_add_response(ADW_ALERT_DIALOG(dialog), "confirm", "Remove");
    adw_alert_dialog_set_response_appearance(ADW_ALERT_DIALOG(dialog), "confirm", ADW_RESPONSE_DESTRUCTIVE);
    adw_alert_dialog_set_default_response(ADW_ALERT_DIALOG(dialog), "cancel");
    adw_alert_dialog_set_close_response(ADW_ALERT_DIALOG(dialog), "cancel");

    adw_alert_dialog_choose(ADW_ALERT_DIALOG(dialog), GTK_WIDGET(state->window), NULL, on_debloat_confirm_response, state);
}

static void on_debug_command_done(GObject *source, GAsyncResult *res, gpointer user_data) {
    AppState *state = (AppState *)user_data;
    gchar *stdout_str = NULL;
    gchar *stderr_str = NULL;
    gint exit_status = 0;
    GError *error = NULL;

    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(state->debug_output_buffer, &iter);

    if (!parse_adb_result(source, res, &exit_status, &stdout_str, &stderr_str, &error)) {
        gchar *err_msg = g_strdup_printf("--- EXECUTION FAILED ---\n%s\n", error ? error->message : "Unknown error");
        gtk_text_buffer_insert(state->debug_output_buffer, &iter, err_msg, -1);
        g_free(err_msg);
        if (error) g_error_free(error);
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
    if (state->debug_run_button) {
        gtk_widget_set_sensitive(state->debug_run_button, TRUE);
    }
}

static void on_run_debug_command_clicked(GtkButton *button, AppState *state) {
    (void)button;
    if (!state->selected_device_id) {
        gtk_text_buffer_set_text(state->debug_output_buffer, "Error: Select a connected device on the 'Debloat' tab first.", -1);
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
    cmd_args[0] = g_strdup("adb");
    cmd_args[1] = g_strdup("-s");
    cmd_args[2] = g_strdup(state->selected_device_id);
    cmd_args[3] = g_strdup("shell");
    for (int i = 0; i < user_cmd_len; i++) {
        cmd_args[4 + i] = g_strdup(user_cmd_parts[i]);
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

    if (state->debug_run_button) {
        gtk_widget_set_sensitive(state->debug_run_button, FALSE);
    }

    run_adb_async((const gchar *const *)cmd_args, on_debug_command_done, state);

    g_strfreev(cmd_args);
    g_strfreev(user_cmd_parts);
}

static void on_stack_page_changed(GtkStack *stack, GParamSpec *pspec, AppState *state) {
    (void)pspec;
    const gchar *visible_name = gtk_stack_get_visible_child_name(stack);
    gboolean on_debloat = (g_strcmp0(visible_name, "debloat") == 0);
    if (state->search_entry) {
        gtk_search_entry_set_key_capture_widget(
            GTK_SEARCH_ENTRY(state->search_entry),
            on_debloat ? GTK_WIDGET(state->window) : NULL
        );
    }
}

// --- LIST POPULATION FUNCTIONS ---

static void update_device_list(AppState *state) {
    GtkListBoxRow *row;
    while ((row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(state->device_list_box), 0))) {
        gtk_list_box_remove(GTK_LIST_BOX(state->device_list_box), GTK_WIDGET(row));
    }

    const gchar *mock_devices[] = {
        "Samsung (Galaxy Devices)", "Xiaomi (Mi/Redmi Devices)", "OnePlus (Oxygen OS)",
        "Oppo (ColorOS)", "RealMe (ColorOS/RealMe UI)", "Vivo (Funtouch OS)",
        "Sony (Xperia Devices)", "Motorola (Moto Devices)", "Lenovo (Tablets/Phones)",
        "Nokia (HMD Devices)", "TCL (Alcatel/TCL Devices)", "ZTE (Nubia/Axon Devices)",
        "Jio (STB/Phone)", NULL
    };

    const gchar *mock_ids[] = {
        "Samsung", "Xiaomi", "OnePlus", "Oppo", "RealMe", "Vivo",
        "Sony", "Motorola", "Lenovo", "Nokia", "TCL", "ZTE",
        "Jio", NULL
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
        g_free(state->selected_manufacturer_id);
        state->selected_manufacturer_id = NULL;
        on_device_selected(GTK_LIST_BOX(state->device_list_box), initial_row, state);
    }
}

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
        else if (g_strcmp0(category_id, "Lenovo") == 0) packages_to_load = LENOVO_APPS;
        else if (g_strcmp0(category_id, "ZTE") == 0) packages_to_load = ZTE_APPS;
        else if (g_strcmp0(category_id, "Jio") == 0) packages_to_load = JIO_APPS;
    }

    if (!packages_to_load) {
        gtk_label_set_text(GTK_LABEL(state->device_status_label), "Select a manufacturer to view packages.");
        return;
    }

    for (int i = 0; packages_to_load[i].display_name != NULL; i++) {
        AppItem *item = g_new0(AppItem, 1);
        item->display_name = g_strdup(packages_to_load[i].display_name);
        item->package_name = g_strdup(packages_to_load[i].package_name);
        item->risk = packages_to_load[i].risk;
        item->is_selected = FALSE;

        GtkWidget *row_widget = gtk_list_box_row_new();
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
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
        gtk_widget_set_hexpand(vbox, TRUE);
        GtkWidget *display_label = gtk_label_new(item->display_name);
        GtkWidget *package_label = gtk_label_new(item->package_name);

        gtk_label_set_xalign(GTK_LABEL(display_label), 0.0);
        gtk_label_set_xalign(GTK_LABEL(package_label), 0.0);
        gtk_widget_add_css_class(package_label, "caption");
        gtk_widget_set_opacity(package_label, 0.75);

        gtk_box_append(GTK_BOX(vbox), display_label);
        gtk_box_append(GTK_BOX(vbox), package_label);
        gtk_box_append(GTK_BOX(hbox), vbox);

        GtkWidget *risk_label = gtk_label_new(item->risk == RISK_CAUTION ? "Caution" : "Safe");
        gtk_widget_add_css_class(risk_label, "caption");
        if (item->risk == RISK_CAUTION) {
            gtk_widget_add_css_class(risk_label, "warning");
        }
        gtk_box_append(GTK_BOX(hbox), risk_label);

        item->install_badge = gtk_label_new("");
        gtk_widget_add_css_class(item->install_badge, "caption");
        gtk_widget_set_opacity(item->install_badge, 0.8);
        gtk_widget_set_visible(item->install_badge, FALSE);
        gtk_box_append(GTK_BOX(hbox), item->install_badge);

        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row_widget), hbox);
        gtk_list_box_append(GTK_LIST_BOX(state->app_list_box), row_widget);

        item->main_list_row = row_widget;
        g_signal_connect(check, "toggled", G_CALLBACK(on_app_toggled), item);

        state->app_list = g_list_append(state->app_list, item);
    }

    refresh_app_install_badges(state);
    update_action_bar_visibility(state);
}

// --- UI SETUP ---

static void activate(GtkApplication *app, AppState *state) {
    state->selected_device_id = NULL;
    state->selected_manufacturer_id = g_strdup("Samsung");
    state->app_list = NULL;
    state->uninstalled_app_list = NULL;
    state->installed_packages_set = NULL;

    state->window = ADW_APPLICATION_WINDOW(adw_application_window_new(GTK_APPLICATION(app)));
    gtk_window_set_title(GTK_WINDOW(state->window), "Android Debloater");
    gtk_window_set_default_size(GTK_WINDOW(state->window), 980, 660);

    GtkWidget *main_content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    adw_application_window_set_content(state->window, main_content_box);

    GtkWidget *header_bar = adw_header_bar_new();
    gtk_box_append(GTK_BOX(main_content_box), header_bar);

    GtkWidget *stack_switcher = gtk_stack_switcher_new();
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(header_bar), stack_switcher);

    GtkWidget *about_button = gtk_button_new_from_icon_name("help-about-symbolic");
    gtk_widget_set_tooltip_text(about_button, "About Android Debloater");
    adw_header_bar_pack_end(ADW_HEADER_BAR(header_bar), about_button);
    g_signal_connect(about_button, "clicked", G_CALLBACK(on_about_clicked), GTK_WINDOW(state->window));

    GtkWidget *stack = gtk_stack_new();
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stack_switcher), GTK_STACK(stack));
    gtk_box_append(GTK_BOX(main_content_box), stack);
    g_signal_connect(stack, "notify::visible-child-name", G_CALLBACK(on_stack_page_changed), state);

    // --- PAGE 1: DEBLOAT ---
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_stack_add_titled(GTK_STACK(stack), paned, "debloat", "Debloat");

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
    g_signal_connect(state->device_list_box, "row-selected", G_CALLBACK(on_device_selected), state);

    GtkWidget *device_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(device_scrolled), state->device_list_box);
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(device_scrolled), 220);
    gtk_widget_set_vexpand(device_scrolled, TRUE);
    gtk_box_append(GTK_BOX(left_box), device_scrolled);

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

    state->installed_only_toggle = gtk_toggle_button_new_with_label("Installed Only");
    gtk_widget_set_tooltip_text(state->installed_only_toggle, "Show only packages installed on the selected device");
    g_signal_connect(state->installed_only_toggle, "toggled", G_CALLBACK(on_installed_only_toggled), state);
    gtk_box_append(GTK_BOX(controls_hbox), state->installed_only_toggle);

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

    state->action_bar = gtk_action_bar_new();
    gtk_widget_set_visible(state->action_bar, FALSE);
    gtk_box_append(GTK_BOX(right_container), state->action_bar);

    state->status_label = gtk_label_new("Select a device and then select apps to debloat.");
    gtk_action_bar_pack_start(GTK_ACTION_BAR(state->action_bar), state->status_label);

    state->debloat_button = gtk_button_new_with_label("Remove Selected");
    gtk_widget_set_tooltip_text(state->debloat_button, "Uninstall or disable selected apps on connected device");
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

    GtkWidget *uninstalled_header = gtk_label_new("Removed or Disabled Packages (Saved Per Device)");
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

    state->debug_run_button = gtk_button_new_with_label("Execute");
    gtk_widget_add_css_class(state->debug_run_button, "suggested-action");
    g_signal_connect(state->debug_run_button, "clicked", G_CALLBACK(on_run_debug_command_clicked), state);
    gtk_box_append(GTK_BOX(debug_hbox), state->debug_run_button);

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

    update_device_list(state);
    populate_adb_devices_async(state);

    gtk_window_present(GTK_WINDOW(state->window));
    show_welcome_dialog(GTK_WINDOW(state->window));
}

static void on_shutdown(GtkApplication *app, gpointer user_data) {
    (void)app;
    (void)user_data;
    if (!app_state) return;

    g_free(app_state->selected_device_id);
    g_free(app_state->selected_manufacturer_id);
    g_list_free(app_state->pending_debloat_queue);

    if (app_state->installed_packages_set) {
        g_hash_table_destroy(app_state->installed_packages_set);
    }
    if (app_state->device_model) {
        g_object_unref(app_state->device_model);
    }
    g_list_free_full(app_state->uninstalled_app_list, (GDestroyNotify)app_item_free);
    g_list_free_full(app_state->app_list, (GDestroyNotify)app_item_free);
    g_free(app_state);
    app_state = NULL;
}

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
