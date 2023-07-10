#include "ui.h"

#include <windows.h>
#include <stdexcept>

#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_dx9.h"
#include "../../external/imgui/imgui_impl_win32.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "etc_elements.h"
#include "nav_elements.h"
#include "../../external/imgui/imgui_shadow.h"
#include <math.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include "../core/byte_array_tab.h"
#include "byte_array_font.h"
#include "byte_array_combo.h"
#include "imgui_notify.h"
#include "weapon_icons.h"
#include "../settingssearchicon.h"
#include "bytes.h"


#pragma region Namespace Fonts
ImFont* medium;
ImFont* bold;
ImFont* tab_icons;
ImFont* titles;
//ImFont* logo;
ImFont* tab_title;
ImFont* tab_title_icon;
ImFont* subtab_title;
ImFont* combo_arroww;
ImFont* tabs;
ImFont* logo_head;
ImFont* colorpalette;
ImFont* Logo;
ImFont* weaponIcons;
ImFont* settingsIcon;

#pragma endregion Namespace Fonts

IDirect3DTexture9* legit_image = nullptr;

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

int selection_count = 0;

enum heads {
	//rage, antiaim, visuals, settings, skins, configs, scripts,
	Legitbot, Ragebot, Visuals, AntiAim, Misc, Pressets, IChanger, Settings
};

enum sub_heads {
	AssistedAim, Triggerbot, Hitboxes, Accuracy, HitboxesRage, Local, Enemies, Teammates, Other, World, Main,
	CheatSettings, Configs, ExtraLegitbot, OtherMisc, General, 
};

enum legitgrouptabs {
	Pistols, SMG, Rifles, Shotguns, Heavy, Snipers
};

enum ragegrouptabs {
	RageGeneral, LightPistols, HeavyPistols, RageSMGS, RageRifles, RageShotguns, RageHeavy, RageAutoSnipers
};



static float switch_alpha[3], open_alpha = 0;

LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);

void u::Setup()
{
	auto creationParams = D3DDEVICE_CREATION_PARAMETERS{ };
	if (FAILED(i::device->GetCreationParameters(&creationParams)))
	{
		throw std::runtime_error("Failed to get device creation parameters");
	}

	window = creationParams.hFocusWindow;
	WindowProcessOriginal = reinterpret_cast<WNDPROC>(
		SetWindowLongPtrW(
			window,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(WindowProcess)
		)
		);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0xE000, 0xE226, // icons
		0,
	};

	if (open)
	{
		menu_animation = menu_animation + 6;
		//border_animation = border_animation + 6;
		//menu_slide = menu_slide + 10;
	}
	else if (!open)
	{
		menu_animation = menu_animation - 6;
	}

	if (menu_animation > 254)
		menu_animation = 255;
	if (menu_animation < 1)
	{
		menu_animation = 0;
		domenuopen = true;
	}

	// Load Fonts
	ImFontConfig font_config;
	font_config.PixelSnapH = false;
	font_config.OversampleH = 5;
	font_config.OversampleV = 5;
	font_config.RasterizerMultiply = 1.2f;
	font_config.FontDataOwnedByAtlas = false;
	font_config.GlyphRanges = ranges;
	io.Fonts->Clear();

	ImGuiStyle& style = ImGui::GetStyle();
	//ImGuiStyle* style = &ImGui::GetStyle();

	style.ScrollbarRounding = 6.0f;
	style.ScrollbarSize = 3.0f;
	//style.WindowMinSize = ImVec2(730, 460);
	style.WindowBorderSize = 3.f;
	style.ChildBorderSize = 0.45f;
	style.ChildRounding = 1.0f;
	style.WindowPadding = ImVec2(0, 0);
	


	//style.Colors[ImGuiCol_WindowBg] = ImColor(16, 16, 16);
	style.Colors[ImGuiCol_WindowBg] = ImColor(255, 255, 255);
	style.Colors[ImGuiCol_ChildBg] = ImColor(245, 245, 245);
	style.Colors[ImGuiCol_Text] = ImColor(24, 24, 24);
	style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);
	style.Colors[ImGuiCol_ScrollbarBg] = ImColor(255, 255, 255, 100);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(24, 24, 24, 50);

	style.Colors[ImGuiCol_Header] = ImColor(24, 24, 24, 24);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(28, 28, 28, 45);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(254, 254, 254, 25);

	style.Colors[ImGuiCol_Button] = ImColor(31, 31, 31);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(34, 34, 34);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(34, 34, 34);


	//style.Colors[ImGuiCol_Border] = ImColor(245, 245, 245);
	style.Colors[ImGuiCol_Border] = ImColor(24, 24, 24, 95);
	//style.Colors[ImGuiCol_WindowShadow] = ImColor(230, 230, 230);

	//ResizeGrip
	style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 100);
	style.Colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 100);
	style.Colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 100);

	ImGui::GetStyle().WindowRounding = 6.0f;//Rounded Window Corners   

	tab_icons = io.Fonts->AddFontFromMemoryCompressedBase85TTF(icomoon_compressed_data_base85, 32.0f, &font_config, ranges);
	titles = io.Fonts->AddFontFromMemoryCompressedBase85TTF(PTRootUIBold, 15.0f, &font_config, ranges);
	medium = io.Fonts->AddFontFromMemoryCompressedBase85TTF(PTRootUIMedium, 15.0f, &font_config, ranges);
	//bold = io.Fonts->AddFontFromMemoryCompressedBase85TTF(PTRootUIBold, 15.0f, &font_config, ranges);
	combo_arroww = io.Fonts->AddFontFromMemoryCompressedBase85TTF(arrow_data_base85, 9.0f, &font_config, ranges);
	weaponIcons = io.Fonts->AddFontFromMemoryCompressedBase85TTF(weaponico, 32.0f, &font_config, ranges);
	settingsIcon = io.Fonts->AddFontFromMemoryCompressedBase85TTF(settingssearch, 32.0f, &font_config, ranges);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(i::device);

	//ImGuiIO* io = &ImGui::GetIO();
	
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs
		| ImGuiColorEditFlags_NoLabel
		| ImGuiColorEditFlags_NoSidePreview
		| ImGuiColorEditFlags_Uint8
		| ImGuiColorEditFlags_AlphaBar);


}



