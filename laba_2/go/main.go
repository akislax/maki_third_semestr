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
	Exit       OptionForMenu = 0
	FirstTask  OptionForMenu = 1
	SecondTask OptionForMenu = 2
	ThirdTask  OptionForMenu = 3
	FourthTask OptionForMenu = 4
	FiveTask   OptionForMenu = 5
	SixOneTask OptionForMenu = 6
	SixTwoTask OptionForMenu = 7
	SevenTask  OptionForMenu = 8
)

var in = bufio.NewReader(os.Stdin)
var words []string

// аналог getline: читает целую строку
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

func readInt() int {
	x, _ := strconv.Atoi(readWord())
	return x
}

func main() {
	for {
		fmt.Print("\n1 - Стек\n2 - АТД Множество\n3 - Множество\n4 - Массив\n5 - Хаффман\n" +
			"6 - Хеш-таблица (подзадание 6.1)\n7 - Хеш-таблица (подзадание 6.2)\n8 - Кэш\n0 - Выход\n>")

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

			// 1. читаем файл
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
			userInt, _ := strconv.Atoi(strings.TrimSpace(numLine))

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
				x := 0
				if len(fields) > 0 {
					cmd = fields[0]
				}
				if len(fields) > 1 {
					x, _ = strconv.Atoi(fields[1])
				}

				if cmd == "EXIT" {
					break
				} else if cmd == "ADD" {
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
				} else if cmd == "FIND" {
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
				} else {
					fmt.Println("Неизвестная команда")
				}
			}

		case SixTwoTask:
			fmt.Print("Введите число: ")
			numLine, _ := readLine()
			x, _ := strconv.Atoi(strings.TrimSpace(numLine))
			printFold(x)

		case SevenTask:
			fmt.Print("Ёмкость кэша: ")
			capacity := readInt()

			fmt.Print("Количество запросов: ")
			q := readInt()

			cache := newLRUCache(capacity)

			fmt.Print("Запросы: ")
			for i := 0; i < q; i++ {
				cmd := readWord()

				if cmd == "SET" {
					x := readInt()
					y := readInt()
					cache.set(x, y)
				} else if cmd == "GET" {
					x := readInt()
					fmt.Print(cache.get(x), " ")
				}
			}
			fmt.Println()
			words = nil // как cin.ignore: остаток строки выбрасываем

		case Exit:
			return

		default:
			fmt.Println("Нет такого пункта меню")
		}
	}
}
