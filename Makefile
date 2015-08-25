
S_SOURCES = $(wildcard boot/*.s) 
S_OBJECTS = $(S_SOURCES:.s=.o) 

C_SOURCES = $(wildcard kernel/*.c cpu/*.c drivers/*.c libc/*.c libc/printk/*.c) 
C_OBJECTS = $(C_SOURCES:.c=.o) 

HEADERS = $(wildcard kernel/*.h cpu/*.h drivers/*.h libc/*.h /libc/printk/*.h) 

OBJECTS = $(S_OBJECTS) $(C_OBJECTS)    

CC = /usr/bin/gcc 
CFLAGS = -m32 -c   

LDFLAGS = -m elf_i386 -T link.ld -o   

AS = /usr/bin/nasm 
ASFLAGS = -f elf32       

all: kernel.bin 

kernel.bin: $(OBJECTS) 
	ld $(LDFLAGS) kernel.bin $(OBJECTS)   

ZedOS.iso: kernel.bin 
	cp kernel.bin ZedOS/boot/kernel.bin   
	grub-mkrescue -o ZedOS.iso ZedOS/  

iso_run: ZedOS.iso  
	qemu-system-i386 ZedOS.iso    

bin_run: kernel.bin  
	qemu-system-i386 -kernel kernel.bin     


%.o: %.c ${HEADERS}     
	${CC} ${CFLAGS} $< -o $@ -ffreestanding 

%.o: %.s    
	${AS} ${ASFLAGS} $< -o $@   

clean:
	rm -rf $(OBJECTS) kernel.bin ZedOS.iso   
