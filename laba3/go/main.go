package main

import (
	"encoding/json"
	"fmt"
	"os"
	"strings"

	"laba3go/structures"
)

// handlePrint печатает любую структуру по имени, смотря на ее тип.
func handlePrint(db structures.DB, name string, ss *strings.Reader) {
	entry, ok := db[name]
	if !ok {
		fmt.Println("нет структуры", name)
		return
	}

	switch entry.Type {
	case "array":
		structures.HandleArray(structures.MPRINT, db, name, ss)
	case "flist":
		structures.HandleFlist(structures.FPRINT, db, name, ss)
	case "dlist":
		structures.HandleDlist(structures.LPRINT, db, name, ss)
	case "stack":
		structures.HandleStack(structures.SPRINT, db, name, ss)
	case "queue":
		structures.HandleQueue(structures.QPRINT, db, name, ss)
	case "tree":
		structures.HandleTree(structures.TPRINT, db, name, ss)
	case "hash":
		structures.HandleHash(structures.HPRINT, db, name, ss)
	case "kukushka":
		structures.HandleKukushka(structures.KPRINT, db, name, ss)
	}
}

func main() {
	if len(os.Args) != 5 {
		fmt.Println("Использование: ./laba3go --file <файл> --query '<команда>'")
		os.Exit(1)
	}
	fileName := os.Args[2]
	query := os.Args[4]

	db := structures.DB{}
	content, err := os.ReadFile(fileName)
	if err == nil && len(content) > 0 {
		if err := json.Unmarshal(content, &db); err != nil {
			fmt.Println("ошибка:", err)
			os.Exit(1)
		}
	}

	ss := strings.NewReader(query)
	var cmd, name string
	fmt.Fscan(ss, &cmd, &name)

	command := structures.ToCommand(cmd)
	if command == structures.UNKNOWN {
		fmt.Println("неизвестная команда")
		os.Exit(1)
	}
	if name == "" {
		fmt.Println("укажи имя структуры")
		os.Exit(1)
	}

	switch cmd[0] {
	case 'M':
		structures.HandleArray(command, db, name, ss)
	case 'F':
		structures.HandleFlist(command, db, name, ss)
	case 'L':
		structures.HandleDlist(command, db, name, ss)
	case 'S':
		structures.HandleStack(command, db, name, ss)
	case 'Q':
		structures.HandleQueue(command, db, name, ss)
	case 'T':
		structures.HandleTree(command, db, name, ss)
	case 'H':
		structures.HandleHash(command, db, name, ss)
	case 'K':
		structures.HandleKukushka(command, db, name, ss)
	case 'P':
		handlePrint(db, name, ss)
	default:
		fmt.Println("неизвестная команда")
	}

	out, _ := json.MarshalIndent(db, "", "    ")
	if err := os.WriteFile(fileName, append(out, '\n'), 0644); err != nil {
		fmt.Println("ошибка:", err)
		os.Exit(1)
	}
}
