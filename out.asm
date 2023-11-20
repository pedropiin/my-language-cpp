global _start
_start:
    mov rax, 1
    push rax
    mov rax, 20
    push rax
    push QWORD [rsp + 8]
    pop rax
    test rax, rax
    jz label0
    push QWORD [rsp + 8]
    push QWORD [rsp + 8]
    mov rax, 15
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    pop rax
    pop rbx
    mul rbx
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
