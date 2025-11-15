#if defined(_DEBUG) && defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <cstdio>

#include "raylib.h"
#include "gui.h"

int main(const int argc, const char** argv)
{
#if defined(_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    ChangeDirectory(GetApplicationDirectory());

    // TODO: Init emulator

    if (argc > 1)
    {
        // TODO: Load rom from cmd args
        printf("TODO: load ROM @ \"%s\"\n", argv[1]);
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "nx_gbc");
    SetTargetFPS(60);

    initUI(true);

    while (!shouldClose())
    {
        // TODO: run emulation frame
        // emu.frame();

        BeginDrawing();
        {
            clearScreen();
            // TODO: draw emu render target
            // if (IsRenderTextureValid(emu.renderTarget))
            // {
            //     const Texture2D& texture = emu.renderTarget.texture;
            //     const Rectangle texRect{ 0, 0, static_cast<float>(texture.width), static_cast<float>(-texture.height) };
            //     const Rectangle scrRect{ 0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };
            //     DrawTexturePro(texture, texRect, scrRect, { 0, 0 }, 0, WHITE);
            // }
            drawUI();
        }
        EndDrawing();
    }

    shutdownUI();
    CloseWindow();
    return 0;
}
