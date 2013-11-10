#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>

class code_editor;
class sidebar_area: public QWidget
{
    Q_OBJECT
public:
    sidebar_area(code_editor *editor);
    QSize sizeHint() const;
protected:
    void paintEvent(QPaintEvent *event);
private:
    code_editor *editor;
};

class code_editor: public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit code_editor(QWidget *parent = 0);
    void sidebar_area_paint_event(QPaintEvent *event);
    int sidebar_area_width();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void update_sidebar_area_width(int new_block_count);
    void highlight_current_line();
    void update_sidebar_area(const QRect&, int);

private:
    QWidget *sidebar;
};



#endif // CODE_EDITOR_H
