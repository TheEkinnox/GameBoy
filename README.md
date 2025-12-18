# Game Boy Emulator

## Description

My second emulator / first emulator of a "real" console.

The goal is to support common features of the Game Boy then, eventually, extend this to emulate the Game Boy Color.

Most of the implementation is based on [PanDocs](https://gbdev.io/pandocs) and Gekkio's [GBCTR](https://gekkio.fi/files/gb-docs/gbctr.pdf)

## Dependencies

- [raylib](https://github.com/raysan5/raylib): Graphics, Inputs & Audio backend
- [Dear ImGui](https://github.com/ocornut/imgui): Bloat-free immediate mode UI
- [rlImGui](https://github.com/raylib-extras/rlImGui): Raylib backend for Dear ImGui

## How To

- Build the executable using CMake (for a quick guide see [this reddit post](https://www.reddit.com/r/cpp_questions/comments/1ct28tc/comment/l4ab1wa/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button))
- Run your cartridge by either
  - Passing its path as a command line argument
  - Starting the executable and loading it by clicking "File" -> "Load Cartridge" (or Ctrl+O)
- Enjoy

Note: on Windows you can set the executable as the default app for your roms or use the "Open With" option in the explorer's context menu
