        lw      0       3       thirty  load reg3 with 30 (symbolic address)
one     add     0       0       1       set reg1 value to 0
        lw      0       2       pos1    load reg2 with 1 (symbolic address)
        noop
loop    add     1       2       1       increment reg1
        beq     1       3       exit    end loop when reg1==30
        beq     0       0       loop    go back to the beginning of the loop
exit    sw      0       1       1       store (reg1==30) to mem[1]
        noop
        lw      0       4       neg1    load reg4 with -1 (symbolic address)        
loop2   add     1       4       1       decrement reg1
        beq     0       1       2       end loop when reg1==0
        beq     0       0       loop2   go back to beginning of the loop2
        noop
        sw      0       1       one     store (reg1==0) to mem[1]
done    halt                            end of program
pos1    .fill   1
neg1    .fill   -1
thirty  .fill   30   
lpAddr  .fill   loop                
