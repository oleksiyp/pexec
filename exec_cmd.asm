bits 64
section .text
        global _start

_start:
        xor rax,rax
        mov rdx,rax             ; No Env
        mov rsi,rax             ; No argv
        lea rdi, [rel _msg_start]

        add al, 0x3b

        syscall
_msg_start:

