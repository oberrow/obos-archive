echo Launching the virutal machine.
cd "/mnt/c/Users/omark/Documents/obos"; 
qemu-system-i386 -drive file=release/obos.iso,format=raw -s -m 1G -mem-prealloc