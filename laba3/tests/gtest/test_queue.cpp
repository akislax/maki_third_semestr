#include <gtest/gtest.h>
#include "queueClass.h"

// конструктор
TEST(QueueTest, NewQueueIsEmpty)
{
    Queue q;
    EXPECT_TRUE(q.isEmptyQ());
}

// push и top
TEST(QueueTest, TopIsFirstPushed)
{
    Queue q;
    q.push("1");
    q.push("2");

    EXPECT_FALSE(q.isEmptyQ());
    EXPECT_EQ(q.top(), "1");         // первым пришел - первым выйдет
}

// pop
TEST(QueueTest, PopRemovesFirst)
{
    Queue q;
    q.push("1");
    q.push("2");
    q.pop();

    EXPECT_EQ(q.top(), "2");
}

TEST(QueueTest, PopLastMakesEmpty)
{
    Queue q;
    q.push("1");
    q.pop();
    EXPECT_TRUE(q.isEmptyQ());

    q.push("2");                     // после опустошения снова работает
    EXPECT_EQ(q.top(), "2");
}

TEST(QueueTest, PopAndTopOnEmpty)
{
    Queue q;
    q.pop();
    EXPECT_TRUE(q.isEmptyQ());
    EXPECT_EQ(q.top(), "");
}

// printQ
TEST(QueueTest, PrintFromHead)
{
    Queue q;
    q.push("1");
    q.push("2");

    testing::internal::CaptureStdout();
    q.printQ();
    string out = testing::internal::GetCapturedStdout();

    EXPECT_EQ(out, "1 -> 2 -> nullptr\n");
}

// handleQueue
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleQueue(command, db, "q", ss);
}

TEST(QueueHandlerTest, AllCommands)
{
    json db = json::object();

    testing::internal::CaptureStdout();
    run(db, Command::QPUSH, "1");
    run(db, Command::QPUSH, "2");
    run(db, Command::QPUSH, "3");
    run(db, Command::QPOP, "");            // выходит 1
    run(db, Command::QPRINT, "");
    run(db, Command::PRINT, "");           // чужая команда - ничего не делает
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("-> 1"), string::npos);
    EXPECT_EQ(db["q"]["type"].get<string>(), "queue");
    EXPECT_EQ(db["q"]["data"].size(), 2u);
    EXPECT_EQ(db["q"]["data"][0].get<string>(), "2");
    EXPECT_EQ(db["q"]["data"][1].get<string>(), "3");
}

TEST(QueueHandlerTest, PopEmpty)
{
    json db = json::object();

    testing::internal::CaptureStdout();
    run(db, Command::QPOP, "");
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("очередь пуста"), string::npos);
}

TEST(QueueHandlerTest, WrongTypeIsRejected)
{
    json db = json::object();
    db["q"]["type"] = "stack";
    db["q"]["data"] = json::array();

    testing::internal::CaptureStdout();
    run(db, Command::QPUSH, "1");
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(db["q"]["type"].get<string>(), "stack");
}
