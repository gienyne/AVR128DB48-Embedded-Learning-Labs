# Exercises 01: Assembler Simulator

Work through each part in order.  
Solutions are in the `solutions/` folder - open them only after solving each part yourself.

---

## Part 1.1: Instruction Set

Read the instruction reference in the simulator sidebar.

Experiment freely with the Hello World program:

- Change the string content
- Modify register values
- Add arithmetic instructions
- Remove or rearrange jumps and observe what breaks

The goal is to get comfortable with the syntax and understand how the simulator reacts to changes.  
Nothing to submit for this part.

---

## Part 1.2: Hello Name

Modify the Hello World program so that it prints your own first and last name.

**Expected output:**
```
Firstname Lastname
```

<details>
<summary>Hint</summary>

Characters are stored as ASCII values - `'A'` = 65, `'a'` = 97, space = 32.  
Define your name as `DB "Firstname Lastname"` followed by `DB 0` as a null terminator.  
The existing `print` subroutine already handles null-terminated strings — only the data needs to change.

</details>

---

## Part 1.3: Print Up To 9

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
Use one register as a counter starting at `48`, increment it each iteration, and stop at `58`  
(one past `'9'` = 57) with a conditional jump.  
Write directly to address `232` and increment it to advance to the next output cell.

</details>

---

## Part 1.4: Count To 42

Write a program that counts from 0 to 42, displaying the value across the first two output memory cells.  
After reaching 42 the program restarts from 0, running in an infinite loop.

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
...
```

<details>
<summary>Hint</summary>

You need two output cells — one for the tens digit, one for the units digit.  
Extract digits using integer arithmetic: tens = `value / 10`, units = `value % 10`.  
The simulator has no native division instruction — implement it with repeated subtraction.  
Example: `37 - 10 - 10 - 10 = 7` (remainder), subtracted 3 times (quotient).

</details>

---

## Part 1.5: Cross Sum

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

Repeatedly extract the last digit using modulo (`value % 10`), add it to a running sum,  
then replace the value with the quotient (`value / 10`).  
Stop when the value reaches `0`.  
Implement both division and modulo using repeated subtraction.

</details>

---

## Part 1.6: Fibonacci *(Bonus)*

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

Keep two registers for the last two values (`prev` and `curr`).  
Each iteration: `next = prev + curr`, then shift — `prev <- curr`, `curr <- next`.  
Use a counter register to stop at F11. Start with `F1 = 1` and `F2 = 1`.  
Use the native `DIV` and `MUL` instructions to split the result into digits for display.

</details>