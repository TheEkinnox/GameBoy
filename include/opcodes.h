#if defined(OPCODE) || defined(OPCODE_EX)
#define OPCODE_ARG(type, value) value
#else // #if defined(OPCODE) || defined(OPCODE_EX)
#include "emu.h"
#define OPCODE_ARG(type, value) type
#define OPCODE_SIG( func, ...) byte func( [[maybe_unused]] Emu& ctx, ##__VA_ARGS__ )
#define OPCODE( op, func, ...) OPCODE_SIG( func, ##__VA_ARGS__ );
#define OPCODE_EX OPCODE
#endif // #else // #if defined(OPCODE) || defined(OPCODE_EX)

#pragma region MISC
OPCODE(0x00, op_nop)
OPCODE(0x10, op_stop)
OPCODE(0x76, op_halt)
OPCODE(0xCB, op_prefix)
OPCODE(0xF3, op_di)
OPCODE(0xFB, op_ei)
#pragma endregion

#pragma region LD16
OPCODE(0x01, op_ldRN16, OPCODE_ARG(Register16, Register16::BC))
OPCODE(0x11, op_ldRN16, OPCODE_ARG(Register16, Register16::DE))
OPCODE(0x21, op_ldRN16, OPCODE_ARG(Register16, Register16::HL))
OPCODE(0x31, op_ldRN16, OPCODE_ARG(Register16, Register16::SP))

OPCODE(0x08, op_ldNptrSP)

OPCODE(0xF8, op_ldHLSPoff)
OPCODE(0xF9, op_ldSPHL)
#pragma endregion

#pragma region LD RptrR8
OPCODE(0x02, op_ldRptrR, OPCODE_ARG(Register16, Register16::BC), OPCODE_ARG(Register, Register::A), OPCODE_ARG(sbyte, 0))
OPCODE(0x12, op_ldRptrR, OPCODE_ARG(Register16, Register16::DE), OPCODE_ARG(Register, Register::A), OPCODE_ARG(sbyte, 0))
OPCODE(0x22, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::A), OPCODE_ARG(sbyte, 1))
OPCODE(0x32, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::A), OPCODE_ARG(sbyte, -1))

OPCODE(0x70, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::B), OPCODE_ARG(sbyte, 0))
OPCODE(0x71, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::C), OPCODE_ARG(sbyte, 0))
OPCODE(0x72, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::D), OPCODE_ARG(sbyte, 0))
OPCODE(0x73, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::E), OPCODE_ARG(sbyte, 0))
OPCODE(0x74, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::H), OPCODE_ARG(sbyte, 0))
OPCODE(0x75, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::L), OPCODE_ARG(sbyte, 0))
OPCODE(0x77, op_ldRptrR, OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(Register, Register::A), OPCODE_ARG(sbyte, 0))
#pragma endregion

#pragma region LD RN8
OPCODE(0x06, op_ldRN, OPCODE_ARG(Register, Register::B))
OPCODE(0x0E, op_ldRN, OPCODE_ARG(Register, Register::C))

OPCODE(0x16, op_ldRN, OPCODE_ARG(Register, Register::D))
OPCODE(0x1E, op_ldRN, OPCODE_ARG(Register, Register::E))

OPCODE(0x26, op_ldRN, OPCODE_ARG(Register, Register::H))
OPCODE(0x2E, op_ldRN, OPCODE_ARG(Register, Register::L))

OPCODE(0x3E, op_ldRN, OPCODE_ARG(Register, Register::A))
#pragma endregion

#pragma region LD RRptr8
OPCODE(0x0A, op_ldRRptr, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register16, Register16::BC), OPCODE_ARG(sbyte, 0))
OPCODE(0x1A, op_ldRRptr, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register16, Register16::DE), OPCODE_ARG(sbyte, 0))
OPCODE(0x2A, op_ldRRptr, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, 1))
OPCODE(0x3A, op_ldRRptr, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, -1))

OPCODE(0x46, op_ldRRptr, OPCODE_ARG(Register, Register::B), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, 0))
OPCODE(0x4E, op_ldRRptr, OPCODE_ARG(Register, Register::C), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, 0))

OPCODE(0x56, op_ldRRptr, OPCODE_ARG(Register, Register::D), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, 0))
OPCODE(0x5E, op_ldRRptr, OPCODE_ARG(Register, Register::E), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, 0))

