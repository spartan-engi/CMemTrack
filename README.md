# CMemTrack

A simple memory tracker written in C, based on a doubly linked list

is it...  
Efficient? no  
Safe? not really  
Working? yes  
Readable? sure do hope so.

## Usage

include mem.h, it has all functions and descriptions you need  
compile mem.c with your code  
enjoy... just don't forget to initialize

> [!TIP]
> main.c is an example and testbed for the functions  
> use the makefile to quickly compile the example to see things working

> [!WARNING]
> pointers acquired via CMemTrack can't be freed with stdlib free(). things **will** crash

## Working Principle

This tracker works on the principle of creating a doubly linked list with all the alocations requested,
 by reserving extra space on the begining for information about the allocation and two pointers.
