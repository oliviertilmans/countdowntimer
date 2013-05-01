#ifndef COUNTDOWNWIDGET_H
#define COUNTDOWNWIDGET_H

#include <QLCDNumber>
#include <QDateTime>
#include <QTimer>
#include <QTime>

class CountdownWidget : public QLCDNumber
{
    Q_OBJECT
public:
    explicit CountdownWidget(
            QWidget *parent = 0,
            QDateTime target=QDateTime::currentDateTime().addSecs(3600),
            int refreshR=83);
    // Return true if the countdown is active (decreasing)
    bool isActive();

signals:
    // Emitted when the countdown expires
    void timeout();

public slots:
    // Start decreasing the countdown
    void startTimer();
    // Stop decreasing the countdown
    void stopTimer();
    // Schedule a countdown which will expire at the given date/time if it is never stopped;
    // If the timer is stopped/resumed, it will expire later than that date, according to how
    // long it has been stopped
    void setTargetDateTime(QDateTime);
    // Set up the frequency at which the countdown will be updated (in msec)
    void updateRefreshRate(int);

private slots:
    void refresh();

private:
    QDateTime endDate;
    int refreshRate;
    QTimer timer;
    QTime delay; // To deal with stop/resume
};

#endif // COUNTDOWNWIDGET_H
