echo Generating the iso \"obos.iso\"
cp bin/oboskrnl.out isodir/boot/oboskrnl.out
cp grub.cfg isodir/boot/grub/grub.cfg
cp icon.png isodir/boot/grub/obos_icon.png
rm release/obos.iso
grub-mkrescue -o release/obos.iso isodir 2> /dev/null