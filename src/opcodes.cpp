#include "opcodes.h"

#include <cstdio>

#pragma region MISC
OPCODE_SIG(op_nop)
{
    return 1;
}

OPCODE_SIG(op_stop)
{
    ctx.setPC(ctx.getPC() + 1);
    ctx.stop();
    return 1;
}

OPCODE_SIG(op_halt)
{
    // TODO: Halt
    return 1;
}

OPCODE_SIG(op_di)
{
    ctx.setContextFlag(ContextFlag::IME, false);
    return 1;
}

OPCODE_SIG(op_ei)
{
    // TODO: Handle interrupts
    ctx.setContextFlag(ContextFlag::IME, true);
    return 1;
}

OPCODE_SIG(op_prefix)
{
    ctx.setRegister(Register::IR, ctx.readN());
    // clang-format off
    switch (const byte opcode = ctx.getRegister(Register::IR))
    {
#define OPCODE(...)
#define OPCODE_EX(op, func, ...) case op: return func(ctx, ##__VA_ARGS__);
#include "opcodes.h"

    default:
        fprintf(stderr, "Unknown extended opcode: 0x%02X\n", opcode);
        ctx.stop(); // Don't clear - might be helpful for investigation
        return 0;
    }
    // clang-format on
}
#pragma endregion

#pragma region PUSH/POP
template <typename T>
static void pushStack(Emu& ctx, const uint16_t data, T callback)
{
    MMU& mmu          = ctx.getMMU();
    const uint16_t sp = ctx.getSP();

    ctx.setSP(sp - 1);
    mmu.addCycle();

    ctx.setSP(sp - 2);
    mmu.writeByte(sp - 1, msb(data));

    callback();
    mmu.writeByte(sp - 2, lsb(data));
}

// POP rr
OPCODE_SIG(op_popR16, const Register16 reg)
{
    ctx.setRegister16(reg, ctx.popStack());
    return 3;
}

// PUSH rr
OPCODE_SIG(op_pushR16, const Register16 reg)
{
    pushStack(ctx, ctx.getRegister16(reg), [] {});
    return 4;
}
#pragma endregion

#pragma region CONTROL
// JR e
OPCODE_SIG(op_jrI8)
{
    const auto offset = static_cast<sbyte>(ctx.readN());
    ctx.setPC(static_cast<uint16_t>(ctx.getPC() + offset));
    ctx.getMMU().addCycle();
    return 3;
}

// JR cc,e
OPCODE_SIG(op_jrCondI8, const Flag flag, const bool expected)
{
    if (ctx.getFlag(flag) == expected)
        return op_jrI8(ctx);

    (void)ctx.readN();
    return 2;
}

// JP nn
OPCODE_SIG(op_jpN16)
{
    ctx.setPC(ctx.readN16());
    ctx.getMMU().addCycle();
    return 4;
}

// JP HL
OPCODE_SIG(op_jpHL)
{
    ctx.setPC(ctx.getRegister16(Register16::HL));
    return 1;
}

// JP cc,e
OPCODE_SIG(op_jpCondN16, const Flag flag, const bool expected)
{
    if (ctx.getFlag(flag) == expected)
        return op_jpN16(ctx);

    (void)ctx.readN16();
    return 3;
}

// RET cc
OPCODE_SIG(op_retCond, const Flag flag, const bool expected)
{
    ctx.getMMU().addCycle(); // WHY???

    if (ctx.getFlag(flag) == expected)
        return op_ret(ctx) + 1;

    return 2;
}

// RET
OPCODE_SIG(op_ret)
{
    ctx.setPC(ctx.popStack());
    ctx.getMMU().addCycle();
    return 4;
}

// RETI
OPCODE_SIG(op_reti)
{
    ctx.setPC(ctx.popStack());
    ctx.setContextFlag(ContextFlag::IME, true);
    ctx.getMMU().addCycle();
    return 4;
}

// CALL cc,nn
OPCODE_SIG(op_callCond, const Flag flag, const bool expected)
{
    if (ctx.getFlag(flag) == expected)
        return op_call(ctx);

    (void)ctx.readN16();
    return 3;
}

static void call(Emu& ctx, const uint16_t addr)
{
    pushStack(ctx, ctx.getPC(), [&ctx, addr] { ctx.setPC(addr); });
}

// CALL nn
OPCODE_SIG(op_call)
{
    call(ctx, ctx.readN16());
    return 6;
}

