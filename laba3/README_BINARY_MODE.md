# Режимы хранения JSON и Binary

В проекте доступны два режима хранения данных для C++ и Go.

## JSON

Старый формат оставлен без изменения. Параметр `--format` можно не указывать.

C++:
```bash
./laba3 --file db.json --query "MPUSH arr hello"
./laba3 --file db.json --query "PRINT arr"
```

Go:
```bash
cd go
make build
./laba3go --file db.json --query "MPUSH arr hello"
./laba3go --file db.json --query "PRINT arr"
```

## Binary

Для бинарного режима добавляется `--format binary`.

C++:
```bash
./laba3 --file db --format binary --query "MPUSH arr hello"
./laba3 --file db --format binary --query "PRINT arr"
```

Go:
```bash
cd go
make build
./laba3go --file db --format binary --query "MPUSH arr hello"
./laba3go --file db --format binary --query "PRINT arr"
```

Для каждой структуры создаётся отдельный файл, например `db.arr.bin`.
Файл `db.meta.json` содержит только имена структур и их типы. Он нужен для команды `PRINT`.

Бинарные данные записываются в общем формате: целые числа и длины строк занимают 4 байта в little-endian; после длины строки записываются её байты. Формат совместим между реализациями C++ и Go.

## Сборка

C++:
```bash
make
```

Для сборки C++ нужен заголовок nlohmann/json (`nlohmann-json3-dev` в Ubuntu), который уже использовался исходным проектом.

Go:
```bash
cd go
make build
```

Тесты Go требуют зависимость `github.com/stretchr/testify` из `go.mod`.
