#include "mainwindow.h"

MainWindow::~MainWindow() {}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setup_ui();
  connect_buttons();
}

void MainWindow::setup_ui() {
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

void MainWindow::connect_buttons() {
  const std::unordered_map<QString, std::function<void()>> buttonActions{
      {CLEAR, [this] { on_clear(); }},
      {EQUAL, [this] { on_equal(); }},
      {DOT, [this] { on_dot(); }},
      {BACKSPACE, [this] { on_backspace(); }},
      {PERCENT, [this] { on_percent(); }},
      {PARENTHESES, [this] { on_parentheses(); }}};

  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLS; ++j) {
      if (!m_buttons[i][j])
        continue;
      const auto &label = m_button_layout[i][j];
      if (auto it = buttonActions.find(label); it != buttonActions.end()) {
        connect(m_buttons[i][j], &QPushButton::clicked, this, it->second);
      } else if (m_operators.count(label) != 0) {
        connect(m_buttons[i][j], &QPushButton::clicked, this,
                &MainWindow::on_operator);
      } else if (label[0].isDigit()) {
        connect(m_buttons[i][j], &QPushButton::clicked, this,
                &MainWindow::on_digit);
      }
    }
  }
}

void MainWindow::on_clear() {
  update_display("0");
  m_result_display->setText("");
  m_last_dot = false;
  m_parenthesis_count = 0;
}

void MainWindow::on_equal() {
  QString expression = m_main_display->text();
  QString error = validate_expression(expression);

  if (!error.isEmpty()) {
    m_result_display->setText(error);
    return;
  }

  try {
    double result = evaluate_expression(expression);
    if (std::isinf(result) || std::isnan(result)) {
      m_result_display->setText("Division by zero");
      return;
    }
    m_main_display->setText(QString::number(result));
    m_result_display->setText("");
  } catch (const std::runtime_error &e) {
    m_result_display->setText(e.what());
  } catch (...) {
    m_result_display->setText("Error");
  }
  m_last_dot = false;
  m_parenthesis_count = 0;
}

void MainWindow::on_backspace() {
  QString expression = m_main_display->text();

  if (expression.isEmpty())
    return;

  if (expression.back() == DOT) {
    m_last_dot = false;
  } else if (expression.back() == ')') {
    m_parenthesis_count++;
  } else if (expression.back() == '(') {
    m_parenthesis_count--;
  }
  expression.chop(1);
  update_display(expression);
}

void MainWindow::on_parentheses() {
  QString expression = m_main_display->text();
  if (expression.isEmpty() || expression == "0") {
    expression = '(';
    m_parenthesis_count++;
  } else if (is_operator(expression) || expression.back() == '(') {
    expression += '(';
    m_parenthesis_count++;
  } else if (m_parenthesis_count > 0) {
    expression += ')';
    m_parenthesis_count--;
  } else {
    expression += QString(MULTIPLY) + '(';
    m_parenthesis_count++;
  }
  m_last_dot = false;
  update_display(expression);
}

void MainWindow::on_percent() {
  QString expression = m_main_display->text();

  if (expression.isEmpty() || is_operator(expression) ||
      is_parenthesis(expression.back()) || expression.back() == PERCENT)
    return;

  if (expression.back() == DOT)
    expression.chop(1);

  expression += PERCENT;
  update_display(expression);
  m_last_dot = false;
}

void MainWindow::on_operator() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (!button)
    return;
  QString op = button->text();
  QString expression = m_main_display->text();

  if (expression.isEmpty())
    return;

  if (is_operator(expression) || expression.back() == DOT) {
    expression.chop(1);
    update_display(expression);
  }

  if (expression.back() == '(' && !(op == ADD || op == SUBTRACT))
    return;

  update_display(expression + op);
  m_last_dot = false;
}

void MainWindow::on_dot() {
  QString expression = m_main_display->text();
  if (!m_last_dot && !is_operator(expression) &&
      !is_parenthesis(expression.back()) && expression.back() != PERCENT) {
    expression += DOT;
    update_display(expression);
    m_last_dot = true;
  }
}

void MainWindow::on_digit() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (!button)
    return;
  QString digit = button->text();
  QString expression = m_main_display->text();

  auto findLastDelimiter = [&](const QString &expr) -> int {
    static const QString delimiters = "+-x÷()%";
    for (int i = expr.size() - 1; i >= 0; --i) {
      if (delimiters.contains(expr[i])) {
        return i;
      }
    }
    return -1; // No delimiter found
  };

  if (expression.isEmpty()) {
    update_display(digit);
    return;
  }

  if (expression.back() == ')' || expression.back() == '%') {
    expression += MULTIPLY;
  }

  int delimiterIndex = findLastDelimiter(expression);
  QString currentToken = expression.mid(delimiterIndex + 1);
  if (currentToken == "0") {
    if (digit == "0") {
      return;
    } else {
      expression.chop(1);
      expression += digit;
      update_display(expression);
      return;
    }
  }
  update_display(expression + digit);
}