void u::Destroy()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	SetWindowLongPtrW(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcessOriginal));
}

#define VISUALS_COLOR_BOOL(name) \
ImGui::Checkbox(#name, &v::visuals.name.first); \
ImGui::SameLine(offset); \
ImGui::ColorEdit4(#name, v::visuals.name.second.data())

#define MISC_COLOR_BOOL(name) \
ImGui::Checkbox(#name, &v::misc.name.first); \
ImGui::SameLine(offset); \
ImGui::ColorEdit4(#name, v::misc.name.second.data())

void HelpMarker(const char* desc)
{
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void Decoration()
{
	auto d = ImGui::GetWindowDrawList();
	auto p = ImGui::GetWindowPos();
	auto s = ImGui::GetWindowSize();

	static float fill;
	fill = ImClamp(fill + (2.f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);
	fill *= ImGui::GetStyle().Alpha;

}

static int m_tabs;
static int m_weapontabs;

void LegitWeaponTabs()
{
	// Tabs
	std::vector<std::string> tabs =
	{
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
	};

	ImGui::PushFont(weaponIcons);
	for (int i = 0; i < tabs.size(); i++)
	{
		
		if (ImGui::MenuTab(tabs.at(i).c_str(), m_weapontabs == i, ImVec2(80, 40)))
		{
			if (m_weapontabs != i)
			{
				switch_alpha[2] = 0;
				m_weapontabs = i;
			}
		}
		if (i < tabs.size() - 1)
			ImGui::SameLine(0, 0);
	}
	ImGui::PopFont();
	//if (u::open && switch_alpha[1] > 0.8f || !u::open)
		//switch_alpha[2] = ImClamp(switch_alpha[2] + (2.f * ImGui::GetIO().DeltaTime * (toggled ? 1.f : -5.f)), 0.f, 1.f);
}









void u::Render()
{
	
	
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiStyle& style = ImGui::GetStyle();

	static heads tab{  };
	static legitgrouptabs legitgroup{ };
	static ragegrouptabs ragegroup{ };
	static sub_heads subtab{  };

	



	const char* tab_name = tab == Legitbot ? "Legitbot" : tab == Ragebot ? "Ragebot" : tab == Visuals ? "Visuals" : tab == AntiAim ? "Anti-Aim" : tab == Misc ? "Misc" : tab == Pressets ? "Presets" : tab == IChanger ? "Inventory Changer" : 0;
	//const char* tab_icon = tab == Legitbot ? "A" : tab == Ragebot ? "B" : tab == Visuals ? "C" : tab == AntiAim ? "D" : tab == Misc ? "E" : tab == Pressets ? "F" :  0;
	//static bool boolean, boolean_1 = false;
	//static int sliderscalar = 60;
	//static bool sideBar = true;

	//static bool enableViewModelChams = false;
	//static bool ViewmodelFOV = false;
	//static bool ViewFOV = false;
	//static float animspeed = 0.4f;
	//int selected = 0;
	//bool boolean = false;
	
	

	//bool RageGeneralEnabled = true;

	//const char* viewmodel_chams[3] = { "Arms", "Sleeves", "Weapon" };
	//static int combo = 0;
	//static int weapon;

	static bool disabled = true;
	static bool enable = false;

	
	
		ImGui::SetNextWindowSize(ImVec2(806, 666));
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushFont(medium);
		ImGui::Begin("Unknown name cheat", &open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
		

			
			Decoration();

			
					
					//Get Window DRAW, POSITION, SIZE, etc.
					auto draw = ImGui::GetWindowDrawList();
					auto pos = ImGui::GetWindowPos();
					auto size = ImGui::GetWindowSize();
					auto windowWidth = ImGui::GetWindowWidth();
					auto windowHeight = ImGui::GetWindowHeight();
					//Background
					//draw->AddRectFilled(ImVec2(pos.x + 800, pos.y + 666),ImVec2(pos.x + 800, pos.y + 666), ImColor(11, 11, 11, 255), 2.f, ImDrawFlags_RoundCornersBottomRight);
					//Draw tabs bg
					draw->AddRectFilled(pos, ImVec2(pos.x + 70, pos.y + size.y), ImColor(245, 245, 245), style.WindowRounding, ImDrawFlags_RoundCornersLeft); //
					//Draw header
					draw->AddRectFilled(pos, ImVec2(pos.x + 871, pos.y + 47), ImColor(235, 235, 235), style.WindowRounding, ImDrawFlags_RoundCornersAll);
					//Header seeparator
					draw->AddLine(ImVec2(pos.x, pos.y + 47), ImVec2(pos.x + 803, pos.y + 47), ImColor(21, 112, 232, 50));
					//Tab seperator
					draw->AddLine(ImVec2(pos.x + 70, pos.y + 47), ImVec2(pos.x + 70, pos.y + size.y - 26), ImColor(24,24,24, 45)); // 65 => 47
					//draw->AddText(tab_title_icon, 18.0f, ImVec2(pos.x + 65, pos.y + 14), ImColor(0, 0, 255), tab_icon);
					//Draw footer
					draw->AddRectFilled(ImVec2(pos.x, pos.y + 640), ImVec2(pos.x + 803, pos.y + 666), ImColor(235, 235, 235));
					draw->AddText(medium, 15.0f, ImVec2(pos.x + 10, pos.y + 645), ImColor(0.0f, 0.0f, 0.0f), "whitetail counter-strike");
					draw->AddText(medium, 15.0f, ImVec2(pos.x + 720, pos.y + 645), ImColor(0.0f, 0.0f, 0.0f), "");
					//draw->AddLine(ImVec2(pos.x, pos.y + 640), ImVec2(pos.x + 803, pos.y + 640), ImColor(24, 24, 24, 45));
					draw->AddLine(ImVec2(pos.x, pos.y + 640), ImVec2(pos.x + 803, pos.y + 640), ImColor(21, 112, 232, 50));
                    draw->AddRect(pos + ImVec2(1, 1), pos + size - ImVec2(1, 1), ImColor(1.0f, 1.0f, 1.0f, 0.03f), style.WindowRounding);

					ImGui::AddShadow(ImVec2(0, 0), ImVec2(800, 655), 20, 4, 7, 2, 20, ImColor(0, 0, 0));

					ImGui::SetCursorPos({ 310, 18 });
					ImGui::Text("UNDER DEVELOPMENT - UNSTABLE!");

						ImGui::SetCursorPos({ 8, 56 });
						ImGui::BeginGroup(); {
							//if (elements::tab("A", tab == Legitbot)) { tab = Legitbot; }
							if (elements::tab("A", tab == Legitbot)) { tab = Legitbot; }
							if (elements::tab("B", tab == Ragebot)) { tab = Ragebot; }
							if (elements::tab("C", tab == AntiAim)) { tab = AntiAim; }
							if (elements::tab("D", tab == Visuals)) { tab = Visuals; }
							if (elements::tab("E", tab == IChanger)) { tab = IChanger; }
							if (elements::tab("F", tab == Misc)) { tab = Misc; }
							if (elements::tab("G", tab == Pressets)) { tab = Pressets; }
						} ImGui::EndGroup();

						ImGui::SetCursorPos({728, 0});
						if (elements::iconButton("S", tab == Settings)) { tab = Settings; }

						

						switch (tab) {
						case Legitbot:


							ImGui::SetCursorPos({ 83, 68 });
							ImGui::BeginGroup(); {
								draw->AddRectFilled(ImVec2(pos.x + 97, pos.y + 64), ImVec2(pos.x + 777, pos.y + 134), ImColor(245, 245, 245), style.WindowRounding, ImDrawFlags_RoundCornersAll);
								draw->AddRect(ImVec2(pos.x + 97, pos.y + 64), ImVec2(pos.x + 777, pos.y + 134), ImColor(24, 24, 24, 45), style.WindowRounding, ImDrawFlags_RoundCornersAll);
								if (elements::weaponSelect("a", legitgroup == Pistols)) { legitgroup = Pistols; }
								ImGui::SameLine();
								if (elements::weaponSelect("6", legitgroup == SMG)) { legitgroup = SMG; }
								ImGui::SameLine();
								if (elements::weaponSelect("7", legitgroup == Rifles)) { legitgroup = Rifles; }
								ImGui::SameLine();
								if (elements::weaponSelect("8", legitgroup == Shotguns)) { legitgroup = Shotguns; }
								ImGui::SameLine();
								if (elements::weaponSelect("9", legitgroup == Heavy)) { legitgroup = Heavy; }
								ImGui::SameLine();
								if (elements::weaponSelect("#", legitgroup == Snipers)) { legitgroup = Snipers; }
							}ImGui::EndGroup();

							switch (legitgroup) {
							case Pistols:
								ImGui::SetCursorPos({ 86, 152 });
								e_elements::begin_child("Aim Assist", ImVec2(347, 282)); {
									//ImGui::Checkbox("legit test 1", &boolean);

									//ImGui::Combo("TEST  COMBO", &combo, viewmodel_chams, IM_ARRAYSIZE(viewmodel_chams));
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 152 });
								e_elements::begin_child("Triggerbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 450 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 173)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 341 });
								e_elements::begin_child("Semi-Rage", ImVec2(347, 282)); {

								}
								e_elements::end_child();

							}


							switch (legitgroup) {
							case SMG:
								ImGui::SetCursorPos({ 86, 152 });
								e_elements::begin_child("Aim Assist", ImVec2(347, 282)); {
									//ImGui::Checkbox("legit test 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 152 });
								e_elements::begin_child("Triggerbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 450 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 173)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 341 });
								e_elements::begin_child("Semi-Rage", ImVec2(347, 282)); {

								}
								e_elements::end_child();
							}


							switch (legitgroup) {
							case Rifles:
								ImGui::SetCursorPos({ 86, 152 });
								e_elements::begin_child("Aim Assist", ImVec2(347, 282)); {
									//ImGui::Checkbox("legit test 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 152 });
								e_elements::begin_child("Triggerbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 450 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 173)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 341 });
								e_elements::begin_child("Semi-Rage", ImVec2(347, 282)); {

								}
								e_elements::end_child();

							}


							switch (legitgroup) {
							case Shotguns:
								ImGui::SetCursorPos({ 86, 152 });
								e_elements::begin_child("Aim Assist", ImVec2(347, 282)); {
									//ImGui::Checkbox("legit test 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 152 });
								e_elements::begin_child("Triggerbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 450 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 173)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 341 });
								e_elements::begin_child("Semi-Rage", ImVec2(347, 282)); {

								}
								e_elements::end_child();

							}


							switch (legitgroup) {
							case Heavy:
								ImGui::SetCursorPos({ 86, 152 });
								e_elements::begin_child("Aim Assist", ImVec2(347, 282)); {
									//ImGui::Checkbox("legit test 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 152 });
								e_elements::begin_child("Triggerbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 450 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 173)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 341 });
								e_elements::begin_child("Semi-Rage", ImVec2(347, 282)); {

								}
								e_elements::end_child();

							}


							switch (legitgroup) {
							case Snipers:
								ImGui::SetCursorPos({ 86, 152 });
								e_elements::begin_child("Aim Assist", ImVec2(347, 282)); {
									//ImGui::Checkbox("legit test 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 152 });
								e_elements::begin_child("Triggerbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 450 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 173)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 341 });
								e_elements::begin_child("Semi-Rage", ImVec2(347, 282)); {

								}
								e_elements::end_child();

							}



						}


						switch (tab) {
						case Ragebot:
							
							ImGui::SetCursorPos({ 85, 68 });
							ImGui::BeginGroup(); {
								draw->AddRectFilled(ImVec2(pos.x + 97, pos.y + 64), ImVec2(pos.x + 777, pos.y + 134), ImColor(245, 245, 245), style.WindowRounding, ImDrawFlags_RoundCornersAll);
								draw->AddRect(ImVec2(pos.x + 97, pos.y + 64), ImVec2(pos.x + 777, pos.y + 134), ImColor(24, 24, 24, 45), style.WindowRounding, ImDrawFlags_RoundCornersAll);
								if (elements::weaponSelectRage("1", ragegroup == RageGeneral)) { ragegroup = RageGeneral; }
								ImGui::SameLine();
								if (elements::weaponSelectRage("3", ragegroup == LightPistols)) { ragegroup = LightPistols; }
								ImGui::SameLine();
								if (elements::weaponSelectRage("4", ragegroup == HeavyPistols)) { ragegroup = HeavyPistols; }
								ImGui::SameLine();
								if (elements::weaponSelectRage("6", ragegroup == RageSMGS)) { ragegroup = RageSMGS; }
								ImGui::SameLine();
								if (elements::weaponSelectRage("7", ragegroup == RageRifles)) { ragegroup = RageRifles; }
								ImGui::SameLine();
								if (elements::weaponSelectRage("8", ragegroup == RageShotguns)) { ragegroup = RageShotguns; }
								ImGui::SameLine();
								if (elements::weaponSelectRage("9", ragegroup == RageHeavy)) { ragegroup = RageHeavy; }
								ImGui::SameLine();
								if (elements::weaponSelectRage("!", ragegroup == RageAutoSnipers)) { ragegroup = RageAutoSnipers; }

							}ImGui::EndGroup();
							switch (ragegroup) {
							case RageGeneral:
								
								ImGui::SetCursorPos({ 102, 140 });
								ImGui::Checkbox("Enable Ragebot", &enable);
								if (enable == true) { disabled == false; }
								else { disabled == true; }
									
								

								ImGui::BeginDisabled(disabled);
								ImGui::SetCursorPos({ 443, 160 });
								e_elements::begin_child("Accuracy", ImVec2(347, 282)); {
									//ImGui::Checkbox("boolean 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 160 });
								e_elements::begin_child("Aimbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 456 });
								e_elements::begin_child("Other", ImVec2(347, 168)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 347 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 277)); {

								}
								e_elements::end_child();
								ImGui::EndDisabled();
								

							}


							switch (ragegroup) {
							case LightPistols:
								ImGui::SetCursorPos({ 102, 140 });
								ImGui::Checkbox("Enable Ragebot", &enable);
								ImGui::SameLine();
								ImGui::Checkbox("Configure Manually", &enable);
								if (enable == true) { disabled == false; }
								else { disabled == true; }



								ImGui::BeginDisabled(disabled);
								ImGui::SetCursorPos({ 443, 160 });
								e_elements::begin_child("Accuracy", ImVec2(347, 282)); {
									//ImGui::Checkbox("boolean 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 160 });
								e_elements::begin_child("Aimbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 456 });
								e_elements::begin_child("Other", ImVec2(347, 168)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 347 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 277)); {

								}
								e_elements::end_child();
								ImGui::EndDisabled();

							}


							switch (ragegroup) {
							case HeavyPistols:
								ImGui::SetCursorPos({ 102, 140 });
								ImGui::Checkbox("Enable Ragebot", &enable);
								ImGui::SameLine();
								ImGui::Checkbox("Configure Manually", &enable);
								if (enable == true) { disabled == false; }
								else { disabled == true; }



								ImGui::BeginDisabled(disabled);
								ImGui::SetCursorPos({ 443, 160 });
								e_elements::begin_child("Accuracy", ImVec2(347, 282)); {
									//ImGui::Checkbox("boolean 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 160 });
								e_elements::begin_child("Aimbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 456 });
								e_elements::begin_child("Other", ImVec2(347, 168)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 347 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 277)); {

								}
								e_elements::end_child();
								ImGui::EndDisabled();

							}


							switch (ragegroup) {
							case RageSMGS:
								ImGui::SetCursorPos({ 102, 140 });
								ImGui::Checkbox("Enable Ragebot", &enable);
								ImGui::SameLine();
								ImGui::Checkbox("Configure Manually", &enable);
								if (enable == true) { disabled == false; }
								else { disabled == true; }



								ImGui::BeginDisabled(disabled);
								ImGui::SetCursorPos({ 443, 160 });
								e_elements::begin_child("Accuracy", ImVec2(347, 282)); {
									//ImGui::Checkbox("boolean 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 160 });
								e_elements::begin_child("Aimbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 456 });
								e_elements::begin_child("Other", ImVec2(347, 168)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 347 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 277)); {

								}
								e_elements::end_child();
								ImGui::EndDisabled();

							}


							switch (ragegroup) {
							case RageRifles:
								ImGui::SetCursorPos({ 102, 140 });
								ImGui::Checkbox("Enable Ragebot", &enable);
								ImGui::SameLine();
								ImGui::Checkbox("Configure Manually", &enable);
								if (enable == true) { disabled == false; }
								else { disabled == true; }



								ImGui::BeginDisabled(disabled);
								ImGui::SetCursorPos({ 443, 160 });
								e_elements::begin_child("Accuracy", ImVec2(347, 282)); {
									//ImGui::Checkbox("boolean 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 160 });
								e_elements::begin_child("Aimbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 456 });
								e_elements::begin_child("Other", ImVec2(347, 168)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 347 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 277)); {

								}
								e_elements::end_child();
								ImGui::EndDisabled();

							}


							switch (ragegroup) {
							case RageShotguns:
								ImGui::SetCursorPos({ 102, 140 });
								ImGui::Checkbox("Enable Ragebot", &enable);
								ImGui::SameLine();
								ImGui::Checkbox("Configure Manually", &enable);
								if (enable == true) { disabled == false; }
								else { disabled == true; }



								ImGui::BeginDisabled(disabled);
								ImGui::SetCursorPos({ 443, 160 });
								e_elements::begin_child("Accuracy", ImVec2(347, 282)); {
									//ImGui::Checkbox("boolean 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 160 });
								e_elements::begin_child("Aimbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 456 });
								e_elements::begin_child("Other", ImVec2(347, 168)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 347 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 277)); {

								}
								e_elements::end_child();
								ImGui::EndDisabled();

							}

							switch (ragegroup) {
							case RageHeavy:
								ImGui::SetCursorPos({ 102, 140 });
								ImGui::Checkbox("Enable Ragebot", &enable);
								ImGui::SameLine();
								ImGui::Checkbox("Configure Manually", &enable);
								if (enable == true) { disabled == false; }
								else { disabled == true; }



								ImGui::BeginDisabled(disabled);
								ImGui::SetCursorPos({ 443, 160 });
								e_elements::begin_child("Accuracy", ImVec2(347, 282)); {
									//ImGui::Checkbox("boolean 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 160 });
								e_elements::begin_child("Aimbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 456 });
								e_elements::begin_child("Other", ImVec2(347, 168)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 347 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 277)); {

								}
								e_elements::end_child();
								ImGui::EndDisabled();

							}

							switch (ragegroup) {
							case RageAutoSnipers:
								ImGui::SetCursorPos({ 102, 140 });
								ImGui::Checkbox("Enable Ragebot", &enable);
								ImGui::SameLine();
								ImGui::Checkbox("Configure Manually", &enable);
								if (enable == true) { disabled == false; }
								else { disabled == true; }



								ImGui::BeginDisabled(disabled);
								ImGui::SetCursorPos({ 443, 160 });
								e_elements::begin_child("Accuracy", ImVec2(347, 282)); {
									//ImGui::Checkbox("boolean 1", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 160 });
								e_elements::begin_child("Aimbot", ImVec2(347, 173)); {
									//ImGui::Checkbox("boolean 2", &boolean);
								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 443, 456 });
								e_elements::begin_child("Other", ImVec2(347, 168)); {

								}
								e_elements::end_child();

								ImGui::SetCursorPos({ 86, 347 });
								e_elements::begin_child("Hitboxes", ImVec2(347, 277)); {

								}
								e_elements::end_child();
								ImGui::EndDisabled();

							}



						}
						switch (tab) {
						case Visuals:

							ImGui::SetCursorPos({ 102, 62 });
							//ImGui::Checkbox("Enable Visuals", &enable);
							ImGui::SetCursorPos({ 600, 62 });
							ImGui::Text("COMBO NEEDS TO BE ADDED!");

							ImGui::SetCursorPos({ 86, 93 });
							e_elements::begin_child("Overlay", ImVec2(347, 282)); {
								//ImGui::Checkbox("boolean 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 443, 93 });
							e_elements::begin_child("World", ImVec2(347, 233)); {

							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 443, 342 });
							e_elements::begin_child("Extra", ImVec2(347, 282)); {
								//ImGui::Checkbox("boolean 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 86, 391 });
							e_elements::begin_child("Chams", ImVec2(347, 233)); {

							}
							e_elements::end_child();

						}

						switch (tab) {
						case IChanger:

							ImGui::SetCursorPos({ 102, 62 });
							//ImGui::Checkbox("Enable Skin Changer", &enable);
							ImGui::SetCursorPos({ 600, 62 });
							ImGui::Text("COMBO NEEDS TO BE ADDED!");

							ImGui::SetCursorPos({ 443, 93 });
							e_elements::begin_child("Skin Changer", ImVec2(347, 532)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 86, 93 });
							e_elements::begin_child("Preview", ImVec2(347, 250)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 86, 359 });
							e_elements::begin_child("Skin List", ImVec2(347, 266)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

						}
						switch (tab) {
						case AntiAim:

							ImGui::SetCursorPos({ 102, 62 });
							//ImGui::Checkbox("Enable Anti-Aim", &enable);
							//if (enable == true) { disabled == false; }
							//else { disabled == true; }

							ImGui::SetCursorPos({ 600, 62 });
							ImGui::Text("COMBO NEEDS TO BE ADDED!");
							//ImGui::Combo("Combobox", &combo, viewmodel_chams, IM_ARRAYSIZE(viewmodel_chams));

							
							ImGui::SetCursorPos({ 86, 93 });
							e_elements::begin_child("General", ImVec2(347, 532)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 443, 93 });
							e_elements::begin_child("Advanced", ImVec2(347, 282)); {
								//ImGui::Checkbox("boolean 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 443, 391 });
							e_elements::begin_child("Other", ImVec2(347, 233)); {

							}
							e_elements::end_child();



						}


						switch (tab) {
						case Misc:

							ImGui::SetCursorPos({ 86, 63 });
							e_elements::begin_child("Visual", ImVec2(347, 273)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 86, 352 });
							e_elements::begin_child("Movement", ImVec2(347, 273)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 443, 63 });
							e_elements::begin_child("Fakelag", ImVec2(347, 273)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 443, 352 });
							e_elements::begin_child("Other", ImVec2(347, 273)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

						}

						switch (tab) {
						case Pressets:

							ImGui::SetCursorPos({ 86, 63 });
							e_elements::begin_child("Manage Scripts", ImVec2(347, 562)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();

							ImGui::SetCursorPos({ 443, 63 });
							e_elements::begin_child("Manage Configs", ImVec2(347, 562)); {
								//ImGui::Checkbox("legit test 1", &boolean);
							}
							e_elements::end_child();


						}
						
						switch (tab) {
						case Settings:

							ImGui::SetCursorPos({ 360, 333 });
							ImGui::Text("UNDER DEVELOPMENT!");

						}
						
						

						

						ImGui::PopFont();
						ImGui::PopStyleVar();
					
				
				ImGui::PopStyleVar(2);
			
			

		
		ImGui::End();
		ImGui::PopStyleVar(2);
		
		
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		//ImGui::RenderNotifications();
		
	}
	//ImGui::RenderNotifications();




extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);
LRESULT CALLBACK WindowProcess(HWND window, UINT message, WPARAM wideParam, LPARAM longParam)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		u::open = !u::open;

	if (u::open && ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam))
		return 1L;

	return CallWindowProc(u::WindowProcessOriginal, window, message, wideParam, longParam);
}
