global _start
_start:
    mov rax, 1
    push rax
    mov rax, 2
    push rax
    mov rax, 10
    push rax
    mov rax, 4
    push rax
    mov rax, 5
    push rax
    mov rax, 8
    push rax
    push QWORD [rsp + 16]
    push QWORD [rsp + 8]
    pop rax
    pop rbx
    mul rbx
    push rax
    add rsp, 0
    mov rax, 60
    push rax
    push QWORD [rsp + 40]
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
