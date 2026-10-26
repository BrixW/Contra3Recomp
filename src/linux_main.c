/* SDL2 desktop runner for Linux. */
#include <SDL.h>
#include <gtk/gtk.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snes_apu.h"
#include "snes_mainthread.h"
#include "snes_ppu.h"
#include "snes_rom.h"

#define AUDIO_FRAMES 1024u

static char g_selected_rom[1024];
static bool g_launch_requested;
static GtkWidget *g_rom_entry;
static GtkApplication *g_app;

static const char *g_bind_names[2][12] = {
    {"Up", "Down", "Left", "Right", "Z", "X", "A", "S", "Q", "E", "Return", "Right Shift"},
    {"KP 8", "KP 5", "KP 4", "KP 6", "KP 1", "KP 2", "KP 3", "KP 0", "KP 7", "KP 9", "KP +", "KP -"}
};
static GtkWidget *g_bind_entries[2][12];
static const char *g_bind_labels[12] = {"Up", "Down", "Left", "Right", "Jump", "Shoot", "Weapon", "Bomb", "Lock aim", "Dual fire", "Start", "Select"};
static GtkWidget *g_pad_entries[8];
static const char *g_pad_labels[8] = {"Jump", "Bomb", "Shoot", "Weapon", "Lock aim", "Dual fire", "Start", "Select"};
static const char *g_pad_defaults[8] = {"A", "B", "X", "Y", "Left shoulder", "Right shoulder", "Start", "Back"};
static SDL_Scancode g_keyboard[2][12];
static SDL_GameControllerButton g_pad_buttons[8];
static SDL_GameController *g_pads[2];
static GtkWidget *g_capture_status;
static GtkWidget *g_capture_button;
static GtkWidget *g_capture_entry;
static int g_capture_kind = -1; /* 0 keyboard, 1 controller */
static bool g_capture_armed;

typedef struct CaptureTarget { GtkWidget *entry; GtkWidget *button; int kind; unsigned player; unsigned index; } CaptureTarget;
static CaptureTarget g_key_targets[2][12];
static CaptureTarget g_pad_targets[8];
static void open_controllers(SDL_GameController **pads);
static void rom_chooser_response(GObject *source, GAsyncResult *result, gpointer user_data);

static SDL_Scancode key_from_name(const char *name) {
    SDL_Scancode code = SDL_GetScancodeFromName(name);
    if (code == SDL_SCANCODE_UNKNOWN && g_str_has_prefix(name, "KP ")) {
        char keypad_name[32];
        if (strlen(name) == 4u) snprintf(keypad_name, sizeof(keypad_name), "Keypad %c", name[3]);
        else snprintf(keypad_name, sizeof(keypad_name), "Keypad %s", name + 3);
        code = SDL_GetScancodeFromName(keypad_name);
    }
    if (code == SDL_SCANCODE_UNKNOWN && name[0] != '\0' && name[1] == '\0') {
        char upper[2] = {(char)g_ascii_toupper(name[0]), '\0'};
        code = SDL_GetScancodeFromName(upper);
    }
    return code;
}

static SDL_GameControllerButton button_from_name(const char *name) {
    static const char *aliases[8][4] = {
        {"a", "", "", ""}, {"b", "", "", ""}, {"x", "", "", ""}, {"y", "", "", ""},
        {"left shoulder", "leftshoulder", "lb", ""}, {"right shoulder", "rightshoulder", "rb", ""},
        {"start", "", "", ""}, {"back", "select", "", ""}
    };
    static const SDL_GameControllerButton buttons[8] = {
        SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y,
        SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        SDL_CONTROLLER_BUTTON_START, SDL_CONTROLLER_BUTTON_BACK
    };
    for (unsigned i = 0; i < 8; ++i)
        for (unsigned j = 0; j < 4; ++j)
            if (aliases[i][j][0] != '\0' && g_ascii_strcasecmp(name, aliases[i][j]) == 0) return buttons[i];
    return SDL_CONTROLLER_BUTTON_INVALID;
}

