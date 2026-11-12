#include <catch2/catch_test_macros.hpp>
#include "capture.h"
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
TEST_CASE("TreeTest.NewTreeIsEmpty")
{
    Tree tr;
    CHECK(tr.getRoot() == nullptr);
}

// insert и search
TEST_CASE("TreeTest.InsertAndSearch")
{
    Tree tr;
    fill(tr);

    CHECK(tr.getRoot()->key == 5);
    CHECK(tr.search(1) != nullptr);
    CHECK(tr.search(8) != nullptr);
    CHECK(tr.search(7) == nullptr);    // такого нет
}

TEST_CASE("TreeTest.InsertGoesLeftAndRight")
{
    Tree tr;
    fill(tr);

    CHECK(tr.getRoot()->left->key == 3);    // меньше - влево
    CHECK(tr.getRoot()->right->key == 8);   // больше - вправо
}

// обходы
TEST_CASE("TreeTest.Traversals")
{
    Tree tr;
    fill(tr);

    startCapture();
    tr.printPreorder();
    CHECK(stopCapture() == "5 3 1 8 ");

    startCapture();
    tr.printInorder();
    CHECK(stopCapture() == "1 3 5 8 ");

    startCapture();
    tr.printPostorder();
    CHECK(stopCapture() == "1 3 8 5 ");

    startCapture();
    tr.bfs();
    CHECK(stopCapture() == "5 3 8 1 ");
}

TEST_CASE("TreeTest.BfsOnEmpty")
{
    Tree tr;
    startCapture();
    tr.bfs();
    CHECK(stopCapture() == "");
}

// isComplete
TEST_CASE("TreeTest.CompleteTree")
{
    Tree tr;
    CHECK(tr.isComplete());        // пустое дерево считается полным

    fill(tr);                            // уровни заполнены слева направо
    CHECK(tr.isComplete());
}

TEST_CASE("TreeTest.NotCompleteTree")
{
    Tree tr;
    tr.insert(5);
    tr.insert(8);                        // только правый ребенок, слева дырка
    tr.insert(9);
    CHECK_FALSE(tr.isComplete());
}

// handleTree
static void run(json& db, Command command, const string& args)
{
    stringstream ss(args);
    handleTree(command, db, "t", ss);
}

TEST_CASE("TreeHandlerTest.AllCommands")
{
    json db = json::object();

    startCapture();
    run(db, Command::TINSERT, "5");
    run(db, Command::TINSERT, "3");
    run(db, Command::TINSERT, "8");
    run(db, Command::TFIND, "3");
    run(db, Command::TFIND, "7");
    run(db, Command::TCOMPLETE, "");
    run(db, Command::TPRINT, "");
    run(db, Command::PRINT, "");             // чужая команда
    string out = stopCapture();

    CHECK(out.find("-> TRUE") != string::npos);
    CHECK(out.find("-> FALSE") != string::npos);
    CHECK(db["t"]["type"].get<string>() == "tree");
    CHECK(db["t"]["data"].size() == 3u);
    CHECK(db["t"]["data"][0].get<int>() == 5);    // сохранено в ширину
    CHECK(db["t"]["data"][1].get<int>() == 3);
    CHECK(db["t"]["data"][2].get<int>() == 8);
}

TEST_CASE("TreeHandlerTest.NotComplete")
{
    json db = json::object();

    startCapture();
    run(db, Command::TINSERT, "5");
    run(db, Command::TINSERT, "8");
    run(db, Command::TINSERT, "9");
    run(db, Command::TCOMPLETE, "");
    string out = stopCapture();

    CHECK(out.find("-> FALSE") != string::npos);
}

TEST_CASE("TreeHandlerTest.WrongTypeIsRejected")
{
    json db = json::object();
    db["t"]["type"] = "array";
    db["t"]["data"] = json::array();

    startCapture();
    run(db, Command::TINSERT, "5");
    stopCapture();

    CHECK(db["t"]["type"].get<string>() == "array");
}
