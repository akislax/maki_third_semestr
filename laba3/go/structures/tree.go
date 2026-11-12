package structures

import (
	"fmt"
	"io"
	"os"
)

// TreeNode - узел двоичного дерева поиска.
type TreeNode struct {
	Key   int
	Left  *TreeNode
	Right *TreeNode
}

// Tree - двоичное дерево поиска.
type Tree struct {
	root *TreeNode
}

// NewTree создает пустое дерево.
func NewTree() *Tree {
	return &Tree{}
}

// Insert вставляет ключ: меньшие уходят влево, остальные вправо.
func (tr *Tree) Insert(key int) {
	newNode := &TreeNode{Key: key}
	if tr.root == nil {
		tr.root = newNode
		return
	}

	var parent *TreeNode
	current := tr.root
	for current != nil {
		parent = current
		if key < current.Key {
			current = current.Left
		} else {
			current = current.Right
		}
	}

	if key < parent.Key {
		parent.Left = newNode
	} else {
		parent.Right = newNode
	}
}

// Рекурсивные обходы. Им нужен узел, с которого начинать,
// поэтому снаружи они вызываются через PrintPreorder и т.д.

func (tr *Tree) preorder(node *TreeNode) {
	if node == nil {
		return
	}
	fmt.Print(node.Key, " ")
	tr.preorder(node.Left)
	tr.preorder(node.Right)
}

func (tr *Tree) inorder(node *TreeNode) {
	if node == nil {
		return
	}
	tr.inorder(node.Left)
	fmt.Print(node.Key, " ")
	tr.inorder(node.Right)
}

func (tr *Tree) postorder(node *TreeNode) {
	if node == nil {
		return
	}
	tr.postorder(node.Left)
	tr.postorder(node.Right)
	fmt.Print(node.Key, " ")
}

// PrintPreorder печатает прямой обход.
func (tr *Tree) PrintPreorder() {
	tr.preorder(tr.root)
}

// PrintInorder печатает симметричный обход.
func (tr *Tree) PrintInorder() {
	tr.inorder(tr.root)
}

// PrintPostorder печатает обратный обход.
func (tr *Tree) PrintPostorder() {
	tr.postorder(tr.root)
}

// Bfs печатает обход в ширину.
func (tr *Tree) Bfs() {
	if tr.root == nil {
		return
	}

	ochered := []*TreeNode{tr.root}
	for i := 0; i < len(ochered); i++ {
		node := ochered[i]
		fmt.Print(node.Key, " ")
		if node.Left != nil {
			ochered = append(ochered, node.Left)
		}
		if node.Right != nil {
			ochered = append(ochered, node.Right)
		}
	}
}

// IsComplete проверяет, что дерево полное: при обходе в ширину
// после первого пустого места не должно встречаться узлов.
func (tr *Tree) IsComplete() bool {
	if tr.root == nil {
		return true
	}

	ochered := []*TreeNode{tr.root}
	sawEmpty := false
	for i := 0; i < len(ochered); i++ {
		node := ochered[i]
		if node == nil {
			sawEmpty = true
		} else {
			if sawEmpty {
				return false
			}
			ochered = append(ochered, node.Left, node.Right)
		}
	}
	return true
}

// Search ищет узел с ключом key, если не нашел - возвращает nil.
func (tr *Tree) Search(key int) *TreeNode {
	current := tr.root
	for current != nil {
		if key == current.Key {
			return current
		}
		if key < current.Key {
			current = current.Left
		} else {
			current = current.Right
		}
	}
	return nil
}

// GetRoot возвращает корень дерева.
func (tr *Tree) GetRoot() *TreeNode {
	return tr.root
}

// bfsKeys возвращает ключи в порядке обхода в ширину.
// Если вставить их в том же порядке, получится такое же дерево.
func (tr *Tree) bfsKeys() []int {
	keys := []int{}
	if tr.root == nil {
		return keys
	}

	ochered := []*TreeNode{tr.root}
	for i := 0; i < len(ochered); i++ {
		node := ochered[i]
		keys = append(keys, node.Key)
		if node.Left != nil {
			ochered = append(ochered, node.Left)
		}
		if node.Right != nil {
			ochered = append(ochered, node.Right)
		}
	}
	return keys
}

// HandleTree выполняет команду над деревом с именем name.
func HandleTree(command Command, db DB, name string, ss io.Reader) {
	if WrongType(db, name, "tree") {
		return
	}

	tr := NewTree()
	for _, key := range loadInts(db, name) {
		tr.Insert(key)
	}

	var value int
	switch command {
	case TINSERT:
		fmt.Fscan(ss, &value)
		tr.Insert(value)
		fmt.Println("->", value)
	case TFIND:
		fmt.Fscan(ss, &value)
		if tr.Search(value) != nil {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case TCOMPLETE:
		if tr.IsComplete() {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case TPRINT:
		fmt.Print("прямой:       ")
		tr.PrintPreorder()
		fmt.Println()
		fmt.Print("симметричный: ")
		tr.PrintInorder()
		fmt.Println()
		fmt.Print("обратный:     ")
		tr.PrintPostorder()
		fmt.Println()
		fmt.Print("в ширину:     ")
		tr.Bfs()
		fmt.Println()
	}

	saveInts(db, name, "tree", tr.bfsKeys())
}

// SaveBinary сохраняет дерево в бинарный файл:
// количество ключей, затем ключи в порядке обхода в ширину.
func (tr *Tree) SaveBinary(fileName string) {
	out, err := os.Create(fileName)
	if err != nil {
		return
	}
	defer out.Close()

	keys := tr.bfsKeys()
	writeInt(out, len(keys))
	for _, key := range keys {
		writeInt(out, key)
	}
}

// LoadBinary загружает дерево из бинарного файла.
// Старые ключи перед загрузкой удаляются.
func (tr *Tree) LoadBinary(fileName string) {
	in, err := os.Open(fileName)
	if err != nil {
		return
	}
	defer in.Close()

	tr.root = nil
	count := readInt(in)
	for i := 0; i < count; i++ {
		tr.Insert(readInt(in))
	}
}
