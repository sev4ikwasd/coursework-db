#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QTranslator>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("mysql");
    db.setPassword("541287");
    db.setDatabaseName("test");
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Couldn't connect to DB"),
            QObject::tr("Couldn't connect to db. "
            "Check if database is online and restart the application. "
            "Press \'Cancel\' to exit."), QMessageBox::Cancel);
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    /*const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "4-db-coursework_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }*/
    translator.load(":/i18n/4-db-coursework_ru_RU");
    a.installTranslator(&translator);

    if(!createConnection())
        return EXIT_FAILURE;

    MainWindow w;
    w.show();
    return a.exec();
}
