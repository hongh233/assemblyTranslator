/**
 * @author hongh233
 * @description: This C program will translate a binary in a simplified RISC-based
 * 16-bit instruction set to x86-64 assembly. We will translate the X Architecture
 * instructions to X86 and generate x86 assembly code.
 */

#include <stdio.h>

/* char * method, get the X86 8-bits register according to the X Architecture
 * @params: unsigned char regX: register from the X Architecture
 * @return: the corresponding X86 8-bits register
 */
static char * getReg8(unsigned char regX) {
    switch (regX) {
        case 0: // r0
            return "%al";
        case 1: // r1
            return "%bl";
        case 2: // r2
            return "%cl";
        case 3: // r3
            return "%dl";
        case 6: // r6
            return "%r8b";
        case 7: // r7
            return "%r9b";
        case 8: // r8
            return "%r10b";
        case 9: // r9
            return "%r11b";
        case 10: // r10
            return "%r12b";
        case 11: // r11
            return "%r13b";
        case 12: // r12
            return "%r14b";
        case 13: // F
            return "%r15b";
        default:
            break;
    }
    // if no register match, print an error message
    return "error, can not find register\n";
}


/* char * method, get the X86 16-bits register according to the X Architecture
 * @params: unsigned char regX: register from the X Architecture
 * @return: the corresponding X86 16-bits register
 */
static char *getReg16(unsigned char regX) {
    switch (regX) {
        case 0: // r0
            return "%ax";
        case 1: // r1
            return "%bx";
        case 2: // r2
            return "%cx";
        case 3: // r3
            return "%dx";
        case 4: // r4
            return "%si";
        case 5: // r5
            return "%di";
        case 6: // r6
            return "%r8w";
        case 7: // r7
            return "%r9w";
        case 8: // r8
            return "%r10w";
        case 9: // r9
            return "%r11w";
        case 10: // r10
            return "%r12w";
        case 11: // r11
            return "%r13w";
        case 12: // r12
            return "%r14w";
        case 13: // r13
            return "%r15w";
        case 14: // r14
            return "%bp";
        case 15: // r15
            return "%sp";
        default:
            break;
    }
    // if no register match, print an error message
    return "error, can not find register\n";
}


/* char * method, get the X86 64-bits register according to the X Architecture
 * @params: unsigned char regX: register from the X Architecture
 * @return: the corresponding X86 64-bits register
 */
static char * getReg64(unsigned char regX) {
    switch (regX) {
        case 0: // r0
            return "%rax";
        case 1: // r1
            return "%rbx";
        case 2: // r2
            return "%rcx";
        case 3: // r3
            return "%rdx";
        case 4: // r4
            return "%rsi";
        case 5: // r5
            return "%rdi";
        case 6: // r6
            return "%r8";
        case 7: // r7
            return "%r9";
        case 8: // r8
            return "%r10";
        case 9: // r9
            return "%r11";
        case 10: // r10
            return "%r12";
        case 11: // r11
            return "%r13";
        case 12: // r12
            return "%r14";
        case 13: // F
            return "%r15";
        case 14: // r14
            return "%rbp";
        case 15: // r15
            return "%rsp";
        default:
            break;
    }
    // if no register match, print an error message
    return "error, can not find register\n";
}


