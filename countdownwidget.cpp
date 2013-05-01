#include "countdownwidget.h"

CountdownWidget::CountdownWidget(
        QWidget *parent,
        QDateTime targetDate,
        int rFreq) :
    QLCDNumber(parent),
    endDate(targetDate),
    refreshRate(rFreq),
    timer()
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
    connect(&timer, SIGNAL(timeout()), this, SLOT(refresh()));
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
    endDate = d;
    refresh();
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
        this->display("00:00:00.000");
        stopTimer();
        emit timeout();
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
    int length = 3+3+2+2+ qMax(h/10, 2);
    this->setDigitCount(length);
    this->display(QString("%1:%2:%3.%4")
                  .arg(QString::number(h),2,'0')
                  .arg(QString::number(m),2,'0')
                  .arg(QString::number(s),2,'0')
                  .arg(QString::number(ms),3,'0'));
}

bool CountdownWidget::isActive()
{
    return timer.isActive();
}
