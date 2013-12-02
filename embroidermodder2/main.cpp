#include "mainwindow.h"
#include <QApplication>
#include <QLocale>

const char* _appName_ = "Embroidermodder";
const char* _appVer_  = "v2.0 alpha";
bool exitApp = false;

static void usage(void)
{
    fprintf(stderr,
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ "           "\n"
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\" "\n"
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/"           "\n"
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|"   "\n"
    " _____________________________________________________________________________ "           "\n"
    "|                                                                             | "          "\n"
    "|                   http://embroidermodder.sourceforge.net                    | "          "\n"
    "|_____________________________________________________________________________| "          "\n"
    "                                                                               "           "\n"
    "Usage: embroidermodder [options] files ..."                                      "\n"
   //80CHARS======================================================================MAX
    "Options:"                                                                        "\n"
    "  -d, --debug      Print lots of debugging information."                         "\n"
    "  -h, --help       Print this message and exit."                                 "\n"
    "  -v, --version    Print the version number of embroidermodder and exit."        "\n"
    "\n"
           );
    exitApp = true;
}

static void version()
{
    fprintf(stdout, "%s %s\n", _appName_, _appVer_);
    exitApp = true;
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(_appName_);
    app.setApplicationVersion(_appVer_);

    QString lang = QLocale::system().languageToString(QLocale::system().language()).toLower();
    qDebug("language: %s", qPrintable(lang));

    //Load translations for the Embroidermodder 2 GUI
    QTranslator translatorEmb;
    translatorEmb.load("translations/" + lang + "/embroidermodder2_" + lang);
    app.installTranslator(&translatorEmb);

    //Load translations for the commands
    QTranslator translatorCmd;
    translatorCmd.load("translations/" + lang + "/commands_" + lang);
    app.installTranslator(&translatorCmd);

    //Load translations provided by Qt - this covers dialog buttons and other common things.
    QTranslator translatorQt;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translatorQt);

    QStringList filesToOpen;

    for(int i = 1; i < argc; i++)
    {
        if     (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")  ) {  }
        else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")   ) { usage(); }
        else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) { version(); }
        else if(QFile::exists(argv[i]) && MainWindow::validFileFormat(argv[i]))
        {
            filesToOpen << argv[i];
        }
        else
        {
            usage();
        }
    }

    if(exitApp)
        return 1;

    MainWindow* mainWin = new MainWindow();

    QObject::connect(&app, SIGNAL(lastWindowClosed()), mainWin, SLOT(quit()));

    mainWin->setWindowTitle(app.applicationName() + " " + app.applicationVersion());
    mainWin->show();

    //NOTE: If openFilesSelected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
    if(!filesToOpen.isEmpty())
        mainWin->openFilesSelected(filesToOpen);

    return app.exec();
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
