
#include "mainwindow.h"
#include "exercise18.h"
#include "exercise19.h"

#include <QSettings>
#include <QListWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStatusBar>
#include <QDockWidget>
#include <QAction>

const QString settingsGeometry("Geometry");
const QString settingsState("State");
const QString settingsTabsIndex("TabIndex");


MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags)
:   QMainWindow(parent, flags)
,   m_tabs(NULL)
,   m_status(NULL)
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    
    QSettings s;
    restoreGeometry(s.value(settingsGeometry).toByteArray());
    restoreState(s.value(settingsState).toByteArray());

    setWindowTitle("3D Computergrafik I - SS2013 - Exercise 1");

    m_status = new QStatusBar();
    setStatusBar(m_status);

    m_tabs = new QTabWidget(this);
	
    setContentsMargins(4, 4, 4, 4);
    setCentralWidget(m_tabs);
	
    attachExercise(new Exercise18(), "Aufgabe 18");
    attachExercise(new Exercise19(), "Aufgabe 19");

    connect(m_tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    const int lastIndex = s.value(settingsTabsIndex, 0).toInt();
    
    if(lastIndex != 0)
        m_tabs->setCurrentIndex(lastIndex);
    else
        tabChanged(0);
}

MainWindow::~MainWindow()
{
    QSettings s;
    s.setValue(settingsGeometry, saveGeometry());
    s.setValue(settingsState, saveState());
    s.setValue(settingsTabsIndex, m_tabs->currentIndex());
}

void MainWindow::tabChanged(int index)
{
    for(int i = 0; i < m_exercises.size(); ++i)
        if(index != i && m_exercises[i]->isActive())
            m_exercises[i]->setActive(false);

    m_exercises[index]->setActive(true);
    m_status->showMessage(m_exercises[index]->hints());

    updateGeometry();
}

void MainWindow::attachExercise(
    AbstractGLExercise * exercise
,   const QString & name)
{
    m_tabs->addTab(exercise, name);
    m_exercises.push_back(exercise);
}
