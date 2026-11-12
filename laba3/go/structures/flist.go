package structures

import (
	"fmt"
	"io"
	"os"
)

// FNode - узел односвязного списка.
type FNode struct {
	Value string
	Next  *FNode
}

// Flist - односвязный список.
type Flist struct {
	head *FNode
}

// NewFlist создает пустой список.
func NewFlist() *Flist {
	return &Flist{}
}

// IsEmptyList проверяет, пуст ли список.
func (fl *Flist) IsEmptyList() bool {
	return fl.head == nil
}

// PushFront добавляет элемент в начало.
func (fl *Flist) PushFront(a string) {
	fl.head = &FNode{Value: a, Next: fl.head}
}

// PopFront удаляет первый элемент.
func (fl *Flist) PopFront() {
	if fl.IsEmptyList() {
		return
	}
	fl.head = fl.head.Next
}

// AddOne вставляет элемент после узла first.
func (fl *Flist) AddOne(first *FNode, a string) {
	if first == nil {
		return
	}
	first.Next = &FNode{Value: a, Next: first.Next}
}

// DeleteNode удаляет элемент после узла abc.
func (fl *Flist) DeleteNode(abc *FNode) {
	if abc == nil || abc.Next == nil {
		return
	}
	abc.Next = abc.Next.Next
}

// AddTail добавляет элемент в конец.
// Хвоста у списка нет, поэтому идем до конца от головы.
func (fl *Flist) AddTail(value string) {
	newNode := &FNode{Value: value}

	if fl.IsEmptyList() {
		fl.head = newNode
		return
	}

	tmp := fl.head
	for tmp.Next != nil {
		tmp = tmp.Next
	}
	tmp.Next = newNode
}

// PopTail удаляет последний элемент.
func (fl *Flist) PopTail() {
	if fl.IsEmptyList() {
		return
	}

	if fl.head.Next == nil {
		fl.head = nil
		return
	}

	tmp := fl.head
	for tmp.Next.Next != nil {
		tmp = tmp.Next
	}
	tmp.Next = nil
}

// AddBefore вставляет элемент перед узлом target.
func (fl *Flist) AddBefore(target *FNode, a string) {
	if target == nil {
		return
	}

	if target == fl.head {
		fl.PushFront(a)
		return
	}

	p := fl.head
	for p.Next != target {
		p = p.Next
	}
	fl.AddOne(p, a)
}

// DeleteBefore удаляет элемент перед узлом target.
func (fl *Flist) DeleteBefore(target *FNode) {
	if fl.IsEmptyList() || target == nil || target == fl.head {
		return
	}

	if fl.head.Next == target {
		fl.PopFront()
		return
	}

	p := fl.head
	for p.Next.Next != target {
		p = p.Next
	}
	fl.DeleteNode(p)
}

// FindValue ищет узел со значением a, если не нашел - возвращает nil.
func (fl *Flist) FindValue(a string) *FNode {
	for current := fl.head; current != nil; current = current.Next {
		if current.Value == a {
			return current
		}
	}
	return nil
}

// DeleteValue удаляет первый элемент со значением a.
func (fl *Flist) DeleteValue(a string) {
	if fl.IsEmptyList() {
		return
	}

	if fl.head.Value == a {
		fl.PopFront()
		return
	}

	p := fl.head
	for p.Next != nil && p.Next.Value != a {
		p = p.Next
	}

	if p.Next != nil {
		fl.DeleteNode(p)
	}
}

// printReverseFrom рекурсивно печатает список с конца, начиная с node.
func (fl *Flist) printReverseFrom(node *FNode) {
	if node == nil {
		return
	}
	fl.printReverseFrom(node.Next)
	fmt.Print(node.Value, " ")
}

// PrintReverse печатает список с конца.
func (fl *Flist) PrintReverse() {
	fl.printReverseFrom(fl.head)
	fmt.Println()
}

// PrintList печатает список с начала.
func (fl *Flist) PrintList() {
	for p := fl.head; p != nil; p = p.Next {
		fmt.Print(p.Value, " -> ")
	}
	fmt.Println("nil")
}

// GetHead возвращает первый узел списка.
func (fl *Flist) GetHead() *FNode {
	return fl.head
}

// HandleFlist выполняет команду над односвязным списком с именем name.
func HandleFlist(command Command, db DB, name string, ss io.Reader) {
	if WrongType(db, name, "flist") {
		return
	}

	fl := NewFlist()
	for _, item := range loadStrings(db, name) {
		fl.AddTail(item)
	}

	var target, value string
	var node *FNode
	switch command {
	case FPUSHH:
		fmt.Fscan(ss, &value)
		fl.PushFront(value)
	case FPUSHT:
		fmt.Fscan(ss, &value)
		fl.AddTail(value)
	case FPUSHA:
		fmt.Fscan(ss, &target, &value)
		node = fl.FindValue(target)
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			fl.AddOne(node, value)
		}
	case FPUSHB:
		fmt.Fscan(ss, &target, &value)
		node = fl.FindValue(target)
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			fl.AddBefore(node, value)
		}
	case FDELH:
		fl.PopFront()
	case FDELT:
		fl.PopTail()
	case FDELA:
		fmt.Fscan(ss, &target)
		node = fl.FindValue(target)
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			fl.DeleteNode(node)
		}
	case FDELB:
		fmt.Fscan(ss, &target)
		node = fl.FindValue(target)
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			fl.DeleteBefore(node)
		}
	case FDEL:
		fmt.Fscan(ss, &value)
		fl.DeleteValue(value)
	case FFIND:
		fmt.Fscan(ss, &value)
		if fl.FindValue(value) != nil {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case FPRINT:
		fl.PrintList()
	case FPRINTR:
		fl.PrintReverse()
	}

	items := []string{}
	for current := fl.GetHead(); current != nil; current = current.Next {
		items = append(items, current.Value)
	}
	saveStrings(db, name, "flist", items)
}

// SaveBinary сохраняет список в бинарный файл:
// количество элементов, затем строки от головы к хвосту.
func (fl *Flist) SaveBinary(fileName string) {
	out, err := os.Create(fileName)
	if err != nil {
		return
	}
	defer out.Close()

	count := 0
	for current := fl.head; current != nil; current = current.Next {
		count++
	}

	writeInt(out, count)
	for current := fl.head; current != nil; current = current.Next {
		writeString(out, current.Value)
	}
}

// LoadBinary загружает список из бинарного файла.
// Старые элементы перед загрузкой удаляются.
func (fl *Flist) LoadBinary(fileName string) {
	in, err := os.Open(fileName)
	if err != nil {
		return
	}
	defer in.Close()

	fl.head = nil
	count := readInt(in)
	for i := 0; i < count; i++ {
		fl.AddTail(readString(in))
	}
}
