global _start
_start:
    mov rax, 10
    push rax
    mov rax, 6
    push rax
    push QWORD [rsp + 8]
    mov rax, 10
    push rax
    pop rbx
    pop rax
    cmp rbx, rax
    jle label0
    push QWORD [rsp + 8]
    push QWORD [rsp + 8]
    pop rax
    pop rbx
    mul rbx
    push rax
    add rsp, 0
label0:
    push QWORD [rsp + 8]
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
