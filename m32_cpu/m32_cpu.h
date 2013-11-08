#ifndef M32_CPU_H
#define M32_CPU_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <functional>

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

class m32_cpu
{
public:
    m32_cpu();
    void run();
    bool step();
    void pause();
    void reset();
    bool is_running();
    void load(std::string file_name);
    void signal_ext1() { registers[intflags]|=(1<<4); }
    void signal_ext2() { registers[intflags]|=(1<<6); }
    bool int_enabled() { return (registers[intflags]&(1<<0)); }
    bool int1_enabled() { return (registers[intflags]&(1<<3)); }
    bool int2_enabled() { return (registers[intflags]&(1<<5)); }
    bool timer_enabled() { return (registers[intflags]&(1<<1)); }
    bool user_mode() { return (registers[status]&(1<<16)); }
    bool system_mode() { return !user_mode(); }
    void mmu_fault() { registers[status]|=(1<<17); pause(); }
    unsigned int map_usermode_register(unsigned int reg_no);
    m32_word read_register(unsigned int reg_no);
    void write_register(unsigned int reg_no, m32_word value);
    std::function<void(std::string)> print_callback;
    m32_word pop();
    void push(m32_word data);
    m32_word spop();
    void spush(m32_word data);
    void print_status();
    void print_memory(m32_word start, m32_word count);

    m32_word memory[65536];

private:
    m32_register registers[32];

    class m32_mmu
    {
    public:
        m32_mmu(m32_cpu *cpu): cpu(cpu) {}
        m32_word read(m32_word address);
        void write(m32_word address, m32_word data);
    private:
        m32_cpu *cpu;
    };

    class m32_operand
    {
    public:
        enum target_type
        {
            target_reg=0,
            target_value=1,
            target_addr=2,
            target_stack=3
        };
        m32_operand(m32_mmu *mmu, m32_cpu *cpu): mmu(mmu), cpu(cpu) {}
        void decode(m32_op_param);
        m32_word read();
        void write(m32_word data);
        target_type type;
    private:
        m32_word indexed(m32_word address);
        m32_mmu *mmu;
        m32_cpu *cpu;

        m32_word value;
    };

    void info(std::string message) { std::stringstream out; out<<message<<std::endl; if(print_callback) print_callback(out.str()); }
    void error(std::string message) { std::stringstream out; out<<"[ERROR] "<<message<<std::endl; if(print_callback) print_callback(out.str()); }
    void debug(std::string message) { std::stringstream out; out<<"[DEBUG] "<<message<<std::endl; if(print_callback) print_callback(out.str()); }
    void set_bit(unsigned int reg_no, unsigned int bit, bool value) { registers[reg_no]&=~(1<<bit); registers[reg_no]|=(value<<bit); }
    bool get_bit(unsigned int reg_no, unsigned int bit) { return registers[reg_no]&(1<<bit); }
    void set_zero_bit(bool value) { set_bit(status,0,value); }
    void set_greater_equal_bit(bool value) { set_bit(status,1,value); }
    void set_timer_run_bit(bool value) { set_bit(status,8,value); }
    void set_user_bit(bool value) { set_bit(status,16,value); }

    bool execute(m32_word data);
    bool running;
    m32_word* decode_param(m32_op_param param);
    m32_mmu *mmu;
    m32_operand *op1;
    m32_operand *op2;
};

#endif // M32_CPU_H
