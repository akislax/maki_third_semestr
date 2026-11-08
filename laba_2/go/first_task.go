package main

import "fmt"

type SNode[T any] struct {
	a    T
	next *SNode[T]
}

type Stack[T any] struct {
	head *SNode[T]
}

func isEmptyStack[T any](stack *Stack[T]) bool {
	return stack.head == nil
}

func push[T any](stack *Stack[T], a T) {
	newNode := &SNode[T]{a, stack.head}
	stack.head = newNode
}

func pop[T any](stack *Stack[T]) {
	if isEmptyStack(stack) {
		return
	}
	stack.head = stack.head.next
}

func top[T any](stack *Stack[T]) T {
	var empty T
	if isEmptyStack(stack) {
		return empty
	}
	return stack.head.a
}

func freeStack[T any](stack *Stack[T]) {
	for !isEmptyStack(stack) {
		pop(stack)
	}
}

func printStack[T any](stack *Stack[T]) {
	current := stack.head
	for current != nil {
		fmt.Print(current.a, " -> ")
		current = current.next
	}
	fmt.Println("nil")
}

func priority(op byte) int {
	if op == '!' {
		return 3
	}
	if op == '&' {
		return 2
	}
	if op == '|' || op == '^' {
		return 1
	}
	return 0
}

func operation(stack *Stack[int], op byte) {
	if op == '!' {
		a := top(stack)
		pop(stack)
		if a == 1 {
			push(stack, 0)
		} else {
			push(stack, 1)
		}
		return
	}

	b := top(stack)
	pop(stack)
	a := top(stack)
	pop(stack)

	result := 0
	if op == '&' {
		result = a & b
	}
	if op == '|' {
		result = a | b
	}
	if op == '^' {
		result = a ^ b
	}

	push(stack, result)
}

func schet(primer string) int {
	var nums Stack[int]
	var ops Stack[byte]

	for i := 0; i < len(primer); i++ {
		c := primer[i]
		if c == ' ' {
			continue
		}

		if c == '0' || c == '1' {
			push(&nums, int(c-'0'))
		} else if c == '(' || c == '!' {
			push(&ops, c)
		} else if c == ')' {
			for top(&ops) != '(' {
				operation(&nums, top(&ops))
				pop(&ops)
			}
			pop(&ops)
		} else if c == '&' || c == '|' || c == '^' {
			for !isEmptyStack(&ops) && top(&ops) != '(' &&
				priority(top(&ops)) >= priority(c) {
				operation(&nums, top(&ops))
				pop(&ops)
			}
			push(&ops, c)
		}
	}

	for !isEmptyStack(&ops) {
		operation(&nums, top(&ops))
		pop(&ops)
	}

	result := top(&nums)
	freeStack(&nums)
	return result
}
