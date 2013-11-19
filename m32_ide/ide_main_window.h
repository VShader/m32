#ifndef IDE_MAIN_WINDOW_H
#define IDE_MAIN_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QDockWidget>
#include <QGroupBox>
#include <QMenu>
#include <QMenuBar>

#include "m32_cpu.h"
#include "code_editor.h"

class ide_main_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit ide_main_window(QWidget *parent = 0);
    ~ide_main_window();
public slots:
    void about();
private:
    void setup_code_editor();

    QLineEdit* setup_register_monitor_line_edit();
    void setup_register_monitor();
    void setup_memory_monitor();

    void setup_cpu_output_log();
    void setup_cpu_info_log();

    void setup_debug_buttons();
    void setup_menu_bar();

    void setup_action();

    QGridLayout *layout_main;
    QLineEdit *le_r0,*le_r1,*le_r2,*le_r3,*le_r4,*le_r5,*le_r6,*le_r7,*le_status,*le_pc;
    QLineEdit *le_ssp,*le_usp,*le_sbase,*le_ubase,*le_slimit,*le_ulimit,*le_intflags,*le_intbase,*le_timer,*le_treload;
    QPushButton *pb_dbg_load_binary,*pb_dbg_reset,*pb_dbg_step,*pb_dbg_run,*pb_dbg_break;
    QPlainTextEdit *txt_cpu_log,*txt_cpu_info;
    code_editor *code;
    QDockWidget *dw_cpu_output,*dw_cpu_info,*dw_debug_controls;

    QAction *aboutAction;

    m32_cpu *cpu;
};

#endif // IDE_MAIN_WINDOW_H
