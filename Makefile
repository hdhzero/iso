TOOLCHAIN=/home/aluno/Downloads/toolchain
NASM="$(TOOLCHAIN)/nasm/bin/nasm"
GCC="$(TOOLCHAIN)/bin/i686-elf-gcc"
LD="$(TOOLCHAIN)/bin/i686-elf-ld"
MKISOFS="$(TOOLCHAIN)/mkisofs"
BOCHS="$(TOOLCHAIN)/bochs/bin/bochs"
SRC=kernel.c common.c TerminalState.c monitor.c isr.c \
    descriptors.c

bootcd:
	rm -rf tmpbootcd
	$(NASM) -f elf -o boot.o boot.s
	$(NASM) -f elf -o boot2.o boot2.s
	$(NASM) -f elf -o x86_asm.o x86_asm.s
	$(GCC) -c $(SRC)
	$(LD) -T bootcd.ld *.o
	mkdir tmpbootcd
	cp kernel.img tmpbootcd
	$(MKISOFS) -o kernel.iso -V BootTest -b kernel.img tmpbootcd

run:
	$(BOCHS) -f bochsrc

clean:
	rm -rf *.o tmpbootcd *.img *.iso
