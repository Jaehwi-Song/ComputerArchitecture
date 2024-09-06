        add     0       0       6       set reg6 to 0 (reg6 will store total count)
        lw      0       1       one     load reg1 with 1 (symbolic address)
        lw      0       2       two     load reg2 with 2 (symbolic address)
        lw      0       3       ten     load reg3 with 10 (symbolic address)
        add     0       0       4       initialize reg4 with 0. (reg4 will store total sum)
start   add     1       4       4       add reg4 value with reg1
        add     2       4       4       add reg4 value with reg2
loop    lw      2       2       one     load reg2 with mem[1 + reg2]
        add     2       4       4       add to total sum
        beq     2       3       exit    exit the loop when reg2 == 10
        beq     0       0       loop    go back to beginning of the loop
exit    beq     6       1       5       branch to the line after jalr instruction
        add     6       1       6       increment reg6
        lw      0       2       two     reload reg2 with value 2
        lw      0       7       five    load the target address to reg7 (contain the adress of start)
        sw      0       4       1       save the total sum result to mem[1]
        jalr    7       5               jump to label start
        sw      0       4       1       save the total sum result to mem[1] (the value of reg1 would be 110)
done    halt                            end of program
one     .fill   1
two     .fill   2
three   .fill   3
four    .fill   4
five    .fill   5
six     .fill   6
seven   .fill   7
eight   .fill   8
nine    .fill   9
ten     .fill   10
