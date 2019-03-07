This is a project of my university years. I experimented with different OS that time and was interesetd in details of system work.

As result, there are two projects written in garbage C++ and C one is for reading FAT16 and FAT32, another is implementation of multiboot loader working along with grub to load kernel. I've stucked on entering to safe mode. May be I'll back to this some time in future )

Basic disk image with configured grub is provided in MultibooLoader folder

To start system with qemu

```
qemu-system-x86_64 -drive -if=floppy,file=bootgrub.img,format=raw
```
