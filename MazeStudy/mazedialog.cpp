#include "mazedialog.h"
#include "DisjSets.h"
#include "mazeview.h"
#include <ctime>
#include <random>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLayout>
#include <QStack>

const int ROWRESERVE = 200;
const int COLRESERVE = 300;
const int CELLLEN = 12;
const int BLOCKLEN = 10;

MazeDialog::MazeDialog(QWidget *parent)
: QDialog(parent), m_row(5), m_col(5)
{
	m_pScene = new QGraphicsScene();
	m_pView = new MazeView(m_pScene);

	QLabel *rowlabel = new QLabel(tr("Row :"));
	m_pRow = new QLineEdit();
	m_pRow->setValidator(new QDoubleValidator(3, 100, 0));
	m_pRow->setText(QString::number(m_row));
	QLabel *collabel = new QLabel(tr("Col :"));
	m_pCol = new QLineEdit();
	m_pCol->setValidator(new QDoubleValidator(3, 100, 0));
	m_pCol->setText(QString::number(m_col));
	QDialogButtonBox *pbox = new QDialogButtonBox();
	m_pGenerateMaze = pbox->addButton(tr("Generate"), QDialogButtonBox::AcceptRole);
	m_pRetry = pbox->addButton(tr("Retry"), QDialogButtonBox::AcceptRole);
	m_pShowAnswer = pbox->addButton(tr("Answer"), QDialogButtonBox::AcceptRole);

	QHBoxLayout *playout1 = new QHBoxLayout();
	playout1->addWidget(rowlabel);
	playout1->addWidget(m_pRow);
	playout1->addWidget(collabel);
	playout1->addWidget(m_pCol);
	playout1->addWidget(pbox);
	QVBoxLayout *playout = new QVBoxLayout();
	playout->addWidget(m_pView);
	playout->addLayout(playout1);
	setLayout(playout);

	GenerateMaze();
	connect(m_pGenerateMaze, SIGNAL(clicked()), this, SLOT(GenerateMaze()));
	connect(m_pRetry, SIGNAL(clicked()), this, SLOT(Retry()));
	connect(m_pShowAnswer, SIGNAL(clicked()), this, SLOT(ShowAnswer()));
	connect(m_pView, SIGNAL(userSelectUpdated(QPointF)), this, SLOT(userSelectUpdate(QPointF)));
}

MazeDialog::~MazeDialog()
{

}

void MazeDialog::GenerateMaze()
{
	m_pGenerateMaze->setEnabled(false);
	m_pRetry->setEnabled(false);
	m_pShowAnswer->setEnabled(false);
	m_pView->blockSignals(true);
	m_pScene->clear();
	m_row = m_pRow->text().toInt();
	m_col = m_pCol->text().toInt();
	m_xstart = -CELLLEN * m_col / 2;
	m_ystart = -CELLLEN * m_row / 2;
	m_reachcells.swap(QVector< QVector<int> >(m_row * m_col));
	m_userselect.swap(QVector<int>(m_row * m_col, 0));
	if (!m_userselect.empty())
		m_userselect[0] = 1;
	m_pathparents.swap(QVector<int>(m_row * m_col, -1));
	resize(COLRESERVE + CELLLEN * m_col, ROWRESERVE + CELLLEN * m_row);
	InitWalls();
	TearWalls();
	PaintMaze();
	m_pGenerateMaze->setEnabled(true);
	m_pRetry->setEnabled(true);
	m_pShowAnswer->setEnabled(true);
	m_pView->blockSignals(false);
}

void MazeDialog::Retry()
{
	m_pGenerateMaze->setEnabled(false);
	m_pRetry->setEnabled(false);
	m_pShowAnswer->setEnabled(false);
	m_pView->blockSignals(true);
	for (int i = 1; i < m_userselect.size(); ++i)
	{
		if (m_userselect[i])
		{
			int current_row = i / m_col;
			int current_col = i % m_col;
			int current_x = m_xstart + current_col * CELLLEN + CELLLEN / 2 - BLOCKLEN / 2;
			int current_y = m_ystart + current_row * CELLLEN + CELLLEN / 2 - BLOCKLEN / 2;
			m_pScene->removeItem(m_pScene->itemAt(QPoint(current_x, current_y), QTransform()));
		}
	}
	m_userselect.swap(QVector<int>(m_row * m_col, 0));
	if (!m_userselect.empty())
		m_userselect[0] = 1;
	m_pGenerateMaze->setEnabled(true);
	m_pRetry->setEnabled(true);
	m_pShowAnswer->setEnabled(true);
	m_pView->blockSignals(false);
}

