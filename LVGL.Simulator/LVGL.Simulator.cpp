/*
 * PROJECT:   LVGL PC Simulator using Visual Studio
 * FILE:      LVGL.Simulator.cpp
 * PURPOSE:   Implementation for LVGL ported to Windows Desktop
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include <Windows.h>

#include "resource.h"

#if _MSC_VER >= 1200
 // Disable compilation warnings.
#pragma warning(push)
// nonstandard extension used : bit field types other than int
#pragma warning(disable:4214)
// 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
#endif

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/win32drv/win32drv.h"

#include "includes.h"

#include "ThermostatUI.h"
#include "LoadingUI.h"

#if _MSC_VER >= 1200
// Restore compilation warnings.
#pragma warning(pop)
#endif

#include <stdio.h>

bool single_display_mode_initialization()
{
    if (!lv_win32_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        800,
        480,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))
    {
        return false;
    }

    lv_win32_add_all_input_devices_to_group(NULL);

    lv_disp_get_default()->driver->dpi = LV_DPI_DEF;

    return true;
}

void setup_app()
{
    static LoadingUI* loading;
    static ThermostatUI* thermostat;

#if true
    thermostat = new ThermostatUI(lv_scr_act());
    thermostat->begin();

    ThermostatState state;
    state.localHumidity = 62;
    state.localTemperature = 19.4;
    state.mode = ThermostatMode::Heat;
    state.setpoint = 19.5;
    state.state = ThermostatRunningState::True;

    thermostat->setState(state);
#elif 0
    loading = new LoadingUI(lv_screen_active());
    loading->begin();

    loading->setTitle("Error connecting to WiFi");

    loading->setTitle("Connecting to WiFi");
    loading->setState(LoadingUIState::Loading);

    loading->redraw();
#else
    loading = new LoadingUI(lv_screen_active(), false);
    loading->begin();

    loading->setTitle("Error connecting to WiFi");

    loading->onRetryClicked([] {
        loading->setTitle("Connecting to WiFi");
        loading->setState(LoadingUIState::Loading);
        loading->redraw();
        });
    loading->setError("Cannot find AP");
    loading->setState(LoadingUIState::Error);
    loading->redraw();
#endif
}

int main()
{
    lv_init();

    if (!single_display_mode_initialization())
    {
        return -1;
    }

    setup_app();

    while (!lv_win32_quit_signal)
    {
        lv_task_handler();
        Sleep(1);
    }

    return 0;
}
