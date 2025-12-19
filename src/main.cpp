#ifndef BUILD_TESTING
#if defined(_DEBUG) && defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "emu.h"
#include "gui.h"

int main(const int argc, const char** argv)
{
#if defined(_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    ChangeDirectory(GetApplicationDirectory());

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "nx_gbc");
    SetTargetFPS(60);
    SetExitKey(0);

    Emu emu;
    emu.init();

    if (argc > 1)
    {
        // TODO: Load rom from cmd args
        printf("TODO: load ROM @ \"%s\"\n", argv[1]);
    }

    initUI(true);

    while (!shouldClose())
    {
        emu.frame();

        BeginDrawing();
        {
            clearScreen();
            drawUI(emu);
        }
        EndDrawing();
    }

    shutdownUI();
    emu.shutdown();
    CloseWindow();
    return 0;
}
#endif