OPCODE(0x66, op_ldRRptr, OPCODE_ARG(Register, Register::H), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, 0))
OPCODE(0x6E, op_ldRRptr, OPCODE_ARG(Register, Register::L), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, 0))

OPCODE(0x7E, op_ldRRptr, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register16, Register16::HL), OPCODE_ARG(sbyte, 0))
#pragma endregion

#pragma region JUMP
OPCODE(0x18, op_jrI8)
OPCODE(0x20, op_jrCondI8, OPCODE_ARG(Flag, Flag::Z), OPCODE_ARG(bool, false))
OPCODE(0x28, op_jrCondI8, OPCODE_ARG(Flag, Flag::Z), OPCODE_ARG(bool, true))
OPCODE(0x30, op_jrCondI8, OPCODE_ARG(Flag, Flag::C), OPCODE_ARG(bool, false))
OPCODE(0x38, op_jrCondI8, OPCODE_ARG(Flag, Flag::C), OPCODE_ARG(bool, true))

OPCODE(0xC2, op_jpCondN16, OPCODE_ARG(Flag, Flag::Z), OPCODE_ARG(bool, false))
OPCODE(0xCA, op_jpCondN16, OPCODE_ARG(Flag, Flag::Z), OPCODE_ARG(bool, true))
OPCODE(0xD2, op_jpCondN16, OPCODE_ARG(Flag, Flag::C), OPCODE_ARG(bool, false))
OPCODE(0xDA, op_jpCondN16, OPCODE_ARG(Flag, Flag::C), OPCODE_ARG(bool, true))

OPCODE(0xC3, op_jpN16)
OPCODE(0xE9, op_jpHL)
#pragma endregion

#pragma region LD8 MISC
OPCODE(0x36, op_ldHLptrN)

OPCODE(0xE0, op_ldhNsptrA)
OPCODE(0xE2, op_ldhCsptrA)
OPCODE(0xEA, op_ldNptrA)

OPCODE(0xF0, op_ldhANsptr)
OPCODE(0xF2, op_ldhACsptr)
OPCODE(0xFA, op_ldANptr)
#pragma endregion

#pragma region LD RR8
OPCODE(0x40, op_ldRR, OPCODE_ARG(Register, Register::B), OPCODE_ARG(Register, Register::B))
OPCODE(0x41, op_ldRR, OPCODE_ARG(Register, Register::B), OPCODE_ARG(Register, Register::C))
OPCODE(0x42, op_ldRR, OPCODE_ARG(Register, Register::B), OPCODE_ARG(Register, Register::D))
OPCODE(0x43, op_ldRR, OPCODE_ARG(Register, Register::B), OPCODE_ARG(Register, Register::E))
OPCODE(0x44, op_ldRR, OPCODE_ARG(Register, Register::B), OPCODE_ARG(Register, Register::H))
OPCODE(0x45, op_ldRR, OPCODE_ARG(Register, Register::B), OPCODE_ARG(Register, Register::L))
OPCODE(0x47, op_ldRR, OPCODE_ARG(Register, Register::B), OPCODE_ARG(Register, Register::A))

OPCODE(0x48, op_ldRR, OPCODE_ARG(Register, Register::C), OPCODE_ARG(Register, Register::B))
OPCODE(0x49, op_ldRR, OPCODE_ARG(Register, Register::C), OPCODE_ARG(Register, Register::C))
OPCODE(0x4A, op_ldRR, OPCODE_ARG(Register, Register::C), OPCODE_ARG(Register, Register::D))
OPCODE(0x4B, op_ldRR, OPCODE_ARG(Register, Register::C), OPCODE_ARG(Register, Register::E))
OPCODE(0x4C, op_ldRR, OPCODE_ARG(Register, Register::C), OPCODE_ARG(Register, Register::H))
OPCODE(0x4D, op_ldRR, OPCODE_ARG(Register, Register::C), OPCODE_ARG(Register, Register::L))
OPCODE(0x4F, op_ldRR, OPCODE_ARG(Register, Register::C), OPCODE_ARG(Register, Register::A))

