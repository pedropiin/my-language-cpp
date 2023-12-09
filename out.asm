global _start
_start:
    mov rax, 1
    push rax
    mov rax, 10
    push rax
    mov rax, 6
    push rax
    mov rax, 5
    push rax
    add rsp, 8
    push QWORD [rsp + 16]
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
