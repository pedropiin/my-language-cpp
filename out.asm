global _start
_start:
    mov rax, 15
    push rax
    mov rax, 10
    push rax
    mov rax, 11
    push rax
    mov rax, 14
    push rax
    add rsp, 8
    push QWORD [rsp + 0]
    mov rax, 60
    pop rdi
    syscall
    add rsp, 8
    add rsp, 8
    mov rax, 9
    push rax
    push QWORD [rsp + 0]
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
