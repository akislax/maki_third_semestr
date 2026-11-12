#include <gtest/gtest.h>
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
    testing::internal::CaptureStdout();
    fl.printList();
    return testing::internal::GetCapturedStdout();
}

static string showDlist(const Dlist& dl)
{
    testing::internal::CaptureStdout();
    dl.printForward();
    return testing::internal::GetCapturedStdout();
}

TEST(SerializationTest, Array)
{
    Array arr;
    arr.pushBack("кот");
    arr.pushBack("пес");
    arr.saveBinary("test_array.bin");

    Array loaded;
    loaded.pushBack("старое");                 // должно стереться при загрузке
    loaded.loadBinary("test_array.bin");

    EXPECT_EQ(loaded.lenght(), 2);
    EXPECT_EQ(loaded.getAt(0), "кот");
    EXPECT_EQ(loaded.getAt(1), "пес");
    remove("test_array.bin");
}

TEST(SerializationTest, Stack)
{
    Stack st;
    st.push("1");
    st.push("2");
    st.push("3");
    st.saveBinary("test_stack.bin");

    Stack loaded;
    loaded.push("старое");
    loaded.loadBinary("test_stack.bin");

    EXPECT_EQ(loaded.top(), "3");              // вершина та же
    loaded.pop();
    EXPECT_EQ(loaded.top(), "2");
    loaded.pop();
    EXPECT_EQ(loaded.top(), "1");
    loaded.pop();
    EXPECT_TRUE(loaded.isEmptyStack());
    remove("test_stack.bin");
}

TEST(SerializationTest, Queue)
{
    Queue q;
    q.push("1");
    q.push("2");
    q.saveBinary("test_queue.bin");

    Queue loaded;
    loaded.push("старое");
    loaded.loadBinary("test_queue.bin");

    EXPECT_EQ(loaded.top(), "1");
    loaded.pop();
    EXPECT_EQ(loaded.top(), "2");
    loaded.pop();
    EXPECT_TRUE(loaded.isEmptyQ());
    remove("test_queue.bin");
}

TEST(SerializationTest, Flist)
{
    Flist fl;
    fl.addTail("a");
    fl.addTail("b");
    fl.saveBinary("test_flist.bin");

    Flist loaded;
    loaded.addTail("старое");
    loaded.loadBinary("test_flist.bin");

    EXPECT_EQ(showFlist(loaded), "a -> b -> nullptr\n");
    remove("test_flist.bin");
}

TEST(SerializationTest, Dlist)
{
    Dlist dl;
    dl.pushBack("a");
    dl.pushBack("b");
    dl.saveBinary("test_dlist.bin");

    Dlist loaded;
    loaded.pushBack("старое");
    loaded.loadBinary("test_dlist.bin");

    EXPECT_EQ(showDlist(loaded), "a <-> b <-> nullptr\n");
    remove("test_dlist.bin");
}

TEST(SerializationTest, Tree)
{
    Tree tr;
    tr.insert(5);
    tr.insert(3);
    tr.insert(8);
    tr.saveBinary("test_tree.bin");

    Tree loaded;
    loaded.insert(100);
    loaded.loadBinary("test_tree.bin");

    EXPECT_EQ(loaded.getRoot()->key, 5);       // форма дерева та же
    EXPECT_EQ(loaded.getRoot()->left->key, 3);
    EXPECT_EQ(loaded.getRoot()->right->key, 8);
    EXPECT_EQ(loaded.search(100), nullptr);    // старое стерлось
    remove("test_tree.bin");
}

TEST(SerializationTest, HashTable)
{
    hashTable ht;
    ht.add(5);
    ht.add(105);
    ht.add(7);
    ht.saveBinary("test_hash.bin");

    hashTable loaded;
    loaded.add(999);
    loaded.loadBinary("test_hash.bin");

    EXPECT_TRUE(loaded.search(5));
    EXPECT_TRUE(loaded.search(105));
    EXPECT_TRUE(loaded.search(7));
    EXPECT_FALSE(loaded.search(999));
    remove("test_hash.bin");
}

TEST(SerializationTest, Kukushka)
{
    Kukushka kt;
    kt.insert(1);
    kt.insert(12);
    kt.insert(42);
    kt.saveBinary("test_kukushka.bin");

    Kukushka loaded;
    loaded.insert(999);
    loaded.loadBinary("test_kukushka.bin");

    EXPECT_TRUE(loaded.search(1));
    EXPECT_TRUE(loaded.search(12));
    EXPECT_TRUE(loaded.search(42));
    EXPECT_FALSE(loaded.search(999));
    remove("test_kukushka.bin");
}

TEST(SerializationTest, LoadMissingFile)
{
    Array arr;
    arr.pushBack("a");
    arr.loadBinary("нет_такого_файла.bin");    // файла нет - ничего не меняется
    EXPECT_EQ(arr.lenght(), 1);

    Stack st;      st.loadBinary("нет_такого_файла.bin");
    Queue q;       q.loadBinary("нет_такого_файла.bin");
    Flist fl;      fl.loadBinary("нет_такого_файла.bin");
    Dlist dl;      dl.loadBinary("нет_такого_файла.bin");
    Tree tr;       tr.loadBinary("нет_такого_файла.bin");
    hashTable ht;  ht.loadBinary("нет_такого_файла.bin");
    Kukushka kt;   kt.loadBinary("нет_такого_файла.bin");

    EXPECT_TRUE(st.isEmptyStack());
    EXPECT_TRUE(q.isEmptyQ());
    EXPECT_TRUE(fl.isEmptyList());
    EXPECT_TRUE(dl.isEmptyList());
    EXPECT_EQ(tr.getRoot(), nullptr);
    EXPECT_EQ(ht.getAll().size(), 0u);
    EXPECT_EQ(kt.getAll().size(), 0u);
}
