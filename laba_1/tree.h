#pragma once
#include <iostream>
#include <vector>
using namespace std;

struct tree 
{
  int key;
  tree* right;
  tree* left;
};

tree* insert(tree* root, int key);
void preorder(tree* node);
void inorder(tree* node);
void postorder(tree* node);
void bfs(tree* root);
void freeTree(tree* node);
bool isComplete(tree* root);
tree* search(tree* root, int key);