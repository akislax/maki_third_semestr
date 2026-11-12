#include <catch2/catch_test_macros.hpp>
#include "capture.h"
#include "dlistClass.h"

// вспомогательные функции: печать списка в строку
static string forward(const Dlist& dl)
{
    startCapture();
    dl.printForward();
    return stopCapture();
}

static string backward(const Dlist& dl)
{
    startCapture();
    dl.printBackward();
    return stopCapture();
}

// конструктор
TEST_CASE("DlistTest.NewListIsEmpty")
{
    Dlist dl;
    CHECK(dl.isEmptyList());
    CHECK(dl.getHead() == nullptr);
}

// pushBack и popBack
TEST_CASE("DlistTest.PushBackAndPopBack")
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    CHECK(forward(dl) == "a <-> b <-> nullptr\n");
    CHECK(backward(dl) == "b <-> a <-> nullptr\n");

    dl.popBack();
    CHECK(forward(dl) == "a <-> nullptr\n");

    dl.popBack();
    dl.popBack();                    // на пустом ничего не ломается
    CHECK(dl.isEmptyList());
}

// pushFront и popFront
TEST_CASE("DlistTest.PushFrontAndPopFront")
{
    Dlist dl;
    dl.pushFront("b");
    dl.pushFront("a");
    CHECK(forward(dl) == "a <-> b <-> nullptr\n");

    dl.popFront();
    CHECK(forward(dl) == "b <-> nullptr\n");

    dl.popFront();
    dl.popFront();                   // на пустом ничего не ломается
    CHECK(dl.isEmptyList());
}

// pushBeetwen - вставка после узла
TEST_CASE("DlistTest.PushAfterNode")
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("c");
    dl.pushBeetwen(dl.findName("a"), "b");   // в середину
    dl.pushBeetwen(dl.findName("c"), "d");   // после хвоста - d станет хвостом
    dl.pushBeetwen(nullptr, "x");

    CHECK(forward(dl) == "a <-> b <-> c <-> d <-> nullptr\n");
    CHECK(backward(dl) == "d <-> c <-> b <-> a <-> nullptr\n");
}

// pushBefore
TEST_CASE("DlistTest.PushBeforeNode")
{
    Dlist dl;
    dl.pushBack("b");
    dl.pushBack("d");
    dl.pushBefore(dl.findName("b"), "a");    // перед головой
    dl.pushBefore(dl.findName("d"), "c");    // в середину
    dl.pushBefore(nullptr, "x");

    CHECK(forward(dl) == "a <-> b <-> c <-> d <-> nullptr\n");
}

// popAfter
TEST_CASE("DlistTest.PopAfterNode")
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.pushBack("c");

    dl.popAfter(dl.findName("a"));           // из середины
    CHECK(forward(dl) == "a <-> c <-> nullptr\n");

    dl.popAfter(dl.findName("a"));           // хвост - a станет хвостом
    CHECK(backward(dl) == "a <-> nullptr\n");

    dl.popAfter(dl.findName("a"));           // после хвоста никого нет
    dl.popAfter(nullptr);
    CHECK(forward(dl) == "a <-> nullptr\n");
}

// popBefore
TEST_CASE("DlistTest.PopBeforeNode")
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.pushBack("c");

    dl.popBefore(dl.findName("c"));          // из середины
    CHECK(forward(dl) == "a <-> c <-> nullptr\n");

    dl.popBefore(dl.findName("c"));          // голову - c станет головой
    CHECK(forward(dl) == "c <-> nullptr\n");

    dl.popBefore(dl.findName("c"));          // перед головой никого нет
    dl.popBefore(nullptr);
    CHECK(forward(dl) == "c <-> nullptr\n");
}

// findName
TEST_CASE("DlistTest.FindName")
{
    Dlist dl;
    dl.pushBack("a");
    CHECK(dl.findName("a") != nullptr);
    CHECK(dl.findName("z") == nullptr);
}

// popName
TEST_CASE("DlistTest.PopName")
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.pushBack("c");

    dl.popName("a");                         // голова
    dl.popName("c");                         // хвост

    startCapture();
    dl.popName("z");                         // такого нет
    string out = stopCapture();

    CHECK(forward(dl) == "b <-> nullptr\n");
    CHECK(out.find("нет такого элемента") != string::npos);
}

// handleDlist
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleDlist(command, db, "dl", ss);
}

TEST_CASE("DlistHandlerTest.AllCommands")
{
    json db = json::object();

    startCapture();
    run(db, Command::LPUSHT, "b");           // b
    run(db, Command::LPUSHH, "a");           // a b
    run(db, Command::LPUSHT, "e");           // a b e
    run(db, Command::LPUSHA, "b c");         // a b c e
    run(db, Command::LPUSHB, "e d");         // a b c d e
    run(db, Command::LPUSHA, "z x");         // нет z
    run(db, Command::LPUSHB, "z x");         // нет z
    run(db, Command::LDELH, "");             // b c d e
    run(db, Command::LDELT, "");             // b c d
    run(db, Command::LDELA, "b");            // b d
    run(db, Command::LDELA, "z");            // нет z
    run(db, Command::LDELB, "d");            // d
    run(db, Command::LDELB, "z");            // нет z
    run(db, Command::LPUSHT, "k");           // d k
    run(db, Command::LDEL, "d");             // k
    run(db, Command::LFIND, "k");
    run(db, Command::LFIND, "z");
    run(db, Command::LPRINT, "");
    run(db, Command::LPRINTR, "");
    run(db, Command::PRINT, "");             // чужая команда
    string out = stopCapture();

    CHECK(out.find("-> TRUE") != string::npos);
    CHECK(out.find("-> FALSE") != string::npos);
    CHECK(db["dl"]["type"].get<string>() == "dlist");
    CHECK(db["dl"]["data"].size() == 1u);
    CHECK(db["dl"]["data"][0].get<string>() == "k");
}

TEST_CASE("DlistHandlerTest.WrongTypeIsRejected")
{
    json db = json::object();
    db["dl"]["type"] = "flist";
    db["dl"]["data"] = json::array();

    startCapture();
    run(db, Command::LPUSHT, "a");
    stopCapture();

    CHECK(db["dl"]["type"].get<string>() == "flist");
}
