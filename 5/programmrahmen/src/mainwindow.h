
#pragma once

#include <QMainWindow>
#include <QVector>

class AbstractExercise;
class AbstractGLExercise;
class QDockWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(
		QWidget * parent = NULL
    ,   Qt::WindowFlags flags = NULL);

	virtual ~MainWindow();

protected slots:
    void tabChanged(int index);

protected:
	void attachExercise(
        AbstractGLExercise * exercise
    ,   const QString & name);

    QStatusBar * m_status;
    QTabWidget * m_tabs;

	QVector<QDockWidget*> m_dockWidgets;
    QVector<AbstractExercise*> m_exercises;
};