int main(int argc, char **argv) {

    // check if there is enough arguments, if not, print the error message and terminate the program
    if (argc < 2) {
        printf("Error! Lack of argument!\n");
        return 1;
    }

    // open the file
    FILE *fin = fopen(argv[1], "rb");

    // if it's not correctly open, print the error message and terminate the program
    if (!fin) {
        printf("Error! Could not open file '%s' for reading\n", argv[1]);
        return 1;
    }

    // The Prologue
    printf(".global test\n");
    printf("test:\n");
    printf("    push %%rbp\n");
    printf("    mov %%rsp, %%rbp\n");

    unsigned char debugMode = 0;                         // the debugMode flag
    unsigned int labelAddress = 0;                       // the address of the current label


    // In this loop, we iteratively read an X architecture instruction, translate and print the X86 instruction
    while (1) {

        // create and print a label for each instruction being translated, start from: .L0000:
        printf(".L%04x:\n", labelAddress);

        // check the debug mode, if it opens, print "call debug"
        if (debugMode) {
            printf("    call debug\n");
        }

        unsigned char buffer0;                           // the first byte of the instruction
        unsigned char buffer1;                           // the second byte of the instruction

        // read the file and store the first byte into buffer0, if not success, print error message
        if (fread(&buffer0, 1, 1, fin) < 1) {
            printf("Error! Could not read from file '%s'", argv[1]);
            return 1;
        }

        // read the file and store the second byte into buffer1, if not success, print error message
        if (fread(&buffer1, 1, 1, fin) < 1) {
            printf("Error! Could not read from file '%s'", argv[1]);
            return 1;
        }

        // if the instruction is 0x00 0x00, break out of the loop
        if (buffer0 == 0x00 && buffer1 == 0x00) {
            break;
        }


        // prepare for several parameters
        unsigned char operand = (buffer0 >> 6) & 0x03;       // buffer0: the value of the first two bits
        unsigned char thirdBit = (buffer0 >> 5) & 0x01;      // buffer0: the value of the third bit
        unsigned char restBits = buffer0 & 0x1f;             // buffer0: the value of the rest five bits

        unsigned char buf1Reg1 = (buffer1 >> 4) & 0x0f;      // buffer1: the value of the first four bits
        unsigned char buf1Reg2 = buffer1 & 0x0f;             // buffer1: the value of the last four bits


        /* The next large if-else statement will translate the instruction and output x86-64 assembly.
         * In each case, we will have different cases depend on thirdBit and restBits.
         */


        // The 0-Operand Instructions, we will match each instruction using restBits
        if (operand == 0) {

            // ret - return from a procedure call
            if (restBits == 1) {
                printf("    ret\n");

            // cld - close debug mode
            } else if (restBits == 2) {
                debugMode = 0;

            // std - start debug mode
            } else if (restBits == 3) {
                debugMode = 1;
            }

            labelAddress += 2;                       // add the label address to 2


        // The 1-Operand Instructions, we will match each instruction using the thirdBit
        } else if (operand == 1) {

            labelAddress += 2;                       // add the label address to 2

            /* If the third most significant bit is 0, the four most significant bits of
             * the second byte encode the register that is to be operated on (0...15)
             * Otherwise, if it's 1, the second byte encodes the immediate value.
             */
            if (thirdBit == 0) {

                // we will match each instruction using restBits
                switch (restBits) {

                    // neg rD - negate register rD
                    case 1:
                        printf("    neg %s\n", getReg64(buf1Reg1));
                        break;

                    // not rD - logically negate register rD
                    case 2:
                        printf("    not %s\n", getReg64(buf1Reg1));
                        break;

                    // inc rD - increment rD
                    case 8:
                        printf("    inc %s\n", getReg64(buf1Reg1));
                        break;

                    // dec rD - decrement rD
                    case 9:
                        printf("    dec %s\n", getReg64(buf1Reg1));
                        break;

                    // push rS - push register rD onto the program stack
                    case 3:
                        printf("    push %s\n", getReg64(buf1Reg1));
                        break;

                    // pop rD - pop value from stack into register rD
                    case 4:
                        printf("    pop %s\n", getReg64(buf1Reg1));
                        break;

                    /* out rS - output character in rS to std-out
                     * This case will save current value of %rdi on the stack,
                     * move the byte to %rdi, to have same size register in mov, we use %di,
                     * call outchar, and restore %rdi.
                     */
                    case 7:
                        printf("    push %%rdi\n");
                        printf("    mov %s, %%di\n", getReg16(buf1Reg1));
                        printf("    call outchar\n");
                        printf("    pop %%rdi\n");
                        break;

                    default:
                        break;
                }

            // The second case: the second byte encodes the immediate value
            } else if (thirdBit == 1) {

                // br L - branch relative to label L if condition bit is true
                if (restBits == 1) {
                    printf("    test $1, %%r15\n");
                    printf("    jne .L%04x\n", labelAddress + buffer1 - 2);

                // jr L - jump relative to label L
                } else if (restBits == 2) {
                    printf("    jmp .L%04x\n", labelAddress + buffer1 - 2);
                }
            }


        // The 2-Operand Instructions, we will match each instruction using restBits
        } else if (operand == 2) {

            switch (restBits) {

                // add rS, rD - add register rS to register rD
                case 1:
                    printf("    add %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // sub rS, rD - subtract register rS from register rD
                case 2:
                    printf("    sub %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // mul rS, rD - multiply register rD by register rS
                case 3:
                    printf("    imul %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // and rS, rD - and register rS with register rD
                case 5:
                    printf("    and %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // or rS, rD - or register rS with register rD
                case 6:
                    printf("    or %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // xor rS, rD - xor register rS with register rD
                case 7:
                    printf("    xor %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // test rS1, rS2 - set condition flag to true if and only if rS1 ^ rS2 is not 0
                case 10:
                    printf("    test %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    printf("    setnz %%r15b\n");   // set flag if not zero
                    break;

                // cmp rS1, rS2 - set condition flag to true if and only if rS1 < rS2
                case 11:
                    printf("    cmp %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    printf("    setg %%r15b\n");    // set flag if greater than
                    break;

                // equ rS1, rS2 - set condition flag to true if and only if rS1 == rS2
                case 12:
                    printf("    cmp %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    printf("    sete %%r15b\n");    // set flag if equal
                    break;

                // mov rS, rD - copy register rS to register rD
                case 13:
                    printf("    mov %s, %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // load rS, rD - load word into register rD from memory pointed to by register rS
                case 14:
                    printf("    mov (%s), %s\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // stor rS, rD - store word from register rS to memory at address in register rD
                case 15:
                    printf("    mov %s, (%s)\n", getReg64(buf1Reg1), getReg64(buf1Reg2));
                    break;

                // loadb rS, rD - load byte into register rD from memory pointed to by register rS
                case 16:
                    // use getReg8 for the second parameter to load byte
                    printf("    mov (%s), %s\n", getReg64(buf1Reg1), getReg8(buf1Reg2));
                    break;

                // storb rS, rD - store byte from register rS to memory at address in register rD
                case 17:
                    // use getReg8 for the first parameter to store byte
                    printf("    mov %s, (%s)\n", getReg8(buf1Reg1), getReg64(buf1Reg2));
                    break;

                default:
                    break;
            }
            labelAddress += 2;                   // add the label address to 2


        // The Extended Instructions, we will match each instruction using the thirdBit
        } else if (operand == 3) {

            unsigned short bufferTemp0 = 0;      // first part of the additional operand
            unsigned short bufferTemp1 = 0;      // second part of the additional operand

            // read the file and store the first byte into bufferTemp0, if not success, print error message
            if (fread(&bufferTemp0, 1, 1, fin) < 1) {
                printf("Error! Could not read from file '%s'", argv[1]);
                return 1;
            }

            // read the file and store the first byte into bufferTemp1, if not success, print error message
            if (fread(&bufferTemp1, 1, 1, fin) < 1) {
                printf("Error! Could not read from file '%s'", argv[1]);
                return 1;
            }

            // the integrated value of the additional operand
            unsigned short secondInstruction = (bufferTemp0 << 8) | bufferTemp1;

            /* If the thirdBit is 0, then the instruction only use the one-word immediate operand,
             * otherwise, if it's 1, the four most significant bits of the second byte encode a
             * register (1...15) that is the second operand.
             */
            if (thirdBit == 0) {

                // jmp L - absolute jump to label L
                if (restBits == 1) {
                    printf("    jmp .L%04x\n", secondInstruction);

                // call L - absolute call to label L
                } else if (restBits == 2) {
                    printf("    call .L%04x\n", secondInstruction);
                }

            } else if (thirdBit == 1) {

                // loadi V, rD - load immediate value or address V into register rD
                if (restBits == 1) {
                    printf("    mov $%hu, %s\n", secondInstruction, getReg64(buf1Reg1));
                }
            }

            // the extended instructions need an additional increment of 2, result in a 4 increment
            labelAddress += 4;
        }
    }

    // Epilogue
    printf("    pop %%rbp\n");
    printf("    ret\n");

    // close the file
    fclose(fin);
    return 0;
}