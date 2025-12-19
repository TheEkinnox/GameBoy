#include "gui.h"

#include "macros.h"

#include <algorithm>
#include <cstdio>

NO_WARNINGS_PUSH
#include <rlImGui.h>
#include <rlImGuiColors.h>
NO_WARNINGS_POP

static struct
{
    bool shouldClose = false;
    bool useDarkMode = true;
} s_uiGlob;

static constexpr ImVec4 setAlpha(ImVec4 color, const float alpha)
{
    color.w = alpha;
    return color;
}

static void setColorsLight(ImVec4* colors)
{
    colors[ImGuiCol_Text]                       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]               = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]                   = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[ImGuiCol_ChildBg]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.02f);
    colors[ImGuiCol_PopupBg]                    = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
    colors[ImGuiCol_Border]                     = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_BorderShadow]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                    = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive]              = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg]                    = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[ImGuiCol_TitleBgActive]              = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]           = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]                  = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]                = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]              = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabHovered]       = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabActive]        = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    colors[ImGuiCol_CheckMark]                  = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SliderGrabActive]           = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
    colors[ImGuiCol_Button]                     = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive]               = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header]                     = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered]              = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive]               = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]                  = ImVec4(0.39f, 0.39f, 0.39f, 0.31f);
    colors[ImGuiCol_SeparatorHovered]           = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
    colors[ImGuiCol_SeparatorActive]            = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
    colors[ImGuiCol_ResizeGrip]                 = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
    colors[ImGuiCol_ResizeGripHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]           = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_InputTextCursor]            = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TabHovered]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_Tab]                        = ImVec4(0.76f, 0.80f, 0.84f, 0.93f);
    colors[ImGuiCol_TabSelected]                = ImVec4(0.60f, 0.73f, 0.88f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline]        = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TabDimmed]                  = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
    colors[ImGuiCol_TabDimmedSelected]          = ImVec4(0.74f, 0.82f, 0.91f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.26f, 0.59f, 1.00f, 0.00f);
    colors[ImGuiCol_PlotLines]                  = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]           = ImVec4(0.93f, 0.20f, 0.19f, 1.00f);
    colors[ImGuiCol_PlotHistogram]              = ImVec4(0.37f, 0.66f, 0.69f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]       = ImVec4(0.93f, 0.57f, 0.19f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]              = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]          = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
    colors[ImGuiCol_TableBorderLight]           = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
    colors[ImGuiCol_TableRowBg]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]              = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
    colors[ImGuiCol_TextLink]                   = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]             = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_TreeLines]                  = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
    colors[ImGuiCol_DragDropTarget]             = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_DragDropTargetBg]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_UnsavedMarker]              = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavCursor]                  = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_NavWindowingHighlight]      = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]          = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]           = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

