i686-elf-as kernel/kernel_bootstrap.S -o int/kernel_bootstrap.o
i686-elf-gcc kernel/kmain.c -o int/kmain.o -c
i686-elf-ld int/kernel_bootstrap.o int/kmain.o -o bin/oboskrnl.out