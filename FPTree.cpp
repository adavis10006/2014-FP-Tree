#ifndef FPTREE_CPP
#define FPTREE_CPP

#include "FPTree.hpp"

#include <iostream>
#include <deque>

FPTreeInterface::FPTreeInterface(FrequentItemMapInterface *frequentItemMap) :
frequentItemMap(frequentItemMap)
{
	nodeMap.clear();
	root = new FPTreeNode(std::string(""), 0, NULL);

	createTree();
}

void FPTreeInterface::createTree()
{
	unsigned int finished = 0;
	std::cout << "Create Tree: " << frequentItemMap->frequentItemMap.size() << std::endl;
	for(ORDER_ITEM_MAP::iterator itMap = frequentItemMap->frequentItemMap.begin(); itMap != frequentItemMap->frequentItemMap.end(); ++itMap)
	{
		FPTreeNode *p = root;

		for(ORDER_ITEM_LIST::iterator itList = itMap->second.begin(); itList != itMap->second.end(); ++itList)
		for(ITEM_LIST::iterator itItem = itList->second.begin(); itItem != itList->second.end(); ++itItem)
			p = insertNode(p, *itItem);

		if(++finished % 10000 == 0)
		{
			std::cout << '\r' << finished;
			std::cout.flush();
		}
	}
	std::cout << '\r' << finished << std::endl;
}

FPTreeNode* FPTreeInterface::insertNode(FPTreeNode *parent, std::string ID)
{
	for(FPTreeNodeLink::iterator it = parent->child.begin(); it != parent->child.end(); ++it)
	if((*it)->ID == ID)
	{
		++((*it)->value);
		return *it;
	}

	nodeMap[ID].push_back(FPTreeNode(ID, 1, parent));
	parent->child.push_back(&(nodeMap[ID].back()));
	return &(nodeMap[ID].back());
}

void FPTreeInterface::debugNodeMap()
{
	std::cout << "debug NodeMap:" << std::endl;
	for(FPTreeNodeMap::iterator itMap = nodeMap.begin(); itMap != nodeMap.end(); ++itMap)
	{
		std::cout << '\t' << itMap->first.c_str() << ":" << std::endl;

		for(FPTreeNodeList::iterator itList = itMap->second.begin(); itList != itMap->second.end(); ++itList)
			std::cout << "\t\t" << (*itList).ID << "->" << (*itList).value << std::endl;
	}
}

void FPTreeInterface::debugTree()
{
	std::cout << "debug Tree:" << std::endl;

	std::deque<FPTreeNode*> deque;

	deque.clear();
	deque.push_back(root);

	while(!deque.empty())
	{
		FPTreeNode *p = deque.front();
		deque.pop_front();

		std::cout << "ID: " << p->ID.c_str() << std::endl;
		std::cout << "value: " << p->value << std::endl;
		if(p->ID != std::string(""))
			std::cout << "parent: " << p->parent->ID.c_str() << std::endl;
		std::cout << std::endl;

		for(FPTreeNodeLink::iterator it = p->child.begin(); it != p->child.end(); ++it)
			deque.push_back(*it);
	}
}

#endif
