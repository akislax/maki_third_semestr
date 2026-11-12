package structures

import (
	"encoding/binary"
	"io"
)

// Общие функции для бинарной сериализации.
// Число пишется как 4 байта, строка - как длина (число) и сами байты.

func writeInt(out io.Writer, value int) {
	_ = binary.Write(out, binary.LittleEndian, int32(value))
}

func readInt(in io.Reader) int {
	var value int32
	_ = binary.Read(in, binary.LittleEndian, &value)
	return int(value)
}

func writeString(out io.Writer, text string) {
	writeInt(out, len(text))
	_, _ = out.Write([]byte(text))
}

func readString(in io.Reader) string {
	length := readInt(in)
	buffer := make([]byte, length)
	_, _ = io.ReadFull(in, buffer)
	return string(buffer)
}
