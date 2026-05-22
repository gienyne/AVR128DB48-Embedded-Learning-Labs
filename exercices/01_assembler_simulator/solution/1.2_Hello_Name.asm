; Part 1.2: Hello Name - Solution
;
; The only change from the original Hello World is:
;   - The variable label is renamed from "hello" to "name"
;   - The string content is replaced with a real name
;   - The print subroutine is completely unchanged
;
; Key concept: the print subroutine works with any null-terminated string.
; A null terminator is a byte with value 0 that marks the end of a string.
; As long as the string ends with DB 0, print handles it correctly.

        JMP start
name:   DB "Dimitry Ntofeu Nyatcha"  ; The string to print
        DB 0                          ; Null terminator 

start:
        MOV C, name   ; Load the address of the first character into C
        MOV D, 232    ; D points to the output memory area (starts at address 232)
        CALL print
        HLT           ; Stop execution

print:                ; Subroutine: copies bytes from [C] to [D] until null byte
        PUSH A        ; Save A and B on the stack so we don't corrupt the caller's state
        PUSH B
        MOV B, 0      ; B = 0, used to detect the null terminator (CMP B, [C])
.loop:
        MOV A, [C]    ; Load the current character from memory
        MOV [D], A    ; Write it to the output area
        INC C         ; Advance the source pointer
        INC D         ; Advance the destination pointer
        CMP B, [C]    ; Is the next byte 0? (null terminator check)
        JNZ .loop     ; If not zero, continue looping
        POP B         ; Restore saved registers
        POP A
        RET           ; Return to caller