static void setColorsDark(ImVec4* colors)
{
    colors[ImGuiCol_Text]                       = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]               = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]                   = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                    = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                     = ImVec4(0.43f, 0.43f, 0.50f, 0.24f);
    colors[ImGuiCol_BorderShadow]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                    = ImVec4(0.17f, 0.18f, 0.19f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]             = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
    colors[ImGuiCol_FrameBgActive]              = ImVec4(0.61f, 0.61f, 0.61f, 0.67f);
    colors[ImGuiCol_TitleBg]                    = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]              = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]           = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]                  = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]                = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]              = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]       = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]        = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]                  = setAlpha(colors[ImGuiCol_FrameBgActive], 1.f);
    colors[ImGuiCol_SliderGrab]                 = colors[ImGuiCol_ScrollbarGrabActive];
    colors[ImGuiCol_SliderGrabActive]           = colors[ImGuiCol_CheckMark];
    colors[ImGuiCol_Button]                     = ImVec4(0.80f, 0.80f, 0.80f, 0.32f);
    colors[ImGuiCol_ButtonHovered]              = setAlpha(colors[ImGuiCol_SliderGrab], .80f);
    colors[ImGuiCol_ButtonActive]               = colors[ImGuiCol_SliderGrab];
    colors[ImGuiCol_Header]                     = colors[ImGuiCol_Button];
    colors[ImGuiCol_HeaderHovered]              = colors[ImGuiCol_ButtonHovered];
    colors[ImGuiCol_HeaderActive]               = colors[ImGuiCol_ButtonActive];
    colors[ImGuiCol_Separator]                  = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]           = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive]            = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip]                 = setAlpha(colors[ImGuiCol_FrameBgActive], .20f);
    colors[ImGuiCol_ResizeGripHovered]          = setAlpha(colors[ImGuiCol_ResizeGrip], .67f);
    colors[ImGuiCol_ResizeGripActive]           = setAlpha(colors[ImGuiCol_ResizeGrip], .95f);
    colors[ImGuiCol_InputTextCursor]            = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TabHovered]                 = setAlpha(colors[ImGuiCol_FrameBgActive], .80f);
    colors[ImGuiCol_Tab]                        = ImVec4(0.37f, 0.37f, 0.37f, 0.86f);
    colors[ImGuiCol_TabSelected]                = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline]        = colors[ImGuiCol_TabHovered];
    colors[ImGuiCol_TabDimmed]                  = ImVec4(0.11f, 0.11f, 0.11f, 0.97f);
    colors[ImGuiCol_TabDimmedSelected]          = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
    colors[ImGuiCol_PlotLines]                  = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]           = ImVec4(0.93f, 0.20f, 0.19f, 1.00f);
    colors[ImGuiCol_PlotHistogram]              = ImVec4(0.37f, 0.66f, 0.69f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]       = ImVec4(0.93f, 0.57f, 0.19f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]              = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]          = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]           = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]              = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink]                   = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]             = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_TreeLines]                  = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_DragDropTarget]             = setAlpha(colors[ImGuiCol_SeparatorActive], .90f);
    colors[ImGuiCol_DragDropTargetBg]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_UnsavedMarker]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_NavCursor]                  = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]      = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]          = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]           = ImVec4(0.00f, 0.00f, 0.00f, 0.37f);
}

static void setCustomStyle(const bool darkMode)
{
    ImGuiStyle style;
    ImVec4* colors = style.Colors;

    if (darkMode)
        setColorsDark(colors);
    else
        setColorsLight(colors);

    // Borders
    style.WindowBorderSize = 1.f;
    style.ChildBorderSize = 1.f;
    style.PopupBorderSize = 1.f;
    style.FrameBorderSize = darkMode ? .0f : 1.f;
    style.SeparatorTextBorderSize = 2.f;

    // Rounding
    style.WindowRounding = 4.f;
    style.ChildRounding = style.WindowRounding;
    style.FrameRounding = style.WindowRounding;
    style.PopupRounding = style.FrameRounding + std::min(style.FramePadding.x, style.FramePadding.y);
    style.GrabRounding = 5.f;
    style.DragDropTargetRounding = style.FrameRounding;

    ImGui::GetStyle() = style; // Setting this at the end to make sure ALL values are overwritten
}

void initUI(const bool useDarkMode)
{
    s_uiGlob.useDarkMode = useDarkMode;
    rlImGuiBeginInitImGui();
    setCustomStyle(useDarkMode);
    rlImGuiEndInitImGui();
}

void clearScreen()
{
    ClearBackground(rlImGuiColors::Convert(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg)));
}

static bool isAnyCtrlDown()
{
    return IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
}

static void showLoadRomDialog()
{
    // TODO: Load ROM
    fputs("Loading ROM\n", stdout);
}

static void handleShortcuts()
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (isAnyCtrlDown() && IsKeyPressed(KEY_O))
    {
        showLoadRomDialog();
    }
}

void drawUI()
{
    rlImGuiBegin();
    {
#if DEBUG
        ImGui::ShowDemoWindow();
#endif

        handleShortcuts();

        ImGui::BeginMainMenuBar();
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Load ROM", "Ctrl+O"))
                {
                    showLoadRomDialog();
                }

                if (ImGui::MenuItem("Exit", "Alt+F4"))
                    s_uiGlob.shouldClose = true;

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Display"))
            {
                if (ImGui::MenuItem("Dark Mode", "", &s_uiGlob.useDarkMode))
                    setCustomStyle(s_uiGlob.useDarkMode);

                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }
    rlImGuiEnd();
}

bool shouldClose()
{
    return s_uiGlob.shouldClose || WindowShouldClose();
}


void shutdownUI()
{
    rlImGuiShutdown();
}
