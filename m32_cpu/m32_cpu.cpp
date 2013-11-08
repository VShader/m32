#include "m32_cpu.h"

using namespace std;

m32_cpu::m32_cpu()
{
    reset();
    mmu=new m32_mmu(this);
    op1=new m32_operand(mmu,this);
    op2=new m32_operand(mmu,this);
}

void m32_cpu::run()
{
    running=true;
    while(is_running())
    {
        step();
    }
}

bool m32_cpu::step()
{
    // TODO: Check interrupts and process timers before executing
    if(registers[pc]>65536)
        pause();
    bool res=execute(memory[registers[pc]++]);
    print_status();
    if(!res)
        pause();
    return res;
}

void m32_cpu::pause()
{
    running=false;
}

void m32_cpu::reset()
{
    for(int i=0;i<32;i++)
        registers[i]=0;
    for(size_t i=0;i<65536;i++)
        memory[i]=0;
    running=false;
}

bool m32_cpu::is_running()
{
    return running;
}

void m32_cpu::load(string file_name)
{
    info("LOADING FROM FILE...");
    ifstream input(file_name,ifstream::binary);
    if(!input.is_open())
    {
        error("COULD NOT OPEN FILE!");
        pause();
        return;
    }
    input.seekg(0,input.end);
    size_t size=input.tellg();
    size_t word_bytes=sizeof(m32_word);
    size_t words=size/word_bytes;
    cout<<"IMAGE SIZE IS: "<<size<<"BYTES ("<<words<<" M32 WORDS)..."<<endl;
    input.seekg(0,input.beg);

    for(size_t word=0;word<words;word++)
    {
        for(size_t byte=word_bytes;byte>0;byte--)
        {
            char data;
            input.read(&data,1);
            //cout<<"read byte: "<<hex<<(int)data<<endl;
            *(((char*)memory)+word_bytes*word+byte-1)=data;
        }
    }

}

unsigned int m32_cpu::map_usermode_register(unsigned int reg_no)
{
    if((reg_no&0x02)==0)
        return(reg_no);
    if(user_mode())
        return(reg_no-1);
    return(reg_no-2);
}

m32_word m32_cpu::read_register(unsigned int reg_no)
{
    if((reg_no&0x10)!=0)
        reg_no=map_usermode_register(reg_no);
    int value=registers[reg_no];
    return value;
}

void m32_cpu::write_register(unsigned int reg_no, m32_word value)
{
    if((reg_no&0x10)!=0)
        reg_no=map_usermode_register(reg_no);
    registers[reg_no]=value;
}

m32_word m32_cpu::pop()
{
    m32_word pop_addr=read_register(sp)+1;
    m32_word data=mmu->read(pop_addr);
    write_register(sp,pop_addr);
    return data;
}

void m32_cpu::push(m32_word data)
{
    m32_word push_addr=read_register(sp);
    mmu->write(push_addr,data);
    write_register(sp,push_addr-1);
}

m32_word m32_cpu::spop()
{
    m32_word pop_addr=read_register(ssp)+1;
    m32_word data=mmu->read(pop_addr);
    write_register(ssp,pop_addr);
    return data;
}

void m32_cpu::spush(m32_word data)
{
    m32_word push_addr=read_register(ssp);
    mmu->write(push_addr,data);
    write_register(ssp,push_addr-1);
}

void m32_cpu::print_status()
{
    if(!print_callback)
        return;

    stringstream out;
    out.fill('0');
    out<<"M32 CPU STATUS:"<<endl
       <<"R0: "<<setw(8)<<hex<<registers[r0]<<" "<<"R1: "<<setw(8)<<hex<<registers[r1]<<endl
       <<"R2: "<<setw(8)<<hex<<registers[r2]<<" "<<"R3: "<<setw(8)<<hex<<registers[r3]<<endl
       <<"R4: "<<setw(8)<<hex<<registers[r4]<<" "<<"R5: "<<setw(8)<<hex<<registers[r5]<<endl
       <<"R6: "<<setw(8)<<hex<<registers[r6]<<" "<<"R7: "<<setw(8)<<hex<<registers[r7]<<endl
       <<"PC: "<<setw(8)<<hex<<registers[pc]<<endl;
    print_callback(out.str());
}

void m32_cpu::print_memory(m32_word start, m32_word count)
{
    if(!print_callback)
        return;
    stringstream out;
    out.fill('0');
    out<<"MEMORY DUMP ("<<setw(8)<<hex<<count<<" BYTES FROM "<<setw(8)<<hex<<start<<")"<<endl;
    for(m32_word it=start;it<start+count;it++)
    {
        out.fill('0');
        out<<setw(8)<<hex<<it<<": "<<setw(8)<<hex<<memory[it]<<endl;
    }
    info(out.str());
}