// RST n
OPCODE_SIG(op_rstX, const byte x)
{
    call(ctx, make16(x, 0));
    return 4;
}
#pragma endregion

#pragma region LD16
// LD rr,nn
OPCODE_SIG(op_ldRN16, const Register16 reg)
{
    ctx.setRegister16(reg, ctx.readN16());
    return 3;
}

// LD (nn),SP
OPCODE_SIG(op_ldNptrSP)
{
    ctx.getMMU().write16(ctx.readN16(), ctx.getSP());
    return 5;
}

// LD HL,SP+e
OPCODE_SIG(op_ldHLSPoff)
{
    const byte offset = ctx.readN();
    const uint16_t sp = ctx.getSP();

    ctx.setRegister(Register::L, lsb(sp) + offset);

    ctx.setFlag(Flag::Z, false);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, carryPerBit(3, lsb(sp), offset));
    ctx.setFlag(Flag::C, carryPerBit(7, lsb(sp), offset));
    ctx.getMMU().addCycle();

    ctx.setRegister(Register::H, msb(sp) + (offset >> 7) * 0xFF + ctx.getFlag(Flag::C));
    return 3;
}

// LD SP, HL
OPCODE_SIG(op_ldSPHL)
{
    ctx.setSP(ctx.getRegister16(Register16::HL));
    return 2;
}
#pragma endregion

#pragma region LD8
// LD (rr),r
OPCODE_SIG(op_ldRptrR, const Register16 to, const Register from, const sbyte offset)
{
    const uint16_t addr = ctx.getRegister16(to);
    ctx.getMMU().writeByte(addr, ctx.getRegister(from));

    if (offset != 0)
    {
        ctx.setRegister16(to, static_cast<uint16_t>(addr + offset));
    }

    return 2;
}

// LD r,n
OPCODE_SIG(op_ldRN, const Register reg)
{
    ctx.setRegister(reg, ctx.readN());
    return 2;
}

// LD r,(rr)
OPCODE_SIG(op_ldRRptr, const Register to, const Register16 from, const sbyte offset)
{
    const uint16_t addr = ctx.getRegister16(from);
    ctx.setRegister(to, ctx.getMMU().readByte(addr));

    if (offset != 0)
    {
        ctx.setRegister16(from, static_cast<uint16_t>(addr + offset));
    }

    return 2;
}

// LD (HL),n
OPCODE_SIG(op_ldHLptrN)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    ctx.getMMU().writeByte(addr, ctx.readN());
    return 3;
}

// LD r,r'
OPCODE_SIG(op_ldRR, const Register to, const Register from)
{
    ctx.setRegister(to, ctx.getRegister(from));
    return 1;
}

// LDH (n),A
OPCODE_SIG(op_ldhNsptrA)
{
    const uint16_t addr = ADDRESSES.io.start | ctx.readN();
    ctx.getMMU().writeByte(addr, ctx.getRegister(Register::A));
    return 3;
}

// LDH (C),A
OPCODE_SIG(op_ldhCsptrA)
{
    const uint16_t addr = ADDRESSES.io.start | ctx.getRegister(Register::C);
    ctx.getMMU().writeByte(addr, ctx.getRegister(Register::A));
    return 2;
}

// LD (nn),A
OPCODE_SIG(op_ldNptrA)
{
    ctx.getMMU().writeByte(ctx.readN16(), ctx.getRegister(Register::A));
    return 4;
}

// LDH A,(n)
OPCODE_SIG(op_ldhANsptr)
{
    const uint16_t addr = ADDRESSES.io.start | ctx.readN();
    ctx.setRegister(Register::A, ctx.getMMU().readByte(addr));
    return 3;
}

// LDH A,(C)
OPCODE_SIG(op_ldhACsptr)
{
    const uint16_t addr = ADDRESSES.io.start | ctx.getRegister(Register::C);
    ctx.setRegister(Register::A, ctx.getMMU().readByte(addr));
    return 2;
}

// LD A,(nn)
OPCODE_SIG(op_ldANptr)
{
    ctx.setRegister(Register::A, ctx.getMMU().readByte(ctx.readN16()));
    return 4;
}
#pragma endregion

#pragma region ALU16
// INC rr
OPCODE_SIG(op_incR16, const Register16 reg)
{
    ctx.setRegister16(reg, ctx.getRegister16(reg) + 1);
    ctx.getMMU().addCycle();
    return 2;
}

