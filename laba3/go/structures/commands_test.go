package structures

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// ToCommand: каждое слово превращается в свою команду
func TestCommandsAllKnownCommands(t *testing.T) {
	all := []struct {
		text    string
		command Command
	}{
		{"MPUSH", MPUSH}, {"MINSERT", MINSERT}, {"MGET", MGET}, {"MSET", MSET},
		{"MDEL", MDEL}, {"MLEN", MLEN}, {"MPRINT", MPRINT},
		{"FPUSHH", FPUSHH}, {"FPUSHT", FPUSHT}, {"FPUSHA", FPUSHA}, {"FPUSHB", FPUSHB},
		{"FDELH", FDELH}, {"FDELT", FDELT}, {"FDELA", FDELA}, {"FDELB", FDELB},
		{"FDEL", FDEL}, {"FFIND", FFIND}, {"FPRINT", FPRINT}, {"FPRINTR", FPRINTR},
		{"LPUSHH", LPUSHH}, {"LPUSHT", LPUSHT}, {"LPUSHA", LPUSHA}, {"LPUSHB", LPUSHB},
		{"LDELH", LDELH}, {"LDELT", LDELT}, {"LDELA", LDELA}, {"LDELB", LDELB},
		{"LDEL", LDEL}, {"LFIND", LFIND}, {"LPRINT", LPRINT}, {"LPRINTR", LPRINTR},
		{"SPUSH", SPUSH}, {"SPOP", SPOP}, {"SPRINT", SPRINT},
		{"QPUSH", QPUSH}, {"QPOP", QPOP}, {"QPRINT", QPRINT},
		{"TINSERT", TINSERT}, {"TFIND", TFIND}, {"TCOMPLETE", TCOMPLETE}, {"TPRINT", TPRINT},
		{"HADD", HADD}, {"HDEL", HDEL}, {"HFIND", HFIND}, {"HPRINT", HPRINT},
		{"KADD", KADD}, {"KDEL", KDEL}, {"KFIND", KFIND}, {"KPRINT", KPRINT},
		{"PRINT", PRINT},
	}

	for _, c := range all {
		assert.Equal(t, c.command, ToCommand(c.text), c.text)
	}
}

func TestCommandsUnknownCommand(t *testing.T) {
	assert.Equal(t, UNKNOWN, ToCommand("HELLO"))
	assert.Equal(t, UNKNOWN, ToCommand(""))
}

// WrongType
func TestCommandsWrongTypeWhenNoStructure(t *testing.T) {
	db := DB{}
	assert.False(t, WrongType(db, "x", "array")) // структуры нет - можно создавать
}

func TestCommandsWrongTypeWhenSameType(t *testing.T) {
	db := DB{"x": {Type: "array"}}
	assert.False(t, WrongType(db, "x", "array"))
}

func TestCommandsWrongTypeWhenOtherType(t *testing.T) {
	db := DB{"x": {Type: "stack"}}

	var result bool
	out := captureOutput(func() { result = WrongType(db, "x", "array") })

	assert.True(t, result)
	assert.Equal(t, "x это не array\n", out)
}
