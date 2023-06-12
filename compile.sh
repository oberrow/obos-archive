echo Assembling "kernel/kernel_bootstrap-i686.S"
i686-elf-as kernel/kernel_bootstrap-i686.S -o int/kernel_bootstrap.o
echo Compiling "kernel/kmain.c"
i686-elf-gcc kernel/kmain.c -o int/kmain.o -c  -O2 -Wall -Werror -Wno-error=misleading-indentation -Wno-builtin-declaration-mismatch
echo Compiling "kernel/terminal.c"
i686-elf-gcc kernel/terminal.c -o int/terminal.o -c -O2 -Wall -Werror -Wno-error=misleading-indentation -Wno-builtin-declaration-mismatch
echo Compiling "kernel/acpi.c"
i686-elf-gcc kernel/acpi.c -o int/acpi.o -c -O2 -Wall -Werror -Wno-error=misleading-indentation -Wno-builtin-declaration-mismatch
echo Compiling "kernel/kalloc.c"
i686-elf-gcc kernel/kalloc.c -o int/kalloc.o -c -O2 -Wall -Werror -Wno-error=misleading-indentation -Wno-builtin-declaration-mismatch
echo Compiling "kernel/inline-asm.c"
i686-elf-gcc kernel/inline-asm.c -o int/inline-asm.o -c -O2 -Wall -Werror -Wno-error=misleading-indentation -Wno-builtin-declaration-mismatch
echo Compiling "kernel/kassert.c"
i686-elf-gcc kernel/kassert.c -o int/kassert.o -c -O2 -Wall -Werror -Wno-error=misleading-indentation -Wno-builtin-declaration-mismatch
echo Linking object files...
i686-elf-gcc -T int/linker.ld int/kernel_bootstrap.o int/kmain.o int/terminal.o int/acpi.o int/inline-asm.o int/kassert.o -o bin/oboskrnl.out -ffreestanding -O2 -nostdlib
if grub-file --is-x86-multiboot bin/oboskrnl.out; then
    echo Found multiboot header in oboskrnl.out
else
    echo Something is wrong. "oboskrnl.out," has no multiboot header.
    rm int/*.o
    rm bin/*.out
    exit
fi