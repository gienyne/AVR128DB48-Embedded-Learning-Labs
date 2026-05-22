; Part 1.4: Count To 42 - Solution
;
; Strategy:
;   Use register C as the current value (0 to 42).
;   For each value, extract the tens digit and units digit separately,
;   convert both to ASCII, and write them to output addresses 232 and 233.
;   After displaying 42, reset C to 0 and loop forever.
;
; Digit extraction (no native DIV for small numbers, use repeated subtraction):
;   tens  = value / 10   (how many times 10 fits into value)
;   units = value % 10   (remainder after removing tens)
;
; Example: value = 37
;   37 - 10 = 27  (count: 1)
;   27 - 10 = 17  (count: 2)
;   17 - 10 =  7  (count: 3)
;   7 < 10 -> stop  -> tens = 3, units = 7

        JMP start

start:
        MOV C, 0      ; C = current counter value (0..42)

.print:
        ; Extract tens digit via repeated subtraction 
        MOV A, C      ; A = working copy of current value
        MOV B, 0      ; B = tens digit counter

.div10:
        CMP A, 10     ; Is A < 10?
        JB .div_result  ; If so, stop dividing
        SUB A, 10     ; Subtract 10
        INC B         ; Increment tens count
        JMP .div10

.div_result:
        ; A now holds the units digit
        ; B now holds the tens digit

        ADD B, 48     ; Convert tens to ASCII
        ADD A, 48     ; Convert units to ASCII
        MOV [232], B  ; Write tens digit to first output cell
        MOV [233], A  ; Write units digit to second output cell

        ;Increment counter
        INC C
        CMP C, 43     ; Has counter exceeded 42?
        JNZ .print  ; If not, display next value

        ; Reset and restart
        MOV C, 0
        JMP .print