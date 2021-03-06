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

#include "mainwindow.h"
#include "countdownwidget.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow),
    lcdNumber(new CountdownWidget(this)),
    fs(false)
{
    ui->setupUi(this);
    ui->centralWidget->layout()->addWidget(lcdNumber);
    ui->time->setTime(QTime(1,0));
    connect(ui->time, SIGNAL(timeChanged(QTime)), this, SLOT(setTargetDuration()));
    connect(ui->fsButton, SIGNAL(clicked()), this, SLOT(goFS()));
    connect(ui->timerBtn, SIGNAL(clicked()), this, SLOT(toggleTimer()));
    connect(lcdNumber, SIGNAL(timeout()), this, SLOT(restartTimer()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTargetDuration()
{
    lcdNumber->scheduleLength(ui->time->time());
}

void MainWindow::goFS()
{
    ui->controlWidget->hide();
    this->showFullScreen();
    fs = true;
}

void MainWindow::leaveFS()
{
    this->showNormal();
    ui->controlWidget->show();
    fs = false;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (fs) {
        leaveFS();
        e->accept();
    } else QMainWindow::mouseReleaseEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Space) toggleTimer();
    else if (fs) {
        leaveFS();
        e->accept();
    } else QMainWindow::keyReleaseEvent(e);
}

void MainWindow::toggleTimer()
{
    if (lcdNumber->isActive()) {
        lcdNumber->stopTimer();
        ui->timerBtn->setText("Start countdown");
    } else {
        lcdNumber->startTimer();
        ui->timerBtn->setText("Stop countdown");
    }
}

void MainWindow::restartTimer()
{
    ui->timerBtn->setText("Restart countdown");
    setTargetDuration();
}
