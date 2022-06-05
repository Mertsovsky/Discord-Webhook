#ifndef _MAIN_HEADER_
#define _MAIN_HEADER_

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define WIN32_LEAN_AND_MEAN // Exclude rarely used components from Windows headers.

#include <windows.h> // WinApi

#include <wininet.h> // Simple windows internet

#include <string> // std::string, std::wstring.
#include <memory> // std::unique_ptr, std::shared_ptr, std::make_unique, std::make_shared
#include <optional> // std::optional

#include <d3d9.h>

// Global variables
#include "misc/cvars.h"

// ImGui
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#define IMGUI_INCLUDE_IMCONFIG_H

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "nlohmann/json.hpp" // Simple JSON

// Secondary functions
#include "utilities/helper.h"

// Discord
#include "discord/embed.h"
#include "discord/webhook.h"

// Overlay
#include "overlay/menu.h"

#endif