static const char *button_display_name(SDL_GameControllerButton button) {
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A: return "A";
        case SDL_CONTROLLER_BUTTON_B: return "B";
        case SDL_CONTROLLER_BUTTON_X: return "X";
        case SDL_CONTROLLER_BUTTON_Y: return "Y";
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return "Left Shoulder";
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return "Right Shoulder";
        case SDL_CONTROLLER_BUTTON_START: return "Start";
        case SDL_CONTROLLER_BUTTON_BACK: return "Back";
        default: return "None";
    }
}

static const char *key_display_name(const char *name) {
    SDL_Scancode code = key_from_name(name);
    return code == SDL_SCANCODE_UNKNOWN ? name : SDL_GetScancodeName(code);
}

static void config_path(char *out, size_t size) {
    const char *base = g_get_user_config_dir();
    snprintf(out, size, "%s/contra3/launcher.ini", base);
}

static void load_launcher_config(void) {
    char filename[1024]; config_path(filename, sizeof(filename));
    GKeyFile *kf = g_key_file_new();
    if (g_key_file_load_from_file(kf, filename, G_KEY_FILE_NONE, NULL)) {
        for (unsigned p = 0; p < 2; ++p) for (unsigned i = 0; i < 12; ++i) {
            char key[24]; snprintf(key, sizeof(key), "p%u_key%u", p + 1, i);
            char *value = g_key_file_get_string(kf, "keyboard", key, NULL);
            if (value != NULL) { g_bind_names[p][i] = g_intern_string(value); g_free(value); }
        }
        for (unsigned i = 0; i < 8; ++i) {
            char key[24]; snprintf(key, sizeof(key), "button%u", i);
            char *value = g_key_file_get_string(kf, "controller", key, NULL);
            if (value != NULL) { g_pad_defaults[i] = g_intern_string(value); g_free(value); }
        }
    }
    g_key_file_unref(kf);
}

static void save_launcher_config(void) {
    char filename[1024]; config_path(filename, sizeof(filename));
    GKeyFile *kf = g_key_file_new();
    for (unsigned p = 0; p < 2; ++p) for (unsigned i = 0; i < 12; ++i) {
        char key[24]; snprintf(key, sizeof(key), "p%u_key%u", p + 1, i);
        g_key_file_set_string(kf, "keyboard", key, gtk_editable_get_text(GTK_EDITABLE(g_bind_entries[p][i])));
    }
    for (unsigned i = 0; i < 8; ++i) {
        char key[24]; snprintf(key, sizeof(key), "button%u", i);
        g_key_file_set_string(kf, "controller", key, gtk_editable_get_text(GTK_EDITABLE(g_pad_entries[i])));
    }
    gsize length = 0; char *data = g_key_file_to_data(kf, &length, NULL);
    char *directory = g_path_get_dirname(filename); g_mkdir_with_parents(directory, 0700); g_free(directory);
    g_file_set_contents(filename, data, (gssize)length, NULL);
    g_free(data); g_key_file_unref(kf);
}

static void choose_rom(GtkButton *button, gpointer user_data) {
    (void)button; GtkWindow *parent = GTK_WINDOW(user_data);
    GtkFileDialog *chooser = gtk_file_dialog_new();
    gtk_file_dialog_set_title(chooser, "Choose Contra III ROM");
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "SNES ROM images (*.sfc, *.smc)");
    gtk_file_filter_add_pattern(filter, "*.sfc"); gtk_file_filter_add_pattern(filter, "*.smc");
    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(filters, filter);
    gtk_file_dialog_set_filters(chooser, G_LIST_MODEL(filters));
    gtk_file_dialog_open(chooser, parent, NULL, rom_chooser_response, NULL);
    g_object_unref(filters); g_object_unref(filter); g_object_unref(chooser);
}

static void capture_clear(GtkButton *button, gpointer user_data) {
    (void)button;
    CaptureTarget *target = user_data;
    if (g_capture_entry == target->entry) {
        gtk_button_set_label(GTK_BUTTON(g_capture_button), "Set");
        gtk_label_set_text(GTK_LABEL(g_capture_status), "Input cleared.");
        g_capture_kind = -1; g_capture_entry = NULL; g_capture_button = NULL;
    }
    gtk_editable_set_text(GTK_EDITABLE(target->entry), target->kind == 0 ? "" : "None");
}

