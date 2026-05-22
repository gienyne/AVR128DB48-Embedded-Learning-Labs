; Part 1.6: Fibonacci (Bonus) - Solution
;
; Strategy:
;   Use two registers to hold the last two Fibonacci values:
;     C = F(n-1)  (previous)
;     D = F(n)    (current)
;   Each iteration: A = C + D, then shift C <- D, D <- A.
;   Use B as an iteration counter, stop when B reaches 11.
;
; After the loop, D holds F11 = 89.
; Display the result by splitting into digits (tens and units).
;
; Trace:
;   Start: C=1 (F1), D=1 (F2), B=2
;   Step:  A=2,  C=1,  D=2,  B=3   -> F3
;   Step:  A=3,  C=2,  D=3,  B=4   -> F4
;   ...
;   Step:  A=89, C=55, D=89, B=11  -> F11 reached, exit loop

        JMP start

start:
        MOV C, 1      ; C = F1 = 1
        MOV D, 1      ; D = F2 = 1
        MOV B, 2      ; B = iteration counter (we already have F1 and F2)

.loop:
        MOV A, D      ; A = current value (F_n)
        ADD A, C      ; A = F(n-1) + F(n) = F(n+1)
        MOV C, D      ; Shift: previous <- current
        MOV D, A      ; Shift: current  <- next
        INC B         ; Advance counter
        CMP B, 11     ; Have we reached F11?
        JNE .loop     ; If not, continue

        ; D now holds F11 = 89
        ; Split into tens (8) and units (9) for display

        MOV A, D      ; A = 89
        MOV C, 0      ; C = tens digit counter

.tens:
        CMP A, 10
        JB .display
        SUB A, 10
        INC C
        JMP .tens

.display:
        
        ADD C, 48     ; Convert tens to ASCII
        ADD A, 48     ; Convert units to ASCII
        MOV [232], C  ; Write tens digit
        MOV [233], A  ; Write units digit
        HLT

---------------------------------------------------------------------------------
Alternative solution:
---------------------------------------------------------------------------------


; Part 1.6: Fibonacci (Bonus) — Solution
;
; Strategy:
;   Use two registers to hold the last two Fibonacci values:
;     C = F(n-1)  (previous)
;     D = F(n)    (current)
;   Each iteration: A = C + D, then shift C <- D, D <- A.
;   Use B as an iteration counter, stop when B reaches 11.
;
; After the loop, A holds F11 = 89.
; For the display, this version uses the native DIV and MUL instructions
; rather than repeated subtraction - showing that when an instruction exists,
; there is no need to reimplement it manually.
;
;   DIV 10  ->  A = A / 10  (quotient replaces A)
;   MUL 10  ->  A = A * 10  (used to recover the remainder)
;
; Trace:
;   Start: C=1 (F1), D=1 (F2), B=2
;   Step:  A=2,  C=1,  D=2,  B=3   -> F3
;   Step:  A=3,  C=2,  D=3,  B=4   -> F4
;   ...
;   Step:  A=89, C=55, D=89, B=11  -> F11 reached, exit loop

start:
        MOV C, 1      ; C = F1 = 1
        MOV D, 1      ; D = F2 = 1
        MOV B, 2      ; B = iteration counter (F1 and F2 are already known)

.loop:
        MOV A, D      ; A = current value F(n)
        ADD A, C      ; A = F(n-1) + F(n) = F(n+1)
        MOV C, D      ; Shift: previous <- current
        MOV D, A      ; Shift: current  <- next
        INC B         ; Advance counter
        CMP B, 11     ; Have we reached F11?
        JNE .loop     ; If not, continue

        ; A = F11 = 89
        MOV D, A      ; Save a copy of the result in D before DIV overwrites A

        DIV 10        ; A = 89 / 10 = 8  (tens digit, quotient)
        MOV C, A      ; C = 8 (tens digit)

reste:
        MUL 10        ; A = 8 * 10 = 80
        SUB D, A      ; D = 89 - 80 = 9  (units digit, remainder)

        ADD C, 48     ; Convert tens digit to ASCII  (8 -> '8' = 56)
        ADD D, 48     ; Convert units digit to ASCII (9 -> '9' = 57)
        MOV [232], C  ; Write tens digit to output
        MOV [233], D  ; Write units digit to output