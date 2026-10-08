#include "tree.h"

tree* insert(tree* root, int key)
{
  tree* new_node = new tree{key, nullptr, nullptr};
  
  if (root == nullptr)
  {
    return new_node;
  }
  tree* parent = nullptr;
  tree* current = root;
  while (current != nullptr)
  {
    parent = current;
    if (key < current -> key)
    {
      current = current -> left;
    }
    else
    {
      current = current -> right;
    }
  }
  if (key < parent -> key)
  {
    parent -> left = new_node;
  }
  else
  {
    parent -> right = new_node;
  }
  return root;
}

void preorder(tree* node)
{
  if (node == nullptr) return;
  cout << node -> key << " ";
  preorder (node -> left);
  preorder (node -> right);
}

void inorder(tree* node)
{
  if (node == nullptr) return;
  inorder(node -> left);
  cout << node -> key << " ";
  inorder(node -> right);
}

void postorder(tree* node)
{
  if (node == nullptr) return;
  postorder(node -> left);
  postorder(node -> right);
  cout << node -> key << " ";
}

void bfs(tree* root)
{
  if (root == nullptr) return;

  vector<tree*> ochered;
  ochered.push_back(root);
  for (size_t i = 0; i < ochered.size(); i++)
  {
    tree* node = ochered[i];
    cout << node -> key << " ";
    if (node -> left != nullptr) ochered.push_back(node -> left);
    if (node -> right != nullptr) ochered.push_back(node -> right);
  }
}

void freeTree(tree* node)
{
  if (node == nullptr) return;
  freeTree(node -> left);
  freeTree(node -> right);
  delete node;
}

bool isComplete(tree* root)
{
    if (root == nullptr) return true;

    vector<tree*> ochered;
    ochered.push_back(root);
    bool sawEmpty = false;

    for (size_t i = 0; i < ochered.size(); i++)
    {
        tree* node = ochered[i];

        if (node == nullptr)
        {
            sawEmpty = true;
        }
        else
        {
            if (sawEmpty) return false;
            ochered.push_back(node -> left);
            ochered.push_back(node -> right);
        }
    }
    return true;
}

tree* search(tree* root, int key)
{
    tree* current = root;
    while (current != nullptr)
    {
        if (key == current -> key) return current;
        if (key < current -> key)
        {
            current = current -> left;
        }
        else
        {
            current = current -> right;
        }
    }
    return nullptr;
}