# 1. Filter Controller Board Firmware FW315 Project

## 1.1. Overview

This project implements the firmware of the PCB/23-315 board 
following the Software specifications of the:
+  Gantry Software Detailed Documentation - Filter Device Specification


## Directory Content

+ firmware: this is the building project;
  + boot: this is the directory where to place the application bootloader if present;
  + doc: this is the Doxygen source documentation project directory;
  + FW315.X: this is the MPLAB-X 6.05 IDE project directory;
  + other_files: this is a non project directory with docs and tools helping the firmware development
  + src: this is the source directory;
    + config: this is the Harmony 3 configuration directory;
    + packs: this is the Harmony 3 packs directory;
    + Protocol: this is the Communication protocol source files;
      + MET_can_protocol.c, MET_can_protocol.h: this is the library module implementing the Metaltronica Can protocol;
      + protocol.c, protocol.h: this is the Power Service board implementation protocol;
    + application.h: this is the application shared object declaration;
    + licence.h: this is the microchip agreement license file (onòy for documentation);
    + main.c: this is main of the application. 

## 2. Related documentation

+ doc_index.html: is a link to the firmware documentation.


 ## COMPILATION TOOLS
  
  This software is compiled with the MPLAB-X IDE 6.05:
  - Compiler: XC32 Vs 3.1

  The libraries are the Harmony 3 with the following packs:
  + MSIS 5.4.0 for ARM processors;
  + SAME51_DFP 3.6.130;
  
## APPLICATION DOCUMENTATION
  
The software has been documented with the Doxygen 1.9.3.
