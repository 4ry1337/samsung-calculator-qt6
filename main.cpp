#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;

  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "calc-qt6_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

  MainWindow w;
  w.show();

  return a.exec();
}
