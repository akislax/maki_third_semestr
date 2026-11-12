#include <boost/test/unit_test.hpp>
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

BOOST_AUTO_TEST_CASE(SerializationTest_Array)
{
    Array arr;
    arr.pushBack("кот");
    arr.pushBack("пес");
    arr.saveBinary("test_array.bin");

    Array loaded;
    loaded.pushBack("старое");                 // должно стереться при загрузке
    loaded.loadBinary("test_array.bin");

    BOOST_CHECK_EQUAL(loaded.lenght(), 2);
    BOOST_CHECK_EQUAL(loaded.getAt(0), "кот");
    BOOST_CHECK_EQUAL(loaded.getAt(1), "пес");
    remove("test_array.bin");
}

BOOST_AUTO_TEST_CASE(SerializationTest_Stack)
{
    Stack st;
    st.push("1");
    st.push("2");
    st.push("3");
    st.saveBinary("test_stack.bin");

    Stack loaded;
    loaded.push("старое");
    loaded.loadBinary("test_stack.bin");

    BOOST_CHECK_EQUAL(loaded.top(), "3");              // вершина та же
    loaded.pop();
    BOOST_CHECK_EQUAL(loaded.top(), "2");
    loaded.pop();
    BOOST_CHECK_EQUAL(loaded.top(), "1");
    loaded.pop();
    BOOST_CHECK(loaded.isEmptyStack());
    remove("test_stack.bin");
}

BOOST_AUTO_TEST_CASE(SerializationTest_Queue)
{
    Queue q;
    q.push("1");
    q.push("2");
    q.saveBinary("test_queue.bin");

    Queue loaded;
    loaded.push("старое");
    loaded.loadBinary("test_queue.bin");

    BOOST_CHECK_EQUAL(loaded.top(), "1");
    loaded.pop();
    BOOST_CHECK_EQUAL(loaded.top(), "2");
    loaded.pop();
    BOOST_CHECK(loaded.isEmptyQ());
    remove("test_queue.bin");
}

BOOST_AUTO_TEST_CASE(SerializationTest_Flist)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.saveBinary("test_flist.bin");

    Flist loaded;
    loaded.addTail("старое");
    loaded.loadBinary("test_flist.bin");

    BOOST_CHECK(showFlist(loaded) == "a -> b -> nullptr\n");
    remove("test_flist.bin");
}

BOOST_AUTO_TEST_CASE(SerializationTest_Dlist)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.saveBinary("test_dlist.bin");

    Dlist loaded;
    loaded.pushBack("старое");
    loaded.loadBinary("test_dlist.bin");

    BOOST_CHECK(showDlist(loaded) == "a <-> b <-> nullptr\n");
    remove("test_dlist.bin");
}

BOOST_AUTO_TEST_CASE(SerializationTest_Tree)
{
    Tree tr;
    tr.insert(5);
    tr.insert(3);
    tr.insert(8);
    tr.saveBinary("test_tree.bin");

    Tree loaded;
    loaded.insert(100);
    loaded.loadBinary("test_tree.bin");

    BOOST_CHECK_EQUAL(loaded.getRoot()->key, 5);       // форма дерева та же
    BOOST_CHECK_EQUAL(loaded.getRoot()->left->key, 3);
    BOOST_CHECK_EQUAL(loaded.getRoot()->right->key, 8);
    BOOST_CHECK(loaded.search(100) == nullptr);    // старое стерлось
    remove("test_tree.bin");
}

BOOST_AUTO_TEST_CASE(SerializationTest_HashTable)
{
    hashTable ht;
    ht.add(5);
    ht.add(105);
    ht.add(7);
    ht.saveBinary("test_hash.bin");

    hashTable loaded;
    loaded.add(999);
    loaded.loadBinary("test_hash.bin");

    BOOST_CHECK(loaded.search(5));
    BOOST_CHECK(loaded.search(105));
    BOOST_CHECK(loaded.search(7));
    BOOST_CHECK(!loaded.search(999));
    remove("test_hash.bin");
}

BOOST_AUTO_TEST_CASE(SerializationTest_Kukushka)
{
    Kukushka kt;
    kt.insert(1);
    kt.insert(12);
    kt.insert(42);
    kt.saveBinary("test_kukushka.bin");

    Kukushka loaded;
    loaded.insert(999);
    loaded.loadBinary("test_kukushka.bin");

    BOOST_CHECK(loaded.search(1));
    BOOST_CHECK(loaded.search(12));
    BOOST_CHECK(loaded.search(42));
    BOOST_CHECK(!loaded.search(999));
    remove("test_kukushka.bin");
}

BOOST_AUTO_TEST_CASE(SerializationTest_LoadMissingFile)
{
    Array arr;
    arr.pushBack("a");
    arr.loadBinary("нет_такого_файла.bin");    // файла нет - ничего не меняется
    BOOST_CHECK_EQUAL(arr.lenght(), 1);

    Stack st;      st.loadBinary("нет_такого_файла.bin");
    Queue q;       q.loadBinary("нет_такого_файла.bin");
    Flist fl;      fl.loadBinary("нет_такого_файла.bin");
    Dlist dl;      dl.loadBinary("нет_такого_файла.bin");
    Tree tr;       tr.loadBinary("нет_такого_файла.bin");
    hashTable ht;  ht.loadBinary("нет_такого_файла.bin");
    Kukushka kt;   kt.loadBinary("нет_такого_файла.bin");

    BOOST_CHECK(st.isEmptyStack());
    BOOST_CHECK(q.isEmptyQ());
    BOOST_CHECK(fl.isEmptyList());
    BOOST_CHECK(dl.isEmptyList());
    BOOST_CHECK(tr.getRoot() == nullptr);
    BOOST_CHECK_EQUAL(ht.getAll().size(), 0u);
    BOOST_CHECK_EQUAL(kt.getAll().size(), 0u);
}