bool m32_cpu::execute(m32_word data)
{
    m32_word opcode=data&0xFFFF0000;
    m32_op_param param2=(m32_op_param)(data>>8);
    m32_op_param param1=(m32_op_param)(data>>0);
    switch (opcode) {
    case m32_nop:
        break;
    case m32_add:
        {
            op1->decode(param1);
            op2->decode(param2);
            m32_word op1_val=op1->read();
            m32_word op2_val=op2->read();
            m32_word res=op1_val+op2_val;
            set_zero_bit(res==0);
            op1->write(res);
        }
        break;
    case m32_sub:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op1_val=op1->read();
        m32_word op2_val=op2->read();
        m32_word res=op1_val-op2_val;
        set_zero_bit(res==0);
        op1->write(res);
    }
        break;
    case m32_and:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op1_val=op1->read();
        m32_word op2_val=op2->read();
        m32_word res=op1_val&op2_val;
        set_zero_bit(res==0);
        op1->write(res);
    }
        break;
    case m32_or:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op1_val=op1->read();
        m32_word op2_val=op2->read();
        m32_word res=op1_val|op2_val;
        set_zero_bit(res==0);
        op1->write(res);
    }
        break;
    case m32_xor:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op1_val=op1->read();
        m32_word op2_val=op2->read();
        m32_word res=op1_val^op2_val;
        set_zero_bit(res==0);
        op1->write(res);
    }
        break;
    case m32_not:
    {
        op1->decode(param1);
        m32_word res=~op1->read();
        set_zero_bit(res==0);
        op1->write(res);
    }
        break;
    case m32_cmp:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op1_val=op1->read();
        m32_word op2_val=op2->read();
        set_zero_bit((op1_val-op2_val)==0);
        set_greater_equal_bit(op1_val>=op2_val);
    }
        break;
    case m32_mul:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op1_val=op1->read();
        m32_word op2_val=op2->read();
        m32_word res=op1_val*op2_val;
        set_zero_bit(res==0);
        op1->write(res);
    }
        break;
    case m32_div:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op1_val=op1->read();
        m32_word op2_val=op2->read();
        if(op2_val==0)
            pause();
        m32_word res=op1_val/op2_val;
        set_zero_bit(res==0);
        op1->write(res);
    }
        break;
    case m32_mod:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op1_val=op1->read();
        m32_word op2_val=op2->read();
        m32_word res=op1_val%op2_val;
        set_zero_bit(res==0);
        op1->write(res);
    }
        break;
    case m32_mov:
    {
        op1->decode(param1);
        op2->decode(param2);
        m32_word op2_val=op2->read();
        op1->write(op2_val);
    }
        break;
    case m32_jmp:
    {
        op1->decode(param1);
        write_register(pc,op1->read());
    }
        break;
    case m32_jnz:
    {
        op1->decode(param1);
        m32_word op1_val=op1->read();
        if(!get_bit(status,0)) // zero bit not set
            write_register(pc,op1_val);
    }
        break;
    case m32_jge:
    {
        op1->decode(param1);
        m32_word op1_val=op1->read();
        if(get_bit(status,1)) // greater or equal bit set
            write_register(pc,op1_val);
    }
        break;
    case m32_jgt:
    {
        op1->decode(param1);
        m32_word op1_val=op1->read();
        if((registers[status]&3)==2) // zero bit and greater equal bit set
            write_register(pc,op1_val);
    }
        break;
    case m32_jz:
    {
        op1->decode(param1);
        m32_word op1_val=op1->read();
        if(get_bit(status,0)) // zero bit set
            write_register(pc,op1_val);
    }
        break;
    case m32_call:
    {
        op1->decode(param1);
        m32_word jmp_addr=op1->read();
        push(read_register(pc));
        write_register(pc,jmp_addr);
    }
        break;
    case m32_ret:
    {
        m32_word ret_addr=mmu->read(pop());
        write_register(pc,ret_addr);
    }
        break;
    case m32_int:
    {
        spush(read_register(status));
        spush(read_register(pc));
        set_user_bit(false);
        m32_word offset=data&0x0F;
        m32_word new_pc=mmu->read(read_register(intbase)+offset);
        write_register(pc,new_pc);
    }
        break;
    case m32_reti:
    {
        write_register(pc,spop());
        write_register(status,spop());
    }
        break;
    case m32_trap:
        break;
    case m32_prn:
    {
        op1->decode(param1);
        m32_word op1_val=op1->read();
        std::stringstream sstream;
        sstream<<std::hex<<op1_val<<"H";

        if(print_callback)
            print_callback(sstream.str());
    }
        break;
    case m32_halt:
        pause();
        break;
    case m32_prt:
    {
        op1->decode(param1);
        m32_word op1_val=op1->read();
        if(op1->type!=m32_operand::target_addr)
            op1_val=0;
        m32_word data=mmu->read(op1_val++);
        std::stringstream out;
        while(data!=0)
        {
            out<<(char)data;
            data=mmu->read(op1_val++);
        }
        if(print_callback)
            print_callback(out.str());

    }
        break;
    case m32_prl:
    {
        op1->decode(param1);
        m32_word op1_val=op1->read();
        if(op1->type!=m32_operand::target_addr)
            op1_val=0;
        m32_word data=mmu->read(op1_val++);
        std::stringstream out;
        while(data!=0)
        {
            out<<(char)data;
            data=mmu->read(op1_val++);
        }
        out<<"\n";
        if(print_callback)
            print_callback(out.str());

    }
        break;
    default:
        error("Unknown Opcode");
        pause();
        return false;
        break;
    }
    return true;
}

