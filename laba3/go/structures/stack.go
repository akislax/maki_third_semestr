package structures

import (
	"fmt"
	"io"
	"os"
)

// SNode - узел стека.
type SNode struct {
	A    string
	Next *SNode
}

// Stack - стек на односвязном списке, head - вершина.
type Stack struct {
	head *SNode
}

// NewStack создает пустой стек.
func NewStack() *Stack {
	return &Stack{}
}

// IsEmptyStack проверяет, пуст ли стек.
func (st *Stack) IsEmptyStack() bool {
	return st.head == nil
}

// Push кладет элемент на вершину.
func (st *Stack) Push(a string) {
	st.head = &SNode{A: a, Next: st.head}
}

// Pop снимает элемент с вершины.
func (st *Stack) Pop() {
	if st.IsEmptyStack() {
		return
	}
	st.head = st.head.Next
}

// Top возвращает вершину, для пустого стека - пустую строку.
func (st *Stack) Top() string {
	if st.IsEmptyStack() {
		return ""
	}
	return st.head.A
}

// Print печатает стек от вершины вниз.
func (st *Stack) Print() {
	for current := st.head; current != nil; current = current.Next {
		fmt.Print(current.A, " -> ")
	}
	fmt.Println("nil")
}

// GetHead возвращает вершину стека.
func (st *Stack) GetHead() *SNode {
	return st.head
}

// HandleStack выполняет команду над стеком с именем name.
func HandleStack(command Command, db DB, name string, ss io.Reader) {
	if WrongType(db, name, "stack") {
		return
	}

	stack := NewStack()
	for _, item := range loadStrings(db, name) {
		stack.Push(item)
	}

	var value string
	switch command {
	case SPUSH:
		fmt.Fscan(ss, &value)
		stack.Push(value)
		fmt.Println("->", value)
	case SPOP:
		if stack.IsEmptyStack() {
			fmt.Println("-> стек пуст")
		} else {
			fmt.Println("->", stack.Top())
			stack.Pop()
		}
	case SPRINT:
		stack.Print()
	}

	// идем от вершины вниз и вставляем в начало,
	// чтобы в json был порядок снизу вверх
	items := []string{}
	for current := stack.GetHead(); current != nil; current = current.Next {
		items = append([]string{current.A}, items...)
	}
	saveStrings(db, name, "stack", items)
}

// SaveBinary сохраняет стек в бинарный файл:
// количество элементов, затем строки снизу вверх.
func (st *Stack) SaveBinary(fileName string) {
	out, err := os.Create(fileName)
	if err != nil {
		return
	}
	defer out.Close()

	items := []string{}
	for current := st.head; current != nil; current = current.Next {
		items = append(items, current.A)
	}

	writeInt(out, len(items))
	for i := len(items) - 1; i >= 0; i-- {
		writeString(out, items[i])
	}
}

// LoadBinary загружает стек из бинарного файла.
// Старые элементы перед загрузкой удаляются.
func (st *Stack) LoadBinary(fileName string) {
	in, err := os.Open(fileName)
	if err != nil {
		return
	}
	defer in.Close()

	st.head = nil
	count := readInt(in)
	for i := 0; i < count; i++ {
		st.Push(readString(in))
	}
}
