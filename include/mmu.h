#pragma once
#include "utility.h"

#include <vector>

struct AddressRange
{
    uint16_t start;
    uint16_t end;

    constexpr uint16_t size() const
    {
        return (end - start) + 1;
    }

    constexpr bool contains(const uint16_t addr) const
    {
        return addr >= start && addr <= end;
    }
};

inline constexpr struct
{
    AddressRange bank0 = { 0x0000, 0x3FFF };
    uint16_t mbc       = 0x0147;
    uint16_t romSize   = 0x0148;
    uint16_t ramSize   = 0x0149;

    AddressRange bankN    = { 0x4000, 0x7FFF };
    AddressRange vram     = { 0x8000, 0x9FFF };
    AddressRange wram     = { 0xC000, 0xDFFF };
    AddressRange echo     = { 0xE000, 0xFDFF };
    AddressRange oam      = { 0xFE00, 0xFE9F };
    AddressRange unusable = { 0xFEA0, 0xFEFF };

    AddressRange io   = { 0xFF00, 0xFF7F };
    uint16_t joypad   = 0xFF00;
    uint16_t bootMode = 0xFF50;

    AddressRange hram           = { 0xFF80, 0xFFFE };
    uint16_t interruptsRegister = 0xFFFF;
} ADDRESSES;

using Buffer = std::vector<byte>;

class MMU
{
public:
    void reset();

    bool isBootMode() const;

    byte readByte(uint16_t addr);
    bool writeByte(uint16_t addr, byte data);

    uint16_t read16(const uint16_t addr)
    {
        return make16(readByte(addr), readByte(addr + 1));
    }

    bool write16(const uint16_t addr, const uint16_t data)
    {
        return writeByte(addr, lsb(data)) && writeByte(addr + 1, msb(data));
    }

    void addCycle();
    size_t getCycle() const;

private:
    byte m_memory[64 * KB]{};
    uint32_t m_cycleCount = 0;
};
