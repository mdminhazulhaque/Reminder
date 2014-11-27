#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QSystemTrayIcon>

#include "popup.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

public slots:
    void resetReminder();
    void restartReminder();

private slots:
    void startTimer();
    void stopTimer();
    void showReminder();
    void showProgress();
    void systemTrayIconActivated(QSystemTrayIcon::ActivationReason);

private:
    Ui::Window *ui;
    QTimer timer_progress;
    QTimer timer_reminder;
    uint reminder_time;
    QSystemTrayIcon *systemTrayIcon;
    Popup *popup;
};

#endif // WIDGET_H
