#include <catch2/catch_test_macros.hpp>
#include "capture.h"
#include <cstdio>
#include "arrayClass.h"
#include "stackClass.h"
#include "queueClass.h"
#include "flistClass.h"
#include "dlistClass.h"
#include "treeClass.h"
#include "hashTable.h"

// Каждый тест: заполнить структуру -> сохранить в файл ->
// загрузить в новую структуру -> проверить, что все совпало.

static string showFlist(const Flist& fl)
{
    startCapture();
    fl.printList();
    return stopCapture();
}

static string showDlist(const Dlist& dl)
{
    startCapture();
    dl.printForward();
    return stopCapture();
}

TEST_CASE("SerializationTest.Array")
{
    Array arr;
    arr.pushBack("кот");
    arr.pushBack("пес");
    arr.saveBinary("test_array.bin");

    Array loaded;
    loaded.pushBack("старое");                 // должно стереться при загрузке
    loaded.loadBinary("test_array.bin");

    CHECK(loaded.lenght() == 2);
    CHECK(loaded.getAt(0) == "кот");
    CHECK(loaded.getAt(1) == "пес");
    remove("test_array.bin");
}

TEST_CASE("SerializationTest.Stack")
{
    Stack st;
    st.push("1");
    st.push("2");
    st.push("3");
    st.saveBinary("test_stack.bin");

    Stack loaded;
    loaded.push("старое");
    loaded.loadBinary("test_stack.bin");

    CHECK(loaded.top() == "3");              // вершина та же
    loaded.pop();
    CHECK(loaded.top() == "2");
    loaded.pop();
    CHECK(loaded.top() == "1");
    loaded.pop();
    CHECK(loaded.isEmptyStack());
    remove("test_stack.bin");
}

TEST_CASE("SerializationTest.Queue")
{
    Queue q;
    q.push("1");
    q.push("2");
    q.saveBinary("test_queue.bin");

    Queue loaded;
    loaded.push("старое");
    loaded.loadBinary("test_queue.bin");

    CHECK(loaded.top() == "1");
    loaded.pop();
    CHECK(loaded.top() == "2");
    loaded.pop();
    CHECK(loaded.isEmptyQ());
    remove("test_queue.bin");
}

TEST_CASE("SerializationTest.Flist")
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.saveBinary("test_flist.bin");

    Flist loaded;
    loaded.addTail("старое");
    loaded.loadBinary("test_flist.bin");

    CHECK(showFlist(loaded) == "a -> b -> nullptr\n");
    remove("test_flist.bin");
}

TEST_CASE("SerializationTest.Dlist")
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.saveBinary("test_dlist.bin");

    Dlist loaded;
    loaded.pushBack("старое");
    loaded.loadBinary("test_dlist.bin");

    CHECK(showDlist(loaded) == "a <-> b <-> nullptr\n");
    remove("test_dlist.bin");
}

TEST_CASE("SerializationTest.Tree")
{
    Tree tr;
    tr.insert(5);
    tr.insert(3);
    tr.insert(8);
    tr.saveBinary("test_tree.bin");

    Tree loaded;
    loaded.insert(100);
    loaded.loadBinary("test_tree.bin");

    CHECK(loaded.getRoot()->key == 5);       // форма дерева та же
    CHECK(loaded.getRoot()->left->key == 3);
    CHECK(loaded.getRoot()->right->key == 8);
    CHECK(loaded.search(100) == nullptr);    // старое стерлось
    remove("test_tree.bin");
}

TEST_CASE("SerializationTest.HashTable")
{
    hashTable ht;
    ht.add(5);
    ht.add(105);
    ht.add(7);
    ht.saveBinary("test_hash.bin");

    hashTable loaded;
    loaded.add(999);
    loaded.loadBinary("test_hash.bin");

    CHECK(loaded.search(5));
    CHECK(loaded.search(105));
    CHECK(loaded.search(7));
    CHECK_FALSE(loaded.search(999));
    remove("test_hash.bin");
}

TEST_CASE("SerializationTest.Kukushka")
{
    Kukushka kt;
    kt.insert(1);
    kt.insert(12);
    kt.insert(42);
    kt.saveBinary("test_kukushka.bin");

    Kukushka loaded;
    loaded.insert(999);
    loaded.loadBinary("test_kukushka.bin");

    CHECK(loaded.search(1));
    CHECK(loaded.search(12));
    CHECK(loaded.search(42));
    CHECK_FALSE(loaded.search(999));
    remove("test_kukushka.bin");
}

TEST_CASE("SerializationTest.LoadMissingFile")
{
    Array arr;
    arr.pushBack("a");
    arr.loadBinary("нет_такого_файла.bin");    // файла нет - ничего не меняется
    CHECK(arr.lenght() == 1);

    Stack st;      st.loadBinary("нет_такого_файла.bin");
    Queue q;       q.loadBinary("нет_такого_файла.bin");
    Flist fl;      fl.loadBinary("нет_такого_файла.bin");
    Dlist dl;      dl.loadBinary("нет_такого_файла.bin");
    Tree tr;       tr.loadBinary("нет_такого_файла.bin");
    hashTable ht;  ht.loadBinary("нет_такого_файла.bin");
    Kukushka kt;   kt.loadBinary("нет_такого_файла.bin");

    CHECK(st.isEmptyStack());
    CHECK(q.isEmptyQ());
    CHECK(fl.isEmptyList());
    CHECK(dl.isEmptyList());
    CHECK(tr.getRoot() == nullptr);
    CHECK(ht.getAll().size() == 0u);
    CHECK(kt.getAll().size() == 0u);
}
