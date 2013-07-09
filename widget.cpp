#include "widget.h"
#include <QVBoxLayout>
#include <QStatusBar>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QMainWindow(parent)
{
    initMenu();
    initWidgets();
    initConnexions();

    statusBar();

    m_timer = new QTimer(this);
    m_timer->setInterval(m_set.value("interval", 1000).toInt());
    connect(m_timer, &QTimer::timeout, this, &Widget::nextWord);
}

Widget::~Widget()
{
    m_set.setValue("interval", m_timer->interval());
}

#include <QMenuBar>
void Widget::initMenu()
{
    QMenu* command = menuBar()->addMenu("Control");
    command->addAction("Start / Stop", this, SLOT(toggleReading()), QKeySequence(Qt::Key_F4));
    command->addAction("Slower", this, SLOT(slower()), QKeySequence(Qt::Key_F5));
    command->addAction("Faster", this, SLOT(faster()), QKeySequence(Qt::Key_F6));
}

void Widget::initWidgets()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);

    m_text = new QPlainTextEdit(this);

    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    pal = m_label->palette();
    pal.setColor(QPalette::WindowText, Qt::white);
    m_label->setPalette(pal);
    QFont font = m_label->font();
    font.setPointSize(70);
    m_label->setFont(font);


    setCentralWidget(new QWidget(this));
    QVBoxLayout* l = new QVBoxLayout(centralWidget());
    l->addWidget(new QLabel("Past your text here and press F4.", this));
    l->addWidget(m_text);
    l->addWidget(m_label);
}

void Widget::initConnexions()
{
    connect(m_text, &QPlainTextEdit::textChanged, this, &Widget::stopReading);
}

void Widget::toggleReading()
{
    if (!m_timer->isActive())
        startReading();
    else
        stopReading();
}

void Widget::startReading()
{
    QPalette pal;
    pal.setColor(QPalette::Base, Qt::darkGray);
    pal.setColor(QPalette::Text, Qt::gray);
    m_text->setPalette(pal);

    statusBar()->showMessage("Start reading", 1000);

    QTextCursor cur = m_text->textCursor();

    if (cur.atEnd())
        cur.movePosition(QTextCursor::Start);
    cur.select(QTextCursor::WordUnderCursor);

    m_label->setText(cur.selectedText());
    m_text->setTextCursor(cur);

    m_timer->start();
}

void Widget::stopReading()
{
    m_text->setPalette(QPalette());

    statusBar()->showMessage("Stop reading", 1000);
    m_timer->stop();
}

void Widget::nextWord()
{
    QTextCursor cur = m_text->textCursor();

    cur.movePosition(QTextCursor::NextWord);
    cur.select(QTextCursor::WordUnderCursor);

    m_label->setText(cur.selectedText());
    m_text->setTextCursor(cur);

    if (cur.atEnd())
        stopReading();
}

void Widget::slower()
{
    int i = m_timer->interval();
    i += qMax(1, i / 10);
    statusBar()->showMessage(QString("interval : %1").arg(i), 1000);
    m_timer->setInterval(i);
}

void Widget::faster()
{
    int i = m_timer->interval();
    i -= qMax(1, i / 10);
    statusBar()->showMessage(QString("interval : %1").arg(i), 1000);
    m_timer->setInterval(i);
}
