#ifndef M32_DEFINES_H
#define M32_DEFINES_H

typedef unsigned int m32_register;
typedef unsigned int m32_word;
typedef unsigned char m32_op_param;

enum m32_operation
{
    m32_nop =0x01000000,
    m32_add =0x11000000,
    m32_sub =0x12000000,
    m32_and =0x13000000,
    m32_or  =0x14000000,
    m32_xor =0x15000000,
    m32_not =0x16000000,
    m32_cmp =0x17000000,
    m32_mul =0x18000000,
    m32_div =0x19000000,
    m32_mod =0x1A000000,
    m32_mov =0x21000000,
    m32_jmp =0x31000000,
    m32_jnz =0x32000000,
    m32_jge =0x33000000,
    m32_jgt =0x34000000,
    m32_jz  =0x35000000,
    m32_call=0x41000000,
    m32_ret =0x42000000,
    m32_int =0x43000000,
    m32_reti=0x44000000,
    m32_trap=0x45000000,
    m32_prn =0xF1000000,
    m32_halt=0xF2000000,
    m32_prt =0xF3000000,
    m32_prl =0xF4000000
};

enum m32_registers
{
    r0=0,
    r1=1,
    r2=2,
    r3=3,
    r4=4,
    r5=5,
    r6=6,
    r7=7,
    pc=8,
    status=9,
    intflags=10,
    intbase=11,
    timer=12,
    treload=13,
    ssp=16,
    usp=17,
    sp=18,
    sbase=20,
    ubase=21,
    slimit=24,
    ulimit=25
};

enum m32_status_bits
{
    bit_zero=0,
    bit_greater_equal=1,
    bit_timer_run=7,
    bit_user_mode=15,
    bit_mmu_error=16
};

enum m32_intflags_bits
{
    bit_interrupts_enabled=0,
    bit_timer_interrupt_enabled=1,
    bit_timer_interrupt=2,
    bit_external_interrupt_1_enabled=3,
    bit_external_interrupt_1=4,
    bit_external_interrupt_2_enabled=5,
    bit_external_interrupt_2=6
};

enum intbase_offset
{
    intbase_offset_timer=0,
    intbase_offset_external_1=1,
    intbase_offset_external_2=2
};

#endif // M32_DEFINES_H
