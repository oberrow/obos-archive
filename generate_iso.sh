echo Generating the iso \"obos.iso\"
cp bin/oboskrnl.out isodir/boot/oboskrnl.out
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o release/obos.iso isodir > /dev/null 2> /dev/null