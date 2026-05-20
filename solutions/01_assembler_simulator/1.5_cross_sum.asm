; Part 1.5: Cross Sum
;
; Goal:
;   Compute the digit sum (Quersumme) of an 8-bit number.
;
; Example:
;   255 -> 2 + 5 + 5 = 12
;
; Strategy:
;   Repeatedly:
;     - extract the last digit using modulo 10
;     - add it to the running sum
;     - divide the number by 10
;
; Registers:
;   A = temporary value / remainder
;   B = digit sum
;   C = current number
;   D = quotient during division

        JMP start

number: DB 255         ; Test value

start:
        MOV C, [number] ; Load number
        MOV B, 0        ; B = digit sum


.next_digit:
        CMP C, 0
        JE .show

        MOV A, C        ; Working copy
        MOV D, 0        ; D = quotient

.divide:
        CMP A, 10
        JB .digit_found

        SUB A, 10       ; Remove one group of 10
        INC D           ; Count subtraction
        JMP .divide

.digit_found:
        ; A = remainder
        ; D = quotient

        ADD B, A        ; Add digit to sum

        MOV C, D        ; Continue with quotient
        JMP .next_digit

.show:
        MOV A, B        ; A = final sum
        MOV C, 0        ; C = tens digit

.tens:
        CMP A, 10
        JB .display

        SUB A, 10
        INC C
        JMP .tens

.display:
        ADD C, 48       ; Convert tens to ASCII
        ADD A, 48       ; Convert units to ASCII

        MOV [232], C
        MOV [233], A

        HLT