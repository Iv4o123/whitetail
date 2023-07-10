#include "nav_elements.h"

struct tab_element {
    float element_opacity, rect_opacity, text_opacity;
};

bool elements::tab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = tab_icons->CalcTextSizeA(15.0f, FLT_MAX, 0.0f, name);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + 45, pos.y + 74));
    const ImRect rectblue (pos, ImVec2(pos.x + 55, pos.y + 85));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.04f : hovered ? 0.01f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.rect_opacity = ImLerp(it_anim->second.rect_opacity, (boolean ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : hovered ? 0.5f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), 6.0f);
    window->DrawList->AddText(tab_icons, 30.0f, (rect.Min + rect.Max - label_size) / 2, ImColor(0.0f, 0.0f, 0.0f, it_anim->second.text_opacity), name); //15.0f size

    window->DrawList->AddRectFilled(ImVec2(rectblue.Max.x + 8, rectblue.Min.y + 7), ImVec2(rectblue.Max.x + 8, rectblue.Max.y - 7), ImColor(0.08, 0.44, 0.91, it_anim->second.rect_opacity), 7.0f, ImDrawFlags_RoundCornersLeft);

    return pressed;
}

struct subtab_element {
    float element_opacity, rect_opacity, text_opacity;
};

bool elements::subtab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + 145, pos.y + 32));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 3), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, subtab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.04f : 0.0f), 0.09f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.rect_opacity = ImLerp(it_anim->second.rect_opacity, (boolean ? 1.0f : 0.0f), 0.20f * (1.0f - ImGui::GetIO().DeltaTime));
    //it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), 3.0f);
    window->DrawList->AddText(ImVec2(rect.Min.x + 15, (rect.Min.y + rect.Max.y) / 2 - label_size.y / 2), ImColor(0.0f, 0.0f, .0f, it_anim->second.text_opacity), name);

    window->DrawList->AddRectFilled(ImVec2(rect.Max.x + 4, rect.Min.y + 5), ImVec2(rect.Max.x + 8, rect.Max.y - 1), ImColor(0.0f,0.0f,1.0f, it_anim->second.rect_opacity), 10.0f, ImDrawFlags_RoundCornersLeft);

    return pressed;
}

struct weaponSelect_element {
    float elementopacity, rectopacity, textopacity;
};

bool elements::weaponSelect(const char* name, bool boolean)
{
    
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = weaponIcons->CalcTextSizeA(15.0f, FLT_MAX, 0.0f, name);
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 poss = window->SetWindowPosVal;

    const ImRect rect(pos, ImVec2(pos.x + 100, pos.y + 45));
    //const ImRect rect(pos, ImVec2(pos.x, pos.y));
    const ImRect rectblue(pos, ImVec2(pos.x + 55, pos.y + 85));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, weaponSelect_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.elementopacity = ImLerp(it_anim->second.elementopacity, (boolean ? 0.04f : hovered ? 0.01f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.rectopacity = ImLerp(it_anim->second.rectopacity, (boolean ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.textopacity = ImLerp(it_anim->second.textopacity, (boolean ? 1.0f : hovered ? 0.5f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    //window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(0.0f, 0.0f, 0.0f, it_anim->second.elementopacity), 3.0f);
    window->DrawList->AddText(weaponIcons, 30.0f, ImVec2((rect.Min.x + rect.Max.x) / 2.f - (label_size.x / 2.f), (rect.Min.y + rect.Max.y) / 2.f - (label_size.y / 2.f)), ImColor(0.0f, 0.0f, 0.0f, it_anim->second.textopacity), name); //15.0f size    

    //window->DrawList->AddRectFilled(ImVec2(rectblue.Max.x + 8, rectblue.Min.y + 7), ImVec2(rectblue.Max.x + 8, rectblue.Max.y - 7), ImColor(0.08, 0.44, 0.91, it_anim->second.textopacity), 7.0f, ImDrawFlags_RoundCornersLeft);

    return pressed;
}

struct weaponSelectRage_element {
    float elementopacity, rectopacity, textopacity;
};

bool elements::weaponSelectRage(const char* name, bool boolean)
{

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = weaponIcons->CalcTextSizeA(15.0f, FLT_MAX, 0.0f, name);
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 poss = window->SetWindowPosVal;

    const ImRect rect(pos, ImVec2(pos.x + 73, pos.y + 45));
    //const ImRect rect(pos, ImVec2(pos.x, pos.y));
    const ImRect rectblue(pos, ImVec2(pos.x + 55, pos.y + 85));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, weaponSelectRage_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.elementopacity = ImLerp(it_anim->second.elementopacity, (boolean ? 0.04f : hovered ? 0.01f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.rectopacity = ImLerp(it_anim->second.rectopacity, (boolean ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.textopacity = ImLerp(it_anim->second.textopacity, (boolean ? 1.0f : hovered ? 0.5f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    //window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(0.0f, 0.0f, 0.0f, it_anim->second.elementopacity), 3.0f);
    window->DrawList->AddText(weaponIcons, 30.0f, ImVec2((rect.Min.x + rect.Max.x) / 2.f - (label_size.x / 2.f), (rect.Min.y + rect.Max.y) / 2.f - (label_size.y / 2.f)), ImColor(0.0f, 0.0f, 0.0f, it_anim->second.textopacity), name); //15.0f size    

    //window->DrawList->AddRectFilled(ImVec2(rectblue.Max.x + 8, rectblue.Min.y + 7), ImVec2(rectblue.Max.x + 8, rectblue.Max.y - 7), ImColor(0.08, 0.44, 0.91, it_anim->second.textopacity), 7.0f, ImDrawFlags_RoundCornersLeft);

    return pressed;
}

struct IconButtonElements {
    float el_opacity, rec_opacity, txt_opacity;
};

bool elements::iconButton(const char* name, bool boolean)
{

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = settingsIcon->CalcTextSizeA(15.0f, FLT_MAX, 0.0f, name);
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 poss = window->SetWindowPosVal;

    const ImRect rect(pos, ImVec2(pos.x + 100, pos.y + 45));
    //const ImRect rect(pos, ImVec2(pos.x, pos.y));
    const ImRect rectblue(pos, ImVec2(pos.x + 55, pos.y + 85));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, IconButtonElements> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    //it_anim->second.el_opacity = ImLerp(it_anim->second.el_opacity, (boolean ? 0.04f : hovered ? 0.01f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    //it_anim->second.rec_opacity = ImLerp(it_anim->second.rec_opacity, (boolean ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.txt_opacity = ImLerp(it_anim->second.txt_opacity, (boolean ? 1.0f : hovered ? 0.5f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    //window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(0.0f, 0.0f, 0.0f, it_anim->second.elementopacity), 3.0f);
    window->DrawList->AddText(settingsIcon, 20.0f, ImVec2((rect.Min.x + rect.Max.x) / 2.f - (label_size.x / 2.f), (rect.Min.y + rect.Max.y) / 2.f - (label_size.y / 2.f)), ImColor(0.0f, 0.0f, 0.0f, it_anim->second.txt_opacity), name); //15.0f size    

    //window->DrawList->AddRectFilled(ImVec2(rectblue.Max.x + 8, rectblue.Min.y + 7), ImVec2(rectblue.Max.x + 8, rectblue.Max.y - 7), ImColor(0.08, 0.44, 0.91, it_anim->second.textopacity), 7.0f, ImDrawFlags_RoundCornersLeft);

    return pressed;
}