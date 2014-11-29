MOD:
    andi    $r3, $r3, 0
    andi    $r0, $r0, 0
    lw      $r1, 1($r0)
    lw      $r0, 0($r0)
    slt     $r2, $r1, $r3
    addi    $r3, $r3, 1
    bne     $r2, $r3, div
    lui     $r3, 0
    sub     $r1, $r3, $r1
    addi    $r3, $r3, 1
div:
    slt     $r2, $r0, $r1
    bne     $r2, $r3, div
    jr      $r3
