#include <gtest/gtest.h>
#include "dlistClass.h"

// вспомогательные функции: печать списка в строку
static string forward(const Dlist& dl)
{
    testing::internal::CaptureStdout();
    dl.printForward();
    return testing::internal::GetCapturedStdout();
}

static string backward(const Dlist& dl)
{
    testing::internal::CaptureStdout();
    dl.printBackward();
    return testing::internal::GetCapturedStdout();
}

// конструктор
TEST(DlistTest, NewListIsEmpty)
{
    Dlist dl;
    EXPECT_TRUE(dl.isEmptyList());
    EXPECT_EQ(dl.getHead(), nullptr);
}

// pushBack и popBack
TEST(DlistTest, PushBackAndPopBack)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    EXPECT_EQ(forward(dl), "a <-> b <-> nullptr\n");
    EXPECT_EQ(backward(dl), "b <-> a <-> nullptr\n");

    dl.popBack();
    EXPECT_EQ(forward(dl), "a <-> nullptr\n");

    dl.popBack();
    dl.popBack();                    // на пустом ничего не ломается
    EXPECT_TRUE(dl.isEmptyList());
}

// pushFront и popFront
TEST(DlistTest, PushFrontAndPopFront)
{
    Dlist dl;
    dl.pushFront("b");
    dl.pushFront("a");
    EXPECT_EQ(forward(dl), "a <-> b <-> nullptr\n");

    dl.popFront();
    EXPECT_EQ(forward(dl), "b <-> nullptr\n");

    dl.popFront();
    dl.popFront();                   // на пустом ничего не ломается
    EXPECT_TRUE(dl.isEmptyList());
}

// pushBeetwen - вставка после узла
TEST(DlistTest, PushAfterNode)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("c");
    dl.pushBeetwen(dl.findName("a"), "b");   // в середину
    dl.pushBeetwen(dl.findName("c"), "d");   // после хвоста - d станет хвостом
    dl.pushBeetwen(nullptr, "x");

    EXPECT_EQ(forward(dl), "a <-> b <-> c <-> d <-> nullptr\n");
    EXPECT_EQ(backward(dl), "d <-> c <-> b <-> a <-> nullptr\n");
}

// pushBefore
TEST(DlistTest, PushBeforeNode)
{
    Dlist dl;
    dl.pushBack("b");
    dl.pushBack("d");
    dl.pushBefore(dl.findName("b"), "a");    // перед головой
    dl.pushBefore(dl.findName("d"), "c");    // в середину
    dl.pushBefore(nullptr, "x");

    EXPECT_EQ(forward(dl), "a <-> b <-> c <-> d <-> nullptr\n");
}

// popAfter
TEST(DlistTest, PopAfterNode)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.pushBack("c");

    dl.popAfter(dl.findName("a"));           // из середины
    EXPECT_EQ(forward(dl), "a <-> c <-> nullptr\n");

    dl.popAfter(dl.findName("a"));           // хвост - a станет хвостом
    EXPECT_EQ(backward(dl), "a <-> nullptr\n");

    dl.popAfter(dl.findName("a"));           // после хвоста никого нет
    dl.popAfter(nullptr);
    EXPECT_EQ(forward(dl), "a <-> nullptr\n");
}

// popBefore
TEST(DlistTest, PopBeforeNode)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.pushBack("c");

    dl.popBefore(dl.findName("c"));          // из середины
    EXPECT_EQ(forward(dl), "a <-> c <-> nullptr\n");

    dl.popBefore(dl.findName("c"));          // голову - c станет головой
    EXPECT_EQ(forward(dl), "c <-> nullptr\n");

    dl.popBefore(dl.findName("c"));          // перед головой никого нет
    dl.popBefore(nullptr);
    EXPECT_EQ(forward(dl), "c <-> nullptr\n");
}

// findName
TEST(DlistTest, FindName)
{
    Dlist dl;
    dl.pushBack("a");
    EXPECT_NE(dl.findName("a"), nullptr);
    EXPECT_EQ(dl.findName("z"), nullptr);
}

// popName
TEST(DlistTest, PopName)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.pushBack("c");

    dl.popName("a");                         // голова
    dl.popName("c");                         // хвост

    testing::internal::CaptureStdout();
    dl.popName("z");                         // такого нет
    string out = testing::internal::GetCapturedStdout();

    EXPECT_EQ(forward(dl), "b <-> nullptr\n");
    EXPECT_NE(out.find("нет такого элемента"), string::npos);
}

// handleDlist
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleDlist(command, db, "dl", ss);
}

TEST(DlistHandlerTest, AllCommands)
{
    json db = json::object();

    testing::internal::CaptureStdout();
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
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("-> TRUE"), string::npos);
    EXPECT_NE(out.find("-> FALSE"), string::npos);
    EXPECT_EQ(db["dl"]["type"].get<string>(), "dlist");
    EXPECT_EQ(db["dl"]["data"].size(), 1u);
    EXPECT_EQ(db["dl"]["data"][0].get<string>(), "k");
}

TEST(DlistHandlerTest, WrongTypeIsRejected)
{
    json db = json::object();
    db["dl"]["type"] = "flist";
    db["dl"]["data"] = json::array();

    testing::internal::CaptureStdout();
    run(db, Command::LPUSHT, "a");
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(db["dl"]["type"].get<string>(), "flist");
}
