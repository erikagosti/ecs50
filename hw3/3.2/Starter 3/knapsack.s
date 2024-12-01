.global knapsack
.equ ws, 4

/*
unsigned int knapsack(int* weights, unsigned int* values, unsigned int num_items, 
              int capacity, unsigned int cur_value){
  //
    solves the knapsack problem
    @weights: an array containing how much each item weighs
    @values: an array containing the value of each item
    @num_items: how many items that we have
    @capacity: the maximum amount of weight that can be carried
    @cur_weight: the current weight
    @cur_value: the current value of the items in the pack
  //
  unsigned int i;
  unsigned int best_value = cur_value;
  
  for(i = 0; i < num_items; i++){//for each remaining item
    if(capacity - weights[i] >= 0 ){//if we can fit this item into our pack
      //see if it will give us a better combination of items
      best_value = max(best_value, knapsack(weights + i + 1, values + i + 1, num_items - i - 1, 
                     capacity - weights[i], cur_value + values[i]));
    }//if we can fit this item into our pack   
  }//try to find the best combination of items among the remaining items
  return best_value;


}//knapsack
*/

.text

# stack is currently:
# ebp + 6: cur_value
# ebp + 5: capacity
# ebp + 4: num_items
# ebp + 3: *values
# ebp + 2: *weights
# ebp + 1: return address
# ebp: old_ebp
# ebp - 1: i
# ebp - 2: best_value

knapsack:
  .equ num_locals, 2
  .equ used_ebx, 1
  .equ used_esi, 0
  .equ used_edi, 0
  .equ num_saved_regs, used_ebx + used_esi + used_edi
  knapsack_prologue_start:
    push %ebp
    movl %esp, %ebp
    subl $(num_locals + num_saved_regs) * ws, %esp

    .equ cur_value, (6 * ws)
    .equ capacity, (5 * ws)
    .equ num_items, (4 * ws)
    .equ values, (3 * ws)
    .equ weights, (2 * ws)
    .equ i, (-1 * ws)
    .equ best_value, (-2 * ws)
    .equ old_ebx, (-3 * ws)

    movl %ebx, old_ebx(%ebp)
  
  knapsack_prologue_end:

  # eax:      ebx: i      ecx:      edx: weights address      esi:      edi:      ebx:
  # for(i = 0; i < num_items; i++){//for each remaining item
  movl cur_value(%ebp), %ecx # ecx = cur_value
  movl %ecx, best_value(%ebp) # best_value = cur_value
  movl $0, %ebx # i = 0
  knapsack_for_loop_start:
    # i < num_items, i - num_items < 0, negation: i - num_items >= 0
    cmpl num_items(%ebp), %ebx
    jae knapsack_for_loop_end

    knapsack_if:
      # capacity - weights[i] >= 0, negation: capacity - weights[i] < 0
      movl capacity(%ebp), %ecx # ecx = capacity
      movl weights(%ebp), %edx # edx = weights address
      cmpl (%edx, %ebx, ws), %ecx
      jb knapsack_if_end

      # eax:      ebx: i      ecx: push values      edx:      esi:      edi:      ebx:

      # push cur_value + values[i]
      movl cur_value(%ebp), %ecx # ecx = cur_value
      movl values(%ebp), %edx # edx = values address
      addl (%edx, %ebx, ws), %ecx # ecx = cur_value + values[i]
      push %ecx

      # push capacity - weights[i]
      movl capacity(%ebp), %ecx # ecx = capacity
      movl weights(%ebp), %edx # edx = weights address
      subl (%edx, %ebx, ws), %ecx # ecx = cur_value - weights[i]
      push %ecx

      # push num_items - i - 1
      movl num_items(%ebp), %ecx # ecx = num_items
      subl %ebx, %ecx # ecx = num_items - i
      subl $1, %ecx # ecx = num_items - i - 1
      push %ecx

      # push values + i + 1
      movl values(%ebp), %edx # edx = values address
      leal (1 * ws)(%edx, %ebx, ws), %ecx # ecx = values + i + 1
      push %ecx
      
      # push weights + i + 1
      movl weights(%ebp), %edx # edx = value address
      leal (1 * ws)(%edx, %ebx, ws), %ecx # ecx = weights + i + 1
      push %ecx

      # call knapsack
      call knapsack
      addl $5 * ws, %esp # clear arguments

      # push eax
      push %eax

      # push best_value
      push best_value(%ebp)

      # call max
      call max
      addl $2 * ws, %esp # clear arguments

      movl %eax, best_value(%ebp) # best_value = max(best_value, knapsack())
    
    knapsack_if_end:

    incl %ebx # i++
    jmp knapsack_for_loop_start
  knapsack_for_loop_end:

  knapsack_epilogue:
    movl best_value(%ebp), %eax # set return value

    # restore saved regs
    movl old_ebx(%ebp), %ebx

    movl %ebp, %esp
    pop %ebp
    ret

max:
  /* 
  int max(int a, int b) {
    if (a > b) {
      return a;
    }
    else {
      return b;
    }
  }
  */
  # stack is currently:
  # ebp + 3: b
  # ebp + 2: a
  # ebp + 1: return address
  # ebp: old_ebp
  .equ num_locals, 0
  .equ used_ebx, 0
  .equ used_esi, 0
  .equ used_edi, 0
  .equ num_saved_regs, used_ebx + used_esi + used_edi
  max_prologue_start:
    push %ebp
    movl %esp, %ebp
    subl $(num_locals + num_saved_regs) * ws, %esp

    .equ b, (3 * ws)
    .equ a, (2 * ws)
  
  max_prologue_end:

  # eax: knapsack      ebx: i      ecx: push values      edx:      esi:      edi:      ebx:

  max_if:
    # i f (a > b)
    # a > b, a - b > 0, negation: a - b <= 0
    movl b(%ebp), %eax # eax = b
    cmpl %eax, a(%ebp)
    jbe max_epilogue

  max_else:
    movl a(%ebp), %eax # eax = a

  max_epilogue:
    # restore saved regs
    # none

    movl %ebp, %esp
    pop %ebp
    ret
