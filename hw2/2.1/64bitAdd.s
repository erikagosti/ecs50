.global _start
.equ ws, 4

.data

num1:
  .space 8
  # .long 5 # upper half
  # .long 5 # lower half
  # upper 32 bits of num1 in edx %hi(value)
  # lower 32 bits of num1 in eax %lo(value)a

num2:
  .space 8
  # .long 1
  # .long 1
  # upper 32 bits of num2 in edx %hi(value)
  # lower 32 bits of num2 in eax %lo(value)

.text
# upper 32 bits of num1 in edx %hi(value)
# upper 32 bits of num2 in edx
# lower 32 bits of num1 in eax %lo(value)
# lower 32 bits of num2 in eax
# first initialize num1, then movl num2 into num1


_start:

movl [num1 + ws], %eax # eax = lower 32 bit of num1
movl [num2 + ws], %ebx # ecx = lower 32 bit of num2

movl num1, %edx # edx = upper 32 bit of num1
movl num2, %ecx # ecx = upper 32 bits of num2

adc %ebx, %eax # eax = eax + ecx, lower num1 += lower num2

adc %ecx, %edx # edx = edx + ecx, upper num1 += upper num2

done:
  nop
