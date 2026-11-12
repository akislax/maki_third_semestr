#include <catch2/catch_test_macros.hpp>
#include "capture.h"
#include "commandsClass.h"

// toCommand: каждое слово превращается в свою команду
TEST_CASE("CommandsTest.AllKnownCommands")
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
        CHECK(toCommand(text) == command);
    }
}

TEST_CASE("CommandsTest.UnknownCommand")
{
    CHECK(toCommand("HELLO") == Command::UNKNOWN);
    CHECK(toCommand("") == Command::UNKNOWN);
}

// wrongType
TEST_CASE("CommandsTest.WrongTypeWhenNoStructure")
{
    json db = json::object();
    CHECK_FALSE(wrongType(db, "x", "array"));     // структуры нет - можно создавать
}

TEST_CASE("CommandsTest.WrongTypeWhenSameType")
{
    json db = json::object();
    db["x"]["type"] = "array";
    CHECK_FALSE(wrongType(db, "x", "array"));
}

TEST_CASE("CommandsTest.WrongTypeWhenOtherType")
{
    json db = json::object();
    db["x"]["type"] = "stack";

    startCapture();
    bool result = wrongType(db, "x", "array");
    string out = stopCapture();

    CHECK(result);
    CHECK(out == "x это не array\n");
}
