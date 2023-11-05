global _start
_start:
    mov rax, 20
    push rax
    mov rax, 3
    push rax
    mov rax, 1
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    mov rax, 1
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    pop rbx
    pop rax
    div rbx
    push rax
    mov rax, 7
    push rax
    push QWORD [rsp + 8]

    pop rax
    pop rbx
    mul rbx
    push rax
    mov rax, 7
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    push QWORD [rsp + 8]

    push QWORD [rsp + 8]

    pop rax
    pop rbx
    mul rbx
    push rax
    push QWORD [rsp + 0]

    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
