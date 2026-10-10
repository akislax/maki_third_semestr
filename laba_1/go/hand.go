package main

import (
	"encoding/json"
	"fmt"
	"strconv"
)

const treeOnlyCpp = "дерево реализовано только в C++ версии"

type Entry struct {
	Data json.RawMessage `json:"data"`
	Type string          `json:"type"`
}

type Database map[string]*Entry

func need(args []string, n int) bool {
	if len(args) < n {
		fmt.Println("не хватает аргументов")
		return false
	}
	return true
}

func toIndex(s string) (int, bool) {
	index, err := strconv.Atoi(s)
	if err != nil {
		fmt.Println("индекс должен быть числом")
		return 0, false
	}
	return index, true
}

func wrongType(db Database, name string, structType string) bool {
	entry, ok := db[name]
	if ok && entry.Type != structType {
		fmt.Println(name, "это не", structType)
		return true
	}
	return false
}

func loadItems(db Database, name string) []string {
	items := []string{}
	entry, ok := db[name]
	if ok {
		_ = json.Unmarshal(entry.Data, &items)
	}
	return items
}

func saveItems(db Database, name string, structType string, items []string) {
	data, _ := json.Marshal(items)
	db[name] = &Entry{Data: data, Type: structType}
}

func handleArray(command Command, db Database, name string, args []string) {
	if wrongType(db, name, "array") {
		return
	}

	var arr Array
	initArray(&arr)
	for _, element := range loadItems(db, name) {
		arrayPushBack(&arr, element)
	}

	switch command {
	case MPUSH:
		if need(args, 1) {
			arrayPushBack(&arr, args[0])
			fmt.Println("->", args[0])
		}
	case MINSERT:
		if need(args, 2) {
			index, ok := toIndex(args[0])
			if ok {
				arrayInsertAt(&arr, index, args[1])
			}
		}
	case MGET:
		if need(args, 1) {
			index, ok := toIndex(args[0])
			if ok {
				value, found := arrayGetAt(&arr, index)
				if found {
					fmt.Println("->", value)
				}
			}
		}
	case MSET:
		if need(args, 2) {
			index, ok := toIndex(args[0])
			if ok {
				arraySetAt(&arr, index, args[1])
			}
		}
	case MDEL:
		if need(args, 1) {
			index, ok := toIndex(args[0])
			if ok {
				arrayRemoveAt(&arr, index)
			}
		}
	case MLEN:
		fmt.Println("->", arrayLength(&arr))
	case MPRINT:
		arrayPrint(&arr)
	}

	items := []string{}
	for i := 0; i < arr.size; i++ {
		items = append(items, arr.data[i])
	}
	saveItems(db, name, "array", items)
}

func handleFlist(command Command, db Database, name string, args []string) {
	if wrongType(db, name, "flist") {
		return
	}

	var fl FList
	for _, element := range loadItems(db, name) {
		flistPushBack(&fl, element)
	}

	switch command {
	case FPUSHH:
		if need(args, 1) {
			flistPushFront(&fl, args[0])
		}
	case FPUSHT:
		if need(args, 1) {
			flistPushBack(&fl, args[0])
		}
	case FPUSHA:
		if need(args, 2) {
			node := flistFind(&fl, args[0])
			if node == nil {
				fmt.Println("нет такого элемента")
			} else {
				flistPushAfter(node, args[1])
			}
		}
	case FPUSHB:
		if need(args, 2) {
			node := flistFind(&fl, args[0])
			if node == nil {
				fmt.Println("нет такого элемента")
			} else {
				flistPushBefore(&fl, node, args[1])
			}
		}
	case FDELH:
		flistPopFront(&fl)
	case FDELT:
		flistPopBack(&fl)
	case FDELA:
		if need(args, 1) {
			node := flistFind(&fl, args[0])
			if node == nil {
				fmt.Println("нет такого элемента")
			} else {
				flistPopAfter(node)
			}
		}
	case FDELB:
		if need(args, 1) {
			node := flistFind(&fl, args[0])
			if node == nil {
				fmt.Println("нет такого элемента")
			} else {
				flistPopBefore(&fl, node)
			}
		}
	case FDEL:
		if need(args, 1) {
			flistDeleteValue(&fl, args[0])
		}
	case FFIND:
		if need(args, 1) {
			if flistFind(&fl, args[0]) != nil {
				fmt.Println("-> TRUE")
			} else {
				fmt.Println("-> FALSE")
			}
		}
	case FPRINT:
		flistPrint(&fl)
	case FPRINTR:
		flistPrintReverse(fl.head)
		fmt.Println()
	}

	items := []string{}
	for current := fl.head; current != nil; current = current.next {
		items = append(items, current.value)
	}
	saveItems(db, name, "flist", items)
}