QString MainWindow::validate_expression(QStringView expression) const {
  int parenthesisCount = 0;
  bool lastWasOperator = false;

  for (int i = 0; i < expression.length(); ++i) {
    QChar ch = expression[i];

    if (ch == '(') {
      parenthesisCount++;
      lastWasOperator = false;
    } else if (ch == ')') {
      if (parenthesisCount == 0) {
        return "Unmatched parenthesis";
      }
      parenthesisCount--;
      lastWasOperator = false;
    } else if (m_operators.count(QString(ch)) != 0) {
      if (lastWasOperator) {
        return "Invalid format";
      }
      lastWasOperator = true;
    } else if (ch.isDigit() || ch == '.' || ch == '%') {
      lastWasOperator = false;
    } else {
      return "Invalid character";
    }
  }

  if (parenthesisCount != 0) {
    return "Unmatched parentheses";
  }
  if (lastWasOperator) {
    return "Invalid format";
  }

  return "";
}

bool MainWindow::is_operator(QStringView expression) const {
  return m_operators.count(expression.back()) != 0;
}

bool MainWindow::is_parenthesis(QChar ch) const {
  return ch == '(' || ch == ')';
}

int MainWindow::precedence(QStringView op) {
  if (op == "+" || op == "-")
    return 1;
  if (op == "x" || op == "÷")
    return 2;
  return 0;
}

double MainWindow::evaluate_expression(const QString &expression) const {
  QStack<double> values;
  QStack<QString> operators;
  QString currentNumber;

  auto processOperator = [&values, &operators](const QString &op) {
    double val2 = values.pop();
    double val1 = values.pop();

    switch (op.front().unicode()) {
    case '+':
      values.push(val1 + val2);
      break;
    case '-':
      values.push(val1 - val2);
      break;
    case 'x':
      values.push(val1 * val2);
      break;
    case L'÷':
      values.push(val1 / val2);
      break;
    }
  };

  for (const QChar &ch : expression) {
    if (ch.isDigit() || ch == '.') {
      currentNumber += ch;
      continue;
    }

    if (!currentNumber.isEmpty()) {
      values.push(currentNumber.toDouble());
      currentNumber.clear();
    }

    if (ch == '(') {
      operators.push(QString(ch));
    } else if (ch == ')') {
      while (!operators.isEmpty() && operators.top() != "(") {
        processOperator(operators.pop());
      }
      operators.pop();
    } else if (m_operators.count(QString(ch)) != 0) {
      while (!operators.isEmpty() &&
             precedence(operators.top()) >= precedence(QString(ch))) {
        processOperator(operators.pop());
      }
      operators.push(QString(ch));
    } else if (ch == '%') {
      if (!values.isEmpty()) {
        double val = values.pop();
        if (!operators.isEmpty() &&
            (operators.top() == "+" || operators.top() == "-")) {
          values.push(val / 100.0 * values.top());
        } else {
          values.push(val / 100.0);
        }
      }
    }
  }

  if (!currentNumber.isEmpty()) {
    values.push(currentNumber.toDouble());
  }

  while (!operators.isEmpty()) {
    processOperator(operators.pop());
  }

  return values.isEmpty() ? 0.0 : values.top();
}

void MainWindow::update_display(const QString &expression) {
  m_main_display->setText(expression);
  m_result_display->setText(expression);

  if (expression.isEmpty() || expression == "0") {
    return;
  }

  if (validate_expression(expression).isEmpty() &&
      evaluate_partial_expression(expression)) {
    QString partialExpr = partial_expression(expression);
    try {
      double result = evaluate_expression(partialExpr);
      m_result_display->setText(QString::number(result));
    } catch (...) {
      m_result_display->clear();
    }
  }
}

bool MainWindow::evaluate_partial_expression(const QString &expression) const {
  if (expression.isEmpty())
    return false;

  QChar lastChar = expression.back();
  if (m_operators.count(QString(lastChar)) != 0)
    return false;
  if (lastChar == '.')
    return false;

  int parenthesesCount = 0;
  for (const QChar &ch : expression) {
    if (ch == '(')
      parenthesesCount++;
    else if (ch == ')')
      parenthesesCount--;
    if (parenthesesCount < 0)
      return false;
  }

  return true;
}

QString MainWindow::partial_expression(const QString &expression) const {
  if (expression.back().isDigit() || expression.back() == '%') {
    return expression;
  }

  QString completeExpr = expression;
  int parenthesesCount = 0;

  for (const QChar &ch : expression) {
    if (ch == '(')
      parenthesesCount++;
    else if (ch == ')')
      parenthesesCount--;
  }

  while (parenthesesCount > 0) {
    completeExpr += ")";
    parenthesesCount--;
  }

  return completeExpr;
}
