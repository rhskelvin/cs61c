OCTAL:
                andi $r1 $r1 0
                lw $r0 0($r1)                
                addi $r1 $r1 6
                srlv $r0 $r0 $r1
                andi $r2 $r0 0x7        
                andi $r1 $r1 0
                addi $r1 $r1 4
                sllv $r2 $r2 $r1
                andi $r1 $r1 0
                lw $r0 0($r1)                
                addi $r1 $r1 3
                srlv $r0 $r0 $r1
                andi $r0 $r0 0x7
                or $r2 $r2 $r0                
                andi $r1 $r1 0
                addi $r1 $r1 4
                sllv $r2 $r2 $r1
                andi $r1 $r1 0
                lw $r0 0($r1)
                andi $r0 $r0 0x7
                or $r2 $r2 $r0
                disp $r2, 0
                jr $r3