m32_word m32_cpu::m32_mmu::read(m32_word address)
{
    if(cpu->user_mode())
    {
        m32_register r_ulimit=cpu->registers[ulimit];
        if(r_ulimit==0||(r_ulimit!=0&&address<=r_ulimit))
            return cpu->memory[address+cpu->registers[ubase]];
        else
            cpu->mmu_fault();
    }
    else
    {
        m32_register r_slimit=cpu->registers[slimit];
        if(r_slimit==0||(r_slimit!=0&&address<=r_slimit))
            return cpu->memory[address+cpu->registers[sbase]];
        else
            cpu->mmu_fault();
    }
    return 0;
}

void m32_cpu::m32_mmu::write(m32_word address, m32_word data)
{
    if(cpu->user_mode())
    {
        m32_register r_ulimit=cpu->registers[ulimit];
        if(r_ulimit==0||(r_ulimit!=0&&address<=r_ulimit))
            cpu->memory[address+cpu->registers[ubase]]=data;
        else
            cpu->mmu_fault();
    }
    else
    {
        m32_register r_slimit=cpu->registers[slimit];
        if(r_slimit==0||(r_slimit!=0&&address<=r_slimit))
            cpu->memory[address+cpu->registers[sbase]]=data;
        else
            cpu->mmu_fault();
    }
}

void m32_cpu::m32_operand::decode(m32_op_param param)
{
    unsigned char reg_no;
    type=(target_type)((param>>5)&7);
    reg_no=(m32_op_param)(param&0x1F);
    if(type==target_reg)
    {
        value=reg_no;
    }
    else if((type==target_value)&(reg_no==0))
    {
        value=mmu->read(cpu->registers[pc]);
        cpu->registers[pc]++;
    }
    else if((type==target_value)&(reg_no==3))
    {
        type=target_stack;
    }
    else if(((type&3)==1)&(reg_no==2))
    {
        type=target_addr;
        value=mmu->read(cpu->registers[pc]) ;
        cpu->registers[pc]++;
        if((type&4)==4)
            value=indexed(value) ;
    } // absolute
    else if((type&3)==2)
    {
        type=target_addr;
        // wrong:         value=cpu_regs[reg_no] ;
        value=cpu->read_register(reg_no);
        int dsp=mmu->read(cpu->registers[pc]) ;
        cpu->registers[pc]++;
        value=value+dsp;
        if ((type&4)==4)
            value=indexed(value) ;
    } // end reg relative
    else
    {
        // unknown type
    }
}

m32_word m32_cpu::m32_operand::read()
{
    switch(type)
    {
    case target_reg:
        return cpu->read_register(value);
        break;
    case target_addr:
        return mmu->read(value);
        break;
    case target_value:
        return value;
        break;
    case target_stack:
        return cpu->pop();
        break;
    default:
        break;
    }
    return 0;
}

void m32_cpu::m32_operand::write(m32_word data)
{
    switch(type)
    {
    case target_reg:
        cpu->write_register(value,data);
        break;
    case target_addr:
        mmu->write(value,data);
        break;
    case target_value:
    {
        cpu->error("Cannot write to literal value");
        cpu->pause();
    }
        break;
    case target_stack:
        cpu->push(data);
        break;
    default:
    {
        cpu->error("Operand write error");
        cpu->pause();
    }
        break;
    }
}

m32_word m32_cpu::m32_operand::indexed(m32_word address)
{
    value=mmu->read(cpu->registers[pc]);
    cpu->registers[pc]++;
    return address+cpu->registers[value];
}
