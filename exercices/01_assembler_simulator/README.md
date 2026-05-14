# Exercise 01: Assembler Simulator

Introduction to low-level programming using a simple 8-bit Assembly simulator.  
No hardware required. This exercise runs entirely in the browser.

**Simulator:** [Simple 8-bit Assembler Simulator](https://schweigi.github.io/assembler-simulator/)

The syntax is inspired by **NASM (Netwide Assembler)**.

---

## What the Simulator Shows You

- **Registers** A, B, C, D : General-purpose 8-bit registers used to store temporary values and addresses.
- **Memory** : The simulator provides 256 bytes of RAM. The output display begins at memory address `232`.
- **Flags (`Z`, `C`, `F`)** : Status flags automatically modified by arithmetic and comparison instructions:
 The simulator provides three status flags:  
  `Z` -> Zero Flag  
  `C` -> Carry Flag  
  `F` -> Fault/Error Flag  
- **Stack (`SP`)** : A special memory area used to temporarily save data during function calls.
  Instructions such as `PUSH`, `POP`, `CALL`, and `RET` use the stack.
- **Instruction Pointer (`IP`)** : Stores the address of the next instruction to execute.

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

The simulator comes preloaded with a Hello World program.  
Read it carefully and step through it before modifying anything:

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

## Program Explanation

This example program prints "Hello World!" into the simulator output memory.

**Main Steps**
- Jump to the start label
- Store the address of the string in register C
- Store the output memory address (232) in register D
- Call the print function
- Copy each character from memory into the output buffer
- Stop execution using HLT

---

Use the **Step** button to execute one instruction at a time and watch registers and memory change.

---

## Parts

### Part 1.1: Instruction Set

Read the instruction reference in the simulator sidebar.

Experiment freely with the Hello World program:
- Change the string content
- Modify register values
- Add arithmetic instructions
- Remove or rearrange jumps and observe what breaks

The goal is to get comfortable with the environment.

---

### Part 1.2 : Hello Name

Modify the Hello World program so that it prints your own first and last name.

**Expected output:**
```
Firstname Lastname for example: John Doe
```

<details>
<summary>Hint</summary>

Characters are stored as ASCII values : `'A'` = 65, `'a'` = 97, space = 32.  
Define your name as: `DB "Firstname Lastname"` followed by `DB 0` as a null terminator.  
The existing `print` subroutine already handles null-terminated strings . Only the data needs to change.

</details>

---

### Part 1.3 : Print Up To 9

Write an Assembly program that prints the digits 0 through 9 in sequence.

**Constraints:**
- Do not define any memory variables
- Use registers exclusively throughout execution

**Expected output:**
```
0 1 2 3 4 5 6 7 8 9
```

<details>
<summary>Hint</summary>

ASCII `'0'` = 48. To print digit `5`, write value `53` to the output address.  
Use one register as a counter, increment it each iteration, and stop at `9` with a conditional jump (`JNZ` or `JE`).  
Write directly to address `232` and increment it to advance to the next output cell.

</details>

---

### Part 1.4 : Count To 42

Write a program that counts from 0 to 42, displaying the value across the first two output memory cells.  
After reaching 42 the program restarts from 0 , running in an infinite loop.

**Expected output (cycling):**
```
0
1
2
...
41
42
0
1

```

<details>
<summary>Hint</summary>

You need two output cells. One for the tens digit, one for the units digit.  
Extract digits using integer arithmetic: tens = `value / 10`, units = `value % 10`.  
The simulator has no native division instruction, implement it with repeated subtraction.

</details>

---

### Part 1.5: Cross Sum

Write a program that calculates the digit sum (Quersumme) of a given 8-bit number stored as a memory variable.

**Variable:**
```asm
DB 255  ; Number variable
```

**Expected output:**
```
12
```

Because `2 + 5 + 5 = 12`.

<details>
<summary>Hint</summary>

Repeatedly extract the last digit using modulo (`value % 10`), add it to a running sum, then reduce the number (`value / 10`).  
Stop when the value reaches `0`.  
Implement division and modulo using repeated subtraction.

</details>

---

### Part 1.6 : Fibonacci *(Bonus)*

Write a program that computes and displays the 11th Fibonacci number.

**Sequence:**
```
F1=1, F2=1, F3=2, F4=3, F5=5, F6=8, F7=13, F8=21, F9=34, F10=55, F11=89
```

**Expected output:**
```
FiboZahl: 89
```

<details>
<summary>Hint</summary>

Keep two registers for the previous two values (`prev` and `curr`).  
Each iteration: `next = prev + curr`, then shift — `prev ← curr`, `curr ← next`.  
Use a counter register to stop at 11. Start with `F1 = 1` and `F2 = 1`.

</details>

---

## Project Structure

```
01-assembler-simulator/
│
├── README.md
├── starter/
│   ├── 1.2-hello-name.asm
│   ├── 1.3-print-up-to-9.asm
│   ├── 1.4-count-to-42.asm
│   ├── 1.5-cross-sum.asm
│   └── 1.6-fibonacci.asm
│
└── solutions/
    ├── 1.2-hello-name.asm
    ├── 1.3-print-up-to-9.asm
    ├── 1.4-count-to-42.asm
    ├── 1.5-cross-sum.asm
    └── 1.6-fibonacci.asm
```

> Work through `starter/` first. The solution files are heavily commented. Reading them after solving is where the real learning happens.

---

## Resources

- [Simple 8-bit Assembler Simulator](https://schweigi.github.io/assembler-simulator/)
- [NASM Documentation](https://www.nasm.us/docs.php)
