package main

import "fmt"

type QNode struct {
	value string
	next  *QNode
}

type Queue struct {
	head *QNode
	tail *QNode
}

func queueIsEmpty(q *Queue) bool {
	return q.head == nil
}

func queuePush(q *Queue, value string) {
	newNode := &QNode{value, nil}
	if queueIsEmpty(q) {
		q.head = newNode
		q.tail = newNode
		return
	}
	q.tail.next = newNode
	q.tail = newNode
}

func queueTop(q *Queue) string {
	if queueIsEmpty(q) {
		return ""
	}
	return q.head.value
}

func queuePop(q *Queue) {
	if queueIsEmpty(q) {
		return
	}
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
}

func queuePrint(q *Queue) {
	for p := q.head; p != nil; p = p.next {
		fmt.Print(p.value, " -> ")
	}
	fmt.Println("nil")
}

func queueToString(q *Queue) string {
	result := ""
	for current := q.head; current != nil; current = current.next {
		if result == "" {
			result = current.value
		} else {
			result = result + " " + current.value
		}
	}
	return result
}
