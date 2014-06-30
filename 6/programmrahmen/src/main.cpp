
#include "mainwindow.h"

#include <iostream>
#include <ctime>

#include <QApplication>
#include <QGLFormat>

int main(int argc, char* argv[])
{
    int result = -1;

    srand(time(NULL));

//    QGLFormat glFormat;
//    glFormat.setVersion(3, 2);
//    glFormat.setProfile(QGLFormat::CoreProfile);

//    // Set the default GL format to OpenGL 3.2 Core
//    QGLFormat::setDefaultFormat(glFormat);

    QApplication::setOrganizationName("Hasso-Plattner-Institut CGS");
    QApplication::setApplicationName("CG1 Exercise 6");

	QApplication * app = new QApplication(argc, argv);

	MainWindow * mw = new MainWindow();
    mw->show();

	result = app->exec();

	// Clean Up

	delete mw;
	delete app;

    return result;
}
