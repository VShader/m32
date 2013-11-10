#include "code_editor.h"

sidebar_area::sidebar_area(code_editor *editor): editor(editor)
{
}

QSize sidebar_area::sizeHint() const
{
    return QSize(editor->sidebar_area_width(),0);
}

void sidebar_area::paintEvent(QPaintEvent *event)
{
    editor->sidebar_area_paint_event(event);
}

code_editor::code_editor(QWidget *parent): QPlainTextEdit(parent)
{
    sidebar=new sidebar_area(this);
    connect(this,&code_editor::blockCountChanged,this,&code_editor::update_sidebar_area_width);
    connect(this,&code_editor::updateRequest,this,&code_editor::update_sidebar_area);
    connect(this,&code_editor::cursorPositionChanged,this,&code_editor::highlight_current_line);

    update_sidebar_area_width(0);
    highlight_current_line();
}

void code_editor::sidebar_area_paint_event(QPaintEvent *event)
{
    QPainter painter(sidebar);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block=firstVisibleBlock();
    int block_number=block.blockNumber();
    int top=(int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom=top+(int)blockBoundingRect(block).height();

    while(block.isValid()&&top<=event->rect().bottom())
    {
        if(block.isVisible()&&bottom>=event->rect().top())
        {
            QString number=QString::number(block_number+1);
            painter.setPen(Qt::black);
            painter.drawText(0,top,sidebar->width(),fontMetrics().height(),Qt::AlignRight,number);
        }

        block=block.next();
        top=bottom;
        bottom=top+(int)blockBoundingRect(block).height();
        block_number++;
    }
}

int code_editor::sidebar_area_width()
{
    int digits=1;
    int max=qMax(1,blockCount());
    while(max>=10)
    {
        max/=10;
        digits++;
    }

    int space=3+fontMetrics().width(QLatin1Char('9'))*digits;
    return space;
}

void code_editor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    sidebar->setGeometry(QRect(cr.left(),cr.top(),sidebar_area_width(),cr.height()));
}

void code_editor::update_sidebar_area_width(int /*unused*/)
{
    setViewportMargins(sidebar_area_width(),0,0,0);
}

void code_editor::highlight_current_line()
{
    QList<QTextEdit::ExtraSelection> extra_selections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extra_selections.append(selection);
    }
    setExtraSelections(extra_selections);
}

void code_editor::update_sidebar_area(const QRect &rect, int dy)
{
    if(dy)
        sidebar->scroll(0, dy);
    else
        sidebar->update(0,rect.y(),sidebar->width(),rect.height());

    if(rect.contains(viewport()->rect()))
        update_sidebar_area_width(0);
}
