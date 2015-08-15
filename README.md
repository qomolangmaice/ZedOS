# Notes:

ZedOS is a hobby built operating system, as you know, developing an OS is not an easy stuff, 
you may need to know a little bit of Assembly programming and C programming.

Before you build your own system, you should install some softwares(In Ubuntu or Debian) like this: 
sudo apt-get install xorriso gcc nasm qemu grub virtualbox

# If you want to compile this os and run it, here is the steps:
1. Compile and link all files. 

 $ make 

2. Compile, link and run *.iso(in qemu). 

 $ make run 

3. If you want generate the new *.iso, you should delete all *.o and *.iso at first.  

 $ make clean 
