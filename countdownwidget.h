/*
 * Copyright (c) 2013, Olivier Tilmans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **/

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
            int refreshR=83,
            bool animateEnd=true);
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
    // Schedule the countdown duration to the given time (! QTime is always < 24h); will ignore msec
    void scheduleLength(QTime);

private slots:
    void refresh();
    void endBlink();

private:
    QDateTime endDate;
    int refreshRate;
    QTimer timer;
    QTime delay; // To deal with stop/resume
    bool animatedEnd;
    int blinkCount;
    QTimer endTimer;
};

#endif // COUNTDOWNWIDGET_H
