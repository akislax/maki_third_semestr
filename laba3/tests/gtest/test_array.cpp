#include <gtest/gtest.h>
#include "arrayClass.h"

// конструктор
TEST(ArrayTest, NewArrayIsEmpty)
{
    Array arr;
    EXPECT_EQ(arr.lenght(), 0);
}

// pushBack
TEST(ArrayTest, PushBackAddsToEnd)
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");

    EXPECT_EQ(arr.lenght(), 2);
    EXPECT_EQ(arr.getAt(0), "a");
    EXPECT_EQ(arr.getAt(1), "b");
}

// resizeArray (через pushBack)
TEST(ArrayTest, GrowsBeyondStartCapacity)
{
    Array arr;
    for (int i = 0; i < 10; i++)     // вместимость 4, должно расшириться
    {
        arr.pushBack(to_string(i));
    }
    EXPECT_EQ(arr.lenght(), 10);
    EXPECT_EQ(arr.getAt(9), "9");
}

// insertAt
TEST(ArrayTest, InsertInMiddle)
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("c");
    arr.insertAt("b", 1);

    EXPECT_EQ(arr.getAt(0), "a");
    EXPECT_EQ(arr.getAt(1), "b");
    EXPECT_EQ(arr.getAt(2), "c");
}

TEST(ArrayTest, InsertWhenFull)
{
    Array arr;
    for (int i = 0; i < 4; i++)      // заполнили всю вместимость
    {
        arr.pushBack("x");
    }
    arr.insertAt("y", 0);            // insertAt должен сам расширить массив
    EXPECT_EQ(arr.lenght(), 5);
    EXPECT_EQ(arr.getAt(0), "y");
}

TEST(ArrayTest, InsertWrongIndex)
{
    Array arr;
    arr.pushBack("a");
    testing::internal::CaptureStdout();
    arr.insertAt("x", 5);            // такого индекса нет
    testing::internal::GetCapturedStdout();
    EXPECT_EQ(arr.lenght(), 1);      // ничего не добавилось
}

// removeAt
TEST(ArrayTest, RemoveShiftsElements)
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");
    arr.pushBack("c");
    arr.removeAt(0);

    EXPECT_EQ(arr.lenght(), 2);
    EXPECT_EQ(arr.getAt(0), "b");
}

TEST(ArrayTest, RemoveWrongIndex)
{
    Array arr;
    arr.pushBack("a");
    testing::internal::CaptureStdout();
    arr.removeAt(3);                 // такого индекса нет
    testing::internal::GetCapturedStdout();
    EXPECT_EQ(arr.lenght(), 1);      // ничего не удалилось
}

// setAt
TEST(ArrayTest, SetAtChangesValue)
{
    Array arr;
    arr.pushBack("a");
    arr.setAt("b", 0);
    EXPECT_EQ(arr.getAt(0), "b");
}

TEST(ArrayTest, SetAtWrongIndex)
{
    Array arr;
    arr.pushBack("a");
    testing::internal::CaptureStdout();
    arr.setAt("b", 3);               // такого индекса нет
    testing::internal::GetCapturedStdout();
    EXPECT_EQ(arr.getAt(0), "a");    // старое значение осталось
}

// getAt
TEST(ArrayTest, GetAtWrongIndex)
{
    Array arr;
    testing::internal::CaptureStdout();
    string got = arr.getAt(0);
    testing::internal::GetCapturedStdout();
    EXPECT_EQ(got, "");              // при ошибке возвращает пустую строку
}

// printArray
TEST(ArrayTest, PrintShowsElements)
{
    Array arr;
    arr.pushBack("a");
    arr.pushBack("b");

    testing::internal::CaptureStdout();
    arr.printArray();
    string out = testing::internal::GetCapturedStdout();

    EXPECT_EQ(out, "a b (size=2, capacity=4)\n");
}

// handleArray
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleArray(command, db, "arr", ss);
}

TEST(ArrayHandlerTest, AllCommands)
{
    json db = json::object();

    testing::internal::CaptureStdout();
    run(db, Command::MPUSH, "a");          // [a]
    run(db, Command::MPUSH, "c");          // [a, c]
    run(db, Command::MINSERT, "1 b");      // [a, b, c]
    run(db, Command::MSET, "0 z");         // [z, b, c]
    run(db, Command::MDEL, "2");           // [z, b]
    run(db, Command::MGET, "1");
    run(db, Command::MLEN, "");
    run(db, Command::MPRINT, "");
    run(db, Command::PRINT, "");           // чужая команда - ничего не делает
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(db["arr"]["type"].get<string>(), "array");
    EXPECT_EQ(db["arr"]["data"].size(), 2u);
    EXPECT_EQ(db["arr"]["data"][0].get<string>(), "z");
    EXPECT_EQ(db["arr"]["data"][1].get<string>(), "b");
}

TEST(ArrayHandlerTest, WrongTypeIsRejected)
{
    json db = json::object();
    db["arr"]["type"] = "stack";             // под этим именем уже стек
    db["arr"]["data"] = json::array();

    testing::internal::CaptureStdout();
    run(db, Command::MPUSH, "a");
    string out = testing::internal::GetCapturedStdout();

    EXPECT_EQ(db["arr"]["type"].get<string>(), "stack");   // тип не поменялся
    EXPECT_NE(out.find("это не array"), string::npos);     // и вывелось сообщение
}