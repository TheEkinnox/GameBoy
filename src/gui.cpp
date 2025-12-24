#include "gui.h"

#include "emu.h"

#include <algorithm>
#include <cstdio>

NO_WARNINGS_PUSH
#include <imgui_memory_editor.h>
#include <rlImGui.h>
#include <rlImGuiColors.h>
NO_WARNINGS_POP

CLANG_IGNORE_WARNING_PUSH("-Wglobal-constructors")
static struct
{
    // Emu
    MemoryEditor memoryEditor{};
    Emu* ctx       = nullptr;
    bool showCPU   = IS_DEBUG;
    bool showStats = false;

    // App
    bool shouldClose = false;
    bool useDarkMode = true;
} s_uiGlob;
CLANG_IGNORE_WARNING_POP

static constexpr ImVec4 setAlpha(ImVec4 color, const float alpha)
{
    color.w = alpha;
    return color;
}

static void setColorsLight(ImVec4* colors)
{
    colors[ImGuiCol_Text]                      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]              = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]                  = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[ImGuiCol_ChildBg]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.02f);
    colors[ImGuiCol_PopupBg]                   = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
    colors[ImGuiCol_Border]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_BorderShadow]              = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]            = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive]             = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg]                   = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[ImGuiCol_TitleBgActive]             = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]          = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]                 = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]               = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]             = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabHovered]      = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabActive]       = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    colors[ImGuiCol_CheckMark]                 = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]                = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SliderGrabActive]          = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
    colors[ImGuiCol_Button]                    = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive]              = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header]                    = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]                 = ImVec4(0.39f, 0.39f, 0.39f, 0.31f);
    colors[ImGuiCol_SeparatorHovered]          = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
    colors[ImGuiCol_SeparatorActive]           = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
    colors[ImGuiCol_ResizeGrip]                = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
    colors[ImGuiCol_ResizeGripHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]          = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_InputTextCursor]           = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TabHovered]                = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_Tab]                       = ImVec4(0.76f, 0.80f, 0.84f, 0.93f);
    colors[ImGuiCol_TabSelected]               = ImVec4(0.60f, 0.73f, 0.88f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TabDimmed]                 = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
    colors[ImGuiCol_TabDimmedSelected]         = ImVec4(0.74f, 0.82f, 0.91f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.26f, 0.59f, 1.00f, 0.00f);
    colors[ImGuiCol_PlotLines]                 = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]          = ImVec4(0.93f, 0.20f, 0.19f, 1.00f);
    colors[ImGuiCol_PlotHistogram]             = ImVec4(0.37f, 0.66f, 0.69f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]      = ImVec4(0.93f, 0.57f, 0.19f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]             = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]         = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
    colors[ImGuiCol_TableBorderLight]          = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
    colors[ImGuiCol_TableRowBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]             = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
    colors[ImGuiCol_TextLink]                  = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]            = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_TreeLines]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
    colors[ImGuiCol_DragDropTarget]            = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_DragDropTargetBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_UnsavedMarker]             = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavCursor]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_NavWindowingHighlight]     = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]         = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]          = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

