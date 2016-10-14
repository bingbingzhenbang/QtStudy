#include "quaternionconvertor.h"
#include <QQuaternion>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLayout>
#include <cmath>

QuaternionConvertor::QuaternionConvertor(QWidget *parent)
	: QDialog(parent)
{
	QLabel *pfromLabel = new QLabel("From :");
	QHBoxLayout *playout1 = new QHBoxLayout();
	playout1->addWidget(pfromLabel);
	QLabel *pfromx = new QLabel("X :");
	m_pFromX = EditorFactory(true);
	QLabel *pfromy = new QLabel("Y :");
	m_pFromY = EditorFactory(true);
	QLabel *pfromz = new QLabel("Z :");
	m_pFromZ = EditorFactory(true);
	QHBoxLayout *playout2 = new QHBoxLayout();
	playout2->addWidget(pfromx);
	playout2->addWidget(m_pFromX);
	playout2->addWidget(pfromy);
	playout2->addWidget(m_pFromY);
	playout2->addWidget(pfromz);
	playout2->addWidget(m_pFromZ);

	QLabel *ptoLabel = new QLabel("To :");
	QHBoxLayout *playout3 = new QHBoxLayout();
	playout3->addWidget(ptoLabel);
	QLabel *ptox = new QLabel("X :");
	m_pToX = EditorFactory(true);
	QLabel *ptoy = new QLabel("Y :");
	m_pToY = EditorFactory(true);
	QLabel *ptoz = new QLabel("Z :");
	m_pToZ = EditorFactory(true);
	QHBoxLayout *playout4 = new QHBoxLayout();
	playout4->addWidget(ptox);
	playout4->addWidget(m_pToX);
	playout4->addWidget(ptoy);
	playout4->addWidget(m_pToY);
	playout4->addWidget(ptoz);
	playout4->addWidget(m_pToZ);

	QLabel *pquaternionLabel = new QLabel("Quaternion :");
	QHBoxLayout *pLayout5 = new QHBoxLayout();
	pLayout5->addWidget(pquaternionLabel);
	QLabel *ps = new QLabel("S :");
	m_pS = EditorFactory(false);
	QLabel *px = new QLabel("X :");
	m_pX = EditorFactory(false);
	QLabel *py = new QLabel("Y :");
	m_pY = EditorFactory(false);
	QLabel *pz = new QLabel("Z :");
	m_pZ = EditorFactory(false);
	QHBoxLayout *pLayout6 = new QHBoxLayout();
	pLayout6->addWidget(ps);
	pLayout6->addWidget(m_pS);
	pLayout6->addWidget(px);
	pLayout6->addWidget(m_pX);
	pLayout6->addWidget(py);
	pLayout6->addWidget(m_pY);
	pLayout6->addWidget(pz);
	pLayout6->addWidget(m_pZ);

	QLabel *peulerLabel = new QLabel("Euler Angles :");
	QHBoxLayout *pLayout7 = new QHBoxLayout();
	pLayout7->addWidget(pquaternionLabel);
	QLabel *proll = new QLabel("Roll :");
	m_pRoll = EditorFactory(false);
	QLabel *ppitch = new QLabel("Pitch :");
	m_pPitch = EditorFactory(false);
	QLabel *pyaw = new QLabel("Yaw :");
	m_pYaw = EditorFactory(false);
	QHBoxLayout *pLayout8 = new QHBoxLayout();
	pLayout8->addWidget(proll);
	pLayout8->addWidget(m_pRoll);
	pLayout8->addWidget(ppitch);
	pLayout8->addWidget(m_pPitch);
	pLayout8->addWidget(pyaw);
	pLayout8->addWidget(m_pYaw);

	QLabel *perrorquaternionLabel = new QLabel("Error Quaternion :");
	QHBoxLayout *pLayout9 = new QHBoxLayout();
	pLayout9->addWidget(perrorquaternionLabel);
	QLabel *pes = new QLabel("S :");
	m_pES = EditorFactory(false);
	QLabel *pex = new QLabel("X :");
	m_pEX = EditorFactory(false);
	QLabel *pey = new QLabel("Y :");
	m_pEY = EditorFactory(false);
	QLabel *pez = new QLabel("Z :");
	m_pEZ = EditorFactory(false);
	QHBoxLayout *pLayout10 = new QHBoxLayout();
	pLayout10->addWidget(pes);
	pLayout10->addWidget(m_pES);
	pLayout10->addWidget(pex);
	pLayout10->addWidget(m_pEX);
	pLayout10->addWidget(pey);
	pLayout10->addWidget(m_pEY);
	pLayout10->addWidget(pez);
	pLayout10->addWidget(m_pEZ);

	QLabel *perrorLabel = new QLabel("Relative Error :");
	m_pError = EditorFactory(false);
	QDialogButtonBox *pBox = new QDialogButtonBox();
	m_pConvertor = pBox->addButton("Convert", QDialogButtonBox::AcceptRole);
	QHBoxLayout *pLayout11 = new QHBoxLayout();
	pLayout11->addWidget(perrorLabel);
	pLayout11->addWidget(m_pError);
	pLayout11->addWidget(pBox);

	QVBoxLayout *pMainLayout = new QVBoxLayout();
	pMainLayout->addLayout(playout1);
	pMainLayout->addLayout(playout2);
	pMainLayout->addLayout(playout3);
	pMainLayout->addLayout(playout4);
	pMainLayout->addLayout(pLayout5);
	pMainLayout->addLayout(pLayout6);
	pMainLayout->addLayout(pLayout7);
	pMainLayout->addLayout(pLayout8);
	pMainLayout->addLayout(pLayout9);
	pMainLayout->addLayout(pLayout10);
	pMainLayout->addLayout(pLayout11);
	setLayout(pMainLayout);

	connect(m_pConvertor, SIGNAL(clicked()), this, SLOT(Convert()));
}

