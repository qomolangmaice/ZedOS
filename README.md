###ZedOS:

ZedOS is a hobby built operating system, as you know, developing an OS is not an easy stuff, 
you may need to know a little bit of Assembly programming and C programming.

Before you build your own operating system, you need to install some softwares(In Ubuntu or Debian) as follows: 
###
 $ sudo apt-get install xorriso gcc nasm qemu grub virtualbox

If you want to compile this os and run it, here is the steps:

1. Compile and link all files. 
###
 $ make 

2. Compile, link and run kernel.bin(in bochs).
###
 $ make bochs-run 

3. Compile, link and run kernel.bin(in qemu).
###
 $ make bin_run 

4. Compile, link and run ZedOS.iso(in qemu).
### 
 $ make iso_run 

5. If you want generate the new ZedOS.iso, you should delete all object files and ZedOS.iso at first.  
### 
 $ make clean 

6. If you want to try this os in a real machine, you can burn ZedOS.iso into an U-disk. 
### 
 $ sudo dd if=Zed.iso of=/dev/sdb1 bs=2M (depends on your device conditions)
 
 Reboot your system and choose U-disk boot.
