# Microchip Studio - Setup Guide

This guide covers everything you need to get started with Microchip Studio
for the AVR128DB48 exercises (Übung 03 onwards).

Download: [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio)

---

## Creating a New Project

Use `Ctrl+Shift+N` or navigate through the menu:

**File -> New -> Project...**

![New project menu](images/projektAnlegen.png)

---

### Project Type Selection

In the New Project dialog:

1. Expand the **Installed** menu in the left panel
2. Select **C/C++**
3. Select **GCC C Executable Project**
4. Enter a name for your project
5. Choose a save location
6. Confirm with **OK**

![New project dialog](images/executable.png)

---

### Device Selection

After confirming, a Device Selection dialog opens:

1. Set the Device Family dropdown to **AVR-Dx**
2. Select **AVR128DB48** from the list
3. Confirm with **OK**

![Device selection](images/deviceFamilie.png)

Your project is now ready with a `main.c` file already open.

![Project ready](images/NavigationIDE.png)

---

## IDE Navigation

Once your project is open, the following areas of the interface are available:

![IDE UI elements](images/UI_element.png)

| # | Element | Description |
|---|---------|-------------|
| 1 | Open file | Opens an existing file in the IDE. |
| 2 | Save | Saves the file currently selected in the editor. |
| 3 | Save all | Saves all open files. |
| 4 | Build & Upload | Compiles the project and uploads it to the board. |
| 5 | Start Debugging (paused) | Starts debug mode with execution suspended at the beginning. |
| 6 | Solution Explorer | Opens the file explorer panel. |
| 7 | Start Debugging (running) | Starts debug mode with execution running immediately. |
| 8 | Step Into (`F11`) | When paused at a function call, enters the function body. |
| 9 | Step Over (`F10`) | When paused at a function call, executes it without entering. |
| 10 | Step Out (`Shift+F11`) | When inside a function, exits it and returns to the caller. |
| 11 | Debugger / Programmer | *Depends on your setup - see [Hardware Requirements](../README.md#hardware-requirements).* |


### Useful shortcuts

| Action | Shortcut |
|--------|----------|
| Build | `Ctrl+Shift+B` |
| Save all | `Ctrl+Shift+S` |
| New project | `Ctrl+Shift+N` |
| Start / continue debug | `F5` |
| Step over | `F10` |
| Step into | `F11` |
| Step out | `Shift+F11` |
| Toggle breakpoint | `F9` |

---

## Typical Workflow

1. Open or create a project
2. Write your code in `main.c`
3. Build with `Ctrl+Shift+B`: check the Output panel for errors
4. Connect the AVR128DB48 board via USB
5. Start debugging with `F5`: the program uploads and starts running
6. Use breakpoints and the debug toolbar to inspect program state

---

*This setup applies to all exercises from Übung 03 onwards.*
