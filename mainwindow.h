#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QPushButton>
#include <functional>

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
  void on_change_sign();
  void on_pow();
  void on_sqrt();
  void on_add();
  void on_subtract();
  void on_equal();
  void on_comma();
  void on_digit(); // Handles all digit buttons dynamically

private:
  QWidget *centralWidget;
  QLabel *historyDisplay;
  QLabel *mainDisplay;
  QVBoxLayout *mainLayout;
  QGridLayout *gridLayout;
  QLabel *label;

  const QString button_order[5][4] = {{"AC", "+/-", "xʸ", "√x"},
                                      {"7", "8", "9", "/"},
                                      {"4", "5", "6", "*"},
                                      {"1", "2", "3", "-"},
                                      {"0", ".", "=", "+"}};

  QPushButton *buttons[5][4];

  // Static map of button actions
  const QMap<QString, std::function<void()>> button_actions = {
      {"AC", [this]() { on_clear(); }},
      {"+/-", [this]() { on_change_sign(); }},
      {"xʸ", [this]() { on_pow(); }},
      {"√x", [this]() { on_sqrt(); }},
      {"+", [this]() { on_add(); }},
      {"-", [this]() { on_subtract(); }},
      {"=", [this]() { on_equal(); }},
      {".", [this]() { on_comma(); }},

      // Digits are mapped to the same function
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
