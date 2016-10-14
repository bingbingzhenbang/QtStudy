#ifndef QUATERNIONCONVERTOR_H
#define QUATERNIONCONVERTOR_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class QuaternionConvertor : public QDialog
{
	Q_OBJECT

public:
	QuaternionConvertor(QWidget *parent = 0);
	~QuaternionConvertor();

	public slots:
	void Convert();

private:
	QLineEdit *EditorFactory(bool editable);

private:
	QLineEdit *m_pFromX, *m_pFromY, *m_pFromZ;
	QLineEdit *m_pToX, *m_pToY, *m_pToZ;
	QLineEdit *m_pS, *m_pX, *m_pY, *m_pZ;
	QLineEdit *m_pRoll, *m_pPitch, *m_pYaw;
	QLineEdit *m_pES, *m_pEX, *m_pEY, *m_pEZ;
	QLineEdit *m_pError;
	QPushButton *m_pConvertor;
};

#endif // QUATERNIONCONVERTOR_H
