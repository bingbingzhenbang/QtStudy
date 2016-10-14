#include "mazeview.h"
#include <QMouseEvent>

MazeView::MazeView(QGraphicsScene *scene, QWidget *parent)
	: QGraphicsView(scene, parent)
{
}

MazeView::~MazeView()
{

}

void MazeView::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);
	if (event->buttons() == Qt::LeftButton)
	{
		QPointF p = mapToScene(event->pos());
		emit userSelectUpdated(p);
	}
}
