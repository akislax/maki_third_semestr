#include <catch2/catch_test_macros.hpp>
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
TEST_CASE("FlistTest.NewListIsEmpty")
{
    Flist fl;
    CHECK(fl.isEmptyList());
    CHECK(fl.getHead() == nullptr);
}

// pushFront и popFront
TEST_CASE("FlistTest.PushFrontAddsToStart")
{
    Flist fl;
    fl.pushFront("b");
    fl.pushFront("a");
    CHECK(show(fl) == "a -> b -> nullptr\n");
}

TEST_CASE("FlistTest.PopFront")
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.popFront();
    CHECK(show(fl) == "b -> nullptr\n");

    fl.popFront();
    fl.popFront();                   // на пустом ничего не ломается
    CHECK(fl.isEmptyList());
}

// addTail и popTail
TEST_CASE("FlistTest.AddTailAddsToEnd")
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    CHECK(show(fl) == "a -> b -> nullptr\n");
}

TEST_CASE("FlistTest.PopTail")
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.popTail();
    CHECK(show(fl) == "a -> b -> nullptr\n");

    fl.popTail();
    fl.popTail();                    // удалили последний элемент
    CHECK(fl.isEmptyList());

    fl.popTail();                    // на пустом ничего не ломается
    CHECK(fl.isEmptyList());
}

// addOne - вставка после узла
TEST_CASE("FlistTest.AddOneAfterNode")
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("c");
    fl.addOne(fl.findValue("a"), "b");
    CHECK(show(fl) == "a -> b -> c -> nullptr\n");

    fl.addOne(nullptr, "x");         // нет узла - ничего не делаем
    CHECK(show(fl) == "a -> b -> c -> nullptr\n");
}

// deleteNode - удаление после узла
TEST_CASE("FlistTest.DeleteNodeAfterNode")
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.deleteNode(fl.findValue("a"));
    CHECK(show(fl) == "a -> c -> nullptr\n");

    fl.deleteNode(fl.findValue("c"));    // после последнего никого нет
    fl.deleteNode(nullptr);
    CHECK(show(fl) == "a -> c -> nullptr\n");
}

// addBefore
TEST_CASE("FlistTest.AddBefore")
{
    Flist fl;
    fl.addTail("b");
    fl.addTail("d");
    fl.addBefore(fl.findValue("b"), "a");    // перед головой
    fl.addBefore(fl.findValue("d"), "c");    // в середину
    CHECK(show(fl) == "a -> b -> c -> d -> nullptr\n");

    fl.addBefore(nullptr, "x");
    CHECK(show(fl) == "a -> b -> c -> d -> nullptr\n");
}

// deleteBefore
TEST_CASE("FlistTest.DeleteBefore")
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.addTail("d");

    fl.deleteBefore(fl.findValue("d"));      // удалит c
    CHECK(show(fl) == "a -> b -> d -> nullptr\n");

    fl.deleteBefore(fl.findValue("b"));      // удалит голову a
    CHECK(show(fl) == "b -> d -> nullptr\n");

    fl.deleteBefore(fl.findValue("b"));      // перед головой никого нет
    fl.deleteBefore(nullptr);
    CHECK(show(fl) == "b -> d -> nullptr\n");
}

// findValue
TEST_CASE("FlistTest.FindValue")
{
    Flist fl;
    fl.addTail("a");
    CHECK(fl.findValue("a") != nullptr);
    CHECK(fl.findValue("z") == nullptr);
}

// deleteValue
TEST_CASE("FlistTest.DeleteValue")
{
    Flist fl;
    fl.deleteValue("a");                     // пустой список

    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.deleteValue("a");                     // голова
    fl.deleteValue("c");                     // хвост
    fl.deleteValue("z");                     // такого нет
    CHECK(show(fl) == "b -> nullptr\n");
}

// printReverse
TEST_CASE("FlistTest.PrintReverse")
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");

    startCapture();
    fl.printReverse();
    string out = stopCapture();

    CHECK(out == "b a \n");
}

// handleFlist
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleFlist(command, db, "fl", ss);
}

TEST_CASE("FlistHandlerTest.AllCommands")
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

    CHECK(out.find("-> TRUE") != string::npos);
    CHECK(out.find("-> FALSE") != string::npos);
    CHECK(db["fl"]["type"].get<string>() == "flist");
    CHECK(db["fl"]["data"].size() == 1u);
    CHECK(db["fl"]["data"][0].get<string>() == "k");
}

TEST_CASE("FlistHandlerTest.WrongTypeIsRejected")
{
    json db = json::object();
    db["fl"]["type"] = "dlist";
    db["fl"]["data"] = json::array();

    startCapture();
    run(db, Command::FPUSHT, "a");
    stopCapture();

    CHECK(db["fl"]["type"].get<string>() == "dlist");
}
