#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->setWindowTitle("Calculator");
  this->resize(300, 400);
  centralWidget = new QWidget(this);
  this->setCentralWidget(centralWidget);

  mainLayout = new QVBoxLayout();

  historyDisplay = new QLabel("");
  historyDisplay->setAlignment(Qt::AlignBottom | Qt::AlignRight);
  mainLayout->addWidget(historyDisplay);

  mainDisplay = new QLabel("0");
  mainDisplay->setAlignment(Qt::AlignBottom | Qt::AlignRight);
  mainLayout->addWidget(mainDisplay);
  gridLayout = new QGridLayout();

  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 4; col++) {
      QString label = button_order[row][col];

      buttons[row][col] = new QPushButton(label);
      buttons[row][col]->setSizePolicy(QSizePolicy::Preferred,
                                       QSizePolicy::Expanding);
      gridLayout->addWidget(buttons[row][col], row, col);

      connect(buttons[row][col], &QPushButton::clicked, this, [this, label]() {
        if (button_actions.contains(label)) {
          button_actions[label]();
        }
      });
    }
  }

  mainLayout->addLayout(gridLayout);
  centralWidget->setLayout(mainLayout);
}

MainWindow::~MainWindow() {}

void MainWindow::on_clear() {}

void MainWindow::on_change_sign() {}

void MainWindow::on_pow() {}

void MainWindow::on_sqrt() {}

void MainWindow::on_add() {}

void MainWindow::on_subtract() {}

void MainWindow::on_equal() {}

void MainWindow::on_comma() {}

void MainWindow::on_digit() {}
