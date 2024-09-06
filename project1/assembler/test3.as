        lw      0       1       seven    load reg1 with 7 (symbolic address)
        lw      0       2       offset   load reg2 with 333333 (symbolic address)
        lw      0       3       333333   error, offsetfields does not fit in 16bits!
start   nor     1       2       3        nor reg1, reg2 and save to reg3
        add     3       1       1        add reg3, reg1 and save to reg1
        sw      0       1       7        store reg1 to mem[3]
        beq     0       0       done     branch to halt
        noop
        lw      0       1       seven    load reg1 with 7
done    halt                             end of program
seven   .fill   7
offset  .fill   333333