OPCODE(0x50, op_ldRR, OPCODE_ARG(Register, Register::D), OPCODE_ARG(Register, Register::B))
OPCODE(0x51, op_ldRR, OPCODE_ARG(Register, Register::D), OPCODE_ARG(Register, Register::C))
OPCODE(0x52, op_ldRR, OPCODE_ARG(Register, Register::D), OPCODE_ARG(Register, Register::D))
OPCODE(0x53, op_ldRR, OPCODE_ARG(Register, Register::D), OPCODE_ARG(Register, Register::E))
OPCODE(0x54, op_ldRR, OPCODE_ARG(Register, Register::D), OPCODE_ARG(Register, Register::H))
OPCODE(0x55, op_ldRR, OPCODE_ARG(Register, Register::D), OPCODE_ARG(Register, Register::L))
OPCODE(0x57, op_ldRR, OPCODE_ARG(Register, Register::D), OPCODE_ARG(Register, Register::A))

OPCODE(0x58, op_ldRR, OPCODE_ARG(Register, Register::E), OPCODE_ARG(Register, Register::B))
OPCODE(0x59, op_ldRR, OPCODE_ARG(Register, Register::E), OPCODE_ARG(Register, Register::C))
OPCODE(0x5A, op_ldRR, OPCODE_ARG(Register, Register::E), OPCODE_ARG(Register, Register::D))
OPCODE(0x5B, op_ldRR, OPCODE_ARG(Register, Register::E), OPCODE_ARG(Register, Register::E))
OPCODE(0x5C, op_ldRR, OPCODE_ARG(Register, Register::E), OPCODE_ARG(Register, Register::H))
OPCODE(0x5D, op_ldRR, OPCODE_ARG(Register, Register::E), OPCODE_ARG(Register, Register::L))
OPCODE(0x5F, op_ldRR, OPCODE_ARG(Register, Register::E), OPCODE_ARG(Register, Register::A))

OPCODE(0x60, op_ldRR, OPCODE_ARG(Register, Register::H), OPCODE_ARG(Register, Register::B))
OPCODE(0x61, op_ldRR, OPCODE_ARG(Register, Register::H), OPCODE_ARG(Register, Register::C))
OPCODE(0x62, op_ldRR, OPCODE_ARG(Register, Register::H), OPCODE_ARG(Register, Register::D))
OPCODE(0x63, op_ldRR, OPCODE_ARG(Register, Register::H), OPCODE_ARG(Register, Register::E))
OPCODE(0x64, op_ldRR, OPCODE_ARG(Register, Register::H), OPCODE_ARG(Register, Register::H))
OPCODE(0x65, op_ldRR, OPCODE_ARG(Register, Register::H), OPCODE_ARG(Register, Register::L))
OPCODE(0x67, op_ldRR, OPCODE_ARG(Register, Register::H), OPCODE_ARG(Register, Register::A))

OPCODE(0x68, op_ldRR, OPCODE_ARG(Register, Register::L), OPCODE_ARG(Register, Register::B))
OPCODE(0x69, op_ldRR, OPCODE_ARG(Register, Register::L), OPCODE_ARG(Register, Register::C))
OPCODE(0x6A, op_ldRR, OPCODE_ARG(Register, Register::L), OPCODE_ARG(Register, Register::D))
OPCODE(0x6B, op_ldRR, OPCODE_ARG(Register, Register::L), OPCODE_ARG(Register, Register::E))
OPCODE(0x6C, op_ldRR, OPCODE_ARG(Register, Register::L), OPCODE_ARG(Register, Register::H))
OPCODE(0x6D, op_ldRR, OPCODE_ARG(Register, Register::L), OPCODE_ARG(Register, Register::L))
OPCODE(0x6F, op_ldRR, OPCODE_ARG(Register, Register::L), OPCODE_ARG(Register, Register::A))

OPCODE(0x78, op_ldRR, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register, Register::B))
OPCODE(0x79, op_ldRR, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register, Register::C))
OPCODE(0x7A, op_ldRR, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register, Register::D))
OPCODE(0x7B, op_ldRR, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register, Register::E))
OPCODE(0x7C, op_ldRR, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register, Register::H))
OPCODE(0x7D, op_ldRR, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register, Register::L))
OPCODE(0x7F, op_ldRR, OPCODE_ARG(Register, Register::A), OPCODE_ARG(Register, Register::A))
#pragma endregion

