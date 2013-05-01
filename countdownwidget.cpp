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
    if (now > endDate) {
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
    // Compute the timestamp length
    if (h > 0) {
        // Compute the timestamp length
        // separators + msec + s + m + h
        int length = 3 + 3 + 2 + 2 + qMax(h/10, 2);
        this->setDigitCount(length);
        this->display(QString("%1:%2:%3.%4")
                      .arg(QString::number(h),1,'0')
                      .arg(QString::number(m),2,'0')
                      .arg(QString::number(s),2,'0')
                      .arg(QString::number(ms),3,'0'));
    } else if (m > 0) {
        int length = 2 + 3 + 2 + (m/10 ? 2 : 1);
        this->setDigitCount(length);
        this->display(QString("%1:%2.%3")
                      .arg(QString::number(m))
                      .arg(QString::number(s),2,'0')
                      .arg(QString::number(ms),3,'0'));
    } else {
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