static void capture_begin(GtkButton *button, gpointer user_data) {
    CaptureTarget *target = user_data;
    g_capture_kind = target->kind;
    g_capture_entry = target->entry;
    g_capture_button = GTK_WIDGET(button);
    g_capture_armed = false;
    gtk_button_set_label(GTK_BUTTON(button), "Press input…");
    gtk_label_set_text(GTK_LABEL(g_capture_status), target->kind == 0
        ? "Waiting for a keyboard key…" : "Release the controller button, then press the one to bind…");
    gtk_widget_grab_focus(GTK_WIDGET(button));
}

static void capture_finish(const char *label) {
    const char *display = g_capture_kind == 1
        ? button_display_name(button_from_name(label)) : key_display_name(label);
    gtk_editable_set_text(GTK_EDITABLE(g_capture_entry), display);
    gtk_button_set_label(GTK_BUTTON(g_capture_button), "Set");
    gtk_label_set_text(GTK_LABEL(g_capture_status), "Input assigned.");
    g_capture_kind = -1;
    g_capture_entry = NULL;
    g_capture_button = NULL;
}

static gboolean launcher_key_pressed(GtkEventControllerKey *controller, guint keyval, guint keycode,
                                     GdkModifierType state, gpointer user_data) {
    (void)controller; (void)keycode; (void)state; (void)user_data;
    if (g_capture_kind != 0) return FALSE;
    const char *name = gdk_keyval_name(keyval);
    if (name == NULL) return TRUE;
    char normalized[64];
    if (g_str_equal(name, "Shift_R")) name = "Right Shift";
    else if (g_str_equal(name, "Shift_L")) name = "Left Shift";
    else if (g_str_equal(name, "Control_R")) name = "Right Ctrl";
    else if (g_str_equal(name, "Control_L")) name = "Left Ctrl";
    else if (g_str_has_prefix(name, "KP_") && strlen(name) == 4u) {
        snprintf(normalized, sizeof(normalized), "KP %c", name[3]); name = normalized;
    }
    SDL_Scancode code = key_from_name(name);
    if (code == SDL_SCANCODE_UNKNOWN) {
        gtk_label_set_text(GTK_LABEL(g_capture_status), "That key is not recognized by SDL; try another.");
        return TRUE;
    }
    capture_finish(SDL_GetScancodeName(code));
    return TRUE;
}

static gboolean poll_launcher_controller_capture(gpointer user_data) {
    (void)user_data;
    if (g_capture_kind != 1) return G_SOURCE_CONTINUE;
    SDL_GameControllerUpdate();
    bool any_down = false;
    static const SDL_GameControllerButton candidates[8] = {
        SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y,
        SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        SDL_CONTROLLER_BUTTON_START, SDL_CONTROLLER_BUTTON_BACK
    };
    for (unsigned p = 0; p < 2; ++p) {
        if (g_pads[p] == NULL || !SDL_GameControllerGetAttached(g_pads[p])) continue;
        for (unsigned i = 0; i < 8; ++i) {
            if (!SDL_GameControllerGetButton(g_pads[p], candidates[i])) continue;
            any_down = true;
            if (g_capture_armed) {
                capture_finish(SDL_GameControllerGetStringForButton(candidates[i]));
                return G_SOURCE_CONTINUE;
            }
        }
    }
    if (!any_down) g_capture_armed = true;
    return G_SOURCE_CONTINUE;
}

static void rom_chooser_response(GObject *source, GAsyncResult *result, gpointer user_data) {
    (void)user_data;
    GFile *file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source), result, NULL);
    if (file != NULL) {
        char *path = file != NULL ? g_file_get_path(file) : NULL;
        if (path != NULL) { gtk_editable_set_text(GTK_EDITABLE(g_rom_entry), path); g_free(path); }
        g_object_unref(file);
    }
}

