#pragma once

/*
* Namespace holding our b1g nigga menu
*/

#include "interfaces.h"
#include "../features/features.h"
#include <d3d9.h>

struct ImFont;
namespace u
{
	void Setup();
	void Destroy();

	void Render();

	inline ImFont* segoe = nullptr;

	inline static int menu_animation = 0;
	inline static int border_animation = 0;
	inline static int domenuopen = false;
	inline bool open = true;
	inline HWND window = nullptr;
	inline WNDPROC WindowProcessOriginal = nullptr;
}
