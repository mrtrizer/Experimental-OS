TARGET:=boot.elf

CPPFLAGS:=-nostdinc -I./

$(TARGET): boot.o main.o io.o
	$(LD) -Ttext 0x100000 -o $@ $^
	nm $(TARGET) | sort > System.map
it.o: io.c

main.o: main.c

boot.o: boot.s

boot.s: boot.S

.PHONY: clean bochs
clean:
	rm -f *.o *.s *.log System.map $(TARGET)
bochs:
	cp -f bochsrc .bochsrc
