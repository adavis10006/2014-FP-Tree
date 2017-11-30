#ifndef FPTREE_HPP
#define FPTREE_HPP

#include "typedef.hpp"
#include "FrequentItemMap.hpp"

class FPTreeInterface
{
public:
	FPTreeInterface(FrequentItemMapInterface *frequentItemMap);

	FPTreeNodeMap nodeMap;
	FPTreeNode* root;

	void debugNodeMap();
	void debugTree();
private:
	FrequentItemMapInterface *frequentItemMap;

	void createTree();

	FPTreeNode* insertNode(FPTreeNode *parent, std::string ID);
};

#endif
