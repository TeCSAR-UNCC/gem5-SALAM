include ../../../common/Makefile

OBJS         = boot.o ../../../common/syscalls.o main.o isr.o

all: clean main.elf

main.elf: $(OBJS) $(LNK_SCRIPT) Makefile
	$(CC) $(LNK_FILE_OPT) -o $@ $(OBJS) $(LNK_OPT)

boot.o: Makefile
	$(CPP) boot.s $(CFLAGS) | $(AS) $(ASFLAGS) -o boot.o

clean:
	rm -f *.o *.elf