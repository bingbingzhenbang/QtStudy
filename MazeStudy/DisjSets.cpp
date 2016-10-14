#include "DisjSets.h"

using namespace std;

DisjSets::DisjSets(int numElements)
: m_s(numElements, -1)
{

}

int DisjSets::find(int x)
{
	if (m_s[x] < 0)
		return x;
	else
		return m_s[x] = find(m_s[x]);
}

void DisjSets::unionSets(int root1, int root2)
{
	if (m_s[root2] < m_s[root1])
	{
		m_s[root1] = root2;
	}
	else
	{
		if (m_s[root2] == m_s[root1])
			--m_s[root1];
		m_s[root2] = root1;
	}
}