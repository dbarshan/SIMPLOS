nasm -f bin -o ./bin/boot.bin ./source/boot.asm
nasm -f bin -o ./bin/STAGE2.SYS ./source/stage2.asm
nasm -f aout -o ./bin/start.o ./source/start.asm

gcc -nostdinc -fno-builtin -I./source -c -o ./bin/k_main.o ./source/k_main.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/system.o ./source/system.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/console.o ./source/console.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/gdt.o ./source/gdt.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/idt.o ./source/idt.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/pic.o ./source/pic.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/pit.o ./source/pit.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/keyboard.o ./source/keyboard.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/dma.o ./source/dma.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/floppy.o ./source/floppy.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/command.o ./source/command.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/filemanager.o ./source/filemanager.c
gcc -nostdinc -fno-builtin -I./source -c -o ./bin/calc.o ./source/calc.c





ld -T link.ld -o ./bin/KERNEL.SYS ./bin/start.o ./bin/k_main.o ./bin/system.o ./bin/console.o ./bin/gdt.o ./bin/idt.o ./bin/pic.o ./bin/pit.o ./bin/keyboard.o ./bin/dma.o ./bin/floppy.o ./bin/command.o ./bin/filemanager.o ./bin/calc.o

dd if=/dev/zero of=empty.bin bs=1440K count=1
mkdosfs -F 12 empty.bin
cat ./bin/boot.bin empty.bin|head -c 1440K > fat12.img
rm empty.bin

mount -o loop fat12.img /mnt/tmp1

cp ./bin/STAGE2.SYS /mnt/tmp1
cp ./bin/KERNEL.SYS /mnt/tmp1
cp ./source/k_main.c /mnt/tmp1/K_MAIN.C
cp ./compile.sh /mnt/tmp1/COMPILE.SH

umount /mnt/tmp1

dd if=fat12.img of=/dev/fd0
