#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>

#include "../../external/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../external/imgui/imgui_internal.h"

extern ImFont* tab_icons;
extern ImFont* weaponIcons;
extern ImFont* settingsIcon;

namespace elements {
    bool tab(const char* icon, bool boolean);
    bool subtab(const char* name, bool boolean);
    bool weaponSelect(const char* name, bool boolean);
    bool weaponSelectRage(const char* name, bool boolean);
    bool iconButton(const char* name, bool boolean);
}
