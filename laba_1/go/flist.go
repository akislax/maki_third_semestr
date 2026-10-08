package main

import "fmt"

type FNode struct {
	value string
	next  *FNode
}

type FList struct {
	head *FNode
}

func flistPushFront(list *FList, value string) {
	list.head = &FNode{value, list.head}
}

func flistPushBack(list *FList, value string) {
	newNode := &FNode{value, nil}
	if list.head == nil {
		list.head = newNode
		return
	}
	current := list.head
	for current.next != nil {
		current = current.next
	}
	current.next = newNode
}

func flistPushAfter(node *FNode, value string) {
	node.next = &FNode{value, node.next}
}

func flistPushBefore(list *FList, target *FNode, value string) {
	if target == list.head {
		flistPushFront(list, value)
		return
	}
	p := list.head
	for p.next != target {
		p = p.next
	}
	flistPushAfter(p, value)
}

func flistPopFront(list *FList) {
	if list.head == nil {
		return
	}
	list.head = list.head.next
}

func flistPopBack(list *FList) {
	if list.head == nil {
		return
	}
	if list.head.next == nil {
		list.head = nil
		return
	}
	p := list.head
	for p.next.next != nil {
		p = p.next
	}
	p.next = nil
}

func flistPopAfter(node *FNode) {
	if node.next == nil {
		return
	}
	node.next = node.next.next
}

func flistPopBefore(list *FList, target *FNode) {
	if target == list.head {
		return
	}
	if list.head.next == target {
		flistPopFront(list)
		return
	}
	p := list.head
	for p.next.next != target {
		p = p.next
	}
	flistPopAfter(p)
}

func flistFind(list *FList, value string) *FNode {
	for current := list.head; current != nil; current = current.next {
		if current.value == value {
			return current
		}
	}
	return nil
}

func flistDeleteValue(list *FList, value string) {
	if list.head == nil {
		fmt.Println("нет такого элемента")
		return
	}
	if list.head.value == value {
		flistPopFront(list)
		return
	}
	p := list.head
	for p.next != nil && p.next.value != value {
		p = p.next
	}
	if p.next != nil {
		flistPopAfter(p)
	} else {
		fmt.Println("нет такого элемента")
	}
}

func flistPrint(list *FList) {
	for p := list.head; p != nil; p = p.next {
		fmt.Print(p.value, " -> ")
	}
	fmt.Println("nil")
}

func flistPrintReverse(node *FNode) {
	if node == nil {
		return
	}
	flistPrintReverse(node.next)
	fmt.Print(node.value, " ")
}

func flistToString(list *FList) string {
	result := ""
	for current := list.head; current != nil; current = current.next {
		if result == "" {
			result = current.value
		} else {
			result = result + " " + current.value
		}
	}
	return result
}