#include <gtest/gtest.h>
#include "flistClass.h"

// вспомогательная функция: печатает список в строку, чтобы удобно сравнивать
static string show(const Flist& fl)
{
    testing::internal::CaptureStdout();
    fl.printList();
    return testing::internal::GetCapturedStdout();
}

// конструктор
TEST(FlistTest, NewListIsEmpty)
{
    Flist fl;
    EXPECT_TRUE(fl.isEmptyList());
    EXPECT_EQ(fl.getHead(), nullptr);
}

// pushFront и popFront
TEST(FlistTest, PushFrontAddsToStart)
{
    Flist fl;
    fl.pushFront("b");
    fl.pushFront("a");
    EXPECT_EQ(show(fl), "a -> b -> nullptr\n");
}

TEST(FlistTest, PopFront)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.popFront();
    EXPECT_EQ(show(fl), "b -> nullptr\n");

    fl.popFront();
    fl.popFront();                   // на пустом ничего не ломается
    EXPECT_TRUE(fl.isEmptyList());
}

// addTail и popTail
TEST(FlistTest, AddTailAddsToEnd)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    EXPECT_EQ(show(fl), "a -> b -> nullptr\n");
}

TEST(FlistTest, PopTail)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.popTail();
    EXPECT_EQ(show(fl), "a -> b -> nullptr\n");

    fl.popTail();
    fl.popTail();                    // удалили последний элемент
    EXPECT_TRUE(fl.isEmptyList());

    fl.popTail();                    // на пустом ничего не ломается
    EXPECT_TRUE(fl.isEmptyList());
}

// addOne - вставка после узла
TEST(FlistTest, AddOneAfterNode)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("c");
    fl.addOne(fl.findValue("a"), "b");
    EXPECT_EQ(show(fl), "a -> b -> c -> nullptr\n");

    fl.addOne(nullptr, "x");         // нет узла - ничего не делаем
    EXPECT_EQ(show(fl), "a -> b -> c -> nullptr\n");
}

// deleteNode - удаление после узла
TEST(FlistTest, DeleteNodeAfterNode)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.deleteNode(fl.findValue("a"));
    EXPECT_EQ(show(fl), "a -> c -> nullptr\n");

    fl.deleteNode(fl.findValue("c"));    // после последнего никого нет
    fl.deleteNode(nullptr);
    EXPECT_EQ(show(fl), "a -> c -> nullptr\n");
}

// addBefore
TEST(FlistTest, AddBefore)
{
    Flist fl;
    fl.addTail("b");
    fl.addTail("d");
    fl.addBefore(fl.findValue("b"), "a");    // перед головой
    fl.addBefore(fl.findValue("d"), "c");    // в середину
    EXPECT_EQ(show(fl), "a -> b -> c -> d -> nullptr\n");

    fl.addBefore(nullptr, "x");
    EXPECT_EQ(show(fl), "a -> b -> c -> d -> nullptr\n");
}

// deleteBefore
TEST(FlistTest, DeleteBefore)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.addTail("d");

    fl.deleteBefore(fl.findValue("d"));      // удалит c
    EXPECT_EQ(show(fl), "a -> b -> d -> nullptr\n");

    fl.deleteBefore(fl.findValue("b"));      // удалит голову a
    EXPECT_EQ(show(fl), "b -> d -> nullptr\n");

    fl.deleteBefore(fl.findValue("b"));      // перед головой никого нет
    fl.deleteBefore(nullptr);
    EXPECT_EQ(show(fl), "b -> d -> nullptr\n");
}

// findValue
TEST(FlistTest, FindValue)
{
    Flist fl;
    fl.addTail("a");
    EXPECT_NE(fl.findValue("a"), nullptr);
    EXPECT_EQ(fl.findValue("z"), nullptr);
}

// deleteValue
TEST(FlistTest, DeleteValue)
{
    Flist fl;
    fl.deleteValue("a");                     // пустой список

    fl.addTail("a");
    fl.addTail("b");
    fl.addTail("c");
    fl.deleteValue("a");                     // голова
    fl.deleteValue("c");                     // хвост
    fl.deleteValue("z");                     // такого нет
    EXPECT_EQ(show(fl), "b -> nullptr\n");
}

// printReverse
TEST(FlistTest, PrintReverse)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");

    testing::internal::CaptureStdout();
    fl.printReverse();
    string out = testing::internal::GetCapturedStdout();

    EXPECT_EQ(out, "b a \n");
}

// handleFlist
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleFlist(command, db, "fl", ss);
}

TEST(FlistHandlerTest, AllCommands)
{
    json db = json::object();

    testing::internal::CaptureStdout();
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
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("-> TRUE"), string::npos);
    EXPECT_NE(out.find("-> FALSE"), string::npos);
    EXPECT_EQ(db["fl"]["type"].get<string>(), "flist");
    EXPECT_EQ(db["fl"]["data"].size(), 1u);
    EXPECT_EQ(db["fl"]["data"][0].get<string>(), "k");
}

TEST(FlistHandlerTest, WrongTypeIsRejected)
{
    json db = json::object();
    db["fl"]["type"] = "dlist";
    db["fl"]["data"] = json::array();

    testing::internal::CaptureStdout();
    run(db, Command::FPUSHT, "a");
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(db["fl"]["type"].get<string>(), "dlist");
}
