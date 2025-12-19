#include "emu.h"

#include "opcodes.h"

#ifdef BUILD_TESTING
#include <gtest/gtest.h>
#endif

template <typename T>
static uint16_t read16(Emu& ctx, const uint16_t addr, T setter)
{
    MMU& mmu = ctx.getMMU();

    setter(addr + 1);
    const byte lo = mmu.readByte(addr);

    setter(addr + 2);
    const byte hi = mmu.readByte(addr + 1);

    return make16(lo, hi);
}

static byte processOpcode(Emu& ctx)
{
    ctx.setRegister(Register::IR, ctx.readN());
    // clang-format off
    switch (const byte opcode = ctx.getRegister(Register::IR))
    {
#define OPCODE(op, func, ...) case op: return func(ctx, ## __VA_ARGS__);
#define OPCODE_EX(...)
#include "opcodes.h"

    default:
        fprintf(stderr, "Unknown opcode: 0x%02X\n", opcode);
        ctx.stop(); // Don't clear - might be helpful for investigation
        return 0;
    }
    // clang-format on
}

void Emu::init()
{
    m_renderTarget = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    // BEGIN TMP - LNOBIANG
    BeginTextureMode(m_renderTarget);
    ClearBackground(PINK);
    EndTextureMode();
    // END TMP
    reset();
}

void Emu::reset()
{
    m_mmu.reset();
    memset(m_registers, 0, sizeof(m_registers));
    setSP(ADDRESSES.hram.end); // NOTE: This is technically handled by the boot rom
    m_pc = 0;

    setContextFlag(ContextFlag::RUNNING, true);
}

void Emu::frame()
{
    if (!getContextFlag(ContextFlag::RUNNING))
        return;

    size_t cycleCount         = m_mmu.getCycle();
    const byte expectedCycles = processOpcode(*this);
    cycleCount                = m_mmu.getCycle() - cycleCount;

    if (cycleCount != expectedCycles)
    {
        fprintf(stderr, "M-Cycles mismatch (%zu/%u)\n", cycleCount, expectedCycles);
        stop();
    }
}

void Emu::stop()
{
    setContextFlag(ContextFlag::RUNNING, false);
}

void Emu::shutdown()
{
    UnloadRenderTexture(m_renderTarget);
    m_renderTarget = {};
    m_mmu.reset();
}

RenderTexture2D Emu::getRenderTarget() const
{
    return m_renderTarget;
}

MMU& Emu::getMMU()
{
    return m_mmu;
}

const MMU& Emu::getMMU() const
{
    return m_mmu;
}

byte Emu::readN()
{
    return m_mmu.readByte(m_pc++);
}

uint16_t Emu::readN16()
{
    return read16(*this, m_pc, [this](const uint16_t pc) { m_pc = pc; });
}

uint16_t Emu::popStack()
{
    return read16(*this, getSP(), [this](const uint16_t sp) { setSP(sp); });
}

byte Emu::getRegister(const Register reg) const
{
    return m_registers[underlying(reg)];
}

void Emu::setRegister(const Register reg, byte val)
{
    // Some cartridges expect the lowest nibble of the F register to be 0, some ignore it - forcing it to 0 seems like the best middle-ground
    if (reg == Register::F)
        val &= 0xF0;

    m_registers[underlying(reg)] = val;
}

uint16_t Emu::getRegister16(const Register16 reg) const
{
    return make16(m_registers[underlying(reg) + 1], m_registers[underlying(reg)]);
}

void Emu::setRegister16(const Register16 reg, uint16_t val)
{
    // Some cartridges expect the lowest nibble of the F register to be 0, some ignore it - forcing it to 0 seems like the best middle-ground
    if (reg == Register16::AF)
        val &= 0xFFF0;

    m_registers[underlying(reg)]     = msb(val);
    m_registers[underlying(reg) + 1] = lsb(val);
}

bool Emu::getFlag(const Flag flag) const
{
    return getRegister(Register::F) & underlying(flag);
}

void Emu::setFlag(const Flag flag, const bool val)
{
    byte& flags = m_registers[underlying(Register::F)];

    if (val)
        flags |= underlying(flag);
    else
        flags &= ~underlying(flag);
}
bool Emu::getContextFlag(const ContextFlag flag) const
{
    return m_contextFlags & underlying(flag);
}

void Emu::setContextFlag(const ContextFlag flag, const bool val)
{
    if (val)
        m_contextFlags |= underlying(flag);
    else
        m_contextFlags &= ~underlying(flag);
}

