default:
	gcc src/* -o build/programm.elf -g

release:
	gcc src/* -o build/programm.elf -O3

