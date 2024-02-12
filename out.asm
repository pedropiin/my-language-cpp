global _start
_start:
    mov rax, 10
    push rax
    mov rax, 20
    push rax
    push QWORD [rsp + 8]
    push QWORD [rsp + 8]
    pop rax
    pop rbx
    add rax, rbx
    push rax
    mov rax, 10
    push rax
    pop rbx
    pop rax
    cmp rax, rbx
    jle label0
    mov rax, 100
    push rax
    add rsp, 0
label0:
    push QWORD [rsp + 0]
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
