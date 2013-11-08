#include "ide_main_window.h"
#include "ui_ide_main_window.h"

ide_main_window::ide_main_window(QWidget *parent) : QMainWindow(parent)
{
    QWidget *central_widget=new QWidget();
    layout_main=new QGridLayout(central_widget);
    setCentralWidget(central_widget);
    /*
    QMenu *file_menu=new QMenu("File");
    menuBar()->addMenu(file_menu);
    */

    cpu=new m32_cpu();

    setup_register_monitor();
    //setup_memory_monitor();
    setup_debug_button_panel();
    setup_code_editor();
    setup_cpu_debug_controls();
}

ide_main_window::~ide_main_window()
{
}

QLineEdit* ide_main_window::setup_register_monitor_line_edit()
{
    QLineEdit *le=new QLineEdit();
    le->setFixedWidth(70);
    le->setReadOnly(true);
    return le;
}

void ide_main_window::setup_register_monitor()
{
    le_r0=setup_register_monitor_line_edit();
    le_r1=setup_register_monitor_line_edit();
    le_r2=setup_register_monitor_line_edit();
    le_r3=setup_register_monitor_line_edit();
    le_r4=setup_register_monitor_line_edit();
    le_r5=setup_register_monitor_line_edit();
    le_r6=setup_register_monitor_line_edit();
    le_r7=setup_register_monitor_line_edit();
    le_status=setup_register_monitor_line_edit();
    le_pc=setup_register_monitor_line_edit();
    le_ssp=setup_register_monitor_line_edit();
    le_usp=setup_register_monitor_line_edit();
    le_sbase=setup_register_monitor_line_edit();
    le_ubase=setup_register_monitor_line_edit();
    le_slimit=setup_register_monitor_line_edit();
    le_ulimit=setup_register_monitor_line_edit();
    le_intflags=setup_register_monitor_line_edit();
    le_intbase=setup_register_monitor_line_edit();
    le_timer=setup_register_monitor_line_edit();
    le_treload=setup_register_monitor_line_edit();

    QGroupBox *grp_registers=new QGroupBox("Registers");
    QGridLayout *layout_register_monitor=new QGridLayout();
    grp_registers->setLayout(layout_register_monitor);

    layout_register_monitor->addWidget(new QLabel("R0"),0,0);
    layout_register_monitor->addWidget(new QLabel("R1"),1,0);
    layout_register_monitor->addWidget(new QLabel("R2"),2,0);
    layout_register_monitor->addWidget(new QLabel("R3"),3,0);
    layout_register_monitor->addWidget(new QLabel("R4"),4,0);
    layout_register_monitor->addWidget(new QLabel("R5"),5,0);
    layout_register_monitor->addWidget(new QLabel("R6"),6,0);
    layout_register_monitor->addWidget(new QLabel("R7"),7,0);
    layout_register_monitor->addWidget(new QLabel("STATUS"),8,0);
    layout_register_monitor->addWidget(new QLabel("PC"),9,0);
    layout_register_monitor->addWidget(new QLabel("SSP"),0,2);
    layout_register_monitor->addWidget(new QLabel("USP"),1,2);
    layout_register_monitor->addWidget(new QLabel("SBASE"),2,2);
    layout_register_monitor->addWidget(new QLabel("UBASE"),3,2);
    layout_register_monitor->addWidget(new QLabel("SLIMIT"),4,2);
    layout_register_monitor->addWidget(new QLabel("ULIMIT"),5,2);
    layout_register_monitor->addWidget(new QLabel("INTFLAGS"),6,2);
    layout_register_monitor->addWidget(new QLabel("INTBASE"),7,2);
    layout_register_monitor->addWidget(new QLabel("TIMER"),8,2);
    layout_register_monitor->addWidget(new QLabel("TRELOAD"),9,2);

    layout_register_monitor->addWidget(le_r0,0,1);
    layout_register_monitor->addWidget(le_r1,1,1);
    layout_register_monitor->addWidget(le_r2,2,1);
    layout_register_monitor->addWidget(le_r3,3,1);
    layout_register_monitor->addWidget(le_r4,4,1);
    layout_register_monitor->addWidget(le_r5,5,1);
    layout_register_monitor->addWidget(le_r6,6,1);
    layout_register_monitor->addWidget(le_r7,7,1);
    layout_register_monitor->addWidget(le_status,8,1);
    layout_register_monitor->addWidget(le_pc,9,1);
    layout_register_monitor->addWidget(le_ssp,0,3);
    layout_register_monitor->addWidget(le_usp,1,3);
    layout_register_monitor->addWidget(le_sbase,2,3);
    layout_register_monitor->addWidget(le_ubase,3,3);
    layout_register_monitor->addWidget(le_slimit,4,3);
    layout_register_monitor->addWidget(le_ulimit,5,3);
    layout_register_monitor->addWidget(le_intflags,6,3);
    layout_register_monitor->addWidget(le_intbase,7,3);
    layout_register_monitor->addWidget(le_timer,8,3);
    layout_register_monitor->addWidget(le_treload,9,3);

    layout_register_monitor->setVerticalSpacing(1);

    layout_main->addWidget(grp_registers,0,0);
    layout_main->setColumnStretch(1,1);

    cpu->callback_after_exec=[&]{
        le_r0->setText(QString::number(cpu->registers[r0],16).toUpper().rightJustified(8,'0'));
        le_r1->setText(QString::number(cpu->registers[r1],16).toUpper().rightJustified(8,'0'));
        le_r2->setText(QString::number(cpu->registers[r2],16).toUpper().rightJustified(8,'0'));
        le_r3->setText(QString::number(cpu->registers[r3],16).toUpper().rightJustified(8,'0'));
        le_r4->setText(QString::number(cpu->registers[r4],16).toUpper().rightJustified(8,'0'));
        le_r5->setText(QString::number(cpu->registers[r5],16).toUpper().rightJustified(8,'0'));
        le_r6->setText(QString::number(cpu->registers[r6],16).toUpper().rightJustified(8,'0'));
        le_r7->setText(QString::number(cpu->registers[r7],16).toUpper().rightJustified(8,'0'));
        le_status->setText(QString::number(cpu->registers[status],16).toUpper().rightJustified(8,'0'));
        le_pc->setText(QString::number(cpu->registers[pc],16).toUpper().rightJustified(8,'0'));
        le_ssp->setText(QString::number(cpu->registers[usp],16).toUpper().rightJustified(8,'0'));
        le_usp->setText(QString::number(cpu->registers[ssp],16).toUpper().rightJustified(8,'0'));
        le_sbase->setText(QString::number(cpu->registers[sbase],16).toUpper().rightJustified(8,'0'));
        le_ubase->setText(QString::number(cpu->registers[ubase],16).toUpper().rightJustified(8,'0'));
        le_slimit->setText(QString::number(cpu->registers[slimit],16).toUpper().rightJustified(8,'0'));
        le_ulimit->setText(QString::number(cpu->registers[ulimit],16).toUpper().rightJustified(8,'0'));
        le_intflags->setText(QString::number(cpu->registers[intflags],16).toUpper().rightJustified(8,'0'));
        le_intbase->setText(QString::number(cpu->registers[intbase],16).toUpper().rightJustified(8,'0'));
        le_timer->setText(QString::number(cpu->registers[timer],16).toUpper().rightJustified(8,'0'));
        le_treload->setText(QString::number(cpu->registers[treload],16).toUpper().rightJustified(8,'0'));
    };
}

