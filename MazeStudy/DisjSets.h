#ifndef DISJSETS_H
#define DISJSETS_H

#include <vector>

class DisjSets
{
public:
	explicit DisjSets(int numElements);
	int find(int x);
	void unionSets(int root1, int root2);
private:
	std::vector<int> m_s;
};

#endif // DISJSETS_H