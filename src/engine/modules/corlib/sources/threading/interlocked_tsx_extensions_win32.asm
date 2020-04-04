;
;  Copyright (c) 2019, Pavel Umnikov
;  All rights reserved.
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions are met:
;
;  * Redistributions of source code must retain the above copyright notice,
;    this list of conditions and the following disclaimer.
;  * Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the distribution.
;
;  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
;  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
;  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
;  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
;  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
;  DAMAGE.
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