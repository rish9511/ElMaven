/** @mainpage Project: Maven opensource 
*   @par MAVEN is an open source cross platform metabolomics data analyser.
*   The aim of this software packages is to reduce complexity of metabolomics 
*   analysis by developing a highly intitute interface for exploring and validating 
*   metabolomics data. The program features multi-file chromatographic aligner, 
*   peak-feature detector, isotope and adduct calculator, formula predictor, pathway
*   visualizer, and isotopic flux animator. Data from both triple quadropole and full
*   spectrum instruments is supported.
*
*   @par The project will not focus on developing or adding new features to the versions 
*   775 of MAVEN that is being currently in use by scientists at Agios. This version has 
*   numerous implementations of many features which enhance the utility of MAVEN.
*   Specifically, it allows for processing of many more samples (~100) for entire KEGG database 
*   (~1000 metabolites) for full scan high resolution data from QExactive
*/
/**
* @file main.cpp
*/
#define _STR(X) #X
#define STR(X) _STR(X)
#include "stable.h"
#include "globals.h"
#include "mainwindow.h"
#include "database.h"
#include "mzfileio.h"
#include "controller.h"
#include "elmavenlogger.h"
#include "common/autoupdate.h"

#ifdef Q_OS_MAC
#include <QDateTime>
#endif

#include "elmavexceptionhandler.h"

#include <QDir>
#include <list>
#include <QProcess>



Database DB;
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

void initializeLogger()
{
    QDir dir;
    QString path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QDir::separator() + \
                   qApp->organizationName() + QDir::separator() + qApp->applicationName() + QDir::separator() + qApp->sessionId() \
                   + QDir::separator() ;

    #ifdef Q_OS_MAC
        // session id is not available on mac os. therefore we use QDatetime to create a unique path for
        // every session
        path = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QDir::separator() + \
            qApp->organizationName() + QDir::separator() + qApp->applicationName() + QDir::separator() + QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss") \
            + QDir::separator() ;
    #endif

    // Logs  won't be saved anywhere if this mkpath returns False;
    dir.mkpath(path);
    ElMavenLogger::init(path.toStdString());
}
// Using maintenance tool for updates
// cons: online installer

// pros: works on all 3 platforms

// start a separate thread before creating mainwindow
// thread will be responsible for all update related operations
// 1. fetch the update file from the remote server
// 2. if a new version is available : every release should have version information available - use git describe --tags --abbrev=0
      //a. match version found in updates file with version stored in app
// 3. prompt for update(yes/no)
// 4. if user agrees, run the maintenance tool in silent upgrade mode
    //a. how to know the location of maintenance tool ?
    //b. to be able to run the tool in slient upgrade mode, something similar to installer scrip must be present on users system
    //c. once the update has been installed, the user should be prompted to restart the application
    //d. if the user agress to restart the applicaiton,
        //d.1 the current progress should be saved
        //d.2 stop the thread and close the application
    //e. if the user disagrees to restart the applicaiton
        //e.1 since the update is complete, we just need to exit the thread
// 5. if user disagrees, quit and stop the thread


// Maintenance tool does not provide a way to look at the progress
// Use colors instead, yellow while the update is going on, green for success, red for failure.
// The update should run in the background without affecting users work
// Once the update is complete user should be prompted to restart the application...


//

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    qApp->setOrganizationName("ElucidataInc");
    qApp->setApplicationName("El-Maven");
    qApp->setApplicationVersion(STR(EL_MAVEN_VERSION));
//    checkForUpdate();


    elmavexceptionhandler::init();

    QPixmap pixmap(":/images/splash.png","PNG",Qt::ColorOnly);
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();


    Controller contrl;
    qInstallMessageHandler(customMessageHandler);

    for (int i = 1; i < argc; ++i)
        contrl.getMainWindow()->fileLoader->addFileToQueue(QString(argv[i]));

    splash.finish(contrl.getMainWindow());
    contrl.getMainWindow()->show();

//    contrl.getMainWindow()->fileLoader->start();
    int rv = app.exec();
    return rv;


}


void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
        switch (type) {
        	case QtDebugMsg:
				cerr << "Debug: " << msg.toStdString() << endl;
                break;
        	case QtWarningMsg:
				cerr << "Warning: " << msg.toStdString() << endl;
                break;
        	case QtCriticalMsg:
				cerr << "Critical: " << msg.toStdString() << endl;
                break;
        	case QtFatalMsg:
                cerr << "Fatal: " << msg.toStdString() << endl;
				break;
                //abort();
        }
        //QFile outFile("debuglog.txt");
        //outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        //QTextStream ts(&outFile);
        //ts << txt << endl;
}