static void launcher_play(GtkButton *button, gpointer user_data) {
    (void)button; (void)user_data;
    const char *path = gtk_editable_get_text(GTK_EDITABLE(g_rom_entry));
    if (path == NULL || path[0] == '\0') return;
    g_strlcpy(g_selected_rom, path, sizeof(g_selected_rom));
    save_launcher_config();
    for (unsigned p = 0; p < 2; ++p) for (unsigned i = 0; i < 12; ++i)
        g_keyboard[p][i] = key_from_name(gtk_editable_get_text(GTK_EDITABLE(g_bind_entries[p][i])));
    for (unsigned i = 0; i < 8; ++i) {
        const char *name = gtk_editable_get_text(GTK_EDITABLE(g_pad_entries[i]));
        g_pad_buttons[i] = button_from_name(name);
    }
    g_launch_requested = true;
    g_application_quit(G_APPLICATION(g_app));
}

static void launcher_activate(GtkApplication *app, gpointer user_data) {
    (void)user_data; g_app = app;
    load_launcher_config();
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Contra III Launcher");
    gtk_window_set_default_size(GTK_WINDOW(window), 920, 820);
    GtkWidget *outer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(outer, 18); gtk_widget_set_margin_bottom(outer, 18);
    gtk_widget_set_margin_start(outer, 20); gtk_widget_set_margin_end(outer, 20);
    gtk_window_set_child(GTK_WINDOW(window), outer);
    GtkWidget *heading = gtk_label_new("Contra III: The Alien Wars");
    gtk_widget_add_css_class(heading, "title-1"); gtk_box_append(GTK_BOX(outer), heading);
    GtkWidget *rom_label = gtk_label_new("ROM image"); gtk_widget_set_halign(rom_label, GTK_ALIGN_START); gtk_box_append(GTK_BOX(outer), rom_label);
    GtkWidget *rom_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8); gtk_box_append(GTK_BOX(outer), rom_row);
    g_rom_entry = gtk_entry_new(); gtk_widget_set_hexpand(g_rom_entry, TRUE); gtk_entry_set_placeholder_text(GTK_ENTRY(g_rom_entry), "Choose a Contra III .sfc or .smc ROM");
    if (g_selected_rom[0] != '\0') gtk_editable_set_text(GTK_EDITABLE(g_rom_entry), g_selected_rom);
    gtk_box_append(GTK_BOX(rom_row), g_rom_entry);
    GtkWidget *browse = gtk_button_new_with_label("Browse…"); gtk_box_append(GTK_BOX(rom_row), browse);
    g_signal_connect(browse, "clicked", G_CALLBACK(choose_rom), window);
    GtkWidget *scroll = gtk_scrolled_window_new(); gtk_widget_set_vexpand(scroll, TRUE); gtk_box_append(GTK_BOX(outer), scroll);
    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12); gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), content);
    GtkWidget *keys_label = gtk_label_new("Keyboard bindings");
    gtk_widget_set_halign(keys_label, GTK_ALIGN_START); gtk_box_append(GTK_BOX(content), keys_label);
    GtkWidget *key_grid = gtk_grid_new(); gtk_grid_set_row_spacing(GTK_GRID(key_grid), 4); gtk_grid_set_column_spacing(GTK_GRID(key_grid), 10); gtk_box_append(GTK_BOX(content), key_grid);
    for (unsigned p = 0; p < 2; ++p) {
        char player[24]; snprintf(player, sizeof(player), "Player %u", p + 1);
        GtkWidget *label = gtk_label_new(player); gtk_widget_set_halign(label, GTK_ALIGN_START); gtk_grid_attach(GTK_GRID(key_grid), label, (int)(p * 4), 0, 4, 1);
        for (unsigned i = 0; i < 12; ++i) {
            GtkWidget *name = gtk_label_new(g_bind_labels[i]); gtk_widget_set_halign(name, GTK_ALIGN_START);
            GtkWidget *entry = gtk_entry_new(); gtk_widget_set_size_request(entry, 105, -1); gtk_editable_set_text(GTK_EDITABLE(entry), key_display_name(g_bind_names[p][i])); gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
            g_bind_entries[p][i] = entry;
            CaptureTarget *target = &g_key_targets[p][i]; *target = (CaptureTarget){entry, NULL, 0, p, i};
            GtkWidget *set = gtk_button_new_with_label("Set"); target->button = set;
            GtkWidget *clear = gtk_button_new_with_label("Clear");
            g_signal_connect(set, "clicked", G_CALLBACK(capture_begin), target);
            g_signal_connect(clear, "clicked", G_CALLBACK(capture_clear), target);
            int col = (int)(p * 4);
            gtk_grid_attach(GTK_GRID(key_grid), name, col, (int)i + 1, 1, 1);
            gtk_grid_attach(GTK_GRID(key_grid), entry, col + 1, (int)i + 1, 1, 1);
            gtk_grid_attach(GTK_GRID(key_grid), set, col + 2, (int)i + 1, 1, 1);
            gtk_grid_attach(GTK_GRID(key_grid), clear, col + 3, (int)i + 1, 1, 1);
        }
    }
    GtkWidget *pad_label = gtk_label_new("Controller bindings");
    gtk_label_set_wrap(GTK_LABEL(pad_label), TRUE); gtk_widget_set_halign(pad_label, GTK_ALIGN_START); gtk_box_append(GTK_BOX(content), pad_label);
    GtkWidget *pad_grid = gtk_grid_new(); gtk_grid_set_row_spacing(GTK_GRID(pad_grid), 4); gtk_grid_set_column_spacing(GTK_GRID(pad_grid), 10); gtk_box_append(GTK_BOX(content), pad_grid);
    for (unsigned i = 0; i < 8; ++i) {
        GtkWidget *label = gtk_label_new(g_pad_labels[i]); gtk_widget_set_halign(label, GTK_ALIGN_START);
        GtkWidget *entry = gtk_entry_new(); gtk_widget_set_size_request(entry, 150, -1); gtk_editable_set_text(GTK_EDITABLE(entry), button_display_name(button_from_name(g_pad_defaults[i]))); gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE); g_pad_entries[i] = entry;
        CaptureTarget *target = &g_pad_targets[i]; *target = (CaptureTarget){entry, NULL, 1, 0, i};
        GtkWidget *set = gtk_button_new_with_label("Set"); target->button = set;
        GtkWidget *clear = gtk_button_new_with_label("Clear");
        g_signal_connect(set, "clicked", G_CALLBACK(capture_begin), target);
        g_signal_connect(clear, "clicked", G_CALLBACK(capture_clear), target);
        gtk_grid_attach(GTK_GRID(pad_grid), label, 0, (int)i, 1, 1); gtk_grid_attach(GTK_GRID(pad_grid), entry, 1, (int)i, 1, 1);
        gtk_grid_attach(GTK_GRID(pad_grid), set, 2, (int)i, 1, 1); gtk_grid_attach(GTK_GRID(pad_grid), clear, 3, (int)i, 1, 1);
    }
    g_capture_status = gtk_label_new("Select Set next to a binding, then press the key or controller button you want.");
    gtk_label_set_wrap(GTK_LABEL(g_capture_status), TRUE); gtk_widget_set_halign(g_capture_status, GTK_ALIGN_START); gtk_box_append(GTK_BOX(content), g_capture_status);
    GtkWidget *footer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8); gtk_widget_set_halign(footer, GTK_ALIGN_END); gtk_box_append(GTK_BOX(outer), footer);
    GtkWidget *quit = gtk_button_new_with_label("Quit"); gtk_box_append(GTK_BOX(footer), quit);
    GtkWidget *play = gtk_button_new_with_label("Play"); gtk_widget_add_css_class(play, "suggested-action"); gtk_box_append(GTK_BOX(footer), play);
    g_signal_connect(play, "clicked", G_CALLBACK(launcher_play), NULL);
    g_signal_connect_swapped(quit, "clicked", G_CALLBACK(g_application_quit), app);
    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(launcher_key_pressed), NULL);
    gtk_widget_add_controller(window, key_controller);
    g_timeout_add(16, poll_launcher_controller_capture, NULL);
    open_controllers(g_pads);
    gtk_window_present(GTK_WINDOW(window));
}

