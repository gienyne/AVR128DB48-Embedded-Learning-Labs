; Part 1.3: Print Up To 9 - Solution
;
; Strategy:
;   Load the ASCII code of '0' (= 48) directly into register C.
;   The output pointer D starts at address 232 (display area).
;   On each iteration, write C directly to the current output cell,
;   then increment both C (next ASCII digit) and D (next output cell).
;   Stop when C reaches 58 (ASCII code of ':', one past '9' = 57).
;
; Key insight:
;   ASCII digits are contiguous: '0'=48, '1'=49, ... '9'=57.
;   Incrementing C steps through the digit characters naturally —
;   no arithmetic conversion needed.


        JMP start

start:
        MOV C, 48          ; C = ASCII '0' -> first digit to print
        MOV D, 232         ; D = start of output memory area
        CALL print
        HLT                ; All digits written, stop execution

print:                     ; Writes ASCII characters from C to [D] until C reaches 58
        PUSH A
        PUSH B
        MOV B, 0           ; B = 0 (not used for comparison here)
.loop:
        MOV [D], C         ; Write current ASCII character directly to output
        INC C              ; Advance to next ASCII character ('0' -> '1' -> ... -> '9')
        INC D              ; Advance to next output memory cell
        CMP C, 58          ; Is C = 58? (58 = ASCII ':', one past '9' = 57)
        JNZ .loop          ; If not yet at 58, continue
        POP B
        POP A
        RET