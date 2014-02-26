D4CD
====

This is the modify version of eGUI(D4D - https://github.com/Gargy007/eGUI) for standard char
displays (mainly for HD44780 driver). The modification has been done due to still big popularity of alpha numeric displays
and effective structure of eGUI that simplify the whole project organization with display interface, so the result
is port of eGUI source code from graphic LCD to char displays.

The overall structure of eGUI is completely comply with new D4CD driver, there is only reduced some
functionality like touch screen support and add some new like cursor support that is usually used in application
with char displays.

For inspiration how the things plus minus works in D4CD look at eGUI documentation **[eGUI Reference manual](http://gargy007.github.io/egui_doc/)**.
For more info check the project **[Wiki page](https://github.com/Gargy007/D4CD/wiki)**


Features
========

The D4CD brings into the project with char LCD topology of application from eGUI that helps create the
application with professional appearance and logic structure for developer.
* Supports character LCD displays of various sizes
* Small RAM (volatile) memory footprint
* Multiple platform support
* Object style of driver
* Smart support-screen-oriented structure of the user code
* Custom screen sizes, position, and a header like window
* Objects:
 * [Button](https://github.com/Gargy007/D4CD/wiki/D4CD_BUTTON)
 * [Check Box / User handled Radio Button](https://github.com/Gargy007/D4CD/wiki/D4CD_CHECKBOX)
 * [Label](https://github.com/Gargy007/D4CD/wiki/D4CD_LABEL)
 * [Menu](https://github.com/Gargy007/D4CD/wiki/D4CD_MENU)
 * [Number](https://github.com/Gargy007/D4CD/wiki/D4CD_NUMBER)
 * [ListBox](https://github.com/Gargy007/D4CD/wiki/D4CD_LISTBOX)
* User defined objects:
 * [Time](https://github.com/Gargy007/D4CD/wiki/D4CD_TIME)
 * [Date](https://github.com/Gargy007/D4CD/wiki/D4CD_DATE)
 * [Password](https://github.com/Gargy007/D4CD/wiki/D4CD_PASSWORD)
* HW cursor support
* Multiple user font tables support
* Since February 2014 is available free under LGPL license, to allow community extend this project.

The main changes to eGUI
========================
This short chapter describe the main changes between the eGUI(D4D) and new created driver D4CD.

Source files
------------
The source files organization of D4CD driver is completely same as in D4D, there is only changes
in some files:
* low_level_drivers - it contains new low level drivers to support low level drivers needs (HD44780,etc.)
* graphic_objects - contains new and modify graphic objects adapted for char display
* common_files - removed bitmap and color scheme source files

Features
--------
There are a few changes in features of graphic driver regarding only to adapt the D4CD for char
organization of display and some features has been removed because the char display doesn’t use it.
Removed features:
* Touch screen support
* Bitmap and font decoders
* Color scheme support
* Orientation support
Added features:
* Hardware cursor support
* User char table support

Graphic object changes
----------------------
The graphic object are changed to reflect capability of char display. The list of object is divided to two
parts:
* Standard:
 * [Button](https://github.com/Gargy007/D4CD/wiki/D4CD_BUTTON)
 * [Check Box / User handled Radio Button](https://github.com/Gargy007/D4CD/wiki/D4CD_CHECKBOX)
 * [Label](https://github.com/Gargy007/D4CD/wiki/D4CD_LABEL)
 * [Menu](https://github.com/Gargy007/D4CD/wiki/D4CD_MENU)
 * [Number](https://github.com/Gargy007/D4CD/wiki/D4CD_NUMBER)
 * [ListBox](https://github.com/Gargy007/D4CD/wiki/D4CD_LISTBOX)
* User defined objects:
 * [Time](https://github.com/Gargy007/D4CD/wiki/D4CD_TIME)
 * [Date](https://github.com/Gargy007/D4CD/wiki/D4CD_DATE)
 * [Password](https://github.com/Gargy007/D4CD/wiki/D4CD_PASSWORD)

Low level drivers API changes
-----------------------------
The low level drivers API had to be changed to reflect different control and capability of char display. The
main idea and structure of low level drivers are same as in eGUI but the interface content is changed.

![D4CD_structure](https://raw.github.com/Gargy007/D4CD/master/DOC/images/D4CD_structure.png)

The LCD controller level API structure content:
* D4CDLCD_Init - standard init function
* D4CDLCD_GoTo - function that set position of HW cursor
* D4CDLCD_SendChar - function writes one character on cursor position
* D4CDLCD_ReadChar - function reads character on cursor position
* D4CDLCD_WriteUserChar - function write the one char bitmap into LCD driver user char memory
* D4CDLCD_SetCursor - function sets the cursor appearance
* D4CDLCD_Delay_ms - function do call simple delay
* D4CDLCD_Deinit - standard de-initialization function

The LCD controller HW interface level API structure content:
* D4CDLCDHW_Init - standard init function
* D4CDLCDHW_SendData - function sends one data byte
* D4CDLCDHW_SendCmd - function sends one command byte
* D4CDLCDHW_SendNibbleCmd - function sends one command in nibble mode
* D4CDLCDHW_ReadData - function reads one data byte
* D4CDLCDHW_ReadCmd - function reads one command byte
* D4CDLCDHW_PinCtl - function control the additional signals (RESET, BACKLIGHT)
* D4CDLCDHW_DeInit - standard de-initialization function

For details just check the source code in “d4cd_lldapi.h“file.

Example demo
============
In repository is also included the demo that is using older Freescale EVB board M68EVB08GB60 that contains alphanumerical display.

![M68EVB08GB60](https://raw.github.com/Gargy007/D4CD/master/DOC/images/M60EVB08GB60.png)

Similar projects :
==================
The parent of this project is eGUI graphical driver.
Please check also:  **[eGUI - Graphic driver] (https://github.com/Gargy007/eGUI)** 

Email :
=======
egui.library@gmail.com