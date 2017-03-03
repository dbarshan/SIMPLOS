# Design of a basic 32 bit Operating System

####      Objective of the Project

 > The project is aimed to implement a mini operating system in protected mode
 > with primitive shell. The project is also a research in the field of design 
 > of operating system.


#### Introduction
Some basic works of an operating system are:
- Creating hardware abstraction layer.
- Managing hardware devices.
- Managing applications.
- Creating an interface to the user.

#### Supporting Features included:
- Console (screen),
- Keyboard Interface,
- Floppy Drive access, and 8253 Timer chip.
- A small set of Commands which user can input.
- All applications run from kernel level and are built in functions. 

#### Aim Afterwards
- Hard-Disk file system,
- User level applications
- CD drive
- Mouse

#### Making the Test-Bed
Tools used are:
1. Host operating system – Fedora 12 ( Linux Kernel - 2.6.32.14)
2. Assembler - NASM ( Netwide Assembler)
3. Compiler - GCC GNU compiler collection)
4. Linker - LD
5. Virtual Machine - Linux Virtual Machine Manager (qemu backend)
6. An actual physical PC for testing (Intel x86 architecture)

#### Approach for Implementation
- Creating a boot-able code and writing in floppy's first sector :
This is loaded by the BIOS just when the machine starts. So this will cause the operating system to boot up.
- Creating 2nd stage loader and switching to 32 bit mode
The 1st stage loader has some limitations and as we are building a 32 bit OS, we need a second stage loader.
- Loading kernel and giving control to kernel.
Second stage loader calls the SIMPLOS kernel. Now kernel has the whole power and control over the machine.
- Creating a video driver.
The very first work of the kernel is showing some text to screen to signal that SIMLOS has got life. So a video driver is needed which will manage the task of displaying any text on screen.
- Setting up Global Descriptor Table.
This is needed according to Intel's specification. To create a 32 bit OS , we need to set up an GDT and have to give the address of GDT to the processor.
 - Setting up Interrupt Descriptor Table and writing ISRs
Interrupt handling. A 32 bit must have ISRs for servicing interrupts and Intel specifies to create the IDT for storing address of ISRs.
 - Initializing the PIC chip and writing IRQ routines.
Hardware interrupts are reprogrammed through the 8259 Programmable Interrupt Controller chip and IRQ routines are needed for them.
 - Initializing PIT chip and starting counter.
A timer chip can manipulate the system delay functions and can help processor in waiting for slower devices (like floppy disk drive).
- Creating a floppy disk driver.
Floppy sector reading and writing routines are needed.
- Creating basic file operations.
Having the floppy read- write facility in hand we can specify a File Access Table to access files. Till now SIPLOS supports only file reading . This files are inserted from host operating system. Writing files is not supported yet, though we shall write a driver for that very soon.

Some screenshots of SIMPLOS running in a 32 bit machine:
[![N|Solid](https://cloud.githubusercontent.com/assets/25990759/23568048/e20123d2-007e-11e7-9d4a-b4e1fff652d2.png)

[![N|Solid](https://cloud.githubusercontent.com/assets/25990759/23568092/1a55d4d0-007f-11e7-8642-9ad60f8369de.png)