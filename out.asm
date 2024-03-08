global _start
_start:
    mov rax, 10
    push rax
    mov rax, 20
    push rax
    add rsp, 16
    mov rax, 50
    push rax
    push QWORD [rsp + 16]
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
