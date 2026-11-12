#pragma once
#include "serialize.h"
#include <vector>
#include "commandsClass.h"
#include <iostream>
#include <vector>
using namespace std;

struct TreeNode
{
    int key;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
};

class Tree
{
private:
    TreeNode *root = nullptr;

    // рекурсивные помощники: им нужен узел, с которого начинать
    void preorder(TreeNode *node) const;
    void inorder(TreeNode *node) const;
    void postorder(TreeNode *node) const;
    void freeTree(TreeNode *node);

public:
    void saveBinary(const string &fileName) const;
    void loadBinary(const string &fileName);
    Tree();
    ~Tree();
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    void insert(int key);
    TreeNode *search(int key) const;
    bool isComplete() const;
    void printPreorder() const;
    void printInorder() const;
    void printPostorder() const; //функция нужна для handletree она работает с полями и принимает корень и дальеш она вызвается в хендли три
    void bfs() const;
    TreeNode *getRoot() const;
};

void handleTree(Command command, json& db, const string& name, stringstream& ss);