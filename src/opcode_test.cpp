#ifdef BUILD_TESTING
#include "emu.h"
CLANG_IGNORE_WARNING_PUSH("-Wglobal-constructors")

#include <fstream>

NO_WARNINGS_PUSH
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
NO_WARNINGS_POP

namespace
{
    struct TestState
    {
        byte registers[underlying(Register::COUNT)];
        uint16_t pc;
        uint16_t sp;
        bool ime;
        bool ie;
        std::vector<std::pair<uint16_t /*addr*/, byte>> ram;
    };

    struct OpcodeTest
    {
        std::string name;
        TestState initial;
        TestState final;
    };

    void from_json(const nlohmann::json& json, TestState& state)
    {
        memset(state.registers, 0, sizeof(state.registers));
        state.ram.clear();

        json.at("pc").get_to(state.pc);
        json.at("sp").get_to(state.sp);
        json.at("a").get_to(state.registers[underlying(Register::A)]);
        json.at("f").get_to(state.registers[underlying(Register::F)]);
        json.at("b").get_to(state.registers[underlying(Register::B)]);
        json.at("c").get_to(state.registers[underlying(Register::C)]);
        json.at("d").get_to(state.registers[underlying(Register::D)]);
        json.at("e").get_to(state.registers[underlying(Register::E)]);
        json.at("h").get_to(state.registers[underlying(Register::H)]);
        json.at("l").get_to(state.registers[underlying(Register::L)]);
        state.ime = json.at("ime").get<int>() != 0;
        state.ie  = json.contains("ie") ? json.at("ie").get<int>() != 0 : false; // TODO: Check what this is

        for (const auto& entry : json.at("ram"))
        {
            state.ram.emplace_back(entry[0], entry[1]);
        }
    }

    void from_json(const nlohmann::json& json, OpcodeTest& test)
    {
        json.at("name").get_to(test.name);
        json.at("initial").get_to(test.initial);
        json.at("final").get_to(test.final);
    }

    class OpcodeTests : public testing::TestWithParam<std::filesystem::path>
    {
    };

    std::vector<std::filesystem::path> getTests()
    {
        std::vector<std::filesystem::path> tests;
        const std::filesystem::path testsRoot = "tests/opcodes";

        assert(std::filesystem::exists(testsRoot));
        assert(std::filesystem::is_directory(testsRoot));

        tests.reserve(501);

        for (const auto& file : std::filesystem::recursive_directory_iterator(testsRoot))
        {
            if (file.is_directory())
                continue;

            const std::filesystem::path filePath = file.path();

            if (filePath.extension() != ".json")
            {
                printf("WARNING: Skipped non-json test file @ \"%s\"\n", filePath.string().c_str());
                continue;
            }

            tests.emplace_back(filePath);
        }

        return tests;
    }
} // namespace

INSTANTIATE_TEST_SUITE_P(, OpcodeTests, testing::ValuesIn(getTests()));

TEST_P(OpcodeTests, SingleStep)
{
    const std::filesystem::path& testPath = GetParam();
    printf("Path: %s\n", testPath.string().c_str());
    nlohmann::json json;

    {
        std::ifstream fs(testPath);
        fs >> json;
    }

    for (const auto& testJson : json)
    {
        const auto& [testName, initialState, finalState] = testJson.get<OpcodeTest>();
        printf("Current test: %s\n", testName.c_str());

        constexpr byte startReg = underlying(Register::A);     // Skip IR - tests don't include it
        constexpr byte endReg   = underlying(Register::SP_HI); // Skip SP - tests handle it as a separate val

        Emu ctx;
        ctx.reset();

        // Init emu state
        for (byte regVal = startReg; regVal < endReg; ++regVal)
        {
            const auto reg = static_cast<Register>(regVal);
            ctx.setRegister(reg, initialState.registers[regVal]);
            ASSERT_EQ(ctx.getRegister(reg), initialState.registers[regVal]) << "Register::" << to_string(reg);
        }

        ctx.setPC(initialState.pc);
        ASSERT_EQ(ctx.getPC(), initialState.pc);

        ctx.setSP(initialState.sp);
        ASSERT_EQ(ctx.getSP(), initialState.sp);

        ctx.setContextFlag(ContextFlag::IME, initialState.ime);
        ASSERT_EQ(ctx.getContextFlag(ContextFlag::IME), initialState.ime);

        MMU& mmu = ctx.getMMU();
        ASSERT_TRUE(mmu.writeByte(ADDRESSES.bootMode, 0xFF)); // Force disable boot mode - boot rom doesn't run in test mode
        ASSERT_EQ(mmu.readByte(ADDRESSES.bootMode), 0xFF);

        ASSERT_TRUE(mmu.writeByte(ADDRESSES.interruptsRegister, initialState.ie)); // TODO: Check what this is
        ASSERT_EQ(mmu.readByte(ADDRESSES.interruptsRegister), initialState.ie);

        for (const auto& [addr, val] : initialState.ram)
        {
            ASSERT_TRUE(mmu.writeByte(addr, val));
            ASSERT_EQ(mmu.readByte(addr), val) << "addr = 0x" << std::hex << addr;
        }

        ctx.step();

        // Validate emu state
        EXPECT_EQ(ctx.getPC(), finalState.pc);
        EXPECT_EQ(ctx.getSP(), finalState.sp);
        EXPECT_EQ(ctx.getContextFlag(ContextFlag::IME), finalState.ime);

        for (byte regVal = startReg; regVal < endReg; ++regVal)
        {
            const auto reg = static_cast<Register>(regVal);
            EXPECT_EQ(ctx.getRegister(reg), finalState.registers[regVal]) << "Register::" << to_string(reg);
        }

        for (const auto& [addr, val] : finalState.ram)
        {
            EXPECT_EQ(mmu.readByte(addr), val) << "addr = 0x" << std::hex << addr;
        }
    }
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
CLANG_IGNORE_WARNING_POP
#endif
