editDist.out: editDist.o
	ld -m elf_i386 -o editDist.out editDist.o

editDist.o: editDist.s
	as --gstabs --32 -o editDist.o editDist.s

clean:
	rm -f *.out *.o