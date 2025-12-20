#pragma once
#include "mmu.h"
#include "utility.h"

#include <cassert>

#include <raylib.h>

enum class Flag : byte
{
    Z = 1 << 7,
    N = 1 << 6,
    H = 1 << 5,
    C = 1 << 4
};

enum class ContextFlag : byte
{
    RUNNING = 1 << 0,
    IME     = 1 << 1
};

// clang-format off
enum class Register : byte
{
    IR,
    A, F,
    B, C,
    D, E,
    H, L,
    SP_HI, SP_LO, // SP could be a separate variable but having it here simplifies opcodes
    COUNT
};
// clang-format on

enum class Register16 : byte
{
    AF = underlying(Register::A),
    BC = underlying(Register::B),
    DE = underlying(Register::D),
    HL = underlying(Register::H),
    SP = underlying(Register::SP_HI),
};

constexpr const char* to_string(const Flag flag)
{
    switch (flag)
    {
    case Flag::Z:
        return "Z";
    case Flag::N:
        return "N";
    case Flag::H:
        return "H";
    case Flag::C:
        return "C";
    default:
        assert(false && "Unknown flag");
        return "?";
    }
}

constexpr const char* to_string(const ContextFlag flag)
{
    switch (flag)
    {
    case ContextFlag::RUNNING:
        return "RUNNING";
    case ContextFlag::IME:
        return "IME";
    default:
        assert(false && "Unknown context flag");
        return "?";
    }
}

constexpr const char* to_string(const Register reg)
{
    switch (reg)
    {
    case Register::IR:
        return "IR";
    case Register::A:
        return "A";
    case Register::F:
        return "F";
    case Register::B:
        return "B";
    case Register::C:
        return "C";
    case Register::D:
        return "F";
    case Register::E:
        return "R";
    case Register::H:
        return "H";
    case Register::L:
        return "L";
    case Register::SP_HI:
        return "SP_hi";
    case Register::SP_LO:
        return "SP_lo";
    case Register::COUNT:
    default:
        assert(false && "Unknown register");
        return "?";
    }
}

constexpr const char* to_string(const Register16 reg)
{
    switch (reg)
    {
    case Register16::AF:
        return "AF";
    case Register16::BC:
        return "BC";
    case Register16::DE:
        return "DE";
    case Register16::HL:
        return "HL";
    case Register16::SP:
        return "SP";
    default:
        assert(false && "Unknown 16-bit register");
        return "?";
    }
}

class Emu
{
public:
    static constexpr uint32_t CLOCK_SPEED = 4 * MB;
    static constexpr byte FRAME_RATE      = 60;
    static constexpr byte SCREEN_WIDTH    = 160;
    static constexpr byte SCREEN_HEIGHT   = 144;

    void init();
    void reset();
    void step();
    void frame();
    bool isRunning() const;
    void stop();
    void shutdown();

    RenderTexture2D getRenderTarget() const;

    MMU& getMMU();
    const MMU& getMMU() const;

    byte readN();
    uint16_t readN16();
    uint16_t popStack();

    byte getRegister(Register reg) const;
    void setRegister(Register reg, byte val);

    uint16_t getRegister16(Register16 reg) const;
    void setRegister16(Register16 reg, uint16_t val);

    bool getFlag(Flag flag) const;
    void setFlag(Flag flag, bool val);

    bool getContextFlag(ContextFlag flag) const;
    void setContextFlag(ContextFlag flag, bool val);

    uint16_t getPC() const;
    void setPC(uint16_t pc);

    uint16_t getSP() const;
    void setSP(uint16_t sp);

private:
    MMU m_mmu{};
    RenderTexture2D m_renderTarget{};

    byte m_contextFlags = 0;
    byte m_registers[underlying(Register::COUNT)]{};
    uint16_t m_pc = 0;
};