static void setColorsDark(ImVec4* colors)
{
    colors[ImGuiCol_Text]                      = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]              = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]                  = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                   = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                    = ImVec4(0.43f, 0.43f, 0.50f, 0.24f);
    colors[ImGuiCol_BorderShadow]              = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                   = ImVec4(0.17f, 0.18f, 0.19f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]            = ImVec4(0.61f, 0.61f, 0.61f, 0.40f);
    colors[ImGuiCol_FrameBgActive]             = ImVec4(0.61f, 0.61f, 0.61f, 0.67f);
    colors[ImGuiCol_TitleBg]                   = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]             = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]          = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]                 = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]               = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]             = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]      = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]       = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]                 = setAlpha(colors[ImGuiCol_FrameBgActive], 1.f);
    colors[ImGuiCol_SliderGrab]                = colors[ImGuiCol_ScrollbarGrabActive];
    colors[ImGuiCol_SliderGrabActive]          = colors[ImGuiCol_CheckMark];
    colors[ImGuiCol_Button]                    = ImVec4(0.80f, 0.80f, 0.80f, 0.32f);
    colors[ImGuiCol_ButtonHovered]             = setAlpha(colors[ImGuiCol_SliderGrab], .80f);
    colors[ImGuiCol_ButtonActive]              = colors[ImGuiCol_SliderGrab];
    colors[ImGuiCol_Header]                    = colors[ImGuiCol_Button];
    colors[ImGuiCol_HeaderHovered]             = colors[ImGuiCol_ButtonHovered];
    colors[ImGuiCol_HeaderActive]              = colors[ImGuiCol_ButtonActive];
    colors[ImGuiCol_Separator]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]          = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive]           = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip]                = setAlpha(colors[ImGuiCol_FrameBgActive], .20f);
    colors[ImGuiCol_ResizeGripHovered]         = setAlpha(colors[ImGuiCol_ResizeGrip], .67f);
    colors[ImGuiCol_ResizeGripActive]          = setAlpha(colors[ImGuiCol_ResizeGrip], .95f);
    colors[ImGuiCol_InputTextCursor]           = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TabHovered]                = setAlpha(colors[ImGuiCol_FrameBgActive], .80f);
    colors[ImGuiCol_Tab]                       = ImVec4(0.37f, 0.37f, 0.37f, 0.86f);
    colors[ImGuiCol_TabSelected]               = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline]       = colors[ImGuiCol_TabHovered];
    colors[ImGuiCol_TabDimmed]                 = ImVec4(0.11f, 0.11f, 0.11f, 0.97f);
    colors[ImGuiCol_TabDimmedSelected]         = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
    colors[ImGuiCol_PlotLines]                 = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]          = ImVec4(0.93f, 0.20f, 0.19f, 1.00f);
    colors[ImGuiCol_PlotHistogram]             = ImVec4(0.37f, 0.66f, 0.69f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]      = ImVec4(0.93f, 0.57f, 0.19f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]             = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]         = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]          = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]             = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink]                  = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]            = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_TreeLines]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_DragDropTarget]            = setAlpha(colors[ImGuiCol_SeparatorActive], .90f);
    colors[ImGuiCol_DragDropTargetBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_UnsavedMarker]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_NavCursor]                 = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]     = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]         = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.37f);
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
    style.WindowBorderSize        = 1.f;
    style.ChildBorderSize         = 1.f;
    style.PopupBorderSize         = 1.f;
    style.FrameBorderSize         = darkMode ? .0f : 1.f;
    style.SeparatorTextBorderSize = 2.f;

    // Rounding
    style.WindowRounding         = 4.f;
    style.ChildRounding          = style.WindowRounding;
    style.FrameRounding          = style.WindowRounding;
    style.PopupRounding          = style.FrameRounding + std::min(style.FramePadding.x, style.FramePadding.y);
    style.GrabRounding           = 5.f;
    style.DragDropTargetRounding = style.FrameRounding;

    ImGui::GetStyle() = style; // Setting this at the end to make sure ALL values are overwritten
}

static ImU8 memReadFn(const ImU8*, const size_t addr, void*)
{
    return s_uiGlob.ctx->getMMU().peekByte(static_cast<uint16_t>(addr));
}

static ImU32 memBgColorFn(const ImU8*, const size_t addr, void*)
{
    Emu& ctx = *s_uiGlob.ctx;

    if (addr == ctx.getPC())
        return s_uiGlob.useDarkMode ? 0xFF7E1F70 : 0xFFFFADFE;

    if (addr == ctx.getSP())
        return s_uiGlob.useDarkMode ? 0xFF101D52 : 0xFF9EABE0;

    if (ctx.getMMU().isBootMode() && addr <= ADDRESSES.bootRom.end)
        return s_uiGlob.useDarkMode ? 0xFF3A5656 : 0xFFC8E4E4;

    if (addr > ctx.getSP() && addr <= ADDRESSES.hram.end)
        return s_uiGlob.useDarkMode ? 0xFF2F3F4C : 0xFFBDCDDA;

    return 0;
}

