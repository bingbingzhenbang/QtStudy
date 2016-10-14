#ifndef CONTROLSTUDY_H
#define CONTROLSTUDY_H

#include <QtWidgets/QDialog>
#include <QtCore/Qtimer>

class QPushButton;
class QProgressBar;

class ControlStudy : public QDialog
{
	Q_OBJECT

public:
	ControlStudy(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ControlStudy();

private slots:
	void StartClick();
	void PauseClick();
	void ProgressRun();

private:
	QPushButton *m_pStartButton;
	QPushButton *m_pPauseButton;
	QProgressBar *m_pProgressBar;
	int m_StartValue;
	QTimer m_timer;
};

#endif // CONTROLSTUDY_H