#pragma region ALU16
OPCODE(0x03, op_incR16, OPCODE_ARG(Register16, Register16::BC))
OPCODE(0x13, op_incR16, OPCODE_ARG(Register16, Register16::DE))
OPCODE(0x23, op_incR16, OPCODE_ARG(Register16, Register16::HL))
OPCODE(0x33, op_incR16, OPCODE_ARG(Register16, Register16::SP))

OPCODE(0x09, op_addHLR16, OPCODE_ARG(Register16, Register16::BC))
OPCODE(0x19, op_addHLR16, OPCODE_ARG(Register16, Register16::DE))
OPCODE(0x29, op_addHLR16, OPCODE_ARG(Register16, Register16::HL))
OPCODE(0x39, op_addHLR16, OPCODE_ARG(Register16, Register16::SP))

OPCODE(0x0B, op_decR16, OPCODE_ARG(Register16, Register16::BC))
OPCODE(0x1B, op_decR16, OPCODE_ARG(Register16, Register16::DE))
OPCODE(0x2B, op_decR16, OPCODE_ARG(Register16, Register16::HL))
OPCODE(0x3B, op_decR16, OPCODE_ARG(Register16, Register16::SP))

OPCODE(0xE8, op_addSPI8)
#pragma endregion

#pragma region INC/DEC8
OPCODE(0x04, op_incR, OPCODE_ARG(Register, Register::B))
OPCODE(0x14, op_incR, OPCODE_ARG(Register, Register::D))
OPCODE(0x24, op_incR, OPCODE_ARG(Register, Register::H))
OPCODE(0x34, op_incHLptr)

OPCODE(0x05, op_decR, OPCODE_ARG(Register, Register::B))
OPCODE(0x15, op_decR, OPCODE_ARG(Register, Register::D))
OPCODE(0x25, op_decR, OPCODE_ARG(Register, Register::H))
OPCODE(0x35, op_decHLptr)

OPCODE(0x0C, op_incR, OPCODE_ARG(Register, Register::C))
OPCODE(0x1C, op_incR, OPCODE_ARG(Register, Register::E))
OPCODE(0x2C, op_incR, OPCODE_ARG(Register, Register::L))
OPCODE(0x3C, op_incR, OPCODE_ARG(Register, Register::A))

OPCODE(0x0D, op_decR, OPCODE_ARG(Register, Register::C))
OPCODE(0x1D, op_decR, OPCODE_ARG(Register, Register::E))
OPCODE(0x2D, op_decR, OPCODE_ARG(Register, Register::L))
OPCODE(0x3D, op_decR, OPCODE_ARG(Register, Register::A))
#pragma endregion

#pragma region RSB
OPCODE(0x07, op_rlca)
OPCODE(0x0F, op_rrca)

OPCODE(0x17, op_rla)
OPCODE(0x1F, op_rra)
#pragma endregion

#pragma region ALU8 MISC
OPCODE(0x27, op_daa)
OPCODE(0x2f, op_cpl)
OPCODE(0x37, op_scf)
OPCODE(0x3f, op_ccf)
#pragma endregion

#pragma region ADD/SUB8
OPCODE(0x80, op_addR, OPCODE_ARG(Register, Register::B))
OPCODE(0x81, op_addR, OPCODE_ARG(Register, Register::C))
OPCODE(0x82, op_addR, OPCODE_ARG(Register, Register::D))
OPCODE(0x83, op_addR, OPCODE_ARG(Register, Register::E))
OPCODE(0x84, op_addR, OPCODE_ARG(Register, Register::H))
OPCODE(0x85, op_addR, OPCODE_ARG(Register, Register::L))
OPCODE(0x87, op_addR, OPCODE_ARG(Register, Register::A))
OPCODE(0x86, op_addHLptr)
OPCODE(0xC6, op_addN)

OPCODE(0x88, op_adcR, OPCODE_ARG(Register, Register::B))
OPCODE(0x89, op_adcR, OPCODE_ARG(Register, Register::C))
OPCODE(0x8A, op_adcR, OPCODE_ARG(Register, Register::D))
OPCODE(0x8B, op_adcR, OPCODE_ARG(Register, Register::E))
OPCODE(0x8C, op_adcR, OPCODE_ARG(Register, Register::H))
OPCODE(0x8D, op_adcR, OPCODE_ARG(Register, Register::L))
OPCODE(0x8F, op_adcR, OPCODE_ARG(Register, Register::A))
OPCODE(0x8E, op_adcHLptr)
OPCODE(0xCE, op_adcN)

