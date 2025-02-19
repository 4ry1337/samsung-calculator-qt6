#include "mainwindow.h"

MainWindow::~MainWindow() {}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Calculator");
  resize(300, 400);

  auto *central_widget = new QWidget(this);
  setCentralWidget(central_widget);

  auto *main_layout = new QVBoxLayout();

  m_main_display = new QLabel("0");
  m_main_display->setFont(QFont("Arial", 32, QFont::Bold));
  m_main_display->setAlignment(Qt::AlignBottom | Qt::AlignRight);
  main_layout->addWidget(m_main_display);

  m_result_display = new QLabel("");
  m_result_display->setFont(QFont("Arial", 18, QFont::Bold));
  m_result_display->setAlignment(Qt::AlignBottom | Qt::AlignRight);
  main_layout->addWidget(m_result_display);

  auto *grid_layout = new QGridLayout();
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      const auto &label = m_button_layout[i][j];
      if (label.isEmpty())
        continue;

      m_buttons[i][j] = new QPushButton(label);
      m_buttons[i][j]->setSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::Expanding);
      m_buttons[i][j]->setFont(QFont("Arial", 12, QFont::Normal));
      grid_layout->addWidget(m_buttons[i][j], i, j);
    }
  }
  main_layout->addLayout(grid_layout);
  central_widget->setLayout(main_layout);
}