QuaternionConvertor::~QuaternionConvertor()
{

}

void QuaternionConvertor::Convert()
{
	float fromX = m_pFromX->text().toFloat();
	float fromY = m_pFromY->text().toFloat();
	float fromZ = m_pFromZ->text().toFloat();
	float toX = m_pToX->text().toFloat();
	float toY = m_pToY->text().toFloat();
	float toZ = m_pToZ->text().toFloat();

	QVector3D fromV(fromX, fromY, fromZ), toV(toX, toY, toZ);
	QQuaternion rotateq = QQuaternion::rotationTo(fromV, toV);
	m_pS->setText(QVariant(rotateq.scalar()).toString());
	m_pX->setText(QVariant(rotateq.x()).toString());
	m_pY->setText(QVariant(rotateq.y()).toString());
	m_pZ->setText(QVariant(rotateq.z()).toString());
	QVector3D anglev = rotateq.toEulerAngles();
	float rolld = anglev.x(), pitchd = anglev.y(), yawd = anglev.z();
	m_pRoll->setText(QVariant(rolld).toString());
	m_pPitch->setText(QVariant(pitchd).toString());
	m_pYaw->setText(QVariant(yawd).toString());

	float PI = acos(-1);
	float roll = rolld * PI / 180, pitch = pitchd * PI / 180, yaw = yawd * PI / 180;
	QQuaternion qx(cos(roll / 2), sin(roll/2), 0, 0);
	QQuaternion qy(cos(pitch / 2), 0, sin(pitch / 2), 0);
	QQuaternion qz(cos(yaw / 2), 0, 0, sin(yaw / 2));
	QQuaternion errorq = qy * qx * qz - rotateq;
	m_pES->setText(QVariant(errorq.scalar()).toString());
	m_pEX->setText(QVariant(errorq.x()).toString());
	m_pEY->setText(QVariant(errorq.y()).toString());
	m_pEZ->setText(QVariant(errorq.z()).toString());
	float error = errorq.length() / rotateq.length();
	m_pError->setText(QVariant(error).toString());
}

QLineEdit* QuaternionConvertor::EditorFactory(bool editable)
{
	QLineEdit *pEdit = new QLineEdit();
	pEdit->setValidator(new QDoubleValidator());
	pEdit->setReadOnly(!editable);
	return pEdit;
}