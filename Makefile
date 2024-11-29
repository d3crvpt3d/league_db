default:
	gcc src/* -o build/programm.elf -g

release:
	gcc src/* -o build/programm.elf -O3

gdb: default
	gdb --args ./build/programm.elf ./data/test_items.json