uint16_t Emu::getPC() const
{
    return m_pc;
}

void Emu::setPC(const uint16_t pc)
{
    m_pc = pc;
}

uint16_t Emu::getSP() const
{
    return getRegister16(Register16::SP);
}

void Emu::setSP(const uint16_t sp)
{
    setRegister16(Register16::SP, sp);
}

#ifdef BUILD_TESTING
CLANG_IGNORE_WARNING_PUSH("-Wglobal-constructors")
TEST(Emu, Registers)
{
    Emu ctx;
    ctx.reset();

    for (byte regVal = underlying(Register::A); regVal < underlying(Register::COUNT); ++regVal)
    {
        const auto reg = static_cast<Register>(regVal);
        ctx.setRegister(reg, 0xFF);
        ASSERT_EQ(ctx.getRegister(reg), reg == Register::F ? 0xF0 : 0xFF);

        ctx.setRegister(reg, 0x00);
        ASSERT_EQ(ctx.getRegister(reg), 0x00);
    }

    const auto checkReg16 = [&](const Register16 reg, const Register hi, const Register lo)
    {
        ctx.setRegister16(reg, 0xFEED);
        ASSERT_EQ(ctx.getRegister16(reg), reg == Register16::AF ? 0xFEE0 : 0xFEED);
        ASSERT_EQ(ctx.getRegister(hi), 0xFE);
        ASSERT_EQ(ctx.getRegister(lo), lo == Register::F ? 0xE0 : 0xED);

        ctx.setRegister16(reg, 0x1234);
        ASSERT_EQ(ctx.getRegister16(reg), reg == Register16::AF ? 0x1230 : 0x1234);
        ASSERT_EQ(ctx.getRegister(hi), 0x12);
        ASSERT_EQ(ctx.getRegister(lo), lo == Register::F ? 0x30 : 0x34);
    };

    checkReg16(Register16::AF, Register::A, Register::F);
    checkReg16(Register16::BC, Register::B, Register::C);
    checkReg16(Register16::DE, Register::D, Register::E);
    checkReg16(Register16::HL, Register::H, Register::L);
    checkReg16(Register16::SP, Register::SP_HI, Register::SP_LO);
}

TEST(Emu, Flags)
{
    Emu ctx;
    ctx.reset();

    ctx.setRegister(Register::F, 0xFF);
    ASSERT_EQ(ctx.getRegister(Register::F), 0xF0);
    ASSERT_TRUE(ctx.getFlag(Flag::Z));
    ASSERT_TRUE(ctx.getFlag(Flag::N));
    ASSERT_TRUE(ctx.getFlag(Flag::H));
    ASSERT_TRUE(ctx.getFlag(Flag::C));

    ctx.setRegister(Register::F, 0);
    ASSERT_EQ(ctx.getRegister(Register::F), 0);
    ASSERT_FALSE(ctx.getFlag(Flag::Z));
    ASSERT_FALSE(ctx.getFlag(Flag::N));
    ASSERT_FALSE(ctx.getFlag(Flag::H));
    ASSERT_FALSE(ctx.getFlag(Flag::C));

    ASSERT_FALSE(ctx.getFlag(Flag::Z));
    ctx.setFlag(Flag::Z, true);
    ASSERT_TRUE(ctx.getFlag(Flag::Z));
    ASSERT_EQ(ctx.getRegister(Register::F), 0b1000'0000);

    ctx.setRegister(Register::F, 0);
    ASSERT_FALSE(ctx.getFlag(Flag::N));
    ctx.setFlag(Flag::N, true);
    ASSERT_TRUE(ctx.getFlag(Flag::N));
    ASSERT_EQ(ctx.getRegister(Register::F), 0b0100'0000);

    ctx.setRegister(Register::F, 0);
    ASSERT_FALSE(ctx.getFlag(Flag::H));
    ctx.setFlag(Flag::H, true);
    ASSERT_TRUE(ctx.getFlag(Flag::H));
    ASSERT_EQ(ctx.getRegister(Register::F), 0b0010'0000);

    ctx.setRegister(Register::F, 0);
    ASSERT_FALSE(ctx.getFlag(Flag::C));
    ctx.setFlag(Flag::C, true);
    ASSERT_TRUE(ctx.getFlag(Flag::C));
    ASSERT_EQ(ctx.getRegister(Register::F), 0b0001'0000);
}
CLANG_IGNORE_WARNING_POP
#endif