func handleDlist(command Command, db Database, name string, args []string) {
	if wrongType(db, name, "dlist") {
		return
	}

	var dl DList
	for _, element := range loadItems(db, name) {
		dlistPushBack(&dl, element)
	}

	switch command {
	case LPUSHH:
		if need(args, 1) {
			dlistPushFront(&dl, args[0])
		}
	case LPUSHT:
		if need(args, 1) {
			dlistPushBack(&dl, args[0])
		}
	case LPUSHA:
		if need(args, 2) {
			node := dlistFind(&dl, args[0])
			if node == nil {
				fmt.Println("нет такого элемента")
			} else {
				dlistPushAfter(&dl, node, args[1])
			}
		}
	case LPUSHB:
		if need(args, 2) {
			node := dlistFind(&dl, args[0])
			if node == nil {
				fmt.Println("нет такого элемента")
			} else {
				dlistPushBefore(&dl, node, args[1])
			}
		}
	case LDELH:
		dlistPopFront(&dl)
	case LDELT:
		dlistPopBack(&dl)
	case LDELA:
		if need(args, 1) {
			node := dlistFind(&dl, args[0])
			if node == nil {
				fmt.Println("нет такого элемента")
			} else {
				dlistPopAfter(&dl, node)
			}
		}
	case LDELB:
		if need(args, 1) {
			node := dlistFind(&dl, args[0])
			if node == nil {
				fmt.Println("нет такого элемента")
			} else {
				dlistPopBefore(&dl, node)
			}
		}
	case LDEL:
		if need(args, 1) {
			dlistDeleteValue(&dl, args[0])
		}
	case LFIND:
		if need(args, 1) {
			if dlistFind(&dl, args[0]) != nil {
				fmt.Println("-> TRUE")
			} else {
				fmt.Println("-> FALSE")
			}
		}
	case LPRINT:
		dlistPrintForward(&dl)
	case LPRINTR:
		dlistPrintBackward(&dl)
	}

	items := []string{}
	for current := dl.head; current != nil; current = current.next {
		items = append(items, current.value)
	}
	saveItems(db, name, "dlist", items)
}

func handleStack(command Command, db Database, name string, args []string) {
	if wrongType(db, name, "stack") {
		return
	}

	var st Stack
	for _, element := range loadItems(db, name) {
		stackPush(&st, element)
	}

	switch command {
	case SPUSH:
		if need(args, 1) {
			stackPush(&st, args[0])
			fmt.Println("->", args[0])
		}
	case SPOP:
		if stackIsEmpty(&st) {
			fmt.Println("-> стек пуст")
		} else {
			fmt.Println("->", stackTop(&st))
			stackPop(&st)
		}
	case SPRINT:
		stackPrint(&st)
	}

	items := []string{}
	for current := st.head; current != nil; current = current.next {
		items = append([]string{current.value}, items...)
	}
	saveItems(db, name, "stack", items)
}

func handleQueue(command Command, db Database, name string, args []string) {
	if wrongType(db, name, "queue") {
		return
	}

	var q Queue
	for _, element := range loadItems(db, name) {
		queuePush(&q, element)
	}

	switch command {
	case QPUSH:
		if need(args, 1) {
			queuePush(&q, args[0])
			fmt.Println("->", args[0])
		}
	case QPOP:
		if queueIsEmpty(&q) {
			fmt.Println("-> очередь пуста")
		} else {
			fmt.Println("->", queueTop(&q))
			queuePop(&q)
		}
	case QPRINT:
		queuePrint(&q)
	}

	items := []string{}
	for current := q.head; current != nil; current = current.next {
		items = append(items, current.value)
	}
	saveItems(db, name, "queue", items)
}

func handlePrint(db Database, name string) {
	entry, ok := db[name]
	if !ok {
		fmt.Println("нет структуры", name)
		return
	}

	switch entry.Type {
	case "array":
		handleArray(MPRINT, db, name, nil)
	case "flist":
		handleFlist(FPRINT, db, name, nil)
	case "dlist":
		handleDlist(LPRINT, db, name, nil)
	case "stack":
		handleStack(SPRINT, db, name, nil)
	case "queue":
		handleQueue(QPRINT, db, name, nil)
	case "tree":
		fmt.Println(treeOnlyCpp)
	}
}