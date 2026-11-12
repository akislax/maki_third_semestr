package structures

import (
	"fmt"
	"io"
	"os"
)

// DNode - узел двусвязного списка.
type DNode struct {
	Person string
	Next   *DNode
	Prev   *DNode
}

// Dlist - двусвязный список с головой и хвостом.
type Dlist struct {
	head *DNode
	tail *DNode
}

// NewDlist создает пустой список.
func NewDlist() *Dlist {
	return &Dlist{}
}

// IsEmptyList проверяет, пуст ли список.
func (dl *Dlist) IsEmptyList() bool {
	return dl.head == nil && dl.tail == nil
}

// PushBack добавляет элемент в конец.
func (dl *Dlist) PushBack(name string) {
	newNode := &DNode{Person: name}

	if dl.IsEmptyList() {
		dl.head = newNode
		dl.tail = newNode
	} else {
		dl.tail.Next = newNode
		newNode.Prev = dl.tail
		dl.tail = newNode
	}
}

// PopBack удаляет последний элемент.
func (dl *Dlist) PopBack() {
	if dl.IsEmptyList() {
		return
	}

	dl.tail = dl.tail.Prev
	if dl.tail == nil {
		// узел был единственным, список опустел
		dl.head = nil
	} else {
		dl.tail.Next = nil
	}
}

// PushFront добавляет элемент в начало.
func (dl *Dlist) PushFront(name string) {
	newNode := &DNode{Person: name}

	if dl.IsEmptyList() {
		dl.head = newNode
		dl.tail = newNode
	} else {
		dl.head.Prev = newNode
		newNode.Next = dl.head
		dl.head = newNode
	}
}

// PopFront удаляет первый элемент.
func (dl *Dlist) PopFront() {
	if dl.IsEmptyList() {
		return
	}

	dl.head = dl.head.Next
	if dl.head == nil {
		// узел был единственным, список опустел
		dl.tail = nil
	} else {
		dl.head.Prev = nil
	}
}

// PushBetween вставляет элемент после узла node.
func (dl *Dlist) PushBetween(node *DNode, name string) {
	if node == nil {
		return
	}

	newNode := &DNode{Person: name, Next: node.Next, Prev: node}
	if node.Next != nil {
		node.Next.Prev = newNode
	} else {
		dl.tail = newNode
	}
	node.Next = newNode
}

// PushBefore вставляет элемент перед узлом node.
func (dl *Dlist) PushBefore(node *DNode, name string) {
	if node == nil {
		return
	}

	if node.Prev == nil {
		dl.PushFront(name)
	} else {
		dl.PushBetween(node.Prev, name)
	}
}

// PopAfter удаляет элемент после узла node.
func (dl *Dlist) PopAfter(node *DNode) {
	if node == nil || node.Next == nil {
		return
	}

	tmp := node.Next
	if tmp.Next == nil {
		// удаляем хвост
		node.Next = nil
		dl.tail = node
		return
	}

	node.Next = tmp.Next
	tmp.Next.Prev = node
}

// PopBefore удаляет элемент перед узлом node.
func (dl *Dlist) PopBefore(node *DNode) {
	if node == nil || node.Prev == nil {
		return
	}

	tmp := node.Prev
	if tmp.Prev == nil {
		// удаляем голову
		node.Prev = nil
		dl.head = node
		return
	}

	tmp.Prev.Next = node
	node.Prev = tmp.Prev
}

// PrintForward печатает список от головы к хвосту.
func (dl *Dlist) PrintForward() {
	for current := dl.head; current != nil; current = current.Next {
		fmt.Print(current.Person, " <-> ")
	}
	fmt.Println("nil")
}

// PrintBackward печатает список от хвоста к голове.
func (dl *Dlist) PrintBackward() {
	for current := dl.tail; current != nil; current = current.Prev {
		fmt.Print(current.Person, " <-> ")
	}
	fmt.Println("nil")
}

// FindName ищет узел со значением name, если не нашел - возвращает nil.
func (dl *Dlist) FindName(name string) *DNode {
	for current := dl.head; current != nil; current = current.Next {
		if current.Person == name {
			return current
		}
	}
	return nil
}

// PopName удаляет первый элемент со значением name.
func (dl *Dlist) PopName(name string) {
	node := dl.FindName(name)
	if node == nil {
		fmt.Println("нет такого элемента")
		return
	}

	if node == dl.head {
		dl.PopFront()
	} else {
		dl.PopAfter(node.Prev)
	}
}

// GetHead возвращает первый узел списка.
func (dl *Dlist) GetHead() *DNode {
	return dl.head
}

// HandleDlist выполняет команду над двусвязным списком с именем name.
func HandleDlist(command Command, db DB, name string, ss io.Reader) {
	if WrongType(db, name, "dlist") {
		return
	}

	dl := NewDlist()
	for _, item := range loadStrings(db, name) {
		dl.PushBack(item)
	}

	var target, value string
	var node *DNode
	switch command {
	case LPUSHH:
		fmt.Fscan(ss, &value)
		dl.PushFront(value)
	case LPUSHT:
		fmt.Fscan(ss, &value)
		dl.PushBack(value)
	case LPUSHA:
		fmt.Fscan(ss, &target, &value)
		node = dl.FindName(target)
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			dl.PushBetween(node, value)
		}
	case LPUSHB:
		fmt.Fscan(ss, &target, &value)
		node = dl.FindName(target)
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			dl.PushBefore(node, value)
		}
	case LDELH:
		dl.PopFront()
	case LDELT:
		dl.PopBack()
	case LDELA:
		fmt.Fscan(ss, &target)
		node = dl.FindName(target)
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			dl.PopAfter(node)
		}
	case LDELB:
		fmt.Fscan(ss, &target)
		node = dl.FindName(target)
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			dl.PopBefore(node)
		}
	case LDEL:
		fmt.Fscan(ss, &value)
		dl.PopName(value)
	case LFIND:
		fmt.Fscan(ss, &value)
		if dl.FindName(value) != nil {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case LPRINT:
		dl.PrintForward()
	case LPRINTR:
		dl.PrintBackward()
	}

	items := []string{}
	for current := dl.GetHead(); current != nil; current = current.Next {
		items = append(items, current.Person)
	}
	saveStrings(db, name, "dlist", items)
}

// SaveBinary сохраняет список в бинарный файл:
// количество элементов, затем строки от головы к хвосту.
func (dl *Dlist) SaveBinary(fileName string) {
	out, err := os.Create(fileName)
	if err != nil {
		return
	}
	defer out.Close()

	count := 0
	for current := dl.head; current != nil; current = current.Next {
		count++
	}

	writeInt(out, count)
	for current := dl.head; current != nil; current = current.Next {
		writeString(out, current.Person)
	}
}

// LoadBinary загружает список из бинарного файла.
// Старые элементы перед загрузкой удаляются.
func (dl *Dlist) LoadBinary(fileName string) {
	in, err := os.Open(fileName)
	if err != nil {
		return
	}
	defer in.Close()

	dl.head = nil
	dl.tail = nil
	count := readInt(in)
	for i := 0; i < count; i++ {
		dl.PushBack(readString(in))
	}
}
