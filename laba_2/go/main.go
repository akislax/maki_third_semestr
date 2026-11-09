package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type OptionForMenu int

const (
	Exit        OptionForMenu = 0
	FirstTask   OptionForMenu = 1
	SecondTask  OptionForMenu = 2
	ThirdTask   OptionForMenu = 3
	FourthTask  OptionForMenu = 4
	FiveTask    OptionForMenu = 5
	SixOneTask  OptionForMenu = 6
	SixTwoTask  OptionForMenu = 7
	CompareTask OptionForMenu = 8
	SevenTask   OptionForMenu = 9
)

var in = bufio.NewReader(os.Stdin)
var words []string

func readLine() (string, bool) {
	words = nil
	line, err := in.ReadString('\n')
	if err != nil && line == "" {
		return "", false
	}
	return strings.TrimRight(line, "\r\n"), true
}

func readWord() string {
	for len(words) == 0 {
		line, err := in.ReadString('\n')
		words = strings.Fields(line)
		if err != nil && len(words) == 0 {
			return ""
		}
	}
	w := words[0]
	words = words[1:]
	return w
}

func readInt() (int, bool) {
	x, err := strconv.Atoi(readWord())
	return x, err == nil
}

func main() {
	for {
		fmt.Print("\n1 - Стек\n2 - АТД Множество\n3 - Множество\n4 - Массив\n5 - Хаффман\n" +
			"6 - Хеш-таблица (подзадание 6.1)\n7 - Хеш-таблица (подзадание 6.2)\n" +
			"8 - Сравнение кукушки и цепочек\n9 - Кэш\n0 - Выход\n> ")

		line, ok := readLine()
		if !ok {
			return
		}
		input, err := strconv.Atoi(strings.TrimSpace(line))
		if err != nil {
			input = -1
		}
		choice := OptionForMenu(input)

		switch choice {
		case FirstTask:
			fmt.Print("Введите выражение: ")
			primer, _ := readLine()
			fmt.Println(schet(primer))

		case SecondTask:
			const fileName = "data.json"

			fmt.Print("Введите команду: ")
			query, _ := readLine()

			fields := strings.Fields(query)
			cmd, name, value := "", "", ""
			if len(fields) > 0 {
				cmd = fields[0]
			}
			if len(fields) > 1 {
				name = fields[1]
			}
			if len(fields) > 2 {
				value = fields[2]
			}

			db := map[string]json.RawMessage{}
			data, err := os.ReadFile(fileName)
			if err == nil && len(data) > 0 {
				if json.Unmarshal(data, &db) != nil {
					fmt.Println("Файл", fileName, "испорчен")
					break
				}
			}

			var set hashTable[string]
			if raw, found := db[name]; found {
				var list []string
				json.Unmarshal(raw, &list)
				set.loadFromJson(list)
			}

			if cmd == "SETADD" {
				set.add(value)
				fmt.Println("Добавлено")
			} else if cmd == "SETDEL" {
				set.remove(value)
				fmt.Println("Удалено")
			} else if cmd == "SET_AT" {
				if set.search(value) {
					fmt.Println("TRUE")
				} else {
					fmt.Println("FALSE")
				}
			} else {
				fmt.Println("Неизвестная команда")
				break
			}

			raw, _ := json.Marshal(set.saveToJson())
			db[name] = raw
			out, _ := json.MarshalIndent(db, "", "    ")
			os.WriteFile(fileName, append(out, '\n'), 0644)

		case ThirdTask:
			fmt.Print("Введите числа через пробел: ")
			line, _ := readLine()

			var set hashTable[int]
			for _, w := range strings.Fields(line) {
				x, err := strconv.Atoi(w)
				if err != nil {
					break
				}
				set.add(x)
			}

			similarSubsets(&set)

		case FourthTask:
			fmt.Print("Введите числа через пробел: ")
			line, _ := readLine()

			var arr Array[int]
			initArray(&arr)
			for _, w := range strings.Fields(line) {
				x, err := strconv.Atoi(w)
				if err != nil {
					break
				}
				arrayPushBack(&arr, x)
			}

			fmt.Print("Введите ваше число: ")
			numLine, _ := readLine()
			userInt, err := strconv.Atoi(strings.TrimSpace(numLine))
			if err != nil {
				fmt.Println("Нужно целое число")
				break
			}

			goodLine(&arr, userInt)

		case FiveTask:
			fmt.Print("Введите строку: ")
			text, _ := readLine()

			if text == "" {
				fmt.Println("Пустая строка")
				break
			}

			var h HuffTree
			h.buildTree(text)

			bits := h.encode(text)
			fmt.Println("Код:", bits)

			fmt.Println("Таблица:")
			h.printTable()

			fmt.Println("Обратно:", h.decode(bits))

		case SixOneTask:
			fmt.Print("1 - Метод цепочек\n2 - Хеширование кукушки\n> ")
			typeLine, _ := readLine()
			tableType, _ := strconv.Atoi(strings.TrimSpace(typeLine))

			if tableType != 1 && tableType != 2 {
				fmt.Println("Нет такой таблицы")
				break
			}

			var chain hashTableSix
			cuckoo := newKukushka()

			fmt.Println("Команды: ADD x, DEL x, FIND x, EXIT")

			for {
				fmt.Print("> ")
				cmdLine, ok := readLine()
				if !ok {
					break
				}

				fields := strings.Fields(cmdLine)
				cmd := ""
				if len(fields) > 0 {
					cmd = fields[0]
				}

				if cmd == "EXIT" {
					break
				}

				if cmd != "ADD" && cmd != "DEL" && cmd != "FIND" {
					fmt.Println("Неизвестная команда")
					continue
				}

				if len(fields) < 2 {
					fmt.Println("Нужно целое число")
					continue
				}
				x, err := strconv.Atoi(fields[1])
				if err != nil {
					fmt.Println("Нужно целое число")
					continue
				}

				if cmd == "ADD" {
					if tableType == 1 {
						chain.add(x)
					} else {
						cuckoo.insert(x)
					}
					fmt.Println("Добавлено")
				} else if cmd == "DEL" {
					if tableType == 1 {
						chain.remove(x)
					} else {
						cuckoo.remove(x)
					}
					fmt.Println("Удалено")
				} else {
					found := false
					if tableType == 1 {
						found = chain.search(x)
					} else {
						found = cuckoo.search(x)
					}
					if found {
						fmt.Println("есть")
					} else {
						fmt.Println("нет")
					}
				}
			}

		case SixTwoTask:
			fmt.Print("Введите число: ")
			numLine, _ := readLine()
			x, err := strconv.Atoi(strings.TrimSpace(numLine))
			if err != nil {
				fmt.Println("Нужно целое число")
				break
			}
			printFold(x)

		case CompareTask:
			compareHash()

		case SevenTask:
			fmt.Print("Ёмкость кэша: ")
			capacity, ok1 := readInt()

			fmt.Print("Количество запросов: ")
			q, ok2 := readInt()

			if !ok1 || !ok2 || capacity <= 0 || q < 0 {
				fmt.Println("Нужны положительные целые числа")
				words = nil
				break
			}

			cache := newLRUCache(capacity)

			fmt.Print("Запросы: ")
			for i := 0; i < q; i++ {
				cmd := readWord()

				if cmd == "SET" {
					x, _ := readInt()
					y, _ := readInt()
					cache.set(x, y)
				} else if cmd == "GET" {
					x, _ := readInt()
					fmt.Print(cache.get(x), " ")
				}
			}
			fmt.Println()
			words = nil

		case Exit:
			return

		default:
			fmt.Println("Нет такого пункта меню")
		}
	}
}
