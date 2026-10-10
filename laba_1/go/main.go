package main

import (
	"encoding/json"
	"fmt"
	"os"
	"strings"
)

func main() {
	if len(os.Args) != 5 {
		fmt.Println("Использование: ./dbms_go --file <файл> --query '<команда>'")
		os.Exit(1)
	}
	fileName := os.Args[2]
	query := os.Args[4]

	db := Database{}
	content, err := os.ReadFile(fileName)
	if err == nil && strings.TrimSpace(string(content)) != "" {
		if err := json.Unmarshal(content, &db); err != nil {
			fmt.Println("не удалось прочитать файл:", err)
			os.Exit(1)
		}
	}

	words := strings.Fields(query)
	if len(words) == 0 {
		fmt.Println("пустой запрос")
		os.Exit(1)
	}

	command := toCommand(words[0])
	if command == UNKNOWN {
		fmt.Println("неизвестная команда")
		os.Exit(1)
	}
	if len(words) < 2 {
		fmt.Println("укажи имя структуры")
		os.Exit(1)
	}
	name := words[1]
	args := words[2:]

	switch words[0][0] {
	case 'M':
		handleArray(command, db, name, args)
	case 'F':
		handleFlist(command, db, name, args)
	case 'L':
		handleDlist(command, db, name, args)
	case 'S':
		handleStack(command, db, name, args)
	case 'Q':
		handleQueue(command, db, name, args)
	case 'T':
		fmt.Println(treeOnlyCpp)
	case 'P':
		handlePrint(db, name)
	}

	output, _ := json.MarshalIndent(db, "", "    ")
	output = append(output, '\n')
	if err := os.WriteFile(fileName, output, 0644); err != nil {
		fmt.Println("не удалось записать файл:", err)
		os.Exit(1)
	}
}