// ADD HL,rr
OPCODE_SIG(op_addHLR16, const Register16 reg)
{
    const uint16_t hl  = ctx.getRegister16(Register16::HL);
    const uint16_t val = ctx.getRegister16(reg);

    // TODO: Check if this is actually valid
    // based on GBCTR's detailed pseudocode H + B and L + C should be computed in 2 cycles ?
    ctx.setRegister16(Register16::HL, hl + val);

    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, carryPerBit(11, hl, val));
    ctx.setFlag(Flag::C, carryPerBit(15, hl, val));
    ctx.getMMU().addCycle();
    return 2;
}

// DEC rr
OPCODE_SIG(op_decR16, const Register16 reg)
{
    ctx.setRegister16(reg, ctx.getRegister16(reg) - 1);
    ctx.getMMU().addCycle();
    return 2;
}

// ADD SP,e
OPCODE_SIG(op_addSPI8)
{
    const byte e      = ctx.readN();
    const uint16_t sp = ctx.getSP();
    MMU& mmu          = ctx.getMMU();

    ctx.setRegister(Register::SP_LO, lsb(sp) + e);

    ctx.setFlag(Flag::Z, false);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, carryPerBit(3, lsb(sp), e));
    ctx.setFlag(Flag::C, carryPerBit(7, lsb(sp), e));
    mmu.addCycle();

    ctx.setRegister(Register::SP_HI, msb(sp) + (e >> 7) * 0xFF + ctx.getFlag(Flag::C));
    mmu.addCycle(); // TODO: Check why 0xF8 doesn't cycle ATE but 0xE8 does
    return 4;
}
#pragma endregion

#pragma region ALU8
// INC r
OPCODE_SIG(op_incR, const Register reg)
{
    const byte val = ctx.getRegister(reg);
    ctx.setRegister(reg, val + 1);
    ctx.setFlag(Flag::Z, val == UINT8_MAX);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, carryPerBit(3, val, 1));
    return 1;
}

// DEC r
OPCODE_SIG(op_decR, const Register reg)
{
    const byte val = ctx.getRegister(reg);
    ctx.setRegister(reg, val - 1);
    ctx.setFlag(Flag::Z, val == 1);
    ctx.setFlag(Flag::N, true);
    ctx.setFlag(Flag::H, subCarryPerBit(3, val, 1));
    return 1;
}

// DAA
OPCODE_SIG(op_daa)
{
    byte adjust      = 0;
    bool hasCarry    = false;
    const bool isSub = ctx.getFlag(Flag::N);
    byte a           = ctx.getRegister(Register::A);

    if ((!isSub && lsn(a) > 0x09) || ctx.getFlag(Flag::H))
        adjust |= 0x06;

    if ((!isSub && a > 0x99) || ctx.getFlag(Flag::C))
    {
        adjust |= 0x60;
        hasCarry = true;
    }

    a = isSub ? a - adjust : a + adjust;

    ctx.setRegister(Register::A, a);
    ctx.setFlag(Flag::Z, a == 0);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, hasCarry);
    return 1;
}

// CPL
OPCODE_SIG(op_cpl)
{
    ctx.setRegister(Register::A, ~ctx.getRegister(Register::A));
    ctx.setFlag(Flag::N, true);
    ctx.setFlag(Flag::H, true);
    return 1;
}

// INC (HL)
OPCODE_SIG(op_incHLptr)
{
    MMU& mmu            = ctx.getMMU();
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    const byte val      = mmu.readByte(addr);
    ctx.setFlag(Flag::Z, val == UINT8_MAX);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, carryPerBit(3, val, 1));
    mmu.writeByte(addr, val + 1);
    return 3;
}

// DEC (HL)
OPCODE_SIG(op_decHLptr)
{
    MMU& mmu            = ctx.getMMU();
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    const byte val      = mmu.readByte(addr);
    ctx.setFlag(Flag::Z, val == 1);
    ctx.setFlag(Flag::N, true);
    ctx.setFlag(Flag::H, subCarryPerBit(3, val, 1));
    mmu.writeByte(addr, val - 1);
    return 3;
}

// SCF
OPCODE_SIG(op_scf)
{
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, true);
    return 1;
}

