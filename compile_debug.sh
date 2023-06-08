echo Assembling "kernel/kernel_bootstrap.S"
i686-elf-as kernel/kernel_bootstrap.S -o int/kernel_bootstrap.o
echo Compiling "kernel/kmain.c"
i686-elf-gcc kernel/kmain.c -o int/kmain.o -c -O0 -ggdb -Wall -Werror -Wno-error=misleading-indentation
echo Compiling "kernel/terminal.c"
i686-elf-gcc kernel/terminal.c -o int/terminal.o -c -O0 -ggdb -Wall -Werror -Wno-error=misleading-indentation
echo Compiling "kernel/acpi.c"
i686-elf-gcc kernel/acpi.c -o int/acpi.o -c -O0 -ggdb -Wall -Werror -Wno-error=misleading-indentation
echo Linking object files...
i686-elf-gcc -T linker.ld int/kernel_bootstrap.o int/kmain.o int/terminal.o int/acpi.o -o bin/oboskrnl.out -ffreestanding -O0 -nostdlib -ggdb
if grub-file --is-x86-multiboot bin/oboskrnl.out; then
    echo Found multiboot header in oboskrnl.out
else
    echo Something is wrong. "oboskrnl.out," has no multiboot header.
    rm int/*
    rm bin/*
    exit
fi
echo Generating debug symbols...
objcopy --only-keep-debug bin/oboskrnl.out oboskrnl.sym
objcopy --strip-debug bin/oboskrnl.out