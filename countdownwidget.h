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

    bool isActive();

signals:
    void timeout();

public slots:
    void startTimer();
    void stopTimer();
    void setTargetDateTime(QDateTime);
    void updateRefreshRate(int);

private slots:
    void refresh();

private:
    QDateTime endDate;
    int refreshRate;
    QTimer timer;
    QTime delay;
};

#endif // COUNTDOWNWIDGET_H
