.global matMult
.equ ws, 4
/*
  int** matMult(int **a, int num_rows_a, int num_cols_a, int** b, int num_rows_b, int num_cols_b) {
    int **c = (int **)calloc(num_rows_a, sizeof(int *));

    for (int i = 0; i < num_rows_a; i++) {
      c[i] = (int *)calloc(num_cols_b, sizeof(int));
      for (int j = 0; j < num_cols_b; j++) {
        for (int k = 0; k < num_rows_b; k++) {
          c[i][j] += a[i][k] * b[k][j];
        }
      }
    }
    return c;
  }
*/
# ebp + 7: num_cols_b
# ebp + 6: num_rows_b
# ebp + 5: b
# ebp + 4: num_cols_a
# ebp + 3: num_rows_a
# ebp + 2: a
# ebp + 1: return address
# ebp: old ebp
# ebp - 1: c
# ebp - 2: i
# ebp - 3: j
# ebp - 4: k

.text

matMult:
  .equ num_locals, 4
  .equ used_ebx, 1
  .equ used_esi, 1
  .equ used_edi, 1
  .equ num_saved_regs, used_ebx + used_esi + used_edi
  prologue_start:
    push %ebp
    movl %esp, %ebp
    subl $(num_locals + num_saved_regs) * ws, %esp

    push %esi
    push %ebx
    push %edi

    .equ num_cols_b, (7 * ws)
    .equ num_rows_b, (6 * ws)
    .equ b, (5 * ws)
    .equ num_cols_a, (4 * ws)
    .equ num_rows_a, (3 * ws)
    .equ a, (2 * ws)
    .equ c, (-1 * ws)
    .equ i, (-2 * ws)
    .equ j, (-3 * ws)
    .equ k, (-4 * ws)
    .equ old_esi, (-5 * ws)
    .equ old_ebx, (-6 * ws)
    .equ old_edi, (-7 * ws)


  prologue_end:

  # eax: calloc
  # ebx:
  # ecx: sizeof(int)
  # edx:
  # esi:
  # edi:

  # int **c = (int **)calloc(num_rows_a, sizeof(int *));
  movl num_rows_a(%ebp), %eax # eax = num_rows_a
  movl $4, %ecx # ecx = 4
  push %ecx
  push %eax # pushes arg to malloc on stack
  call calloc # return value in eax
  movl %eax, c(%ebp) # c = (int **)calloc(num_rows_a, sizeof(int *))
  addl $2 * ws, %esp # clear argument from stack


  # eax: 
  # ebx:
  # ecx: i
  # edx: num_rows_a
  # esi:
  # edi:

  # for (int i = 0; i < num_rows_a; i++) {
  movl $0, %ecx # ecx = 0; i = 0
  outer_loop_start:
    # i < num_rows_a
    # i - num_rows_a < 0
    # i - num_rows_a >= 0
    cmpl num_rows_a(%ebp), %ecx
    jge outer_loop_end

    # eax: c[i]
    # ebx: 
    # ecx: i
    # edx: C
    # esi: 4
    # edi:

    # c[i] = (int *)calloc(num_cols_b, sizeof(int));

    movl num_cols_b(%ebp), %eax # eax = num_cols_b
    movl $4, %esi # esi = 4
    push %esi
    push %eax
    movl %ecx, i(%ebp) # save ecx in i
    call calloc
    addl $2 * ws, %esp # clear argument from stack 
    movl c(%ebp), %edx # edx = c
    movl i(%ebp), %ecx # restore ecx
    movl %eax, (%edx, %ecx, ws) # c[i] = calloc

    # eax: 
    # ebx: 
    # ecx: i
    # edx: j
    # esi: 4
    # edi:

    # for (int j = 0; j < num_cols_b; j++) {
    movl $0, %edx # edx = 0; j = 0
    middle_loop_start:
      # j < num_cols_ b, j - num_cols_b < 0, j - num_cols_b >= 0
      cmpl num_cols_b(%ebp), %edx
      jge middle_loop_end

      # for (int k = 0; k < num_rows_b; k++) {
        movl $0, %esi # esi = 0; k = 0
        inner_loop_start:
          cmpl num_rows_b(%ebp), %esi
          jge inner_loop_end

          # c[i][j] += a[i][k] * b[k][j];
          # *(*(c + i) + j)
          # a[i][k], b[k][j], save eax, imull, restore eax, c[i][j], add to c[i][j]

          # eax: *(*(c + i) + j)
          # ebx: c[i]
          # ecx: i
          # edx: j
          # esi: k
          # edi: b
          movl %eax, %ebx # move eax to ebx
          movl %edx, j(%ebp) # save j
          movl a(%ebp), %eax # eax = a
          movl (%eax, %ecx, ws), %eax # eax = *(a + i)
          movl (%eax, %esi, ws), %eax # eax = *(*(a + i) + k)

          movl b(%ebp), %edi # edi = b
          movl (%edi, %esi, ws), %edi # edi = *(b + k)
          imull (%edi, %edx, ws) # eax = *(*(a + i) + k) * *(*(b + k) + j)

          movl j(%ebp), %edx # restore j
          addl %eax, (%ebx, %edx, ws) # *(*(c + i) + j) += *(*(a + i) + k) * *(*(b + k) + j)
          movl %ebx, %eax # restore eax

          incl %esi # k++
          jmp inner_loop_start
        inner_loop_end:

      incl %edx # j++
      jmp middle_loop_start
    middle_loop_end:

    incl %ecx # i++
    jmp outer_loop_start
  outer_loop_end:

epilogue:
  movl c(%ebp), %eax # set return value

  # restore saved regs
  movl old_ebx(%ebp), %ebx
  movl old_esi(%ebp), %esi
  movl old_edi(%ebp), %edi

  movl %ebp, %esp
  pop %ebp
  ret
