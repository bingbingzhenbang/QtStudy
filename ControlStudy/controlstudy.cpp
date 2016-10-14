#include "controlstudy.h"
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLayout>

ControlStudy::ControlStudy(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags), m_StartValue(0)
{
	m_pProgressBar = new QProgressBar();

	QDialogButtonBox *pBox = new QDialogButtonBox();
	m_pStartButton = new QPushButton(tr("Start/Continue"));
	m_pPauseButton = new QPushButton(tr("Pause"));
	pBox->addButton(m_pStartButton, QDialogButtonBox::AcceptRole);
	pBox->addButton(m_pPauseButton, QDialogButtonBox::AcceptRole);

	QVBoxLayout *pLayout = new QVBoxLayout();
	pLayout->addWidget(m_pProgressBar);
	pLayout->addWidget(pBox);
	setLayout(pLayout);

	m_timer.start(20);		

	connect(m_pStartButton, SIGNAL(clicked()), this, SLOT(StartClick()));
	connect(m_pPauseButton, SIGNAL(clicked()), this, SLOT(PauseClick()));
}

ControlStudy::~ControlStudy()
{

}

void ControlStudy::StartClick()
{
	m_pStartButton->setEnabled(false);
	m_pPauseButton->setEnabled(true);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(ProgressRun()));
}

void ControlStudy::PauseClick()
{
	if (m_pStartButton->isEnabled())
		return;
	m_pStartButton->setEnabled(true);
	m_pPauseButton->setEnabled(false);
	disconnect(&m_timer, SIGNAL(timeout()), this, SLOT(ProgressRun()));
}

void ControlStudy::ProgressRun()
{
	if (m_StartValue > 100)
		m_StartValue = 0;
	m_pProgressBar->setValue(m_StartValue++);
}
