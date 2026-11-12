package structures

import (
	"encoding/json"
	"fmt"
)

// Command - команда, которую ввел пользователь.
type Command int

const (
	MPUSH Command = iota
	MINSERT
	MGET
	MSET
	MDEL
	MLEN
	MPRINT

	FPUSHH
	FPUSHT
	FPUSHA
	FPUSHB
	FDELH
	FDELT
	FDELA
	FDELB
	FDEL
	FFIND
	FPRINT
	FPRINTR

	LPUSHH
	LPUSHT
	LPUSHA
	LPUSHB
	LDELH
	LDELT
	LDELA
	LDELB
	LDEL
	LFIND
	LPRINT
	LPRINTR

	SPUSH
	SPOP
	SPRINT

	QPUSH
	QPOP
	QPRINT

	TINSERT
	TFIND
	TCOMPLETE
	TPRINT

	HADD
	HDEL
	HFIND
	HPRINT

	KADD
	KDEL
	KFIND
	KPRINT

	PRINT
	UNKNOWN
)

// commandNames связывает текст команды с ее значением.
var commandNames = map[string]Command{
	"MPUSH": MPUSH, "MINSERT": MINSERT, "MGET": MGET, "MSET": MSET,
	"MDEL": MDEL, "MLEN": MLEN, "MPRINT": MPRINT,

	"FPUSHH": FPUSHH, "FPUSHT": FPUSHT, "FPUSHA": FPUSHA, "FPUSHB": FPUSHB,
	"FDELH": FDELH, "FDELT": FDELT, "FDELA": FDELA, "FDELB": FDELB,
	"FDEL": FDEL, "FFIND": FFIND, "FPRINT": FPRINT, "FPRINTR": FPRINTR,

	"LPUSHH": LPUSHH, "LPUSHT": LPUSHT, "LPUSHA": LPUSHA, "LPUSHB": LPUSHB,
	"LDELH": LDELH, "LDELT": LDELT, "LDELA": LDELA, "LDELB": LDELB,
	"LDEL": LDEL, "LFIND": LFIND, "LPRINT": LPRINT, "LPRINTR": LPRINTR,

	"SPUSH": SPUSH, "SPOP": SPOP, "SPRINT": SPRINT,

	"QPUSH": QPUSH, "QPOP": QPOP, "QPRINT": QPRINT,

	"TINSERT": TINSERT, "TFIND": TFIND, "TCOMPLETE": TCOMPLETE, "TPRINT": TPRINT,

	"HADD": HADD, "HDEL": HDEL, "HFIND": HFIND, "HPRINT": HPRINT,

	"KADD": KADD, "KDEL": KDEL, "KFIND": KFIND, "KPRINT": KPRINT,

	"PRINT": PRINT,
}

// ToCommand превращает текст команды в Command.
// Если такой команды нет, возвращает UNKNOWN.
func ToCommand(text string) Command {
	if command, ok := commandNames[text]; ok {
		return command
	}
	return UNKNOWN
}

// Entry - одна структура в файле базы: ее тип и данные.
type Entry struct {
	Type string          `json:"type"`
	Data json.RawMessage `json:"data"`
}

// DB - вся база: имя структуры и ее запись.
type DB map[string]*Entry

// WrongType проверяет, что под этим именем не лежит структура другого типа.
func WrongType(db DB, name, typ string) bool {
	entry, ok := db[name]
	if ok && entry.Type != typ {
		fmt.Println(name, "это не", typ)
		return true
	}
	return false
}

// loadStrings достает из базы данные структуры, которая хранит строки.
func loadStrings(db DB, name string) []string {
	var items []string
	if entry, ok := db[name]; ok {
		_ = json.Unmarshal(entry.Data, &items)
	}
	return items
}

// loadInts достает из базы данные структуры, которая хранит числа.
func loadInts(db DB, name string) []int {
	var items []int
	if entry, ok := db[name]; ok {
		_ = json.Unmarshal(entry.Data, &items)
	}
	return items
}

// saveStrings записывает структуру со строками в базу.
func saveStrings(db DB, name, typ string, items []string) {
	if items == nil {
		items = []string{}
	}
	data, _ := json.Marshal(items)
	db[name] = &Entry{Type: typ, Data: data}
}

// saveInts записывает структуру с числами в базу.
func saveInts(db DB, name, typ string, items []int) {
	if items == nil {
		items = []int{}
	}
	data, _ := json.Marshal(items)
	db[name] = &Entry{Type: typ, Data: data}
}