void initUI(Emu& ctx)
{
    s_uiGlob.ctx = &ctx;

    s_uiGlob.memoryEditor.Open               = IS_DEBUG;
    s_uiGlob.memoryEditor.ReadOnly           = true;
    s_uiGlob.memoryEditor.Cols               = 8;
    s_uiGlob.memoryEditor.OptShowDataPreview = true;
    s_uiGlob.memoryEditor.OptShowAscii       = false;
    s_uiGlob.memoryEditor.OptUpperCaseHex    = false;
    s_uiGlob.memoryEditor.ReadFn             = memReadFn;
    s_uiGlob.memoryEditor.BgColorFn          = memBgColorFn;
    s_uiGlob.memoryEditor.PreviewDataType    = ImGuiDataType_U16;

    rlImGuiBeginInitImGui();
    setCustomStyle(s_uiGlob.useDarkMode);
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

static bool isAnyShiftDown()
{
    return IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
}

static bool isAnyAltDown()
{
    return IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
}

static void showLoadCartDialog()
{
    // TODO: Load cartridge
    fputs("Loading cartridge\n", stdout);
}

static void togglePlay()
{
    Emu& ctx = *s_uiGlob.ctx;
    if (ctx.isRunning())
        ctx.stop();
    else
        ctx.resume();
}

static void toggleDarkMode()
{
    setCustomStyle(toggle(s_uiGlob.useDarkMode));
}

static void toggleFullscreen()
{
    // Init those to the current value on first call - Handles the edge-case where we start in fullscreen
    static int cachedWidth  = GetScreenWidth();
    static int cachedHeight = GetScreenHeight();

    if (IsWindowFullscreen())
    {
        SetWindowSize(cachedWidth, cachedHeight);
    }
    else
    {
        cachedWidth       = GetScreenWidth();
        cachedHeight      = GetScreenHeight();
        const int display = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
    }

    ToggleFullscreen();
}

static void handleShortcuts()
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (isAnyCtrlDown())
    {
        if (IsKeyPressed(KEY_O))
            showLoadCartDialog();
    }
    else if (isAnyShiftDown())
    {
        if (IsKeyPressed(KEY_P))
            togglePlay();
        else if (IsKeyPressed(KEY_S))
            s_uiGlob.ctx->step();
        else if (IsKeyPressed(KEY_F))
            s_uiGlob.ctx->frame();
        else if (IsKeyPressed(KEY_R))
            s_uiGlob.ctx->reset();
    }
    else if (isAnyAltDown())
    {
        if (IsKeyPressed(KEY_C))
            toggle(s_uiGlob.showCPU);
        else if (IsKeyPressed(KEY_M))
            toggle(s_uiGlob.memoryEditor.Open);
        else if (IsKeyPressed(KEY_S))
            toggle(s_uiGlob.showStats);
    }
    else
    {
        if (IsKeyPressed(KEY_F10))
            toggleDarkMode();
        else if (IsKeyPressed(KEY_F11))
            toggleFullscreen();
    }
}

template <typename T>
static void drawFlag(const T flag)
    requires(std::is_same_v<T, Flag> || std::is_same_v<T, ContextFlag>)
{
    ImGui::PushID(underlying(flag));
    {
        bool flagVal;
        if constexpr (std::is_same_v<T, Flag>)
            flagVal = s_uiGlob.ctx->getFlag(flag);
        else
            flagVal = s_uiGlob.ctx->getContextFlag(flag);

        ImGui::Text("%s:", to_string(flag));
        ImGui::SameLine();
        // ImGui::Checkbox("", &flagVal);
        if (flagVal)
            ImGui::TextColored(rlImGuiColors::Convert(s_uiGlob.useDarkMode ? GREEN : DARKBLUE), "On");
        else
            ImGui::TextDisabled("Off");
    }
    ImGui::PopID();
}

