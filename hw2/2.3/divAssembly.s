.global _start
.equ ws, 4

.data

dividend:
  .space 4 # 4 bytes of space

divisor:
  .space 4 # 4 bytes of space




.text

_start:
# quotient should be in %eax
# remainder should be in %edx

movl $0, %eax # eax = 0; quotient = 0
movl $0, %edx # edx = 0; remainder = 0

movl $31, %ecx # ecx = 31; i = 31

for_start:
  # 
  cmpl $0, %ecx # i - 0
  jl for_end

  # left shift remainder by 1
  # right shift dividend by i
  # and dividend with 1
  # or remainder with dividend
  shl $1, %edx # remainder = remaidner << 1
  movl dividend, %ebx # ebx = dividend 
  shr %cl, %ebx # ebx = dividend >> i 
  and $1, %ebx # ebx = (dividend >> i) & 1 
  or %ebx, %edx # edx = (remainder << 1) | ((dividend >> i) & 1) 

  # if_start
  # remainder - divisor
  # jb if_end
  # movl ebx 1 << i
  # or quotient with ebx
  # remainder -= divisor
  # if_end
  if_start:
    cmpl divisor, %edx # remainder - divisor
    jb if_end

    movl $1, %ebx # ebx = 1 
    shl %cl, %ebx # ebx = ebx << ebx; 1 << i 
    or %ebx, %eax # eax = eax | ebx; quotient | (1 << i) 
    subl divisor, %edx # remainder = remainder - divisor
  if_end:

  decl %ecx # i--
  jmp for_start
for_end:

done:
  nop
