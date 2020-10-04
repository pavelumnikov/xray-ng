; This file is a part of xray-ng engine
;

;------------------------------------------------------------------------------
.code
        ALIGN 8
        PUBLIC _TSXTryLockTransactive
_TSXTryLockTransactive:
        xor  rax, rax
        mov  al, 1
        BYTE 0F2H
        xchg al, byte ptr [rcx]
        xor  al, 1
        ret

;------------------------------------------------------------------------------
.code
        ALIGN 8
        PUBLIC _TSXUnlockTransactive
_TSXUnlockTransactive:
        BYTE 0F3H
        mov  byte ptr [rcx], 0
        ret

;------------------------------------------------------------------------------
.code 
	ALIGN 8
	PUBLIC _TSXBeginTransaction
_TSXBeginTransaction:
        mov  eax, -1
        BYTE 0C7H
        BYTE 0F8H
        BYTE 000H
        BYTE 000H
        BYTE 000H
        BYTE 000H
        ret

;------------------------------------------------------------------------------
.code 
	ALIGN 8
	PUBLIC _TSXEndTransaction
_TSXEndTransaction:
        BYTE 00FH
        BYTE 001H
        BYTE 0D5H
        ret

;------------------------------------------------------------------------------
.code 
	ALIGN 8
	PUBLIC _TSXConflictAbortTransaction
_TSXConflictAbortTransaction:
        BYTE 0C6H
        BYTE 0F8H
        BYTE 0FFH  ; 12.4.5 Abort argument: lock not free when tested
        ret

;------------------------------------------------------------------------------
.code 
        ALIGN 8
	PUBLIC _TSXIsInTransaction
_TSXIsInTransaction:
        xor eax, eax
        BYTE 00FH  ; _xtest sets or clears ZF???
        BYTE 001H
        BYTE 0D6H
        jz   rset
        mov  al,1
rset:
        ret
end