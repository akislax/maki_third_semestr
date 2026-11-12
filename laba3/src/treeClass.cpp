#include "treeClass.h"

Tree::Tree()
{
    root = nullptr;
}

Tree::~Tree()
{
    freeTree(root);
}

void Tree::freeTree(TreeNode* node)
{
    if (node == nullptr) return;
    freeTree(node -> left);
    freeTree(node -> right);
    delete node;
}

void Tree::insert(int key)
{
    TreeNode* new_node = new TreeNode{key, nullptr, nullptr};
    if (root == nullptr)
    {
        root = new_node;
        return;
    }

    TreeNode* parent = nullptr;
    TreeNode* current = root;
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
}

void Tree::preorder(TreeNode* node) const
{
    if (node == nullptr) return;
    cout << node -> key << " ";
    preorder(node -> left);
    preorder(node -> right);
}

void Tree::inorder(TreeNode* node) const
{
    if (node == nullptr) return;
    inorder(node -> left);
    cout << node -> key << " ";
    inorder(node -> right);
}

void Tree::postorder(TreeNode* node) const
{
    if (node == nullptr) return;
    postorder(node -> left);
    postorder(node -> right);
    cout << node -> key << " ";
}
//обходу нужен корень, корень спрятан в объекте, printPreorder - это метод, который достает его изнутри и передает в обход.
void Tree::printPreorder() const
{
    preorder(root);
}

void Tree::printInorder() const
{
    inorder(root);
}

void Tree::printPostorder() const
{
    postorder(root);
}

void Tree::bfs() const
{
    if (root == nullptr) return;

    vector<TreeNode*> ochered;
    ochered.push_back(root);
    for (size_t i = 0; i < ochered.size(); i++)
    {
        TreeNode* node = ochered[i];
        cout << node -> key << " ";
        if (node -> left != nullptr) ochered.push_back(node -> left);
        if (node -> right != nullptr) ochered.push_back(node -> right);
    }
}

bool Tree::isComplete() const
{
    if (root == nullptr) return true;

    vector<TreeNode*> ochered;
    ochered.push_back(root);
    bool sawEmpty = false;

    for (size_t i = 0; i < ochered.size(); i++)
    {
        TreeNode* node = ochered[i];

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

TreeNode* Tree::search(int key) const
{
    TreeNode* current = root;
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

TreeNode* Tree::getRoot() const
{
    return root;
}

void handleTree(Command command, json& db, const string& name, stringstream& ss)
{
    if (wrongType(db, name, "tree")) return;

    Tree tr;
    if (db.contains(name))
    {
        for (auto& element : db[name]["data"])
        {
            tr.insert(element.get<int>());
        }
    }

    int value;
    switch (command)
    {
        case Command::TINSERT:
            ss >> value;
            tr.insert(value);
            cout << "-> " << value << "\n";
            break;
        case Command::TFIND:
            ss >> value;
            if (tr.search(value) != nullptr) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::TCOMPLETE:
            if (tr.isComplete()) cout << "-> TRUE\n";
            else cout << "-> FALSE\n";
            break;
        case Command::TPRINT:
            cout << "прямой:       "; tr.printPreorder();  cout << "\n";
            cout << "симметричный: "; tr.printInorder();   cout << "\n";
            cout << "обратный:     "; tr.printPostorder(); cout << "\n";
            cout << "в ширину:     "; tr.bfs();            cout << "\n";
            break;
        default:
            break;
    }

    // сохраняем обходом в ширину, чтобы при загрузке дерево собралось таким же
    json data = json::array();
    vector<TreeNode*> ochered;
    if (tr.getRoot() != nullptr) ochered.push_back(tr.getRoot());
    for (size_t i = 0; i < ochered.size(); i++)
    {
        TreeNode* node = ochered[i];
        data.push_back(node -> key);
        if (node -> left != nullptr) ochered.push_back(node -> left);
        if (node -> right != nullptr) ochered.push_back(node -> right);
    }
    db[name]["type"] = "tree";
    db[name]["data"] = data;
}

// сериализация: [количество] [ключи в порядке обхода в ширину]
// при загрузке вставляем в том же порядке - дерево получится таким же
void Tree::saveBinary(const string& fileName) const
{
    vector<TreeNode*> ochered;
    if (root != nullptr) ochered.push_back(root);
    for (size_t i = 0; i < ochered.size(); i++)
    {
        TreeNode* node = ochered[i];
        if (node -> left != nullptr) ochered.push_back(node -> left);
        if (node -> right != nullptr) ochered.push_back(node -> right);
    }

    ofstream out(fileName, ios::binary);
    writeInt(out, static_cast<int>(ochered.size()));
    for (TreeNode* node : ochered)
    {
        writeInt(out, node -> key);
    }
}

void Tree::loadBinary(const string& fileName)
{
    ifstream in(fileName, ios::binary);
    if (!in.is_open()) return;

    freeTree(root);                            // очищаем дерево перед загрузкой
    root = nullptr;
    int count = readInt(in);
    for (int i = 0; i < count; i++)
    {
        insert(readInt(in));
    }
}
