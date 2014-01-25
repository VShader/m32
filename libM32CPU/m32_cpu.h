#ifndef M32_CPU_H
#define M32_CPU_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <functional>

#include "m32_defines.h"

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
    void signal_ext1() { set_bit(intflags,bit_external_interrupt_1,true); }
    void signal_ext2() { set_bit(intflags,bit_external_interrupt_2,true); }
    bool int_enabled() { return get_bit(intflags,bit_interrupts_enabled); }
    bool int1_enabled() { return get_bit(intflags,bit_external_interrupt_1_enabled); }
    bool int2_enabled() { return get_bit(intflags,bit_external_interrupt_2_enabled); }
    bool timer_int_enabled() { return get_bit(intflags,bit_timer_interrupt_enabled); }
    bool user_mode() { return get_bit(status,bit_user_mode); }
    bool system_mode() { return !user_mode(); }
    void mmu_fault() { set_bit(status,bit_mmu_error,true); pause(); }
    m32_word pop();
    void push(m32_word data);
    m32_word upop();
    void upush(m32_word data);
    m32_word spop();
    void spush(m32_word data);
    void print_status();
    void print_memory(m32_word start, m32_word count);

    std::function<void(std::string)> callback_print;
    std::function<void(std::string)> callback_log;
    std::function<void()> callback_before_exec;
    std::function<void()> callback_after_exec;
    std::function<void(m32_word)> callback_mmu_read;
    std::function<void(m32_word)> callback_mmu_write;

    m32_word memory[65536];
    m32_register registers[32];
private:
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
        void indexed(m32_word address);
        m32_mmu *mmu;
        m32_cpu *cpu;

        m32_word value;
    };

    void info(std::string message) { std::stringstream out; out<<message<<std::endl; if(callback_log) callback_log(out.str()); }
    void error(std::string message) { std::stringstream out; out<<"[ERROR] "<<message<<std::endl; if(callback_log) callback_log(out.str()); }
    void debug(std::string message) { std::stringstream out; out<<"[DEBUG] "<<message<<std::endl; if(callback_log) callback_log(out.str()); }
    void set_bit(unsigned int reg_no, unsigned int bit, bool value) { registers[reg_no]&=~(1<<bit); registers[reg_no]|=(value<<bit); }
    bool get_bit(unsigned int reg_no, unsigned int bit) { return registers[reg_no]&(1<<bit); }
    void set_zero_bit(bool value) { set_bit(status,bit_zero,value); }
    void set_greater_equal_bit(bool value) { set_bit(status,bit_greater_equal,value); }
    void set_timer_run_bit(bool value) { set_bit(status,bit_timer_run,value); }
    void set_user_bit(bool value) { set_bit(status,bit_user_mode,value); }
    m32_register map_stack_to_mode(){ return sp-2+user_mode(); }
    m32_word read_register(unsigned int reg_no) { if(reg_no==sp) return registers[map_stack_to_mode()]; else return registers[reg_no]; }
    void write_register(unsigned int reg_no, m32_word value) { if(reg_no==sp) registers[map_stack_to_mode()]=value; else registers[reg_no]=value; }

    bool execute(m32_word data);
    bool running;
    m32_word* decode_param(m32_op_param param);
    m32_mmu *mmu;
    m32_operand *op1;
    m32_operand *op2;
};

#endif // M32_CPU_H
