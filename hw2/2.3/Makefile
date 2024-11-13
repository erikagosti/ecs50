divAssembly.out: divAssembly.o
	ld -m elf_i386 -o divAssembly.out divAssembly.o

divAssembly.o: divAssembly.s
	as --gstabs --32 -o divAssembly.o divAssembly.s

clean:
	rm -f *.out *.o