// CCF
OPCODE_SIG(op_ccf)
{
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, !ctx.getFlag(Flag::C));
    return 1;
}

template <typename... T>
static void addImpl(Emu& ctx, const T... val)
{
    const byte a   = ctx.getRegister(Register::A);
    const byte res = a + (val + ...);
    ctx.setRegister(Register::A, res);
    ctx.setFlag(Flag::Z, res == 0);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, carryPerBit(3, a, val...));
    ctx.setFlag(Flag::C, carryPerBit(7, a, val...));
}

// ADD r
OPCODE_SIG(op_addR, const Register reg)
{
    addImpl(ctx, ctx.getRegister(reg));
    return 1;
}

// ADD (HL)
OPCODE_SIG(op_addHLptr)
{
    addImpl(ctx, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)));
    return 2;
}

// ADD n
OPCODE_SIG(op_addN)
{
    addImpl(ctx, ctx.readN());
    return 2;
}

// ADC r
OPCODE_SIG(op_adcR, const Register reg)
{
    addImpl(ctx, ctx.getRegister(reg), ctx.getFlag(Flag::C));
    return 1;
}

// ADC (HL)
OPCODE_SIG(op_adcHLptr)
{
    addImpl(ctx, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)), ctx.getFlag(Flag::C));
    return 2;
}

// ADC n
OPCODE_SIG(op_adcN)
{
    addImpl(ctx, ctx.readN(), ctx.getFlag(Flag::C));
    return 2;
}

template <typename... T>
static void subImpl(Emu& ctx, const T... val)
{
    const byte a   = ctx.getRegister(Register::A);
    const byte res = a - (val + ...);
    ctx.setRegister(Register::A, res);
    ctx.setFlag(Flag::Z, res == 0);
    ctx.setFlag(Flag::N, true);
    ctx.setFlag(Flag::H, subCarryPerBit(3, a, val...));
    ctx.setFlag(Flag::C, subCarryPerBit(7, a, val...));
}

// SUB r
OPCODE_SIG(op_subR, const Register reg)
{
    subImpl(ctx, ctx.getRegister(reg));
    return 1;
}

// SUB (HL)
OPCODE_SIG(op_subHLptr)
{
    subImpl(ctx, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)));
    return 2;
}

// SUB n
OPCODE_SIG(op_subN)
{
    subImpl(ctx, ctx.readN());
    return 2;
}

// SBC r
OPCODE_SIG(op_sbcR, const Register reg)
{
    subImpl(ctx, ctx.getRegister(reg), ctx.getFlag(Flag::C));
    return 1;
}

// SBC (HL)
OPCODE_SIG(op_sbcHLptr)
{
    subImpl(ctx, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)), ctx.getFlag(Flag::C));
    return 2;
}

// SBC n
OPCODE_SIG(op_sbcN)
{
    subImpl(ctx, ctx.readN(), ctx.getFlag(Flag::C));
    return 2;
}

static void andImpl(Emu& ctx, const byte val)
{
    const byte result = ctx.getRegister(Register::A) & val;
    ctx.setRegister(Register::A, result);
    ctx.setFlag(Flag::Z, result == 0);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, true);
    ctx.setFlag(Flag::C, false);
}

// AND r
OPCODE_SIG(op_andR, const Register reg)
{
    andImpl(ctx, ctx.getRegister(reg));
    return 1;
}

// AND (HL)
OPCODE_SIG(op_andHLptr)
{
    andImpl(ctx, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)));
    return 2;
}

// AND n
OPCODE_SIG(op_andN)
{
    andImpl(ctx, ctx.readN());
    return 2;
}

static void xorImpl(Emu& ctx, const byte val)
{
    const byte result = ctx.getRegister(Register::A) ^ val;
    ctx.setRegister(Register::A, result);
    ctx.setFlag(Flag::Z, result == 0);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, false);
}

// XOR r
OPCODE_SIG(op_xorR, const Register reg)
{
    xorImpl(ctx, ctx.getRegister(reg));
    return 1;
}

// XOR (HL)
OPCODE_SIG(op_xorHLptr)
{
    xorImpl(ctx, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)));
    return 2;
}

// XOR n
OPCODE_SIG(op_xorN)
{
    xorImpl(ctx, ctx.readN());
    return 2;
}

static void orImpl(Emu& ctx, const byte val)
{
    const byte result = ctx.getRegister(Register::A) | val;
    ctx.setRegister(Register::A, result);
    ctx.setFlag(Flag::Z, result == 0);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, false);
}