OPCODE(0x90, op_subR, OPCODE_ARG(Register, Register::B))
OPCODE(0x91, op_subR, OPCODE_ARG(Register, Register::C))
OPCODE(0x92, op_subR, OPCODE_ARG(Register, Register::D))
OPCODE(0x93, op_subR, OPCODE_ARG(Register, Register::E))
OPCODE(0x94, op_subR, OPCODE_ARG(Register, Register::H))
OPCODE(0x95, op_subR, OPCODE_ARG(Register, Register::L))
OPCODE(0x97, op_subR, OPCODE_ARG(Register, Register::A))
OPCODE(0x96, op_subHLptr)
OPCODE(0xD6, op_subN)

OPCODE(0x98, op_sbcR, OPCODE_ARG(Register, Register::B))
OPCODE(0x99, op_sbcR, OPCODE_ARG(Register, Register::C))
OPCODE(0x9A, op_sbcR, OPCODE_ARG(Register, Register::D))
OPCODE(0x9B, op_sbcR, OPCODE_ARG(Register, Register::E))
OPCODE(0x9C, op_sbcR, OPCODE_ARG(Register, Register::H))
OPCODE(0x9D, op_sbcR, OPCODE_ARG(Register, Register::L))
OPCODE(0x9F, op_sbcR, OPCODE_ARG(Register, Register::A))
OPCODE(0x9E, op_sbcHLptr)
OPCODE(0xDE, op_sbcN)
#pragma endregion

#pragma region AND/XOR/OR/CP
OPCODE(0xA0, op_andR, OPCODE_ARG(Register, Register::B))
OPCODE(0xA1, op_andR, OPCODE_ARG(Register, Register::C))
OPCODE(0xA2, op_andR, OPCODE_ARG(Register, Register::D))
OPCODE(0xA3, op_andR, OPCODE_ARG(Register, Register::E))
OPCODE(0xA4, op_andR, OPCODE_ARG(Register, Register::H))
OPCODE(0xA5, op_andR, OPCODE_ARG(Register, Register::L))
OPCODE(0xA7, op_andR, OPCODE_ARG(Register, Register::A))
OPCODE(0xA6, op_andHLptr)
OPCODE(0xE6, op_andN)

OPCODE(0xA8, op_xorR, OPCODE_ARG(Register, Register::B))
OPCODE(0xA9, op_xorR, OPCODE_ARG(Register, Register::C))
OPCODE(0xAA, op_xorR, OPCODE_ARG(Register, Register::D))
OPCODE(0xAB, op_xorR, OPCODE_ARG(Register, Register::E))
OPCODE(0xAC, op_xorR, OPCODE_ARG(Register, Register::H))
OPCODE(0xAD, op_xorR, OPCODE_ARG(Register, Register::L))
OPCODE(0xAF, op_xorR, OPCODE_ARG(Register, Register::A))
OPCODE(0xAE, op_xorHLptr)
OPCODE(0xEE, op_xorN)

OPCODE(0xB0, op_orR, OPCODE_ARG(Register, Register::B))
OPCODE(0xB1, op_orR, OPCODE_ARG(Register, Register::C))
OPCODE(0xB2, op_orR, OPCODE_ARG(Register, Register::D))
OPCODE(0xB3, op_orR, OPCODE_ARG(Register, Register::E))
OPCODE(0xB4, op_orR, OPCODE_ARG(Register, Register::H))
OPCODE(0xB5, op_orR, OPCODE_ARG(Register, Register::L))
OPCODE(0xB7, op_orR, OPCODE_ARG(Register, Register::A))
OPCODE(0xB6, op_orHLptr)
OPCODE(0xF6, op_orN)

