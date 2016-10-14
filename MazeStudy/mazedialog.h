#ifndef MAZEDIALOG_H
#define MAZEDIALOG_H

#include <QDialog>

class QGraphicsScene;
class QGraphicsView;
class QLineEdit;
class QPushButton;

struct MazeUnitWall
{
	QPoint m_start;
	QPoint m_end;
	int m_cell_1;
	int m_cell_2;
	MazeUnitWall(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, int cell_1 = 0, int cell2 = 0)
		: m_start(x1, y1), m_end(x2, y2), m_cell_1(cell_1), m_cell_2(cell2)
	{}
};

class MazeDialog : public QDialog
{
	Q_OBJECT

public:
	MazeDialog(QWidget *parent = 0);
	~MazeDialog();

public slots:
    void GenerateMaze();
	void Retry();
	void ShowAnswer();
	void userSelectUpdate(QPointF p);

private:
	void InitWalls();
	void TearWalls();
	void PaintMaze();
	void GeneratePath();
	void PaintPath();

private:
	QGraphicsScene *m_pScene;
	QGraphicsView *m_pView;
	QLineEdit *m_pRow;
	QLineEdit *m_pCol;
	QPushButton *m_pGenerateMaze;
	QPushButton *m_pRetry;
	QPushButton *m_pShowAnswer;
	int m_row;
	int m_col;
	int m_xstart;
	int m_ystart;
	QVector<MazeUnitWall> m_walls;
	QVector< QVector<int> > m_reachcells;
	QVector<int> m_userselect;
	QVector<int> m_pathparents;
};

#endif // MAZEDIALOG_H