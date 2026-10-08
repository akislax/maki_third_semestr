package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

const treeOnlyCpp = "дерево реализовано только в C++ версии"

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

func main() {
	if len(os.Args) != 5 {
		fmt.Println("Использование: ./dbms_go --file <файл> --query '<команда>'")
		os.Exit(1)
	}
	fileName := os.Args[2]
	query := os.Args[4]

	var lines [6]string
	content, err := os.ReadFile(fileName)
	if err == nil {
		fileLines := strings.Split(string(content), "\n")
		for i := 0; i < 6 && i < len(fileLines); i++ {
			lines[i] = fileLines[i]
		}
	}

	words := strings.Fields(query)
	if len(words) == 0 {
		fmt.Println("пустой запрос")
		os.Exit(1)
	}
	cmd := words[0]
	args := words[1:]

	var arr Array
	initArray(&arr)
	for _, e := range strings.Fields(lines[0]) {
		arrayPushBack(&arr, e)
	}

	var fl FList
	for _, e := range strings.Fields(lines[1]) {
		flistPushBack(&fl, e)
	}

	var dl DList
	for _, e := range strings.Fields(lines[2]) {
		dlistPushBack(&dl, e)
	}

	var st Stack
	for _, e := range strings.Fields(lines[3]) {
		stackPush(&st, e)
	}

	var q Queue
	for _, e := range strings.Fields(lines[4]) {
		queuePush(&q, e)
	}

	switch cmd {

	// массив
	case "MPUSH":
		if !need(args, 1) {
			break
		}
		arrayPushBack(&arr, args[0])
		fmt.Println("->", args[0])
	case "MINSERT":
		if !need(args, 2) {
			break
		}
		index, ok := toIndex(args[0])
		if ok {
			arrayInsertAt(&arr, index, args[1])
		}
	case "MGET":
		if !need(args, 1) {
			break
		}
		index, ok := toIndex(args[0])
		if ok {
			value, found := arrayGetAt(&arr, index)
			if found {
				fmt.Println("->", value)
			}
		}
	case "MSET":
		if !need(args, 2) {
			break
		}
		index, ok := toIndex(args[0])
		if ok {
			arraySetAt(&arr, index, args[1])
		}
	case "MDEL":
		if !need(args, 1) {
			break
		}
		index, ok := toIndex(args[0])
		if ok {
			arrayRemoveAt(&arr, index)
		}
	case "MLEN":
		fmt.Println("->", arrayLength(&arr))
	case "MPRINT":
		arrayPrint(&arr)

	// односвязный список
	case "FPUSHH":
		if need(args, 1) {
			flistPushFront(&fl, args[0])
		}
	case "FPUSHT":
		if need(args, 1) {
			flistPushBack(&fl, args[0])
		}
	case "FPUSHA":
		if !need(args, 2) {
			break
		}
		node := flistFind(&fl, args[0])
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			flistPushAfter(node, args[1])
		}
	case "FPUSHB":
		if !need(args, 2) {
			break
		}
		node := flistFind(&fl, args[0])
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			flistPushBefore(&fl, node, args[1])
		}
	case "FDELH":
		flistPopFront(&fl)
	case "FDELT":
		flistPopBack(&fl)
	case "FDELA":
		if !need(args, 1) {
			break
		}
		node := flistFind(&fl, args[0])
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			flistPopAfter(node)
		}
	case "FDELB":
		if !need(args, 1) {
			break
		}
		node := flistFind(&fl, args[0])
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			flistPopBefore(&fl, node)
		}
	case "FDEL":
		if need(args, 1) {
			flistDeleteValue(&fl, args[0])
		}
	case "FFIND":
		if !need(args, 1) {
			break
		}
		if flistFind(&fl, args[0]) != nil {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case "FPRINT":
		flistPrint(&fl)
	case "FPRINTR":
		flistPrintReverse(fl.head)
		fmt.Println()

	// двусвязный список
	case "LPUSHH":
		if need(args, 1) {
			dlistPushFront(&dl, args[0])
		}
	case "LPUSHT":
		if need(args, 1) {
			dlistPushBack(&dl, args[0])
		}
	case "LPUSHA":
		if !need(args, 2) {
			break
		}
		node := dlistFind(&dl, args[0])
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			dlistPushAfter(&dl, node, args[1])
		}
	case "LPUSHB":
		if !need(args, 2) {
			break
		}
		node := dlistFind(&dl, args[0])
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			dlistPushBefore(&dl, node, args[1])
		}
	case "LDELH":
		dlistPopFront(&dl)
	case "LDELT":
		dlistPopBack(&dl)
	case "LDELA":
		if !need(args, 1) {
			break
		}
		node := dlistFind(&dl, args[0])
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			dlistPopAfter(&dl, node)
		}
	case "LDELB":
		if !need(args, 1) {
			break
		}
		node := dlistFind(&dl, args[0])
		if node == nil {
			fmt.Println("нет такого элемента")
		} else {
			dlistPopBefore(&dl, node)
		}
	case "LDEL":
		if need(args, 1) {
			dlistDeleteValue(&dl, args[0])
		}
	case "LFIND":
		if !need(args, 1) {
			break
		}
		if dlistFind(&dl, args[0]) != nil {
			fmt.Println("-> TRUE")
		} else {
			fmt.Println("-> FALSE")
		}
	case "LPRINT":
		dlistPrintForward(&dl)
	case "LPRINTR":
		dlistPrintBackward(&dl)

	// стек
	case "SPUSH":
		if !need(args, 1) {
			break
		}
		stackPush(&st, args[0])
		fmt.Println("->", args[0])
	case "SPOP":
		if stackIsEmpty(&st) {
			fmt.Println("-> стек пуст")
		} else {
			fmt.Println("->", stackTop(&st))
			stackPop(&st)
		}
	case "SPRINT":
		stackPrint(&st)

	// очередь
	case "QPUSH":
		if !need(args, 1) {
			break
		}
		queuePush(&q, args[0])
		fmt.Println("->", args[0])
	case "QPOP":
		if queueIsEmpty(&q) {
			fmt.Println("-> очередь пуста")
		} else {
			fmt.Println("->", queueTop(&q))
			queuePop(&q)
		}
	case "QPRINT":
		queuePrint(&q)

	// дерево
	case "TINSERT", "TFIND", "TCOMPLETE", "TPRINT":
		fmt.Println(treeOnlyCpp)

	// печать
	case "PRINT":
		if !need(args, 1) {
			break
		}
		switch args[0] {
		case "M":
			arrayPrint(&arr)
		case "F":
			flistPrint(&fl)
		case "L":
			dlistPrintForward(&dl)
		case "S":
			stackPrint(&st)
		case "Q":
			queuePrint(&q)
		case "T":
			fmt.Println(treeOnlyCpp)
		default:
			fmt.Println("укажи структуру: M, F, L, S, Q или T")
		}

	default:
		fmt.Println("неизвестная команда")
	}

	lines[0] = arrayToString(&arr)
	lines[1] = flistToString(&fl)
	lines[2] = dlistToString(&dl)
	lines[3] = stackToString(&st)
	lines[4] = queueToString(&q)

	output := strings.Join(lines[:], "\n") + "\n"
	if err := os.WriteFile(fileName, []byte(output), 0644); err != nil {
		fmt.Println("не удалось записать файл:", err)
		os.Exit(1)
	}
}