OPCODE(0xB8, op_cpR, OPCODE_ARG(Register, Register::B))
OPCODE(0xB9, op_cpR, OPCODE_ARG(Register, Register::C))
OPCODE(0xBA, op_cpR, OPCODE_ARG(Register, Register::D))
OPCODE(0xBB, op_cpR, OPCODE_ARG(Register, Register::E))
OPCODE(0xBC, op_cpR, OPCODE_ARG(Register, Register::H))
OPCODE(0xBD, op_cpR, OPCODE_ARG(Register, Register::L))
OPCODE(0xBF, op_cpR, OPCODE_ARG(Register, Register::A))
OPCODE(0xBE, op_cpHLptr)
OPCODE(0xFE, op_cpN)
#pragma endregion

#pragma region RET/CALL/RST
OPCODE(0xC0, op_retCond, OPCODE_ARG(Flag, Flag::Z), OPCODE_ARG(bool, false))
OPCODE(0xC8, op_retCond, OPCODE_ARG(Flag, Flag::Z), OPCODE_ARG(bool, true))
OPCODE(0xD0, op_retCond, OPCODE_ARG(Flag, Flag::C), OPCODE_ARG(bool, false))
OPCODE(0xD8, op_retCond, OPCODE_ARG(Flag, Flag::C), OPCODE_ARG(bool, true))
OPCODE(0xC9, op_ret)
OPCODE(0xD9, op_reti)

OPCODE(0xC4, op_callCond, OPCODE_ARG(Flag, Flag::Z), OPCODE_ARG(bool, false))
OPCODE(0xCC, op_callCond, OPCODE_ARG(Flag, Flag::Z), OPCODE_ARG(bool, true))
OPCODE(0xD4, op_callCond, OPCODE_ARG(Flag, Flag::C), OPCODE_ARG(bool, false))
OPCODE(0xDC, op_callCond, OPCODE_ARG(Flag, Flag::C), OPCODE_ARG(bool, true))
OPCODE(0xCD, op_call)

OPCODE(0xC7, op_rstX, OPCODE_ARG(byte, 0x00))
OPCODE(0xCF, op_rstX, OPCODE_ARG(byte, 0x08))
OPCODE(0xD7, op_rstX, OPCODE_ARG(byte, 0x10))
OPCODE(0xDF, op_rstX, OPCODE_ARG(byte, 0x18))
OPCODE(0xE7, op_rstX, OPCODE_ARG(byte, 0x20))
OPCODE(0xEF, op_rstX, OPCODE_ARG(byte, 0x28))
OPCODE(0xF7, op_rstX, OPCODE_ARG(byte, 0x30))
OPCODE(0xFF, op_rstX, OPCODE_ARG(byte, 0x38))
#pragma endregion

#pragma region POP/PUSH
OPCODE(0xC1, op_popR16, OPCODE_ARG(Register16, Register16::BC))
OPCODE(0xD1, op_popR16, OPCODE_ARG(Register16, Register16::DE))
OPCODE(0xE1, op_popR16, OPCODE_ARG(Register16, Register16::HL))
OPCODE(0xF1, op_popR16, OPCODE_ARG(Register16, Register16::AF))

OPCODE(0xC5, op_pushR16, OPCODE_ARG(Register16, Register16::BC))
OPCODE(0xD5, op_pushR16, OPCODE_ARG(Register16, Register16::DE))
OPCODE(0xE5, op_pushR16, OPCODE_ARG(Register16, Register16::HL))
OPCODE(0xF5, op_pushR16, OPCODE_ARG(Register16, Register16::AF))
#pragma endregion

#pragma region PREFIXED
#pragma region RLC
OPCODE_EX(0x00, op_rlcR, OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x01, op_rlcR, OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x02, op_rlcR, OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x03, op_rlcR, OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x04, op_rlcR, OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x05, op_rlcR, OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x07, op_rlcR, OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x06, op_rlcHLptr)
#pragma endregion

#pragma region RRC
OPCODE_EX(0x08, op_rrcR, OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x09, op_rrcR, OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x0A, op_rrcR, OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x0B, op_rrcR, OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x0C, op_rrcR, OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x0D, op_rrcR, OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x0F, op_rrcR, OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x0E, op_rrcHLptr)
#pragma endregion

#pragma region RL
OPCODE_EX(0x10, op_rlR, OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x11, op_rlR, OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x12, op_rlR, OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x13, op_rlR, OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x14, op_rlR, OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x15, op_rlR, OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x17, op_rlR, OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x16, op_rlHLptr)
#pragma endregion

