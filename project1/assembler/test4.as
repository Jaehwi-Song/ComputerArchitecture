        lw      0       4       one      load reg4 with 1 (symbolic address)
        lw      0       1       one      load reg1 with 1 (symbolic address)
        lw      0       6       neg1     load reg1 with -1
        lw      0       7       ten      load reg7 with 10
        add     0       0       5        set reg5 with 0
odd     add     5       4       5        increment reg5 by 1
        add     1       2       2        add reg1 value with reg2 if reg1 is odd number
        beq     0       0       judge    branch to judge label
even    add     5       6       5        decrement reg5 by 1
        nor     0       1       3        nor reg0, reg1 and save to reg3
        add     3       4       3        increment 4 by 1 to calculate negative value
        add     2       3       2        add negative of reg1 from reg2 if reg1 is even number
judge   beq     1       7       exit     branch to exit if reg1 == 10
        add     1       4       1        increment reg1 by 1
        beq     0       5       odd      branch to odd
        beq     0       0       even     branch to even
exit    sw      0       2       0        save reg2 result to mem[0] (stored value would be -5)
        beq     0       0       random   error! Use of undefined label (the test case will execute correctly when deleting this line)
done    halt                             end of program
one     .fill   1
neg1    .fill   -1
ten     .fill   10