static uint16_t keyboard_pad(const uint8_t *keys, unsigned player) {
    static const uint16_t masks[] = {SNES_BTN_UP,SNES_BTN_DOWN,SNES_BTN_LEFT,SNES_BTN_RIGHT,SNES_BTN_B,SNES_BTN_Y,SNES_BTN_X,SNES_BTN_A,SNES_BTN_L,SNES_BTN_R,SNES_BTN_START,SNES_BTN_SELECT};
    uint16_t result = 0;
    for (unsigned i = 0; i < sizeof(masks)/sizeof(masks[0]); ++i)
        if (g_keyboard[player][i] != SDL_SCANCODE_UNKNOWN && keys[g_keyboard[player][i]]) result |= masks[i];
    return result;
}

static void open_controllers(SDL_GameController **pads) {
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (!SDL_IsGameController(i)) continue;
        SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(i);
        bool already_open = false;
        for (unsigned p = 0; p < 2; ++p)
            if (pads[p] != NULL && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pads[p])) == id) already_open = true;
        if (already_open) continue;
        for (unsigned p = 0; p < 2; ++p) {
            if (pads[p] == NULL) { pads[p] = SDL_GameControllerOpen(i); break; }
        }
    }
}

static uint16_t controller_pad(SDL_GameController *pad) {
    if (pad == NULL || !SDL_GameControllerGetAttached(pad)) return 0;
    uint16_t result = 0;
    static const uint16_t target_masks[8] = {SNES_BTN_B,SNES_BTN_A,SNES_BTN_Y,SNES_BTN_X,SNES_BTN_L,SNES_BTN_R,SNES_BTN_START,SNES_BTN_SELECT};
    for (unsigned i = 0; i < 8; ++i)
        if (g_pad_buttons[i] != SDL_CONTROLLER_BUTTON_INVALID && SDL_GameControllerGetButton(pad, g_pad_buttons[i])) result |= target_masks[i];
    Sint16 x = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX);
    Sint16 y = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTY);
    if (x < -10000) result |= SNES_BTN_LEFT;
    if (x > 10000) result |= SNES_BTN_RIGHT;
    if (y < -10000) result |= SNES_BTN_UP;
    if (y > 10000) result |= SNES_BTN_DOWN;
    return result;
}

