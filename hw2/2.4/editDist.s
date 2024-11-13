.global _start
.equ ws, 4

.data

string1:
  .space 101

string2:
  .space 101

oldDist: # allots space for pointers
  .space 101 * ws

curDist: # allots space for pointers
  .space 101 * ws

.text
# edit distance between string1 and 2 should be in eax

strlen_start:
  # length should be in edx
  # while loop for string from lecture
  # parameter string is in %eax
  # int i = 0;, i is ecx
  # while (string[i] != '/0') {
  #   i++;
  # }
  # return i;
  movl $0, %ecx # ecx = 0; i = 0
  strlen_for_start:
    cmpb $0, (%eax, %ecx, 1) # string[i], 0
    je strlen_for_end
    incl %ecx # i++
    jmp strlen_for_start
  strlen_for_end:
  movl %ecx, %eax
  ret


min_start:
  /*
    if (a < b) {
      return a;
    }
    else {
      return b;
    }
  */
  /*
    a, b
    if:
    cmpl a, b
    a should be in edi, b should be in esi
  */
  cmpl %esi, %edi
  jge min_end
  movl %edi, %eax # set return value to a
  ret

min_end:
  movl %esi, %eax # set return value to b
  ret


swap_start:
  /*
    curDist's address shoudl be in %eax
    oldDist's address should be in %ebx
  */
  push %eax
  push %ebx
  push %ecx
  push %edx
  push %edi
  push %esi

  movl $0, %ecx # ecx = 0; temp = 0
  movl $0, %edi # edi = 0; i = 0
  swap_for_start:
    cmpl -8(%ebp), %edi # i - word2_len
    jg swap_for_end

    movl oldDist(, %edi, ws), %eax # eax = oldDist[i]
    movl curDist(, %edi, ws), %ebx # ebx = curDist[i]
    movl %eax, %ecx
    movl %ebx, oldDist(, %edi, ws) # oldDist[i] = curDist[i]
    movl %ecx, curDist(, %edi, ws) # curDist[i] = temp

    incl %edi # i++
    jmp swap_for_start
  swap_for_end:

  pop %esi
  pop %edi
  pop %edx
  pop %ecx
  pop %ebx
  pop %eax
  ret

edit_dist_start:
  /*
    find length of word1
    find length of word2
    oldDist and curDist are in labels

  */

  push %ebp # prologue
  mov %esp, %ebp # prologue

  movl $string1, %eax
  call strlen_start
  push %eax # word1_len saved onto stack

  movl $string2, %eax
  call strlen_start
  push %eax # word2_len saved onto stack

  movl $0, %ecx # ecx = 0; i = 0
  init_for_start:
    cmpl -8(%ebp), %ecx # i - word2_len
    jg init_for_end

    movl %ecx, oldDist(, %ecx, ws) # oldDist[i] = i
    movl %ecx, curDist(, %ecx, ws) # curDist[i] = i

    incl %ecx # i++
    jmp init_for_start
  init_for_end:

  /*
  for(i = 1; i < word1_len + 1; i++){
    curDist[0] = i;
    for(j = 1; j < word2_len + 1; j++){
      if(word1[i-1] == word2[j-1]){
        curDist[j] = oldDist[j - 1];
      }//the characters in the words are the same
      else{
        curDist[j] = min(min(oldDist[j], //deletion
                          curDist[j-1]), //insertion
                          oldDist[j-1]) + 1; //subs titution
      }
    }//for each character in the second word
    swap(&oldDist, &curDist);
  }//for each character in the first word
  */

  movl $1, %ecx # ecx = 1; i = 1
  for_start:
    cmpl -4(%ebp), %ecx # i - word1_len
    jg for_end
    movl %ecx, curDist # curDist[0] = i
    movl $1, %ebx # ebx = 1; j = 1
    inner_for_start:
      cmpl -8(%ebp), %ebx # j - word2_len
      jg inner_for_end
        movl %ecx, %edx # edx = ecx; edx = i
        subl $1, %edx # edx -= 1; edx = i - 1
        movzbl string1(, %edx), %edx # lower 8 bits of edx = string1[i - 1]

        movl %ebx, %eax # eax = j
        subl $1, %eax # eax = j - 1
        movzbl string2(, %eax), %eax # lower 8 bits of eax = string2[j - 1]
        cmpb %al, %dl
        jne edit_else

        movl %ebx, %eax # eax = ebx, eax = j
        subl $1, %eax # eax = j - 1
        movl oldDist(, %eax, ws), %edx # edx = oldDist[j - 1]
        movl %edx, curDist(, %ebx, ws) # curDist[j] = oldDist[j - 1]
        jmp if_else_end
      edit_else:
        /*
          movl $string1, %eax
          call strlen
          push %eax # word1_len saved onto stack
        */

        # min call: a is edi, b is esi
        push %ebx # save j onto stack
        push %ecx # save i onto stack
        movl oldDist(, %ebx, ws), %edi # edi = oldDist[j]
        subl $1, %ebx # j = j - 1
        movl curDist(, %ebx, ws), %esi # esi = curDist[j]
        call min_start

        movl %eax, %edi # edi = eax; edi = min(oldDist[j], curDist[j - 1])
        movl oldDist(, %ebx, ws), %esi #esi oldDist[j - 1]
        call min_start

        incl %eax # min++

        pop %ecx
        pop %ebx

        movl %eax, curDist(, %ebx, ws) # curDist[j] = min
      if_else_end:
      incl %ebx # j++
      jmp inner_for_start
    inner_for_end:
    call swap_start
    movl $1, %ebx # j = 1
    incl %ecx # i++
    jmp for_start
  for_end:

  movl -8(%ebp), %eax # eax = word2_len
  movl oldDist(, %eax, ws), %eax # eax = oldDist[word2_len]

  pop %edx
  pop %edx

  mov %ebp, %esp # epilogue
  pop %ebp # epilogue

  ret


_start:

  # have two labels, one for argv[1] one for argv[2]
  # string1, string2
  call edit_dist_start
  jmp done

done:
  nop
  