#pragma region RR
OPCODE_EX(0x18, op_rrR, OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x19, op_rrR, OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x1A, op_rrR, OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x1B, op_rrR, OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x1C, op_rrR, OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x1D, op_rrR, OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x1F, op_rrR, OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x1E, op_rrHLptr)
#pragma endregion

#pragma region SLA
OPCODE_EX(0x20, op_slaR, OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x21, op_slaR, OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x22, op_slaR, OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x23, op_slaR, OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x24, op_slaR, OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x25, op_slaR, OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x27, op_slaR, OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x26, op_slaHLptr)
#pragma endregion

#pragma region SRA
OPCODE_EX(0x28, op_sraR, OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x29, op_sraR, OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x2A, op_sraR, OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x2B, op_sraR, OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x2C, op_sraR, OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x2D, op_sraR, OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x2F, op_sraR, OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x2E, op_sraHLptr)
#pragma endregion

#pragma region SWAP
OPCODE_EX(0x30, op_swapR, OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x31, op_swapR, OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x32, op_swapR, OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x33, op_swapR, OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x34, op_swapR, OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x35, op_swapR, OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x37, op_swapR, OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x36, op_swapHLptr)
#pragma endregion

#pragma region SRL
OPCODE_EX(0x38, op_srlR, OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x39, op_srlR, OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x3A, op_srlR, OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x3B, op_srlR, OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x3C, op_srlR, OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x3D, op_srlR, OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x3F, op_srlR, OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x3E, op_srlHLptr)
#pragma endregion

#pragma region BIT
OPCODE_EX(0x40, op_bitXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x41, op_bitXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x42, op_bitXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x43, op_bitXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x44, op_bitXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x45, op_bitXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x47, op_bitXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x46, op_bitXHLptr, OPCODE_ARG(byte, 0))

OPCODE_EX(0x48, op_bitXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x49, op_bitXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x4A, op_bitXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x4B, op_bitXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x4C, op_bitXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x4D, op_bitXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x4F, op_bitXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x4E, op_bitXHLptr, OPCODE_ARG(byte, 1))

OPCODE_EX(0x50, op_bitXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x51, op_bitXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x52, op_bitXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x53, op_bitXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x54, op_bitXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x55, op_bitXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x57, op_bitXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x56, op_bitXHLptr, OPCODE_ARG(byte, 2))

OPCODE_EX(0x58, op_bitXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x59, op_bitXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x5A, op_bitXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x5B, op_bitXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x5C, op_bitXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x5D, op_bitXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x5F, op_bitXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x5E, op_bitXHLptr, OPCODE_ARG(byte, 3))

OPCODE_EX(0x60, op_bitXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x61, op_bitXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x62, op_bitXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x63, op_bitXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x64, op_bitXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x65, op_bitXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x67, op_bitXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x66, op_bitXHLptr, OPCODE_ARG(byte, 4))

OPCODE_EX(0x68, op_bitXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x69, op_bitXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x6A, op_bitXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x6B, op_bitXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x6C, op_bitXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x6D, op_bitXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x6F, op_bitXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x6E, op_bitXHLptr, OPCODE_ARG(byte, 5))

OPCODE_EX(0x70, op_bitXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x71, op_bitXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x72, op_bitXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x73, op_bitXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x74, op_bitXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x75, op_bitXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x77, op_bitXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x76, op_bitXHLptr, OPCODE_ARG(byte, 6))

OPCODE_EX(0x78, op_bitXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x79, op_bitXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x7A, op_bitXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x7B, op_bitXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x7C, op_bitXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x7D, op_bitXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x7F, op_bitXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x7E, op_bitXHLptr, OPCODE_ARG(byte, 7))
#pragma endregion

#pragma region RES
OPCODE_EX(0x80, op_resXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x81, op_resXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x82, op_resXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x83, op_resXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x84, op_resXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x85, op_resXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x87, op_resXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x86, op_resXHLptr, OPCODE_ARG(byte, 0))

OPCODE_EX(0x88, op_resXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x89, op_resXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x8A, op_resXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x8B, op_resXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x8C, op_resXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x8D, op_resXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x8F, op_resXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x8E, op_resXHLptr, OPCODE_ARG(byte, 1))

OPCODE_EX(0x90, op_resXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x91, op_resXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x92, op_resXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x93, op_resXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x94, op_resXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x95, op_resXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x97, op_resXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x96, op_resXHLptr, OPCODE_ARG(byte, 2))

