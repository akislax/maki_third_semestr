package main

import "fmt"

type SNode struct {
	value string
	next  *SNode
}

type Stack struct {
	head *SNode
}

func stackIsEmpty(st *Stack) bool {
	return st.head == nil
}

func stackPush(st *Stack, value string) {
	st.head = &SNode{value, st.head}
}

func stackTop(st *Stack) string {
	if stackIsEmpty(st) {
		return ""
	}
	return st.head.value
}

func stackPop(st *Stack) {
	if stackIsEmpty(st) {
		return
	}
	st.head = st.head.next
}

func stackPrint(st *Stack) {
	for p := st.head; p != nil; p = p.next {
		fmt.Print(p.value, " -> ")
	}
	fmt.Println("nil")
}

func stackToString(st *Stack) string {
	result := ""
	for current := st.head; current != nil; current = current.next {
		if result == "" {
			result = current.value
		} else {
			result = current.value + " " + result
		}
	}
	return result
}