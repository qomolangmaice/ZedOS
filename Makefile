
S_SOURCES = $(wildcard boot/*.s) 
S_OBJECTS = $(S_SOURCES:.s=.o) 

C_SOURCES = $(wildcard kernel/*.c cpu/*.c drivers/*.c libc/*.c mm/*.c fs/*.c) 
C_OBJECTS = $(C_SOURCES:.c=.o) 

HEADERS = $(wildcard cpu/*.h drivers/*.h libc/*.h mm/*.h fs/*.h) 

OBJECTS = $(S_OBJECTS) $(C_OBJECTS)    

CC = /usr/bin/gcc 
CFLAGS = -m32 -c  

LDFLAGS = -m elf_i386 -T link.ld -o   

AS = /usr/bin/nasm 
ASFLAGS = -f elf32  

all: kernel.bin  

%.o: %.c ${HEADERS}     
	${CC} ${CFLAGS} $< -o $@ -ffreestanding 

%.o: %.s    
	${AS} ${ASFLAGS} $< -o $@   

all: kernel.bin  

kernel.bin: $(OBJECTS) 
	ld $(LDFLAGS) kernel.bin $(OBJECTS)   

ZedOS.iso: kernel.bin 
	cp kernel.bin ZedOS/boot/kernel.bin   
	grub-mkrescue -o ZedOS.iso ZedOS/  

bochs-run: kernel.bin  
	sudo mount floppy.img /mnt/floppy
	sudo cp kernel.bin /mnt/floppy 
	sleep 1 
	sudo umount /mnt/floppy
	bochs -f bochsrc  

# For Callgraph generation 
include Makefile.callgraph 

# There is always some bugs by this method 
qemu_iso_run: ZedOS.iso  
	qemu-system-i386 ZedOS.iso    

# Bugs too 
qemu_bin_run: kernel.bin  
	qemu-system-i386 -kernel kernel.bin     

clean:
#rm -rf $(OBJECTS) kernel.bin ZedOS.iso   
	rm -rf $(OBJECTS) kernel.bin ZedOS.iso *.svg  
