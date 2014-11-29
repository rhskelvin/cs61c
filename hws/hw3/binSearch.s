    .data
# do not modify
LIST:   
    	.word -44 -4 0 1 2 4 7 9 23 126
line:   
    	.asciiz " was found at index "
newline: 
    	.asciiz "\n"
expect:
    	.asciiz ", expected "
    
    	.text 
    
    	li $s0 10 # length of the LIST
    	li $s1 0
    
    	la $a1 LIST # get address of the array
printLoop:
    	li $a2 0 # search from index = 0
    	li $a3 10 # to index = 10
    
    	sll $t0 $s1 2
    	addu $t0 $t0 $a1
    	lw $s2 0($t0)
        
    	move $a0 $s2
    	li $v0 1
    	syscall # PRINT value
    
    	la $a0 line
    	li $v0 4
    	syscall # PRINT " was found at index "
    
    	move $a0 $s2
    	jal binSearch
    	move $s2 $v0
    	move $a0 $v0
    	li $v0 1
    	syscall # PRINT index
    
    	beq $s2 $s1 skipError
    	la $a0 expect
    	li $v0 4
    	syscall # PRINT ", expected "
    
    	move $a0 $s1
    	li $v0 1
    	syscall # PRINT expected index
        
skipError:
    	la $a0 newline
    	li $v0 4
    	syscall # PRINT "\n"
    
    	addiu $s1 $s1 1 # s1++
    	blt $s1 $s0 printLoop # repeat
    
    	li $v0 10
    	syscall # EXIT
    
binSearch: # binSearch($a0,$a1,$a2,$a3)
    	# ****** YOUR CODE STARTS HERE ******
        #
    	# Try not to deviate too greatly (if at all) from the C version.
        #
    	# Ex: DO NOT look up the values of the $s registers, LIST, 
        # etc from above. You will receive no credit if you do.
        #
        # This problem can be solved using only the $a0-$a3,$t0-$t?,
        # $v0,$ra registers.
loop:
        slt $t0 $a3 $a2
        bne $t0 0 notFound
        add $t2 $a2 $a3
        li $t7 2
        div $t2 $t2 $t7
        mul $t1 $t2 4
        add $t3 $t1 $a1
        lw $t4 0($t3)
        slt $t0 $a0 $t4
        bne $t0 1 updatemin
        addi $a3 $t2 -1
        j loop
updatemin:
    	slt $t0 $t4 $a0
    	beq $t0 0 gotit
    	addi $a2 $t2 1
    	j loop
gotit:
    	move $v0 $t2
    	j return
    
notFound: # you can delete / alter this label and line as needed
    	li $v0 -1 # return value = -1
    
return:
	jr $ra #return