static void set_default_inputs(void) {
    static const char *defaults[2][12] = {
        {"Up","Down","Left","Right","Z","X","A","S","Q","E","Return","Right Shift"},
        {"KP 8","KP 5","KP 4","KP 6","KP 1","KP 2","KP 3","KP 0","KP 7","KP 9","KP +","KP -"}
    };
    static const SDL_GameControllerButton pad_defaults[8] = {
        SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y,
        SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        SDL_CONTROLLER_BUTTON_START, SDL_CONTROLLER_BUTTON_BACK
    };
    for (unsigned p = 0; p < 2; ++p) for (unsigned i = 0; i < 12; ++i) g_keyboard[p][i] = key_from_name(defaults[p][i]);
    memcpy(g_pad_buttons, pad_defaults, sizeof(pad_defaults));
}

static void apply_saved_inputs(void) {
    for (unsigned p = 0; p < 2; ++p) for (unsigned i = 0; i < 12; ++i)
        g_keyboard[p][i] = key_from_name(g_bind_names[p][i]);
    for (unsigned i = 0; i < 8; ++i) g_pad_buttons[i] = button_from_name(g_pad_defaults[i]);
}

static void audio_queue(SnesApu *apu, SDL_AudioDeviceID device) {
    int16_t pcm[AUDIO_FRAMES * 2u];
    size_t n = snes_apu_pop_samples(apu, pcm, AUDIO_FRAMES);
    if (n != 0u) SDL_QueueAudio(device, pcm, (Uint32)(n * 2u * sizeof(int16_t)));
}