static void drawHex16(const char* label, const uint16_t value)
{
    ImGui::PushID(label);
    {
        ImGui::Text("%s:", label);
        ImGui::SameLine();
        ImGui::TextDisabled("0x%04X", value);
    }
    ImGui::PopID();
}

static void drawCPUView()
{
    if (!s_uiGlob.showCPU)
        return;

    Emu& ctx                   = *s_uiGlob.ctx;
    ImGuiChildFlags childFlags = ImGuiChildFlags_Borders;

    if (s_uiGlob.memoryEditor.Open)
        childFlags |= ImGuiChildFlags_AutoResizeY;
    else
        childFlags |= ImGuiChildFlags_AutoResizeX;

    ImGui::BeginChild("CPU", ImVec2(0, 0), childFlags, ImGuiWindowFlags_NoInputs);
    {
        ImGui::SeparatorText("State");
        ImGui::PushID("State");
        {
            ImGui::Text("M-Cycle:");
            ImGui::SameLine();
            ImGui::TextDisabled("%zu", ctx.getMMU().getCycle());

            drawFlag(ContextFlag::RUNNING);

            ImGui::SameLine();
            drawFlag(ContextFlag::IME);
        }
        ImGui::PopID();

        ImGui::SeparatorText("Registers");
        ImGui::PushID("Registers");
        {
            ImGui::PushID("IR");
            {
                ImGui::Text("%s:", to_string(Register::IR));
                ImGui::SameLine();
                ImGui::TextDisabled("0x%02X", ctx.getRegister(Register::IR));
            }
            ImGui::PopID();

            for (auto i = underlying(Register::A); i < underlying(Register::COUNT); i += 2)
            {
                const auto reg = static_cast<Register16>(i);
                drawHex16(to_string(reg), ctx.getRegister16(reg));
            }

            drawHex16("PC", ctx.getPC());
        }
        ImGui::PopID();

        ImGui::SeparatorText("Flags");
        ImGui::PushID("Flags");
        {
            drawFlag(Flag::Z);

            ImGui::SameLine();
            drawFlag(Flag::N);

            ImGui::SameLine();
            drawFlag(Flag::H);

            ImGui::SameLine();
            drawFlag(Flag::C);
        }
        ImGui::PopID();
    }
    ImGui::EndChild();
}

static void drawMemoryView()
{
    if (!s_uiGlob.memoryEditor.Open)
        return;

    constexpr ImGuiChildFlags childFlags   = ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeX;
    constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar;
    ImGui::BeginChild("Memory", ImVec2(0, 0), childFlags, windowFlags);
    {
        s_uiGlob.memoryEditor.DrawContents(nullptr, ADDRESSES.interruptsRegister + 1);
    }
    ImGui::EndChild();
}

