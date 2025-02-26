#include "main_diat.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "global_config.h"
#include "q_grab_windows.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //q_grab_windows s;
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "vedio_rec_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Widget w;
    w.show();
    //s.show();
    return a.exec();
}
