#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>
#include <QTimer>
#include <QSettings>

class Widget : public QMainWindow
{
    Q_OBJECT
    
public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    void initMenu();
    void initWidgets();
    void initConnexions();

private slots:
    void toggleReading();
    void startReading();
    void stopReading();
    void nextWord();
    void slower();
    void faster();

private:
    QPlainTextEdit* m_text;
    QLabel* m_label;

    QTimer* m_timer;
    QSettings m_set;
};

#endif // WIDGET_H