void MazeDialog::ShowAnswer()
{
	m_pGenerateMaze->setEnabled(false);
	m_pRetry->setEnabled(false);
	m_pShowAnswer->setEnabled(false);
	m_pView->blockSignals(true);
	GeneratePath();
	PaintPath();
	m_pGenerateMaze->setEnabled(true);
	m_pRetry->setEnabled(true);
	m_pShowAnswer->setEnabled(true);
	m_pView->blockSignals(false);
}

void MazeDialog::userSelectUpdate(QPointF p)
{
	int currentx = p.x();
	int currenty = p.y();
	if (currentx >= m_xstart && currentx < m_xstart + CELLLEN * m_col && currenty >= m_ystart && currenty < m_ystart + CELLLEN * m_row)
	{
		int currentcol = (currentx - m_xstart) / CELLLEN;
		int currentrow = (currenty - m_ystart) / CELLLEN;
		int currentindex = currentrow * m_col + currentcol;
		if (currentindex == 0)
			return;
		int itemx = m_xstart + currentcol * CELLLEN + CELLLEN / 2 - BLOCKLEN / 2;
		int itemy = m_ystart + currentrow * CELLLEN + CELLLEN / 2 - BLOCKLEN / 2;
		if (m_userselect[currentindex])
		{
			m_pScene->removeItem(m_pScene->itemAt(QPoint(itemx, itemy), QTransform()));
			m_userselect[currentindex] = 0;
		}
		else
		{
			bool canconnect = false;
			for (auto itr = m_reachcells[currentindex].begin(); itr != m_reachcells[currentindex].end(); ++itr)
			{
				if (m_userselect[*itr])
				{
					canconnect = true;
					break;
				}
			}
			if (canconnect)
			{
				m_pScene->addRect(QRect(itemx, itemy, BLOCKLEN, BLOCKLEN), QPen(Qt::cyan), QBrush(Qt::cyan));
				m_userselect[currentindex] = 1;
			}
		}
	}
}

void MazeDialog::InitWalls()
{
	m_walls.clear();
	for (int i = 0; i < m_row; ++i)
	{
		for (int j = 1; j < m_col; ++j)
		{
			m_walls.push_back(MazeUnitWall(m_xstart + CELLLEN * j, m_ystart + CELLLEN * i, m_xstart + CELLLEN * j, m_ystart + CELLLEN * (i + 1), m_col * i + j - 1, m_col * i + j));
		}
	}
	for (int i = 1; i < m_row; ++i)
	{
		for (int j = 0; j < m_col; ++j)
		{
			m_walls.push_back(MazeUnitWall(m_xstart + CELLLEN * j, m_ystart + CELLLEN * i, m_xstart + CELLLEN * (j + 1), m_ystart + CELLLEN * i, m_col * (i - 1) + j, m_col * i + j));
		}
	}
}

void MazeDialog::TearWalls()
{
	using namespace std;
	int cellnum = m_row * m_col;
	DisjSets cellset(cellnum);
	int set1 = cellset.find(0);
	int set2 = cellset.find(cellnum - 1);
	while (set1 != set2 && !m_walls.empty())
	{
		default_random_engine e1(time(0));
		uniform_int_distribution<unsigned> u1(0, m_walls.size() - 1);
		int index = -1, cell_1 = -1, cell_2 = -1, s1 = -1, s2 = -1;
		do 
		{
			index = u1(e1);
			cell_1 = m_walls[index].m_cell_1;
			cell_2 = m_walls[index].m_cell_2;
			s1 = cellset.find(cell_1);
			s2 = cellset.find(cell_2);
			if (s1 != s2)
				break;
		} while (true);
		if (index != m_walls.size() - 1)
			swap(m_walls[index], m_walls[m_walls.size() - 1]);
		m_walls.pop_back();
		cellset.unionSets(s1, s2);
		m_reachcells[cell_1].push_back(cell_2);
		m_reachcells[cell_2].push_back(cell_1);
		set1 = cellset.find(0);
		set2 = cellset.find(cellnum - 1);
	}
}

