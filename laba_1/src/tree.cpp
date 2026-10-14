#include "tree.h"

tree* insert(tree* root, int key)
{
    tree* new_node = new tree{key, nullptr, nullptr};
    if (root == nullptr) return new_node;

    vector<tree*> ochered;
    ochered.push_back(root);
    for (size_t i = 0; i < ochered.size(); i++)
    {
        tree* node = ochered[i];

        if (node -> left == nullptr)
        {
            node -> left = new_node;
            return root;
        }
        ochered.push_back(node -> left);

        if (node -> right == nullptr)
        {
            node -> right = new_node;
            return root;
        }
        ochered.push_back(node -> right);
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
    if (root == nullptr) return nullptr;

    vector<tree*> ochered;
    ochered.push_back(root);
    for (size_t i = 0; i < ochered.size(); i++)
    {
        tree* node = ochered[i];
        if (node -> key == key) return node;
        if (node -> left != nullptr) ochered.push_back(node -> left);
        if (node -> right != nullptr) ochered.push_back(node -> right);
    }
    return nullptr;
}

void handleTree(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "tree")) return;

    tree* root = nullptr;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            root = insert(root, element.get<int>());
        }
    }

    int value;
    switch (command)
    {
        case Command::TINSERT:
            ss >> value;
            root = insert(root, value);
            cout << "-> " << value << "\n";
            break;
        case Command::TFIND:
            ss >> value;
            if (search(root, value) != nullptr) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::TCOMPLETE:
            if (isComplete(root)) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::TPRINT:
            cout << "прямой:       "; preorder(root);  cout << "\n";
            cout << "симметричный: "; inorder(root);   cout << "\n";
            cout << "обратный:     "; postorder(root); cout << "\n";
            cout << "в ширину:     "; bfs(root);       cout << "\n";
            break;
        default:
            break;
    }

    json data = json::array();
    vector<tree*> ochered;
    if (root != nullptr) ochered.push_back(root);
    for (size_t i = 0; i < ochered.size(); i++)
    {
        tree* node = ochered[i];
        data.push_back(node -> key);
        if (node -> left != nullptr) ochered.push_back(node -> left);
        if (node -> right != nullptr) ochered.push_back(node -> right);
    }
    db[name]["type"] = "tree";
    db[name]["data"] = data;

    freeTree(root);
}