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

#include "countdownwidget.h"

#define BLINKCOUNT 11
#define BLINKDELAY 250

CountdownWidget::CountdownWidget(
        QWidget *parent,
        QDateTime targetDate,
        int rFreq,
        bool animateEnd) :
    QLCDNumber(parent),
    endDate(targetDate),
    refreshRate(rFreq),
    timer(),
    animatedEnd(animateEnd),
    blinkCount(0),
    endTimer()
{
    delay.start();
    this->setAutoFillBackground(true);
    QPalette palette;
    // Set text color
    QBrush textBrush(QColor(3, 134, 0, 255));
    textBrush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, textBrush);
    // Set background color
    QBrush blackBrush(QColor(0, 0, 0, 255));
    blackBrush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Window, blackBrush);
    this->setPalette(palette);
    // Setup timer
    timer.setSingleShot(false);
    endTimer.setSingleShot(false);
    connect(&timer, SIGNAL(timeout()), this, SLOT(refresh()));
    connect(&endTimer, SIGNAL(timeout()), this, SLOT(endBlink()));
    this->refresh();
}

void CountdownWidget::startTimer()
{
    endDate = endDate.addMSecs(delay.elapsed());
    timer.start(refreshRate);
}

void CountdownWidget::stopTimer()
{
    delay.start();
    timer.stop();
}

void CountdownWidget::setTargetDateTime(QDateTime d)
{
    delay.start();
    endDate = d;
    refresh();
}

void CountdownWidget::scheduleLength(QTime t)
{
    QDateTime endDate = QDateTime::currentDateTime().addSecs(
                                                   t.second()
                                                   + t.minute()*60
                                                   + t.hour()*3600);
    setTargetDateTime(endDate);
}

void CountdownWidget::updateRefreshRate(int r)
{
    refreshRate = r;
    if (timer.isActive()) {
        stopTimer();
        startTimer();
    }
}

void CountdownWidget::refresh()
{
    QDateTime now = QDateTime::currentDateTime();
    if (now > endDate && timer.isActive()) {
        stopTimer();
        this->display("0.000");
        if (animatedEnd)  endTimer.start(BLINKDELAY);
        else              emit timeout();
        return;
    }
    // Extract the timestamp
    qint64 left = now.msecsTo(endDate);
    int ms = left % 1000;
    left = left/1000;
    int s = left%60;
    left = left/60;
    int m = left%60;
    int h = left/60;
    // Adapt the string length to its content (remove leading 0's before separators)
    if (h > 0) {
        // Compute the timestamp length
        // separators + msec + s + m + h with padding
        int length = 3 + 3 + 2 + 2 + qMax(h/10, 2);
        this->setDigitCount(length);
        this->display(QString("%1:%2:%3.%4")
                      .arg(QString::number(h),1,'0')
                      .arg(QString::number(m),2,'0')
                      .arg(QString::number(s),2,'0')
                      .arg(QString::number(ms),3,'0'));
    } else if (m > 0) {
        // separators + msec + s + m without padding
        int length = 2 + 3 + 2 + (m/10 ? 2 : 1);
        this->setDigitCount(length);
        this->display(QString("%1:%2.%3")
                      .arg(QString::number(m))
                      .arg(QString::number(s),2,'0')
                      .arg(QString::number(ms),3,'0'));
    } else {
        // separators + msec + s without padding
        int length = 1 + 3 + (s/10 ? 2 : 1);
        this->setDigitCount(length);
        this->display(QString("%1.%2")
                      .arg(QString::number(s))
                      .arg(QString::number(ms),3,'0'));
    }
}

bool CountdownWidget::isActive()
{
    return timer.isActive();
}

void CountdownWidget::endBlink()
{
    if (blinkCount - BLINKCOUNT) {
        if (!(blinkCount % 2)) this->display("");
        else this->display("0.000");
        ++blinkCount;
    } else {
        blinkCount = 0;
        endTimer.stop();
        emit timeout();
    }
}