void MazeDialog::PaintMaze()
{
	m_pScene->addLine(QLine(m_xstart + CELLLEN, m_ystart, m_xstart + CELLLEN * m_col, m_ystart));
	m_pScene->addLine(QLine(m_xstart, m_ystart + CELLLEN, m_xstart, m_ystart + CELLLEN * m_row));
	m_pScene->addLine(QLine(m_xstart + CELLLEN * m_col, m_ystart, m_xstart + CELLLEN * m_col, m_ystart + CELLLEN * (m_row - 1)));
	m_pScene->addLine(QLine(m_xstart, m_ystart + CELLLEN * m_row, m_xstart + CELLLEN * (m_col - 1), m_ystart + CELLLEN * m_row));
	for (int i = 0; i < m_walls.size(); ++i)
	{
		m_pScene->addLine(QLine(m_walls[i].m_start, m_walls[i].m_end));
	}
	for (int i = 0; i < m_userselect.size(); ++i)
	{
		if (m_userselect[i])
		{
			int current_row = i / m_col;
			int current_col = i % m_col;
			int current_x = m_xstart + current_col * CELLLEN + CELLLEN / 2 - BLOCKLEN / 2;
			int current_y = m_ystart + current_row * CELLLEN + CELLLEN / 2 - BLOCKLEN / 2;
			m_pScene->addRect(QRect(current_x, current_y, BLOCKLEN, BLOCKLEN), QPen(Qt::cyan), QBrush(Qt::cyan));
		}
	}
}

void MazeDialog::GeneratePath()
{
	QVector<int> visited(m_row * m_col, 0);
	QStack< QPair<int, QVector<int>::iterator> > cell_iters;
	for (int i = 0; i < m_reachcells.size(); )
	{
		if (visited[i] == 0)
		{
			visited[i] = 1;
			cell_iters.push(QPair<int, QVector<int>::iterator>(i, m_reachcells[i].begin()));
			while (!cell_iters.empty())
			{
				int u = cell_iters.top().first;
				QVector<int>::iterator edgebegin = cell_iters.top().second;
				bool finished = true;
				for (auto itr = edgebegin; itr != m_reachcells[u].end(); ++itr)
				{
					int v = *itr;
					if (visited[v] == 0)
					{
						m_pathparents[v] = u;
						visited[v] = 1;
						cell_iters.top().second = ++itr;
						cell_iters.push(QPair<int, QVector<int>::iterator>(v, m_reachcells[v].begin()));
						finished = false;
						break;
					}
				}
				if (finished)
				{
					visited[u] = 2;
					cell_iters.pop();
				}
			}
		}
		else
			++i;
	}
}

void MazeDialog::PaintPath()
{
	int currentindex = m_row * m_col - 1;
	int parentindex = m_pathparents[currentindex];
	int current_row = currentindex / m_col;
	int current_col = currentindex % m_col;
	int current_x = m_xstart + current_col * CELLLEN + CELLLEN / 2;
	int current_y = m_ystart + current_row * CELLLEN + CELLLEN / 2;
	while (parentindex != -1)
	{
		int parent_row = parentindex / m_col;
		int parent_col = parentindex % m_col;
		int parent_x = m_xstart + parent_col * CELLLEN + CELLLEN / 2;
		int parent_y = m_ystart + parent_row * CELLLEN + CELLLEN / 2;
		m_pScene->addLine(QLine(current_x, current_y, parent_x, parent_y), QPen(Qt::red));
		currentindex = parentindex;
		current_row = parent_row;
		current_col = parent_col;
		current_x = parent_x;
		current_y = parent_y;
		parentindex = m_pathparents[currentindex];
	}
}