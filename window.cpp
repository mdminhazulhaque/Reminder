#include "window.h"
#include "ui_window.h"

// inherit QWidget to create a window
Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window)
{
    // init reminder_time to zero
    reminder_time = 0;
    // create a Popup window variable
    popup = new Popup;

    // create a system tray
    systemTrayIcon = new QSystemTrayIcon(this);
    // set it's icon
    systemTrayIcon->setIcon(QIcon(":/reminder-icon"));
    // show the tray icon
    systemTrayIcon->show();

    // create reminder window form
    ui->setupUi(this);
    // disable Stop button on startup
    ui->bStop->setDisabled(true);

    // call startTimer() when Start button is clicked
    connect(ui->bStart, SIGNAL(clicked()), this, SLOT(startTimer()));
    // call sopTimer() when Stop button is clicked
    connect(ui->bStop, SIGNAL(clicked()), this, SLOT(stopTimer()));
    // quit the application when Exit button is clicked
    connect(ui->bExit, SIGNAL(clicked()), qApp, SLOT(quit()));

    // show progress on 1s timer
    connect(&timer_progress, SIGNAL(timeout()), this, SLOT(showProgress()));
    // actual timer used by the application
    connect(&timer_reminder, SIGNAL(timeout()), this, SLOT(showReminder()));

    // when the cross button of the popup is clicked, call resetReminder()
    connect(popup, SIGNAL(closed()), this, SLOT(resetReminder()));
    // when the popup is clicked anywhere, call restartReminder()
    connect(popup, SIGNAL(clicked()), this, SLOT(restartReminder()));

    // show or hide the window when system tray icon is clicked
    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systemTrayIconActivated(QSystemTrayIcon::ActivationReason)));

    // center the window inside the desktop
    setGeometry(QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),
                    qApp->desktop()->availableGeometry()));

    setWindowTitle("Remind Me");
}

Window::~Window()
{
    delete ui;
}

void Window::resetReminder()
{
    // clear all data
    ui->about->clear();
    ui->timeAfter->setTime(QTime(0,0,0));
    ui->timeAfter->setFocus();
}

void Window::restartReminder()
{
    // show the window and let user set custom time
    ui->timeAfter->setFocus();
    showNormal();
}

void Window::startTimer()
{
    // get time value from user
    QTime after = ui->timeAfter->time();
    // convert it into microseconds
    reminder_time = (after.hour()*3600 + after.minute()*60 + after.second()) * 1000;

    // if the time is less than 1 second or 1 microsecond
    if(reminder_time < 1000)
    {
        // show error
        QMessageBox::critical(this,
                              "Error",
                              "Cannot set timer for less than 1s");
        return;
    }

    // start the timer with user specified time
    timer_reminder.start(reminder_time);
    // set progressbar maximum value
    ui->progressBar->setMaximum(reminder_time);
    ui->progressBar->setValue(reminder_time);
    // start updating progress
    showProgress();
    timer_progress.start(1000);

    // enable the progressbar, disable Start button, enable Stop button
    ui->progressBar->setEnabled(true);
    ui->bStart->setDisabled(true);
    ui->bStop->setEnabled(true);

    // hide itself
    close();
}

void Window::stopTimer()
{
    // stop everything
    timer_progress.stop();
    timer_reminder.stop();
    ui->progressBar->reset();
    // disable the progressbar
    ui->progressBar->setEnabled(false);
    // enable Start button
    ui->bStart->setDisabled(false);
    // disable Stop button
    ui->bStop->setEnabled(false);
}

void Window::showReminder()
{
    // stop the timer
    stopTimer();
    // show desktop popup
    popup->showPopup(
                "Reminder",
                ui->about->toPlainText());
}

void Window::showProgress()
{
    // update values to the progressbar
    ui->progressBar->setValue(timer_reminder.remainingTime());
}

void Window::systemTrayIconActivated(QSystemTrayIcon::ActivationReason)
{
    // if the window is hidden, show it
    if(!isVisible())
        showNormal();
    // else hide it
    else
        close();
}
