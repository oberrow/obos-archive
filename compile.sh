echo Assembling "kernel/kernel_bootstrap.S"
i686-elf-as kernel/kernel_bootstrap.S -o int/kernel_bootstrap.o
if $? -eq 1
then
    echo Linker failed with code $?.
    exit -1
fi
echo Compiling "kernel/kmain.c"
i686-elf-gcc kernel/kmain.c -o int/kmain.o -c  -O2
if $? -eq 1
then
    echo Compiler failed with code $?.
    exit -1
fi
echo Compiling "kernel/terminal.c"
i686-elf-gcc kernel/terminal.c -o int/terminal.o -c -O2
if $? -eq 1
then
    echo Compiler failed with code $?.
    exit -1
fi
echo Linking object files...
i686-elf-gcc -T linker.ld int/kernel_bootstrap.o int/kmain.o int/terminal.o -o bin/oboskrnl.out -ffreestanding -O2 -nostdlib
if $? -eq 1
then
    echo Linker failed with code $?.
    exit -1
fi
if grub-file --is-x86-multiboot bin/oboskrnl.out; then
    echo Found multiboot header in oboskrnl.out
else
    echo Something is wrong. "oboskrnl.out," has no multiboot header.
    rm int/*
    rm bin/*
fi