// OR r
OPCODE_SIG(op_orR, const Register reg)
{
    orImpl(ctx, ctx.getRegister(reg));
    return 1;
}

// OR (HL)
OPCODE_SIG(op_orHLptr)
{
    orImpl(ctx, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)));
    return 2;
}

// OR n
OPCODE_SIG(op_orN)
{
    orImpl(ctx, ctx.readN());
    return 2;
}

static void cpImpl(Emu& ctx, const byte val)
{
    const byte a = ctx.getRegister(Register::A);
    ctx.setFlag(Flag::Z, a == val);
    ctx.setFlag(Flag::N, true);
    ctx.setFlag(Flag::H, subCarryPerBit(3, a, val));
    ctx.setFlag(Flag::C, subCarryPerBit(7, a, val));
}

// CP r
OPCODE_SIG(op_cpR, const Register reg)
{
    cpImpl(ctx, ctx.getRegister(reg));
    return 1;
}

// CP (HL)
OPCODE_SIG(op_cpHLptr)
{
    cpImpl(ctx, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)));
    return 2;
}

// CP n
OPCODE_SIG(op_cpN)
{
    cpImpl(ctx, ctx.readN());
    return 2;
}
#pragma endregion

#pragma region RSB
// RLCA
OPCODE_SIG(op_rlca)
{
    op_rlcR(ctx, Register::A);
    ctx.setFlag(Flag::Z, false);
    return 1;
}

// RRCA
OPCODE_SIG(op_rrca)
{
    op_rrcR(ctx, Register::A);
    ctx.setFlag(Flag::Z, false);
    return 1;
}

// RLA
OPCODE_SIG(op_rla)
{
    op_rlR(ctx, Register::A);
    ctx.setFlag(Flag::Z, false);
    return 1;
}

// RRA
OPCODE_SIG(op_rra)
{
    op_rrR(ctx, Register::A);
    ctx.setFlag(Flag::Z, false);
    return 1;
}
#pragma endregion

#pragma region EXTENDED
static byte rlc(Emu& ctx, const byte val)
{
    const byte result = static_cast<byte>((val << 1) | (val >> 7));
    ctx.setFlag(Flag::Z, result == 0);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, val >> 7);
    return result;
}

static byte rrc(Emu& ctx, const byte val)
{
    const byte result = static_cast<byte>((val >> 1) | (val << 7));
    ctx.setFlag(Flag::Z, result == 0);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, val & 0b1);
    return result;
}

static byte rl(Emu& ctx, const byte val, const byte mask = 0xFF)
{
    const byte result = static_cast<byte>((val << 1) | static_cast<byte>(ctx.getFlag(Flag::C))) & mask;
    ctx.setFlag(Flag::Z, result == 0);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, val >> 7);
    return result;
}

static byte rr(Emu& ctx, const byte val, const byte mask = 0xFF)
{
    const byte result = static_cast<byte>((val >> 1) | (ctx.getFlag(Flag::C) << 7)) & mask;
    ctx.setFlag(Flag::Z, result == 0);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, false);
    ctx.setFlag(Flag::C, val & 0b1);
    return result;
}

// RLC r
OPCODE_SIG(op_rlcR, const Register reg)
{
    ctx.setRegister(reg, rlc(ctx, ctx.getRegister(reg)));
    return 2;
}

// RLC (HL)
OPCODE_SIG(op_rlcHLptr)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    MMU& mmu            = ctx.getMMU();
    mmu.writeByte(addr, rlc(ctx, mmu.readByte(addr)));
    return 4;
}

// RRC r
OPCODE_SIG(op_rrcR, const Register reg)
{
    ctx.setRegister(reg, rrc(ctx, ctx.getRegister(reg)));
    return 2;
}

// RRC (HL)
OPCODE_SIG(op_rrcHLptr)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    MMU& mmu            = ctx.getMMU();
    mmu.writeByte(addr, rrc(ctx, mmu.readByte(addr)));
    return 4;
}

// RL r
OPCODE_SIG(op_rlR, const Register reg)
{
    ctx.setRegister(reg, rl(ctx, ctx.getRegister(reg)));
    return 2;
}

// RL (HL)
OPCODE_SIG(op_rlHLptr)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    MMU& mmu            = ctx.getMMU();
    mmu.writeByte(addr, rl(ctx, mmu.readByte(addr)));
    return 4;
}

