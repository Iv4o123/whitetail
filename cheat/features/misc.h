#pragma once
#include "../core/interfaces.h"
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_dx9.h"
#include "../../external/imgui/imgui_impl_win32.h"

extern ImFont* medium;

class Misc
{
public:
	void RevealRanks(UserCmd* cmd);
	void RevealRadar();
	void RevealVotes(GameEvent* event);
	void BunnyHop(UserCmd* cmd);
	void SniperCrosshair();
	void HitEffect(GameEvent* event);
	void HitSound(GameEvent* event);
	
	
};
