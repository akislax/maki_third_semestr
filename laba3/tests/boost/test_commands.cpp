#include <boost/test/unit_test.hpp>
#include "capture.h"
#include "commandsClass.h"

// toCommand: каждое слово превращается в свою команду
BOOST_AUTO_TEST_CASE(CommandsTest_AllKnownCommands)
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
        BOOST_CHECK(toCommand(text) == command);
    }
}

BOOST_AUTO_TEST_CASE(CommandsTest_UnknownCommand)
{
    BOOST_CHECK(toCommand("HELLO") == Command::UNKNOWN);
    BOOST_CHECK(toCommand("") == Command::UNKNOWN);
}

// wrongType
BOOST_AUTO_TEST_CASE(CommandsTest_WrongTypeWhenNoStructure)
{
    json db = json::object();
    BOOST_CHECK(!wrongType(db, "x", "array"));     // структуры нет - можно создавать
}

BOOST_AUTO_TEST_CASE(CommandsTest_WrongTypeWhenSameType)
{
    json db = json::object();
    db["x"]["type"] = "array";
    BOOST_CHECK(!wrongType(db, "x", "array"));
}

BOOST_AUTO_TEST_CASE(CommandsTest_WrongTypeWhenOtherType)
{
    json db = json::object();
    db["x"]["type"] = "stack";

    startCapture();
    bool result = wrongType(db, "x", "array");
    string out = stopCapture();

    BOOST_CHECK(result);
    BOOST_CHECK_EQUAL(out, "x это не array\n");
}
