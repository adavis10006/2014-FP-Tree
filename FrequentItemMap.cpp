#ifndef FREQUENTITEMMAP_CPP
#define FREQUENTITEMMAP_CPP

#include "FrequentItemMap.hpp"

#include <cstdio>
#include <cmath>
#include <iostream>

//this may take a lots of time but free memory that not use after
#define __CLEAN_MEMORY

#define CACHESIZE 65536 

clock_t time_start;

FrequentItemMapInterface::FrequentItemMapInterface(std::ifstream *ifs, double supportThreshold)
{
	transactionMap.clear();
	itemCount.clear();
	frequentItemMap.clear();
	
	createMap(ifs, supportThreshold);

	time_start = clock();

	orderMap(supportCount);

#ifdef CLEAN_MEMORY
	std::cout << "Clear unused memory to free space..." << std::endl;
	std::cout << "Clear Transaction Map..." << std::endl;
	itemMap.clear();
	std::cout << "Clear Transaction Count..." << std::endl;
	itemCount.clear();
#endif
}

void FrequentItemMapInterface::createMap(std::ifstream *ifs, double supportThreshold)
{
	unsigned int read_data = 0;
	char cacheWord[CACHESIZE];
	unsigned int cacheCount;

	std::cout << "Read transaction:" << std::endl;
	//read userid and productcount
	while(*ifs >> cacheWord >> cacheCount)
	{
		std::string userID(cacheWord);

		//read product
		for(unsigned int i = cacheCount; i > 0; --i)
		{
			*ifs >> cacheWord;

			std::string productID(cacheWord);
			transactionMap[userID].insert(productID);
		}

		if(++read_data % 10000 == 0)
		{
			std::cout << '\r' << read_data;
			std::cout.flush();
		}
	}
	supportCount = unsigned int(ceil(read_data * supportThreshold));
	std::cout << '\r' << read_data << " SupportCount " << supportCount << std::endl;
}

void FrequentItemMapInterface::orderMap(unsigned int supportCount)
{
	unsigned int finished = 0;
	std::cout << "Count Item: " << transactionMap.size() << std::endl;
	for(TRANSACTION_MAP::iterator itMap = transactionMap.begin(); itMap != transactionMap.end(); ++itMap)
	{
		for(TRANSACTION_LIST::iterator itList = itMap->second.begin(); itList != itMap->second.end(); ++itList)
			if(itemCount.count(*itList) == 0)
				itemCount[*itList] = 1;
			else
				++itemCount[*itList];
		
		if(++finished % 10000 == 0)
		{
			std::cout << '\r' << finished;
			std::cout.flush();
		}
	}
	std::cout << '\r' << finished << std::endl;

	finished = 0;
	std::cout << "Reorder size: " << transactionMap.size() << std::endl;
	for(TRANSACTION_MAP::iterator itMap = transactionMap.begin(); itMap != transactionMap.end(); ++itMap)
	{
		if(++finished % 10000 == 0)
		{
			std::cout << '\r' << finished;
			std::cout.flush();
		}

		for(TRANSACTION_LIST::iterator itList = itMap->second.begin(); itList != itMap->second.end(); ++itList)
		if(itemCount[*itList] >= supportCount)
			frequentItemMap[itMap->first][itemCount[*itList]].insert(*itList);
	}
	std::cout << '\r' << finished << std::endl;
}

void FrequentItemMapInterface::debugItemMap()
{
	std::cout << "ItemMap debug" << std::endl;
	for(TRANSACTION_MAP::iterator itMap = transactionMap.begin(); itMap != transactionMap.end(); ++itMap)
	{
		std::cout << "\t" << itMap->first.c_str() << ":";

		for(TRANSACTION_LIST::iterator itList = itMap->second.begin(); itList != itMap->second.end(); ++itList)
			std::cout << " " << itList->c_str();
		std::cout << std::endl;
	}
}

void FrequentItemMapInterface::debugItemCount()
{
	std::cout << "ItemCount debug" << std::endl;
	for(ITEM_COUNT::iterator it = itemCount.begin(); it != itemCount.end(); ++it)
		std::cout << "\t" << it->first.c_str() << ": " << it->second << std::endl;
}

void FrequentItemMapInterface::debugFrequentItemMap()
{
	std::cout << "FrequentItemMap debug" << std::endl;
	for(ORDER_ITEM_MAP::iterator itMap = frequentItemMap.begin(); itMap != frequentItemMap.end(); ++itMap)
	{
		std::cout << "\t" << itMap->first.c_str() << ":";

		for(ORDER_ITEM_LIST::iterator itList = itMap->second.begin(); itList != itMap->second.end(); ++itList)
		for(ITEM_LIST::iterator itItem = itList->second.begin(); itItem != itList->second.end(); ++itItem)
			std::cout << " " << itItem->c_str();
		std::cout << std::endl;
	}
}

#endif
