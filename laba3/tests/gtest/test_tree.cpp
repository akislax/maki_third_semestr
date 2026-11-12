#include <gtest/gtest.h>
#include "treeClass.h"

// дерево для тестов:
//       5
//      / \
//     3   8
//    /
//   1
static void fill(Tree& tr)
{
    tr.insert(5);
    tr.insert(3);
    tr.insert(8);
    tr.insert(1);
}

// конструктор
TEST(TreeTest, NewTreeIsEmpty)
{
    Tree tr;
    EXPECT_EQ(tr.getRoot(), nullptr);
}

// insert и search
TEST(TreeTest, InsertAndSearch)
{
    Tree tr;
    fill(tr);

    EXPECT_EQ(tr.getRoot()->key, 5);
    EXPECT_NE(tr.search(1), nullptr);
    EXPECT_NE(tr.search(8), nullptr);
    EXPECT_EQ(tr.search(7), nullptr);    // такого нет
}

TEST(TreeTest, InsertGoesLeftAndRight)
{
    Tree tr;
    fill(tr);

    EXPECT_EQ(tr.getRoot()->left->key, 3);    // меньше - влево
    EXPECT_EQ(tr.getRoot()->right->key, 8);   // больше - вправо
}

// обходы
TEST(TreeTest, Traversals)
{
    Tree tr;
    fill(tr);

    testing::internal::CaptureStdout();
    tr.printPreorder();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "5 3 1 8 ");

    testing::internal::CaptureStdout();
    tr.printInorder();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "1 3 5 8 ");

    testing::internal::CaptureStdout();
    tr.printPostorder();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "1 3 8 5 ");

    testing::internal::CaptureStdout();
    tr.bfs();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "5 3 8 1 ");
}

TEST(TreeTest, BfsOnEmpty)
{
    Tree tr;
    testing::internal::CaptureStdout();
    tr.bfs();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "");
}

// isComplete
TEST(TreeTest, CompleteTree)
{
    Tree tr;
    EXPECT_TRUE(tr.isComplete());        // пустое дерево считается полным

    fill(tr);                            // уровни заполнены слева направо
    EXPECT_TRUE(tr.isComplete());
}

TEST(TreeTest, NotCompleteTree)
{
    Tree tr;
    tr.insert(5);
    tr.insert(8);                        // только правый ребенок, слева дырка
    tr.insert(9);
    EXPECT_FALSE(tr.isComplete());
}

// handleTree
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleTree(command, db, "t", ss);
}

TEST(TreeHandlerTest, AllCommands)
{
    json db = json::object();

    testing::internal::CaptureStdout();
    run(db, Command::TINSERT, "5");
    run(db, Command::TINSERT, "3");
    run(db, Command::TINSERT, "8");
    run(db, Command::TFIND, "3");
    run(db, Command::TFIND, "7");
    run(db, Command::TCOMPLETE, "");
    run(db, Command::TPRINT, "");
    run(db, Command::PRINT, "");             // чужая команда
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("-> TRUE"), string::npos);
    EXPECT_NE(out.find("-> FALSE"), string::npos);
    EXPECT_EQ(db["t"]["type"].get<string>(), "tree");
    EXPECT_EQ(db["t"]["data"].size(), 3u);
    EXPECT_EQ(db["t"]["data"][0].get<int>(), 5);    // сохранено в ширину
    EXPECT_EQ(db["t"]["data"][1].get<int>(), 3);
    EXPECT_EQ(db["t"]["data"][2].get<int>(), 8);
}

TEST(TreeHandlerTest, NotComplete)
{
    json db = json::object();

    testing::internal::CaptureStdout();
    run(db, Command::TINSERT, "5");
    run(db, Command::TINSERT, "8");
    run(db, Command::TINSERT, "9");
    run(db, Command::TCOMPLETE, "");
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("-> FALSE"), string::npos);
}

TEST(TreeHandlerTest, WrongTypeIsRejected)
{
    json db = json::object();
    db["t"]["type"] = "array";
    db["t"]["data"] = json::array();

    testing::internal::CaptureStdout();
    run(db, Command::TINSERT, "5");
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(db["t"]["type"].get<string>(), "array");
}
