#ifndef MAZEVIEW_H
#define MAZEVIEW_H

#include <QGraphicsView>

class MazeView : public QGraphicsView
{
	Q_OBJECT

public:
	MazeView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
	~MazeView();

protected:
	void mousePressEvent(QMouseEvent *event);

signals:
	void userSelectUpdated(QPointF p);
};

#endif // MAZEVIEW_H
