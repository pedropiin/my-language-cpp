global _start
_start:
    mov rax, 15
    push rax
    mov rax, 92
    push rax
    mov rax, 81
    push rax
    push QWORD [rsp + 0]

    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
