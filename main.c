#include <stdio.h>
#include <stdlib.h>
#include "mipsFuncMap.h"

int main(void)
{

    char myName[] = "\nJay Huang ";
    char labNum[] = "CS 350 - Final Project\n\n";
    printf("%s%s\n", myName, labNum);

    char *registers[32] = {"$0", "$at", "$v0", "$v1",
                           "$a0", "$a1", "$a2", "$a3",
                           "$t0", "$t1", "$t2", "$t3",
                           "$t4", "$t5", "$t6", "$t7",
                           "$s0", "$s1", "$s2", "$s3",
                           "$s4", "$s5", "$s6", "$s7",
                           "$t8", "$t9", "$k0", "$k1",
                           "$gp", "$sp", "$fp", "$ra"};

    typedef unsigned char byte;
    struct R_Format
    {
        byte rs, rt, rd, shamt, funct;
    };
    struct I_Format
    {
        byte rs, rt;
        signed short immediate;
    };
    struct J_Format
    {
        unsigned int address;
    };

    struct instruction
    {
        byte opcode;
        union {
            struct R_Format r_fmt;
            struct I_Format i_fmt;
            struct J_Format j_fmt;
        };
    };
    // pc += offset * 4
    // pc is prompt from user 6

    typedef struct instruction Data;
    Data data[9999];

    // MIPS I - OPCODE(6) - rs(5) - rt(5) - immediate(16)
    // MIPS R - OPCODE(6) - rs(5) - rt(5) - rd(5) - shift(5) - funct(6)
    // MIPS J - OPCODE (6) - Address (26)

    char *datafile_name = "data.txt";
    FILE *datafile = fopen(datafile_name, "r");

    int i = 0;
    if (!datafile)
    {
        printf("; open failed; program ends\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Load File Success...Reading data.txt\n");
        int inst = 0;
        while (fscanf(datafile, "%x", &inst) != EOF)
        {
            data[i].opcode = (inst >> 26) & 0x3F;
            if (data[i].opcode == RTYPEOP)
            {
                int y = inst << 26;
                y = (y >> 26) & 0x3F;
                data[i].r_fmt.rs = (inst >> 21) & 0x1F;
                data[i].r_fmt.rt = (inst >> 16) & 0x1F;
                data[i].r_fmt.rd = (inst >> 11) & 0x1F;
                data[i].r_fmt.shamt = (inst >> 6) & 0x1F;
                data[i].r_fmt.funct = y;
                // printf(, rShamt, rFunc, rShamt, rFunc"y: %x\n", y);
            }
            else
            { // this is either I-TYPE and J-TYPE
                if (data[i].opcode == J || data[i].opcode == JAL)
                { //J-Type Instruction
                    int z = (inst)&0x3FFFFFF;
                    // z = (inst >> 6) & 0x3FFFFFF;
                    data[i].j_fmt.address = z;
                }
                else
                { // I-TYPE Instruction
                    data[i].i_fmt.rs = (inst >> 21) & 0x1F;
                    data[i].i_fmt.rt = (inst >> 16) & 0x1F;
                    data[i].i_fmt.immediate = inst & 0xFFFF;
                }
            }

            i++;
        }
    }

    unsigned int pc;
    printf("Read %d lines of data\n\n", i);
    printf("Enter program counter (hex digits): ");
    scanf("%x", &pc);

    printf("\n\nInitial Program Counter = x%x\n\n", pc);
    printf("Location    Instruction\n");

    for (int j = 0; j < i; j++)
    {
        byte opcode = data[j].opcode;
        if (opcode == RTYPEOP)
        {
            int rFunc = data[j].r_fmt.funct;
            int rShamt = data[j].r_fmt.shamt;
            int rsIndex = data[j].r_fmt.rs;
            int rtIndex = data[j].r_fmt.rt;
            int rdIndex = data[j].r_fmt.rd;
            if (rFunc == ADD)
            {
                printf("x%x   add  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rtIndex], registers[rsIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == SUB)
            {
                printf("x%x   sub  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == AND)
            {
                printf("x%x   and  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == OR)
            {
                printf("x%x   or   %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == SLT)
            {
                printf("x%x   slt  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == JR)
            {
                //printf("x%x   jr   %4s               (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rsIndex], rShamt, rFunc, rShamt, rFunc);
                printf("x%x   jr   %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == DIV)
            {
                printf("x%x   div  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == MUL)
            {
                printf("x%x   mul  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == NOR)
            {
                printf("x%x   nor  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else if (rFunc == XOR)
            {
                printf("x%x   xor  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
            else
            {
                printf("x%x   nop  %4s, %4s,%4s    (s/f: x%02x, x%02x = dec %d, %d)\n", pc, registers[rdIndex], registers[rsIndex], registers[rtIndex], rShamt, rFunc, rShamt, rFunc);
            }
        }
        else
        {
            if (opcode == J || opcode == JAL)
            {
                if (opcode == J)
                {
                    printf("x%x   j     x%7.7x\n", pc, data[j].j_fmt.address);
                }
                else if (opcode == JAL)
                {
                    printf("x%x   jal   x%7.7x\n", pc, data[j].j_fmt.address);
                }
                else
                {
                    printf("--------- J-TYPE FAILS! ---------\n");
                }
            }
            else
            {
                int rsIndex = data[j].i_fmt.rs;
                int rtIndex = data[j].i_fmt.rt;
                int immd = data[j].i_fmt.immediate;
                unsigned short immdu = immd;
                if (opcode == BEQ)
                {
                    printf("x%x   beq  %4s, %4s, x%4.4x    (dec %6d, target = x%x)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd, pc + 4 + (immd * 4));
                }
                else if (opcode == BNE)
                {
                    printf("x%x   bne  %4s, %4s, x%4.4x    (dec %6d, target = x%x)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd, pc + 4 + (immd * 4));
                }
                else if (opcode == ADDI)
                {
                    printf("x%x   addi %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
                else if (opcode == SLTI)
                {
                    printf("x%x   slti %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
                else if (opcode == ANDI)
                {
                    printf("x%x   andi %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
                else if (opcode == ORI)
                {
                    printf("x%x   ori  %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
                else if (opcode == XORI)
                {
                    printf("x%x   xori %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
                else if (opcode == LUI)
                {
                    printf("x%x   lui  %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
                else if (opcode == LW)
                {
                    printf("x%x   lw   %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
                else if (opcode == SW)
                {
                    printf("x%x   sw   %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
                else
                {
                    printf("x%x   nop  %4s, %4s, x%4.4x    (dec %6d)\n", pc, registers[rsIndex], registers[rtIndex], immdu, immd);
                }
            }
        }
        pc += 4;

        // printf(afterShift == BEQ ? "true\n" : "false\n");
    }

    printf("\n\nProgram ending...\n\n");
    printf("\n");
    fclose(datafile);
    return 0;
}