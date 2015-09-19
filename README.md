###ZedOS:

ZedOS is a hobby built operating system, as you know, developing an OS is not an easy stuff, 
you may need to know a little bit of Assembly programming and C programming.
This OS is developed in the GNU/Linux system. Ubuntu or Debian is a very good start for you.  

Development environment: 
------------------------

###Bochs
If you want to simulate and debug you new OS, Bochs is good choice for you. 
You can compile and install it as follows: 
### 
        $ wget http://sourceforge.net/projects/bochs/files/bochs/2.6.8/bochs-2.6.8.tar.gz
        $ tar xvf bochs-2.6.8.tar.gz
        $ cd bochs-2.6.8 
        $ ./configure --enable-debugger --enable-disasm 
        $ make 
        $ sudo make install 

You may use bochs before, of course you can install bochs like this: 
### 
        $ sudo apt-get install bochs 

There is no debug function and disassembly function in bochs by this method. So i recommend you compile bochs source code and install it

###Qemu
Qemu is another choice for you to simulate you new OS, but it may contains some odd problems. you call install some softwares(In Ubuntu or Debian) as follows: 
###
	 	$ sudo apt-get install xorriso gcc nasm qemu grub virtualbox


If you want to compile this os and run it, here is the 

steps:
-----
1. Compile and link all files. 
###
		$ make 

2. Compile, link and run kernel.bin(in bochs).
###
		$ make bochs-run 

3. Compile, link and run kernel.bin(in qemu).
###
		$ make qemu_bin_run 

4. Compile, link and run ZedOS.iso(in qemu).
### 
		$ make qemu_iso_run 

5. If you want generate the new ZedOS.iso, you should delete all object files and ZedOS.iso at first.  
### 
		$ make clean 

6. If you want to try this os in a real machine, you can burn ZedOS.iso into an U-disk. 
### 
		$ sudo dd if=Zed.iso of=/dev/sdb1 bs=2M (depends on your device conditions)
 
7. Reboot your system and choose U-disk boot. Have funs!  
