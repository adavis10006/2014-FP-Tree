#ifndef FREQUENTITEMSET_CPP
#define FREQUENTITEMSET_CPP

#include "FrequentItemSet.hpp"

#include <cstdio>
#include <iostream>

unsigned int patternCount;

clock_t time_end;

void choseItem(std::string ID, ITEM_COUNT::iterator itItem, ITEM_COUNT::iterator itEnd, ITEM_LIST initialSet, unsigned int support, unsigned int supportCount, FREQUENT_PATTERN *frequentPattern)
{
	ITEM_COUNT::iterator itItemTemp = itItem;

	if(itItem == itEnd)
		return;

	if(itItem->first.compare(ID) == 0 || itItem->second < supportCount)
		choseItem(ID, ++itItemTemp, itEnd, initialSet, support, supportCount, frequentPattern);
	else
	{
		itItemTemp = itItem;
		choseItem(ID, ++itItemTemp, itEnd, initialSet, support, supportCount, frequentPattern);

		itItemTemp = itItem;
		initialSet.insert(itItem->first);
		support = support > itItem->second ? itItem->second : support;
		(*frequentPattern)[initialSet] = support;
		choseItem(ID, ++itItemTemp, itEnd, initialSet, support, supportCount, frequentPattern);

		if(++patternCount % 100 == 0)
		{
			std::cout << '\r' << patternCount;
			std::cout.flush();
		}
	}
}

void findItemSet(std::string ID, ITEM_COUNT *itemCount, ITEM_LIST initialSet, unsigned int support, unsigned int supportCount, FREQUENT_PATTERN *frequentPattern)
{
	choseItem(ID, itemCount->begin(), itemCount->end(), initialSet, support, supportCount, frequentPattern);
}

FrequentItemSetInterface::FrequentItemSetInterface(FPTreeInterface *FPTree, unsigned int supportCount) :
FPTree(FPTree),
supportCount(supportCount)
{
	frequentPatternCount.clear();
	frequentPattern.clear();

	findPatternCount();
	findPattern();

	time_end = clock();
}

void FrequentItemSetInterface::findPatternCount()
{
	int finished = 0;
	std::cout << "Find Pattern Count: " << FPTree->nodeMap.size() << std::endl;
	for(FPTreeNodeMap::iterator itMap = FPTree->nodeMap.begin(); itMap != FPTree->nodeMap.end(); ++itMap)
	{
		std::string ID(itMap->first);

		for(FPTreeNodeList::iterator itList = itMap->second.begin(); itList != itMap->second.end(); ++itList)
		{
			unsigned int addsupport = itList->value;
			FPTreeNode *p = itList->parent;

			if(frequentPatternCount[ID].count(ID) == 0)
				frequentPatternCount[ID][ID] = addsupport;
			else
				frequentPatternCount[ID][ID] += addsupport;

			while(p->ID != std::string(""))
			{
				if(frequentPatternCount[ID].count(p->ID) == 0)
					frequentPatternCount[ID][p->ID] = addsupport;
				else
					frequentPatternCount[ID][p->ID] += addsupport;

				p = p->parent;
			}
		}
		if(++finished % 10000 == 0)
		{
			std::cout << '\r' << finished;
			std::cout.flush();
		}
	}
	std::cout << '\r' << finished << std::endl;
}

void FrequentItemSetInterface::findPattern()
{
	patternCount = 0;
	std::cout << "Find Pattern:" << std::endl;
	for(FREQUENT_PATTERN_COUNT::iterator itPattern = frequentPatternCount.begin(); itPattern != frequentPatternCount.end(); ++itPattern)
	{
		int support = 0;

		ITEM_LIST initialSet;

		initialSet.clear();
		initialSet.insert(itPattern->first);
		support = itPattern->second[itPattern->first];
		frequentPattern[initialSet] = support;
		if(++patternCount % 100 == 0)
		{
			std::cout << '\r' << patternCount;
			std::cout.flush();
		}

		findItemSet(itPattern->first, &(itPattern->second), initialSet, support, supportCount, &frequentPattern);
	}
	std::cout << '\r' << patternCount << std::endl;
}

void FrequentItemSetInterface::output(std::ofstream *ofs)
{
	unsigned int finished = 0;
	std::cout << "Output Frequent Pattern: " << frequentPattern.size() << std::endl;
	for(FREQUENT_PATTERN::iterator itPattern = frequentPattern.begin(); itPattern != frequentPattern.end(); ++itPattern)
	{
		*ofs << itPattern->second << " " << itPattern->first.size() << " ";
		for(ITEM_LIST::iterator itItem = itPattern->first.begin(); itItem != itPattern->first.end(); ++itItem)
			*ofs << itItem->c_str() << " ";
		*ofs << std::endl;

		if(++finished % 10000 == 0)
		{
			std::cout << '\r' << finished;
			std::cout.flush();
		}
	}
	std::cout << '\r' << finished << std::endl;
}

void FrequentItemSetInterface::debugPatternCount()
{
	std::cout << "debug Pattern Count" << std::endl;
	for(FREQUENT_PATTERN_COUNT::iterator itPattern = frequentPatternCount.begin(); itPattern != frequentPatternCount.end(); ++itPattern)
	{
		std::cout << "\tpattern for " << itPattern->first.c_str() << std::endl;
		for(ITEM_COUNT::iterator itCount = itPattern->second.begin(); itCount != itPattern->second.end(); ++itCount)
			std::cout << "\t\t" << itCount->first.c_str() << ": " << itCount->second << std::endl;
		std::cout << std::endl;
	}
}

void FrequentItemSetInterface::debugPattern()
{
	std::cout << "debug Pattern" << std::endl;
	for(FREQUENT_PATTERN::iterator itPattern = frequentPattern.begin(); itPattern != frequentPattern.end(); ++itPattern)
	{
		std::cout << "value: " << itPattern->second << " key: ";
		for(ITEM_LIST::iterator itItem = itPattern->first.begin(); itItem != itPattern->first.end(); ++itItem)
			std::cout << itItem->c_str() << " ";
		std::cout << std::endl;
	}
}

#endif
