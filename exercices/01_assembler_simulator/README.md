# Exercise 01: Assembler Simulator

Introduction to low-level programming using a simple 8-bit Assembly simulator.  
No hardware required. This exercise runs entirely in the browser.

**Simulator:** [Simple 8-bit Assembler Simulator](https://schweigi.github.io/assembler-simulator/)

The syntax is inspired by **NASM (Netwide Assembler)**.

---

## What the Simulator Shows You

- **Registers A, B, C, D** — General-purpose 8-bit registers used to store temporary values and addresses.
- **Memory** — The simulator provides 256 bytes of RAM. The output display begins at memory address `232`.
- **Flags (`Z`, `C`, `F`)** — Status flags automatically set by arithmetic and comparison instructions.
  - `Z` — Zero Flag
  - `C` — Carry Flag
  - `F` — Fault/Error Flag
- **Stack (`SP`)** — A special memory area used to temporarily save data during function calls. Instructions such as `PUSH`, `POP`, `CALL`, and `RET` use the stack.
- **Instruction Pointer (`IP`)** — Stores the address of the next instruction to execute.

---

## Learning Goals

- Read and write basic Assembly syntax
- Use and manipulate CPU registers
- Write loops with counters and conditional jumps
- Perform arithmetic operations (addition, division, modulo)
- Read from and write to memory addresses
- Implement simple iterative algorithms in Assembly

---

## Starter Code

The simulator comes preloaded with the following Hello World program.  
Read it carefully and step through it instruction by instruction before modifying anything.

```asm
; Simple example
; Writes Hello World to the output
        JMP start
hello:  DB "Hello World!"  ; Variable (string)
        DB 0               ; String terminator (null byte)

start:
        MOV C, hello       ; Point C to the start of the string
        MOV D, 232         ; Point D to the output area
        CALL print
        HLT                ; Stop execution

print:                     ; Subroutine: print(C: source, D: destination)
        PUSH A
        PUSH B
        MOV B, 0
.loop:
        MOV A, [C]         ; Load character from memory
        MOV [D], A         ; Write to output
        INC C
        INC D
        CMP B, [C]         ; Check for null terminator
        JNZ .loop
        POP B
        POP A
        RET
```

### Program Explanation

This program prints `"Hello World!"` into the simulator output memory.

- `JMP start` — skips over the data definition and jumps directly to the code
- `DB "Hello World!"` — defines the string as a sequence of bytes in memory
- `DB 0` — marks the end of the string (null terminator)
- `MOV C, hello` — loads the memory address of the string into register C
- `MOV D, 232` — points D to the output area (starts at address 232)
- `CALL print` — calls the print subroutine
- Inside `print`, each character is read from `[C]` and written to `[D]` until a null byte is found
- `HLT` — stops execution

Use the **Step** button to execute one instruction at a time and observe how registers and memory change at each step.

---

## Exercises

The exercise parts are described in [EXERCISES.md](./EXERCISES.md).  
Work through them in order. Solutions are in the `solutions/` folder — open them only after solving each part yourself.

---

## Project Structure

```
01_assembler_simulator/
│
├── README.md        — this file
├── EXERCISES.md     — exercise parts 1.1 to 1.6 with hints
│
└── solutions/
    ├── 1.2-hello-name.asm
    ├── 1.3-print-up-to-9.asm
    ├── 1.4-count-to-42.asm
    ├── 1.5-cross-sum.asm
    └── 1.6-fibonacci.asm
```

---

## Resources

- [Simple 8-bit Assembler Simulator](https://schweigi.github.io/assembler-simulator/)
- [NASM Documentation](https://www.nasm.us/docs.php)