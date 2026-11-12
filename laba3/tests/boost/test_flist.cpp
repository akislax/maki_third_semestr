#include <boost/test/unit_test.hpp>
#include "capture.h"
#include "flistClass.h"

// вспомогательная функция: печатает список в строку, чтобы удобно сравнивать
static string show(const Flist& fl)
{
    startCapture();
    fl.printList();
    return stopCapture();
}

// конструктор
BOOST_AUTO_TEST_CASE(FlistTest_NewListIsEmpty)
{
    Flist fl;
    BOOST_CHECK(fl.isEmptyList());
    BOOST_CHECK(fl.getHead() == nullptr);
}

// pushFront и popFront
BOOST_AUTO_TEST_CASE(FlistTest_PushFrontAddsToStart)
{
    Flist fl;
    fl.pushFront("b");
    fl.pushFront("a");
    BOOST_CHECK(show(fl) == "a -> b -> nullptr\n");
}

BOOST_AUTO_TEST_CASE(FlistTest_PopFront)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.popFront();
    BOOST_CHECK(show(fl) == "b -> nullptr\n");

    fl.popFront();
    fl.popFront();                   // на пустом ничего не ломается
    BOOST_CHECK(fl.isEmptyList());
}

// addTail и popTail
BOOST_AUTO_TEST_CASE(FlistTest_AddTailAddsToEnd)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    BOOST_CHECK(show(fl) == "a -> b -> nullptr\n");
}

BOOST_AUTO_TEST_CASE(FlistTest_PopTail)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.popTail();
    BOOST_CHECK(show(fl) == "a -> b -> nullptr\n");

    fl.popTail();
    fl.popTail();                    // удалили последний элемент
    BOOST_CHECK(fl.isEmptyList());

    fl.popTail();                    // на пустом ничего не ломается
    BOOST_CHECK(fl.isEmptyList());
}

// addOne - вставка после узла
BOOST_AUTO_TEST_CASE(FlistTest_AddOneAfterNode)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("c");
    fl.addOne(fl.findValue("a"), "b");
    BOOST_CHECK(show(fl) == "a -> b -> c -> nullptr\n");

    fl.addOne(nullptr, "x");         // нет узла - ничего не делаем
    BOOST_CHECK(show(fl) == "a -> b -> c -> nullptr\n");
}

// deleteNode - удаление после узла
BOOST_AUTO_TEST_CASE(FlistTest_DeleteNodeAfterNode)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.deleteNode(fl.findValue("a"));
    BOOST_CHECK(show(fl) == "a -> c -> nullptr\n");

    fl.deleteNode(fl.findValue("c"));    // после последнего никого нет
    fl.deleteNode(nullptr);
    BOOST_CHECK(show(fl) == "a -> c -> nullptr\n");
}

// addBefore
BOOST_AUTO_TEST_CASE(FlistTest_AddBefore)
{
    Flist fl;
    fl.addTail("b");
    fl.addTail("d");
    fl.addBefore(fl.findValue("b"), "a");    // перед головой
    fl.addBefore(fl.findValue("d"), "c");    // в середину
    BOOST_CHECK(show(fl) == "a -> b -> c -> d -> nullptr\n");

    fl.addBefore(nullptr, "x");
    BOOST_CHECK(show(fl) == "a -> b -> c -> d -> nullptr\n");
}

// deleteBefore
BOOST_AUTO_TEST_CASE(FlistTest_DeleteBefore)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.addTail("d");

    fl.deleteBefore(fl.findValue("d"));      // удалит c
    BOOST_CHECK(show(fl) == "a -> b -> d -> nullptr\n");

    fl.deleteBefore(fl.findValue("b"));      // удалит голову a
    BOOST_CHECK(show(fl) == "b -> d -> nullptr\n");

    fl.deleteBefore(fl.findValue("b"));      // перед головой никого нет
    fl.deleteBefore(nullptr);
    BOOST_CHECK(show(fl) == "b -> d -> nullptr\n");
}

// findValue
BOOST_AUTO_TEST_CASE(FlistTest_FindValue)
{
    Flist fl;
    fl.addTail("a");
    BOOST_CHECK(fl.findValue("a") != nullptr);
    BOOST_CHECK(fl.findValue("z") == nullptr);
}

// deleteValue
BOOST_AUTO_TEST_CASE(FlistTest_DeleteValue)
{
    Flist fl;
    fl.deleteValue("a");                     // пустой список

    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.deleteValue("a");                     // голова
    fl.deleteValue("c");                     // хвост
    fl.deleteValue("z");                     // такого нет
    BOOST_CHECK(show(fl) == "b -> nullptr\n");
}

// printReverse
BOOST_AUTO_TEST_CASE(FlistTest_PrintReverse)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");

    startCapture();
    fl.printReverse();
    string out = stopCapture();

    BOOST_CHECK_EQUAL(out, "b a \n");
}

// handleFlist
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleFlist(command, db, "fl", ss);
}

BOOST_AUTO_TEST_CASE(FlistHandlerTest_AllCommands)
{
    json db = json::object();

    startCapture();
    run(db, Command::FPUSHT, "b");           // b
    run(db, Command::FPUSHH, "a");           // a b
    run(db, Command::FPUSHT, "e");           // a b e
    run(db, Command::FPUSHA, "b c");         // a b c e
    run(db, Command::FPUSHB, "e d");         // a b c d e
    run(db, Command::FPUSHA, "z x");         // нет z
    run(db, Command::FPUSHB, "z x");         // нет z
    run(db, Command::FDELH, "");             // b c d e
    run(db, Command::FDELT, "");             // b c d
    run(db, Command::FDELA, "b");            // b d
    run(db, Command::FDELA, "z");            // нет z
    run(db, Command::FDELB, "d");            // d
    run(db, Command::FDELB, "z");            // нет z
    run(db, Command::FPUSHT, "k");           // d k
    run(db, Command::FDEL, "d");             // k
    run(db, Command::FFIND, "k");
    run(db, Command::FFIND, "z");
    run(db, Command::FPRINT, "");
    run(db, Command::FPRINTR, "");
    run(db, Command::PRINT, "");             // чужая команда
    string out = stopCapture();

    BOOST_CHECK_NE(out.find("-> TRUE"), string::npos);
    BOOST_CHECK_NE(out.find("-> FALSE"), string::npos);
    BOOST_CHECK_EQUAL(db["fl"]["type"].get<string>(), "flist");
    BOOST_CHECK_EQUAL(db["fl"]["data"].size(), 1u);
    BOOST_CHECK_EQUAL(db["fl"]["data"][0].get<string>(), "k");
}

BOOST_AUTO_TEST_CASE(FlistHandlerTest_WrongTypeIsRejected)
{
    json db = json::object();
    db["fl"]["type"] = "dlist";
    db["fl"]["data"] = json::array();

    startCapture();
    run(db, Command::FPUSHT, "a");
    stopCapture();

    BOOST_CHECK_EQUAL(db["fl"]["type"].get<string>(), "dlist");
}
