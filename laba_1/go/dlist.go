package main

import "fmt"

type DNode struct {
	value string
	next  *DNode
	prev  *DNode
}

type DList struct {
	head *DNode
	tail *DNode
}

func dlistPushFront(list *DList, value string) {
	newNode := &DNode{value: value}
	if list.head == nil {
		list.head = newNode
		list.tail = newNode
		return
	}
	newNode.next = list.head
	list.head.prev = newNode
	list.head = newNode
}

func dlistPushBack(list *DList, value string) {
	newNode := &DNode{value: value}
	if list.tail == nil {
		list.head = newNode
		list.tail = newNode
		return
	}
	newNode.prev = list.tail
	list.tail.next = newNode
	list.tail = newNode
}

func dlistPushAfter(list *DList, node *DNode, value string) {
	newNode := &DNode{value: value, next: node.next, prev: node}
	if node.next != nil {
		node.next.prev = newNode
	} else {
		list.tail = newNode
	}
	node.next = newNode
}

func dlistPushBefore(list *DList, node *DNode, value string) {
	if node.prev == nil {
		dlistPushFront(list, value)
	} else {
		dlistPushAfter(list, node.prev, value)
	}
}

func dlistPopFront(list *DList) {
	if list.head == nil {
		return
	}
	list.head = list.head.next
	if list.head == nil {
		list.tail = nil
	} else {
		list.head.prev = nil
	}
}

func dlistPopBack(list *DList) {
	if list.tail == nil {
		return
	}
	list.tail = list.tail.prev
	if list.tail == nil {
		list.head = nil
	} else {
		list.tail.next = nil
	}
}

func dlistPopAfter(list *DList, node *DNode) {
	removed := node.next
	if removed == nil {
		return
	}
	if removed == list.tail {
		dlistPopBack(list)
		return
	}
	node.next = removed.next
	removed.next.prev = node
}

func dlistPopBefore(list *DList, node *DNode) {
	removed := node.prev
	if removed == nil {
		return
	}
	if removed == list.head {
		dlistPopFront(list)
		return
	}
	removed.prev.next = node
	node.prev = removed.prev
}

func dlistFind(list *DList, value string) *DNode {
	for current := list.head; current != nil; current = current.next {
		if current.value == value {
			return current
		}
	}
	return nil
}

func dlistDeleteValue(list *DList, value string) {
	node := dlistFind(list, value)
	if node == nil {
		fmt.Println("нет такого элемента")
		return
	}
	if node == list.head {
		dlistPopFront(list)
	} else {
		dlistPopAfter(list, node.prev)
	}
}

func dlistPrintForward(list *DList) {
	for p := list.head; p != nil; p = p.next {
		fmt.Print(p.value, " <-> ")
	}
	fmt.Println("nil")
}

func dlistPrintBackward(list *DList) {
	for p := list.tail; p != nil; p = p.prev {
		fmt.Print(p.value, " <-> ")
	}
	fmt.Println("nil")
}

func dlistToString(list *DList) string {
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