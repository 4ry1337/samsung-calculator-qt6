#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStack>
#include <QString>
#include <QStringLiteral>
#include <qcontainerfwd.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_clear();
  void on_digit();
  void on_dot();
  void on_backspace();
  void on_percent();
  void on_parentheses();
  void on_operator();
  void on_equal();

private:
  static constexpr int ROWS = 5;
  static constexpr int COLS = 4;

  static constexpr const char *ADD = "+";
  static constexpr const char *SUBTRACT = "-";
  static constexpr const char *MULTIPLY = "x";
  static constexpr const char *DIVIDE = "÷";

  static constexpr const char *PERCENT = "%";
  static constexpr const char *DOT = ".";

  static constexpr const char *PARENTHESES = "()";

  static constexpr const char *EQUAL = "=";
  static constexpr const char *CLEAR = "C";
  static constexpr const char *BACKSPACE = "<";

  QLabel *m_main_display = nullptr;
  QLabel *m_result_display = nullptr;

  QPushButton *m_buttons[ROWS][COLS];

  const QString m_button_layout[ROWS][COLS] = {
      {CLEAR, BACKSPACE, PARENTHESES, EQUAL},
      {"7", "8", "9", DIVIDE},
      {"4", "5", "6", MULTIPLY},
      {"1", "2", "3", SUBTRACT},
      {PERCENT, "0", DOT, ADD}};

  const QMap<QString, std::function<void()>> button_actions = {
      {CLEAR, [this]() { on_clear(); }},
      {EQUAL, [this]() { on_equal(); }},
      {ADD, [this]() { on_operator(); }},
      {SUBTRACT, [this]() { on_operator(); }},
      {MULTIPLY, [this]() { on_operator(); }},
      {DIVIDE, [this]() { on_operator(); }},
      {PARENTHESES, [this]() { on_parentheses(); }},
      {PERCENT, [this]() { on_percent(); }},
      {DOT, [this]() { on_dot(); }},
      {BACKSPACE, [this]() { on_backspace(); }},
      {"0", [this]() { on_digit(); }},
      {"1", [this]() { on_digit(); }},
      {"2", [this]() { on_digit(); }},
      {"3", [this]() { on_digit(); }},
      {"4", [this]() { on_digit(); }},
      {"5", [this]() { on_digit(); }},
      {"6", [this]() { on_digit(); }},
      {"7", [this]() { on_digit(); }},
      {"8", [this]() { on_digit(); }},
      {"9", [this]() { on_digit(); }}};
};

#endif // MAINWINDOW_H