OPCODE_EX(0x98, op_resXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0x99, op_resXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0x9A, op_resXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0x9B, op_resXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0x9C, op_resXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0x9D, op_resXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0x9F, op_resXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0x9E, op_resXHLptr, OPCODE_ARG(byte, 3))

OPCODE_EX(0xA0, op_resXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xA1, op_resXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xA2, op_resXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xA3, op_resXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xA4, op_resXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xA5, op_resXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xA7, op_resXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xA6, op_resXHLptr, OPCODE_ARG(byte, 4))

OPCODE_EX(0xA8, op_resXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xA9, op_resXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xAA, op_resXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xAB, op_resXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xAC, op_resXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xAD, op_resXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xAF, op_resXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xAE, op_resXHLptr, OPCODE_ARG(byte, 5))

OPCODE_EX(0xB0, op_resXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xB1, op_resXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xB2, op_resXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xB3, op_resXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xB4, op_resXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xB5, op_resXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xB7, op_resXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xB6, op_resXHLptr, OPCODE_ARG(byte, 6))

OPCODE_EX(0xB8, op_resXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xB9, op_resXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xBA, op_resXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xBB, op_resXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xBC, op_resXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xBD, op_resXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xBF, op_resXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xBE, op_resXHLptr, OPCODE_ARG(byte, 7))
#pragma endregion

#pragma region SET
OPCODE_EX(0xC0, op_setXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xC1, op_setXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xC2, op_setXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xC3, op_setXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xC4, op_setXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xC5, op_setXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xC7, op_setXR, OPCODE_ARG(byte, 0), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xC6, op_setXHLptr, OPCODE_ARG(byte, 0))

OPCODE_EX(0xC8, op_setXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xC9, op_setXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xCA, op_setXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xCB, op_setXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xCC, op_setXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xCD, op_setXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xCF, op_setXR, OPCODE_ARG(byte, 1), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xCE, op_setXHLptr, OPCODE_ARG(byte, 1))

OPCODE_EX(0xD0, op_setXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xD1, op_setXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xD2, op_setXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xD3, op_setXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xD4, op_setXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xD5, op_setXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xD7, op_setXR, OPCODE_ARG(byte, 2), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xD6, op_setXHLptr, OPCODE_ARG(byte, 2))

OPCODE_EX(0xD8, op_setXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xD9, op_setXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xDA, op_setXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xDB, op_setXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xDC, op_setXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xDD, op_setXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xDF, op_setXR, OPCODE_ARG(byte, 3), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xDE, op_setXHLptr, OPCODE_ARG(byte, 3))

OPCODE_EX(0xE0, op_setXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xE1, op_setXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xE2, op_setXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xE3, op_setXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xE4, op_setXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xE5, op_setXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xE7, op_setXR, OPCODE_ARG(byte, 4), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xE6, op_setXHLptr, OPCODE_ARG(byte, 4))

OPCODE_EX(0xE8, op_setXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xE9, op_setXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xEA, op_setXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xEB, op_setXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xEC, op_setXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xED, op_setXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xEF, op_setXR, OPCODE_ARG(byte, 5), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xEE, op_setXHLptr, OPCODE_ARG(byte, 5))

OPCODE_EX(0xF0, op_setXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xF1, op_setXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xF2, op_setXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xF3, op_setXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xF4, op_setXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xF5, op_setXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xF7, op_setXR, OPCODE_ARG(byte, 6), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xF6, op_setXHLptr, OPCODE_ARG(byte, 6))

OPCODE_EX(0xF8, op_setXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::B))
OPCODE_EX(0xF9, op_setXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::C))
OPCODE_EX(0xFA, op_setXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::D))
OPCODE_EX(0xFB, op_setXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::E))
OPCODE_EX(0xFC, op_setXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::H))
OPCODE_EX(0xFD, op_setXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::L))
OPCODE_EX(0xFF, op_setXR, OPCODE_ARG(byte, 7), OPCODE_ARG(Register, Register::A))
OPCODE_EX(0xFE, op_setXHLptr, OPCODE_ARG(byte, 7))
#pragma endregion

#pragma endregion

#undef OPCODE
#undef OPCODE_EX
#undef OPCODE_ARG
