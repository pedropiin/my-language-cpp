global _start
_start:
    mov rax, 7
    push rax
    push QWORD [rsp + 0]
    mov rax, 7
    push rax
    pop rax
    pop rbx
    sub rbx, rax
    push rbx
    pop rax
    test rax, rax
    jz label0
    mov rax, 5
    push rax
    push QWORD [rsp + 0]
    mov rax, 60
    pop rdi
    syscall
    add rsp, 8
label0:
    mov rax, 10
    push rax
    push QWORD [rsp + 0]
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
