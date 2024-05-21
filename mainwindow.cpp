#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建菜单栏
    QMenuBar *menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);

    // 创建文件菜单
    QMenu *fileMenu = menuBar->addMenu(tr("文件"));

    // 创建动作
    QAction *openAction = new QAction(tr("打开"), this);
    QAction *saveAction = new QAction(tr("保存"), this);
    QAction *saveAsAction = new QAction(tr("另存为"), this);

    // 将动作添加到菜单
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    // 连接动作的信号到槽函数
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, [this](){ saveFile(); });

    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("警告"), tr("无法打开文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    ui->textEdit->setPlainText(in.readAll());
    file.close();
}

void MainWindow::saveFile() {
    if (curFile.isEmpty()) {
        saveFileAs();
    } else {
        saveFile(curFile);
    }
}

void MainWindow::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    saveFile(fileName);
}

void MainWindow::saveFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("警告"), tr("无法写入文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();
    curFile = fileName;
    setWindowTitle(curFile);
}


