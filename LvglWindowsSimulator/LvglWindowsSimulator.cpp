#include "includes.h"

#include <LvglWindowsIconResource.h>
#include <Windows.h>

#include "Application.h"
#include "lvgl/lvgl.h"

int main() {
    lv_init();

    int32_t zoom_level = 50;
    bool allow_dpi_override = false;
    bool simulator_mode = true;
    lv_display_t* display = lv_windows_create_display(L"Kitchen Display", 1024, 600, zoom_level,
                                                      allow_dpi_override, simulator_mode);

    if (!display) {
        return -1;
    }

    HWND window_handle = lv_windows_get_display_window_handle(display);
    if (!window_handle) {
        return -1;
    }

    HICON icon_handle = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL_WINDOWS));
    if (icon_handle) {
        SendMessageW(window_handle, WM_SETICON, TRUE, (LPARAM)icon_handle);
        SendMessageW(window_handle, WM_SETICON, FALSE, (LPARAM)icon_handle);
    }

    lv_indev_t* pointer_indev = lv_windows_acquire_pointer_indev(display);
    if (!pointer_indev) {
        return -1;
    }

    lv_indev_t* keypad_indev = lv_windows_acquire_keypad_indev(display);
    if (!keypad_indev) {
        return -1;
    }

    lv_indev_t* encoder_indev = lv_windows_acquire_encoder_indev(display);
    if (!encoder_indev) {
        return -1;
    }

    // lv_demo_widgets();
    // lv_demo_benchmark();

    _putenv("CURL_CA_BUNDLE=../Output/Binaries/Debug/x64/curl-ca-bundle.crt");

    Application application;

    application.begin();

    while (1) {
        application.process();

        uint32_t time_till_next = lv_timer_handler();
        Sleep(time_till_next);
    }

    return 0;
}