int main(int argc, char **argv) {
    char rom_path[1024], error[256];
    const char *path = NULL;
    bool explicit_rom = false;
    bool force_launcher = false;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            printf("Usage: contra3 [rom.sfc]\nKeyboard: arrows, Z/J jump, X/K shoot, A/U weapon, S/I bomb, Q/E L/R; Esc quits.\n");
            return 0;
        }
        if (!strcmp(argv[i], "--launcher")) { force_launcher = true; continue; }
        if (argv[i][0] == '-') { fprintf(stderr, "Unknown option: %s\n", argv[i]); return 2; }
        path = argv[i]; explicit_rom = true;
    }
    if (path == NULL || force_launcher) {
        set_default_inputs();
        load_launcher_config();
        apply_saved_inputs();
        char exe_dir[1024];
        snes_rom_dirname(argv[0], exe_dir, sizeof(exe_dir));
        if (path != NULL) g_strlcpy(g_selected_rom, path, sizeof(g_selected_rom));
        else if (snes_rom_find(exe_dir, rom_path, sizeof(rom_path))) g_strlcpy(g_selected_rom, rom_path, sizeof(g_selected_rom));
        if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
            fprintf(stderr, "SDL controller initialization failed: %s\n", SDL_GetError()); return 1;
        }
        g_app = gtk_application_new("org.codex.contra3", G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(g_app, "activate", G_CALLBACK(launcher_activate), NULL);
        int gui_status = g_application_run(G_APPLICATION(g_app), 1, argv);
        g_object_unref(g_app);
        if (gui_status != 0) return gui_status;
        path = g_launch_requested && g_selected_rom[0] != '\0' ? g_selected_rom : NULL;
    } else {
        set_default_inputs();
        load_launcher_config();
        apply_saved_inputs();
    }
    (void)explicit_rom;
    if (path == NULL) return 0;
    size_t rom_size = 0;
    uint8_t *rom = snes_rom_load(path, &rom_size, error, sizeof(error));
    if (rom == NULL) { fprintf(stderr, "ROM error: %s\n", error); return 1; }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError()); free(rom); return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Contra III - The Alien Wars", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SNES_SCREEN_WIDTH * 3, SNES_SCREEN_HEIGHT * 3, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) : NULL;
    if (renderer == NULL && window) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_Texture *texture = renderer ? SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SNES_SCREEN_WIDTH, SNES_SCREEN_HEIGHT) : NULL;
    if (texture == NULL) { fprintf(stderr, "SDL video setup failed: %s\n", SDL_GetError()); SDL_Quit(); free(rom); return 1; }
    SDL_AudioSpec want = {0}, have = {0};
    want.freq = 32000; want.format = AUDIO_S16SYS; want.channels = 2; want.samples = AUDIO_FRAMES;
    SDL_AudioDeviceID audio = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (audio) SDL_PauseAudioDevice(audio, 0);
    open_controllers(g_pads);

    SnesCpuContext ctx;
    SnesApu apu;
    uint32_t frame[SNES_SCREEN_WIDTH * SNES_SCREEN_HEIGHT];
    snes_context_init(&ctx, rom, rom_size);
    snes_apu_init(&apu); snes_apu_attach(&ctx, &apu); snes_power_on(&ctx);
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F11)
                SDL_SetWindowFullscreen(window, SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
            if (event.type == SDL_CONTROLLERDEVICEADDED) open_controllers(g_pads);
            if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
                for (unsigned i = 0; i < 2; ++i) {
                    if (g_pads[i] != NULL && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(g_pads[i])) == event.cdevice.which) {
                        SDL_GameControllerClose(g_pads[i]); g_pads[i] = NULL;
                    }
                }
                open_controllers(g_pads);
            }
        }
        const uint8_t *keys = SDL_GetKeyboardState(NULL);
        snes_step_frame(&ctx, (uint16_t)(keyboard_pad(keys, 0) | controller_pad(g_pads[0])),
                        (uint16_t)(keyboard_pad(keys, 1) | controller_pad(g_pads[1])), frame);
        SDL_UpdateTexture(texture, NULL, frame, SNES_SCREEN_WIDTH * (int)sizeof(uint32_t));
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL); SDL_RenderPresent(renderer);
        if (audio) {
            if (SDL_GetQueuedAudioSize(audio) < AUDIO_FRAMES * 4u) audio_queue(&apu, audio);
            else snes_apu_clear_samples(&apu);
        } else snes_apu_clear_samples(&apu);
    }
    snes_mainthread_release(&ctx);
    for (unsigned i = 0; i < 2; ++i) if (g_pads[i] != NULL) SDL_GameControllerClose(g_pads[i]);
    if (audio) SDL_CloseAudioDevice(audio);
    SDL_DestroyTexture(texture); SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); SDL_Quit(); free(rom);
    return 0;
}