void ide_main_window::setup_code_editor()
{
    txt_code=new QTextEdit();
    layout_main->addWidget(txt_code,1,0,1,2);
    cpu->callback_print=[&](std::string msg){
        txt_code->append(QString::fromStdString(msg));
    };
}

void ide_main_window::setup_memory_monitor()
{
    QLabel *lbl_dummy=new QLabel("memory stuff here...");
    layout_main->addWidget(lbl_dummy,0,2,2,1);
}

void ide_main_window::setup_debug_button_panel()
{
    QGroupBox *grp_debug_buttons=new QGroupBox("Debug Controls");
    QVBoxLayout *layout_debug=new QVBoxLayout();
    grp_debug_buttons->setLayout(layout_debug);
    pb_dbg_load_binary=new QPushButton("Load Binary File");
    pb_dbg_reset=new QPushButton("Reset CPU");
    pb_dbg_step=new QPushButton("Step");
    pb_dbg_run=new QPushButton("Run");
    pb_dbg_break=new QPushButton("Break");

    layout_debug->addWidget(pb_dbg_load_binary);
    layout_debug->addWidget(pb_dbg_reset);
    layout_debug->addWidget(pb_dbg_step);
    layout_debug->addWidget(pb_dbg_run);
    layout_debug->addWidget(pb_dbg_break);
    layout_debug->addStretch(1);

    layout_main->addWidget(grp_debug_buttons,0,2,2,1);
}

void ide_main_window::setup_cpu_debug_controls()
{
    connect(pb_dbg_load_binary,&QPushButton::clicked,[&]{
        QString file_name=QFileDialog::getOpenFileName(this,"Select binary file to load","","Binaries (*.bin)");
        if(file_name=="")
            return;
        cpu->load(file_name.toStdString());
    });

    connect(pb_dbg_reset,&QPushButton::clicked,[&]{
        cpu->reset();
    });

    connect(pb_dbg_step,&QPushButton::clicked,[&]{
        cpu->step();
    });

    connect(pb_dbg_run,&QPushButton::clicked,[&]{
        //cpu->run(); //this will hang whithout multithreading
    });
}
