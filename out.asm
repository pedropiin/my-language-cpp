global _start
_start:
    mov rax, 10
    push rax
    mov rax, 5
    push rax
    pop rbx
    pop rax
    div rbx
    push rax
    push QWORD [rsp + 0]

    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
