# Boot

This is a custom bootloader that strictly loads this operaing system into memory (no other OS or multiboot).

## Purpose

There will be a separate bootloader for each architecture (currently only i686).
Currently these bootloaders will have minimal tasks so that most of the work is done by the kernel.

The reason I have chosen to create my own bootloader instead of using a multiboot bootloader is simply because I want to.
