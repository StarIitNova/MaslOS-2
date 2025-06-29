# MaslOS-2
MaslOS but 2

This is essentially [MaslOS](https://github.com/marceldobehere/MaslOS) on the outside, but completely different on the inside.
The main difference being a working scheduler and support for ELF execution!

Of course it is 64bit and boots using BIOS.
And it works on real hardware!


## Features
 + Basics
   - Scheduler
   - Elf loading
   - User Space
   - Inter Process Communication
   - Stdio using IPC
   - Multithreading (still experimental?)
 + GUI
   - MaslOS GUI Framework port
   - Mouse/Keyboard interaction
   - MaslOS Desktop / WM
   - Taskbar
   - Start Menu
 + Apps
   - Tic Tac Toe
   - Flappy Rocc
   - Terminal
   - Explorer
   - Image Viewer
   - Music Player (You can see a demo [here](https://cdn.marceldobehere.com/file/747710745.mp4))
   + MAAB interpreter
     - Pong
     - Maalbrot
   - Notepad
   - Paint (mostly working)
   - Doom
   + Minesweeper
     - Can be launched in 3 modes (easy, medium, hard)
     - Example: `run minesweeper.elf medium`
   - Connect Four
   - MicroEdit (has some issues)
   - 3D Cube Renderer (has some issues)
   - CalcRocc (has some issues)
   - Snake (soon)
   + Dialogs
     - Open File Dialog
     - Save File Dialog (soon)
     - Generic Information Dialog (soon)
     - Generic Text Input Dialog (soon)
     - Generic Information YES/NO Dialog (soon)
   - Other Apps ported from MaslOS and some extra ones (not yet)
 + Audio
   + Drivers
     - PC-Speaker
     - AC97
   - Audio System (exists, still a bit wip)
 + Other
   - SMP (not added yet)
   - idk more stuff

## How to use

### Start Menu
You can click on the start menu button in the bottom left corner, to open the start menu.

In the start menu, you can click on the programs and it will open them.

### Taskbar
 - You can left click on one of the tabs to select it.
 - You can middle click a tab to close it.


### Desktop / Windows
 - You can click on a window to select it.
 - You can drag the window around by holding its top bar.
 - You can also resize it, if allowed, by dragging the sides/corners.
 - You can also minimize the window by clicking the minimize button.
 - You cannot currently maximize the window.
 - To close a window, you can click on the X button.
 - You can quit the whole process of the window by holding SHIFT and pressing the close button.
 - You can also close the window by pressing ALT + F4.

### Mouse Capture
A window can capture your mouse. This makes sense for games and such.

You can escape the capture by pressing CONTROL and ESCAPE.


### All Shortcuts
Here are all the shortcuts for quick reference: (including debug ones)
 - Press F10 to open the explorer
 - Press F12 to open a shell (there is a help command)
 - Press F11 to redraw the desktop (eg. If you get an error)
 - Press F5 to launch an empty elf
 - Press F6 to launch a mini window test
 - Press ALT + F4 to force close a program
 - Press CTRL + ESC to escape mouse capture



## How to build (Linux)
 - Install `qemu-system` if you haven't.
 - Install `make` if you haven't.
 - Install `gcc` if you haven't.
 - Install `g++` if you haven't.
 - Install `binutils` if you haven't.
 - Install `nasm` if you haven't.
 - Install `xorriso` if you haven't.


Go into the MaslOS2 Folder and run `make run --clean`

If you are trying this and getting weird compiler/linker errors, please let me know!

gcc and ld like to break things in newer versions for some reason.  *:D*

GOD PLEASE IF YOU ARE EDITING THIS GENERATE A `compile_commands.json` FILE WITH `$ python3 ecc.py` (NOTE: this requires `bear` to be installed). Also if you're editing on Windows but need to run ecc.py on WSL2/Some linux distro, hop over to your windows terminal and run `$ python3 ecc.py --wsl2win` and this
will convert `/mnt/c/` or whatever drive letter into `C:/`

## How to run (No need to build)

### How to run (Linux)
Install QEMU if you haven't.

Download/clone the repo.

Run the `cDisk.sh` file to generate an empty disk so QEMU can boot correctly.

Run the `JUST RUN OS.sh` file to run it!

You can also use the `JUST RUN OS_KVM.sh` file to run it faster if you are natively on linux.

### How to run (Windows)

#### QEMU

Install QEMU if you haven't.

Add QEMU To your PATH variable. [If you need help, click here!](https://linuxhint.com/qemu-windows/)

Then download/clone the repo and run the `run.cmd` file

#### Other

You can also boot the ISO in other software like Virtualbox and it tends to run way faster there.

### Hardware

You can flash the ISO to a usb drive using tools like Rufus and if you are lucky, your device should boot MaslOS2 fine.

NOTE: As MaslOS2 uses PS/2 for mouse and keyboard input, it could happen that you cannot use it without actual PS/2 devices if your bios does not emulate them.

## Contributing
I am still working on quite a few things that need to be done for MaslOS2 to be in a good state.

Meaning that not everything is 100% implemented yet and some needed things could be missing.

The Docs can be found [here](https://github.com/marceldobehere/MaslOS-2/wiki/Application-Development-for-MaslOS2).

If you want to contribute by adding your own little apps/tools/games/ports I would be very happy!

If you need any help / have any questions or are curious about MaslOS2, feel free to contact me per discord. (@marceldarcel)






## Screenshots
![Showing off some apps](/images/demo.png)
![MaslOS2 on display at my schools day of open doors](/images/20240126_181449.jpg)
![Minesweeper](/images/minesweeper.PNG)
![The start menu](/images/wm%207.png)
![Doom](/images/doom-test.png)
![More apps ported](/images/some%20stuff.jpg)
![Working more on the terminal](/images/wm%206.png)
![Working on the terminal](/images/terminal%201.gif)
![Taskbar yes](/images/wm%205.png)
![WM almost on par with MaslOS](/images/wm%204.png)
![More WM progress](/images/wm%203.png)
![WM progress](/images/wm%202.png)
![Beginning to work on WM](/images/start%20of%20wm%201.png)
![A Bootscreen yes](/images/img1.png)




## Thanks to:
 - [MaslOS](https://github.com/marceldobehere/MaslOS)
 - [VisualOS](https://github.com/nothotscott/VisualOS)
 - [MicroOS](https://github.com/Glowman554/MicroOS)
 - [SAF](https://github.com/chocabloc/saf)