// RR r
OPCODE_SIG(op_rrR, const Register reg)
{
    ctx.setRegister(reg, rr(ctx, ctx.getRegister(reg)));
    return 2;
}

// RR (HL)
OPCODE_SIG(op_rrHLptr)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    MMU& mmu            = ctx.getMMU();
    mmu.writeByte(addr, rr(ctx, mmu.readByte(addr)));
    return 4;
}

// SLA r
OPCODE_SIG(op_slaR, const Register reg)
{
    ctx.setRegister(reg, rl(ctx, ctx.getRegister(reg), 0xFE));
    return 2;
}

// SLA (HL)
OPCODE_SIG(op_slaHLptr)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    MMU& mmu            = ctx.getMMU();
    mmu.writeByte(addr, rl(ctx, mmu.readByte(addr), 0xFE));
    return 4;
}

// SRA r
OPCODE_SIG(op_sraR, const Register reg)
{
    const byte val = ctx.getRegister(reg);
    ctx.setRegister(reg, rr(ctx, val, 0x7F) | (val & 0x80));
    return 2;
}

// SRA (HL)
OPCODE_SIG(op_sraHLptr)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    MMU& mmu            = ctx.getMMU();
    const byte val = mmu.readByte(addr);
    mmu.writeByte(addr, rr(ctx, val, 0x7F) | (val & 0x80));
    return 4;
}

static byte swap(Emu& ctx, const byte val)
{
    ctx.setRegister(Register::F, 0);
    ctx.setFlag(Flag::Z, val == 0);
    return static_cast<byte>(val << 4 | val >> 4);
}

// SWAP r
OPCODE_SIG(op_swapR, const Register reg)
{
    ctx.setRegister(reg, swap(ctx, ctx.getRegister(reg)));
    return 2;
}

// SWAP (HL)
OPCODE_SIG(op_swapHLptr)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    MMU& mmu            = ctx.getMMU();
    mmu.writeByte(addr, swap(ctx, mmu.readByte(addr)));
    return 4;
}

// SRL r
OPCODE_SIG(op_srlR, const Register reg)
{
    ctx.setRegister(reg, rr(ctx, ctx.getRegister(reg), 0x7F));
    return 2;
}

// SRL (HL)
OPCODE_SIG(op_srlHLptr)
{
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    MMU& mmu            = ctx.getMMU();
    mmu.writeByte(addr, rr(ctx, mmu.readByte(addr), 0x7F));
    return 4;
}

static void bitImpl(Emu& ctx, const byte bit, const byte val)
{
    ctx.setFlag(Flag::Z, ~(val >> bit) & 0b1);
    ctx.setFlag(Flag::N, false);
    ctx.setFlag(Flag::H, true);
}

// BIT b,r
OPCODE_SIG(op_bitXR, const byte bit, const Register reg)
{
    bitImpl(ctx, bit, ctx.getRegister(reg));
    return 2;
}

// BIT b,(HL)
OPCODE_SIG(op_bitXHLptr, const byte bit)
{
    bitImpl(ctx, bit, ctx.getMMU().readByte(ctx.getRegister16(Register16::HL)));
    return 3;
}

static OPCODE_SIG(setXR, const byte bit, const Register reg, const bool val)
{
    ctx.setRegister(reg, setBit(ctx.getRegister(reg), bit, val));
    return 2;
}

static OPCODE_SIG(setXHLptr, const byte bit, const bool val)
{
    MMU& mmu            = ctx.getMMU();
    const uint16_t addr = ctx.getRegister16(Register16::HL);
    mmu.writeByte(addr, setBit(mmu.readByte(addr), bit, val));
    return 4;
}

// RES b,r
OPCODE_SIG(op_resXR, const byte bit, const Register reg)
{
    return setXR(ctx, bit, reg, false);
}

// RES b,(HL)
OPCODE_SIG(op_resXHLptr, const byte bit)
{
    return setXHLptr(ctx, bit, false);
}

// SET b,r
OPCODE_SIG(op_setXR, const byte bit, const Register reg)
{
    return setXR(ctx, bit, reg, true);
}

// SET b,(HL)
OPCODE_SIG(op_setXHLptr, const byte bit)
{
    return setXHLptr(ctx, bit, true);
}
#pragma endregion
