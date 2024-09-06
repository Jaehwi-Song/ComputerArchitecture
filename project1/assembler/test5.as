        lw      0       4       neg10    load reg4 with -10 (symbolic address)
        lw      0       1       neg1     load reg1 with -1 (symbolic address)
        lb      0       1       neg1     error! Undefined opcodes (delete this line to continue)
        add     0       0       2        set reg2 with 0 (reg2 will store the total sum)
        add     0       1       3        set reg3 with -1
loop    add     2       3       2        add reg2, reg3 and save to reg2 (reg3 is negative value)
        add     3       1       3        decrement reg3 by 1
        beq     3       4       exit     branch to exit when reg3 == -10
        beq     0       zero    loop     error! Non-integer register arguments (change zero to 0 to fix this error.)
exit    sw      8       2       0        error! Register out of range [0,7]  (change 8 to 0 to fix this error.) (if fix all the errors, value in mem[0]==-45)
done    halt                             end of program
neg1    .fill   -1
neg10   .fill   -10
neg1    .fill   -1                       error! duplicated definition of labels (delete this line to continue)
