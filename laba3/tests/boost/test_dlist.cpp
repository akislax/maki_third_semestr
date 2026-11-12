#include <boost/test/unit_test.hpp>
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
BOOST_AUTO_TEST_CASE(DlistTest_NewListIsEmpty)
{
    Dlist dl;
    BOOST_CHECK(dl.isEmptyList());
    BOOST_CHECK(dl.getHead() == nullptr);
}

// pushBack и popBack
BOOST_AUTO_TEST_CASE(DlistTest_PushBackAndPopBack)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    BOOST_CHECK(forward(dl) == "a <-> b <-> nullptr\n");
    BOOST_CHECK(backward(dl) == "b <-> a <-> nullptr\n");

    dl.popBack();
    BOOST_CHECK(forward(dl) == "a <-> nullptr\n");

    dl.popBack();
    dl.popBack();                    // на пустом ничего не ломается
    BOOST_CHECK(dl.isEmptyList());
}

// pushFront и popFront
BOOST_AUTO_TEST_CASE(DlistTest_PushFrontAndPopFront)
{
    Dlist dl;
    dl.pushFront("b");
    dl.pushFront("a");
    BOOST_CHECK(forward(dl) == "a <-> b <-> nullptr\n");

    dl.popFront();
    BOOST_CHECK(forward(dl) == "b <-> nullptr\n");

    dl.popFront();
    dl.popFront();                   // на пустом ничего не ломается
    BOOST_CHECK(dl.isEmptyList());
}

// pushBeetwen - вставка после узла
BOOST_AUTO_TEST_CASE(DlistTest_PushAfterNode)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("c");
    dl.pushBeetwen(dl.findName("a"), "b");   // в середину
    dl.pushBeetwen(dl.findName("c"), "d");   // после хвоста - d станет хвостом
    dl.pushBeetwen(nullptr, "x");

    BOOST_CHECK(forward(dl) == "a <-> b <-> c <-> d <-> nullptr\n");
    BOOST_CHECK(backward(dl) == "d <-> c <-> b <-> a <-> nullptr\n");
}

// pushBefore
BOOST_AUTO_TEST_CASE(DlistTest_PushBeforeNode)
{
    Dlist dl;
    dl.pushBack("b");
    dl.pushBack("d");
    dl.pushBefore(dl.findName("b"), "a");    // перед головой
    dl.pushBefore(dl.findName("d"), "c");    // в середину
    dl.pushBefore(nullptr, "x");

    BOOST_CHECK(forward(dl) == "a <-> b <-> c <-> d <-> nullptr\n");
}

// popAfter
BOOST_AUTO_TEST_CASE(DlistTest_PopAfterNode)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.pushBack("c");

    dl.popAfter(dl.findName("a"));           // из середины
    BOOST_CHECK(forward(dl) == "a <-> c <-> nullptr\n");

    dl.popAfter(dl.findName("a"));           // хвост - a станет хвостом
    BOOST_CHECK(backward(dl) == "a <-> nullptr\n");

    dl.popAfter(dl.findName("a"));           // после хвоста никого нет
    dl.popAfter(nullptr);
    BOOST_CHECK(forward(dl) == "a <-> nullptr\n");
}

// popBefore
BOOST_AUTO_TEST_CASE(DlistTest_PopBeforeNode)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.pushBack("c");

    dl.popBefore(dl.findName("c"));          // из середины
    BOOST_CHECK(forward(dl) == "a <-> c <-> nullptr\n");

    dl.popBefore(dl.findName("c"));          // голову - c станет головой
    BOOST_CHECK(forward(dl) == "c <-> nullptr\n");

    dl.popBefore(dl.findName("c"));          // перед головой никого нет
    dl.popBefore(nullptr);
    BOOST_CHECK(forward(dl) == "c <-> nullptr\n");
}

// findName
BOOST_AUTO_TEST_CASE(DlistTest_FindName)
{
    Dlist dl;
    dl.pushBack("a");
    BOOST_CHECK(dl.findName("a") != nullptr);
    BOOST_CHECK(dl.findName("z") == nullptr);
}

// popName
BOOST_AUTO_TEST_CASE(DlistTest_PopName)
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

    BOOST_CHECK(forward(dl) == "b <-> nullptr\n");
    BOOST_CHECK_NE(out.find("нет такого элемента"), string::npos);
}

// handleDlist
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleDlist(command, db, "dl", ss);
}

BOOST_AUTO_TEST_CASE(DlistHandlerTest_AllCommands)
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

    BOOST_CHECK_NE(out.find("-> TRUE"), string::npos);
    BOOST_CHECK_NE(out.find("-> FALSE"), string::npos);
    BOOST_CHECK_EQUAL(db["dl"]["type"].get<string>(), "dlist");
    BOOST_CHECK_EQUAL(db["dl"]["data"].size(), 1u);
    BOOST_CHECK_EQUAL(db["dl"]["data"][0].get<string>(), "k");
}

BOOST_AUTO_TEST_CASE(DlistHandlerTest_WrongTypeIsRejected)
{
    json db = json::object();
    db["dl"]["type"] = "flist";
    db["dl"]["data"] = json::array();

    startCapture();
    run(db, Command::LPUSHT, "a");
    stopCapture();

    BOOST_CHECK_EQUAL(db["dl"]["type"].get<string>(), "flist");
}