static void drawStatsOverlay()
{
    if (!s_uiGlob.showStats)
        return;

    // Adapted from Dear ImGui "Simple Overlay" Example
    static int location          = 0;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    if (location >= 0)
    {
        const ImVec2 padding    = ImGui::GetStyle().WindowPadding;
        const ImVec2 windowPos  = ImGui::GetWindowPos();
        const ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 pos, pivot;
        pos.x   = (location & 1) ? (windowPos.x + windowSize.x - padding.x) : (windowPos.x + padding.x);
        pos.y   = (location & 2) ? (windowPos.y + windowSize.y - padding.y) : (windowPos.y + padding.y);
        pivot.x = (location & 1) ? 1.0f : 0.0f;
        pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
        windowFlags |= ImGuiWindowFlags_NoMove;
    }

    ImGui::Begin("Stats", nullptr, windowFlags);
    {
        ImGui::Text("Stats (right-click to move)");
        ImGui::Separator();
        ImGui::Text("Frame time: %.2fms (%d FPS)", static_cast<double>(GetFrameTime()) * 1000, GetFPS());

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", nullptr, location == -1))
                location = -1;
            if (ImGui::MenuItem("Top-left", nullptr, location == 0))
                location = 0;
            if (ImGui::MenuItem("Top-right", nullptr, location == 1))
                location = 1;
            if (ImGui::MenuItem("Bottom-left", nullptr, location == 2))
                location = 2;
            if (ImGui::MenuItem("Bottom-right", nullptr, location == 3))
                location = 3;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

static void drawEmuView()
{
    const RenderTexture2D renderTarget = s_uiGlob.ctx->getRenderTarget();
    if (!IsRenderTextureValid(renderTarget))
        return;

    const bool isChild                 = s_uiGlob.showCPU || s_uiGlob.memoryEditor.Open;
    const ImGuiChildFlags childFlags   = isChild ? ImGuiChildFlags_Borders : ImGuiChildFlags_None;
    const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | (isChild ? ImGuiWindowFlags_None : ImGuiWindowFlags_NoBackground);
    ImGui::BeginChild("Emu", ImVec2(0, 0), childFlags, windowFlags);
    {
        rlImGuiImageRenderTextureFit(&renderTarget, true);
        drawStatsOverlay();
    }
    ImGui::EndChild();
}

static void drawMainWindow()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("Main", nullptr, flags);
    ImGui::PopStyleVar(1);
    {
        ImGui::BeginChild("CPU_Mem", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX);
        {
            drawCPUView();
            drawMemoryView();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        drawEmuView();
    }
    ImGui::End();
}

static void drawFileMenu()
{
    if (!ImGui::BeginMenu("File"))
        return;

    if (ImGui::MenuItem("Load Cartridge", "Ctrl+O"))
        showLoadCartDialog();

    if (ImGui::MenuItem("Exit", "Alt+F4"))
        s_uiGlob.shouldClose = true;

    ImGui::EndMenu();
}

static void drawEmulationMenu()
{
    if (!ImGui::BeginMenu("Emulation"))
        return;

    Emu& ctx             = *s_uiGlob.ctx;
    const bool isRunning = ctx.isRunning();
    if (ImGui::MenuItem(isRunning ? "Pause" : "Play", "Shift+P"))
        togglePlay();

    ImGui::BeginDisabled(isRunning);
    if (ImGui::MenuItem("Step", "Shift+S"))
        ctx.step();

    if (ImGui::MenuItem("Frame", "Shift+F"))
        ctx.frame();
    ImGui::EndDisabled();

    if (ImGui::MenuItem("Reset", "Shift+R"))
        ctx.reset();

    ImGui::EndMenu();
}

static void drawDisplayMenu()
{
    if (!ImGui::BeginMenu("Display"))
        return;

    if (ImGui::MenuItem("Dark Mode", "F10", s_uiGlob.useDarkMode))
        toggleDarkMode();

    if (ImGui::MenuItem("Fullscreen", "F11", IsWindowFullscreen()))
        toggleFullscreen();

    ImGui::EndMenu();
}

static void drawDebugMenu()
{
    if (!ImGui::BeginMenu("Debug"))
        return;

    ImGui::MenuItem("CPU", "Alt+C", &s_uiGlob.showCPU);
    ImGui::MenuItem("Memory", "Alt+M", &s_uiGlob.memoryEditor.Open);
    ImGui::MenuItem("Stats", "Alt+S", &s_uiGlob.showStats);

    ImGui::EndMenu();
}

static void drawMenuBar()
{
    ImGui::BeginMainMenuBar();
    {
        drawFileMenu();
        drawEmulationMenu();
        drawDisplayMenu();
        drawDebugMenu();
    }
    ImGui::EndMainMenuBar();
}

void drawUI()
{
    rlImGuiBegin();
    {
        handleShortcuts();
        drawMainWindow();
        drawMenuBar();

#ifdef DEBUG
        ImGui::ShowDemoWindow();
#endif
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
    s_uiGlob.ctx = nullptr;
}
