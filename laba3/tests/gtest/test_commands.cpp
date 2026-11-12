#include <gtest/gtest.h>
#include "commandsClass.h"

// toCommand: каждое слово превращается в свою команду
TEST(CommandsTest, AllKnownCommands)
{
    vector<pair<string, Command>> all = {
        {"MPUSH", Command::MPUSH}, {"MINSERT", Command::MINSERT}, {"MGET", Command::MGET},
        {"MSET", Command::MSET}, {"MDEL", Command::MDEL}, {"MLEN", Command::MLEN},
        {"MPRINT", Command::MPRINT},
        {"FPUSHH", Command::FPUSHH}, {"FPUSHT", Command::FPUSHT}, {"FPUSHA", Command::FPUSHA},
        {"FPUSHB", Command::FPUSHB}, {"FDELH", Command::FDELH}, {"FDELT", Command::FDELT},
        {"FDELA", Command::FDELA}, {"FDELB", Command::FDELB}, {"FDEL", Command::FDEL},
        {"FFIND", Command::FFIND}, {"FPRINT", Command::FPRINT}, {"FPRINTR", Command::FPRINTR},
        {"LPUSHH", Command::LPUSHH}, {"LPUSHT", Command::LPUSHT}, {"LPUSHA", Command::LPUSHA},
        {"LPUSHB", Command::LPUSHB}, {"LDELH", Command::LDELH}, {"LDELT", Command::LDELT},
        {"LDELA", Command::LDELA}, {"LDELB", Command::LDELB}, {"LDEL", Command::LDEL},
        {"LFIND", Command::LFIND}, {"LPRINT", Command::LPRINT}, {"LPRINTR", Command::LPRINTR},
        {"SPUSH", Command::SPUSH}, {"SPOP", Command::SPOP}, {"SPRINT", Command::SPRINT},
        {"QPUSH", Command::QPUSH}, {"QPOP", Command::QPOP}, {"QPRINT", Command::QPRINT},
        {"TINSERT", Command::TINSERT}, {"TFIND", Command::TFIND},
        {"TCOMPLETE", Command::TCOMPLETE}, {"TPRINT", Command::TPRINT},
        {"HADD", Command::HADD}, {"HDEL", Command::HDEL}, {"HFIND", Command::HFIND},
        {"HPRINT", Command::HPRINT},
        {"KADD", Command::KADD}, {"KDEL", Command::KDEL}, {"KFIND", Command::KFIND},
        {"KPRINT", Command::KPRINT},
        {"PRINT", Command::PRINT}
    };

    for (auto& [text, command] : all)
    {
        EXPECT_EQ(toCommand(text), command);
    }
}

TEST(CommandsTest, UnknownCommand)
{
    EXPECT_EQ(toCommand("HELLO"), Command::UNKNOWN);
    EXPECT_EQ(toCommand(""), Command::UNKNOWN);
}

// wrongType
TEST(CommandsTest, WrongTypeWhenNoStructure)
{
    json db = json::object();
    EXPECT_FALSE(wrongType(db, "x", "array"));     // структуры нет - можно создавать
}

TEST(CommandsTest, WrongTypeWhenSameType)
{
    json db = json::object();
    db["x"]["type"] = "array";
    EXPECT_FALSE(wrongType(db, "x", "array"));
}

TEST(CommandsTest, WrongTypeWhenOtherType)
{
    json db = json::object();
    db["x"]["type"] = "stack";

    testing::internal::CaptureStdout();
    bool result = wrongType(db, "x", "array");
    string out = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(result);
    EXPECT_EQ(out, "x это не array\n");
}
