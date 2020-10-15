// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <sol/sol.hpp>

#include "tolua/LuaImGui.hpp"
#include "imgui/ImGui.hpp"

void tnt::lua::loadImGui(sol::state_view lua_)
{
    auto imgui{lua_["imgui"].get_or_create<sol::table>()};

    imgui.new_enum("win_flags",
                   "colapsible", ImGui::WindowFlags::Collapsible,
                   "closable", ImGui::WindowFlags::Closable,
                   "resizable", ImGui::WindowFlags::Resizable,
                   "movable", ImGui::WindowFlags::Movable,
                   "with_titlebar", ImGui::WindowFlags::WithTitleBar,
                   "opaque_bg", ImGui::WindowFlags::OpaqueBackground,
                   "widget_first", ImGui::WindowFlags::WidgetThenText);

    imgui["init"] = &tnt_imgui_init;

    imgui["Begin"] = sol::overload(
        [](Window const &win, std::string_view name, int x, int y) -> bool {
            return ImGui::Begin(win, name, x, y);
        },
        &ImGui::Begin);
    imgui["End"] = &ImGui::End;

    imgui["begin_section"] = &ImGui::BeginSection;
    imgui["end_section"] = &ImGui::EndSection;

    imgui["begin_list"] = &ImGui::BeginList;
    imgui["list_item"] = &ImGui::list_item;
    imgui["end_list"] = &ImGui::EndList;

    imgui["begin_menubar"] = &ImGui::BeginMenuBar;
    imgui["menu_button"] = &ImGui::menu_button;
    imgui["menu_item"] = &ImGui::menu_item;
    imgui["end_menubar"] = &ImGui::EndMenuBar;

    imgui["button"] = &ImGui::button;

    imgui["slider_int"] = &ImGui::slider_int;
    imgui["slider_float"] = &ImGui::slider_float;

    imgui["hslider_int"] = &ImGui::hslider_int;
    imgui["hslider_float"] = &ImGui::hslider_float;
    imgui["hslider_int2"] = &ImGui::hslider_int2;
    imgui["hslider_float2"] = &ImGui::hslider_float2;
    imgui["hslider_vec"] = &ImGui::hslider_vec;

    imgui["checkbox"] = &ImGui::checkbox;

    imgui["progress_bar"] = &ImGui::progress_bar;
    imgui["newline"] = &ImGui::newline;

    imgui["text"] = &ImGui::text;
    imgui["colored_text"] = &ImGui::colored_text;
}