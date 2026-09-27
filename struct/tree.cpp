#include <iostream>
#include <string>

struct tree
{
    int key;
    tree* left;
    tree* right;
};

struct QNode
{
    tree* data;
    QNode* next;
};

struct Queue
{
    QNode* head;
    QNode* tail;
};



tree* insert(tree* root, int key)
{
    tree* new_node = new tree{key, nullptr, nullptr};
    
    if (root == nullptr) {
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

void preorder (tree* node) //сверху вниз
{
    if (node == nullptr) return;

    std::cout << node -> key << " ";
    preorder (node -> left);
    preorder (node -> right);
}

void inorder(tree* node) //симметричный
{
    if (node == nullptr) return;

    inorder(node->left);
    std::cout << node->key << " ";
    inorder(node->right);
}

void postorder(tree* node) //обратный
{
    if (node == nullptr) return;

    postorder(node->left);           // 1. всё левое
    postorder(node->right);          // 2. всё правое
    std::cout << node->key << " ";   // 3. только потом сам узел
}

void freeTree(tree* node)
{
    if (node == nullptr) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

// сделать очередь пустой
void initQueue(Queue* q)
{
    q->head = nullptr;
    q->tail = nullptr;
}

// пустая ли очередь
bool isEmptyQueue(Queue* q)
{
    return q->head == nullptr;
}

void enqueue(Queue* q, tree* node)
{
    QNode* new_node = new QNode{node, nullptr};
    if (isEmptyQueue(q))
    {
        q->head = new_node;
        q->tail = new_node;
    }
    else
    {
        q->tail->next = new_node;
        q->tail = new_node;
    }
}

// достать первого из очереди и вернуть адрес его узла дерева
tree* dequeue(Queue* q)
{
    QNode* tmp = q->head;
    tree* result = tmp->data;
    q->head = tmp->next;
    if (q->head == nullptr)
    {
        q->tail = nullptr;
    }
    delete tmp;
    return result;
}


// обход в ширину
void bfs(tree* root)
{
    if (root == nullptr) return;

    Queue q;
    initQueue(&q);
    enqueue(&q, root);

    while (!isEmptyQueue(&q))
    {
        tree* node = dequeue(&q);
        std::cout << node->key << " ";

        if (node->left != nullptr)
            enqueue(&q, node->left);
        if (node->right != nullptr)
            enqueue(&q, node->right);
    }
}

bool isFull(tree* node)
{
    if (node == nullptr) return true;
    if (node->left == nullptr && node->right == nullptr) return true;   // лист
    if (node->left != nullptr && node->right != nullptr)                // 2 ребёнка
        return isFull(node->left) && isFull(node->right);
    return false;                                                       // 1 ребёнок
}

int main()
{
    // ДЕРЕВО 1: full
    tree* root = nullptr;
    int keys1[] = {15, 6, 18, 3, 7, 17, 20, 2, 4};
    for (int k : keys1) root = insert(root, k);

    std::cout << "=== Дерево 1 ===" << std::endl;
    std::cout << "прямой:       "; preorder(root);  std::cout << std::endl;
    std::cout << "симметричный: "; inorder(root);   std::cout << std::endl;
    std::cout << "обратный:     "; postorder(root); std::cout << std::endl;
    std::cout << "в ширину:     "; bfs(root);       std::cout << std::endl;
    std::cout << "full: " << (isFull(root) ? "да" : "нет") << std::endl;
    freeTree(root);

    // ДЕРЕВО 2: не full (у 7 и 13 по одному ребёнку)
    root = nullptr;
    int keys2[] = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};
    for (int k : keys2) root = insert(root, k);

    std::cout << std::endl << "=== Дерево 2 ===" << std::endl;
    std::cout << "в ширину: "; bfs(root); std::cout << std::endl;
    std::cout << "full: " << (isFull(root) ? "да" : "нет") << std::endl;
    freeTree(root);
    root = nullptr;

    return 0;
}
