#ifndef ASSOCIATIONRULES_CPP
#define ASSOCIATIONRULES_CPP

#include "AssociationRules.hpp"

#include <iostream>

#define CACHESIZE 102400

AssociationRulesInterface::AssociationRulesInterface(std::ifstream *ifs, unsigned int topK, double confidence) :
findRules(0)
{
	associationRules.clear();

	createItemSetMap(ifs);
	findAssociationRules(topK, confidence);
}

void AssociationRulesInterface::output(unsigned int topK, std::ofstream *ofs)
{
	unsigned int total = 0;
	for(ASSOCIATION_RULES::iterator itRules = associationRules.begin(); total < topK && itRules != associationRules.end(); ++itRules)
	for(CONFIDENCE_MAP::iterator itConfidence = itRules->second.begin(); total < topK && itConfidence != itRules->second.end(); ++itConfidence)
	for(CONFIDENCE_SET::iterator itRule = itConfidence->second.begin(); total < topK && itRule != itConfidence->second.end(); ++itRule, ++total)
	{
		*ofs << itRules->first << " " << itConfidence->first << " ";
		for(ITEM_LIST::iterator itItems = itRule->first.begin(); itItems != itRule->first.end(); ++itItems)
			*ofs << itItems->c_str() << " ";
		*ofs << "-> ";
		for(ITEM_LIST::iterator itItems = itRule->second.begin(); itItems != itRule->second.end(); ++itItems)
			*ofs << itItems->c_str() << " ";
		*ofs << std::endl;

		if(total % 100 == 0)
		{
			std::cout << '\r' << total;
			std::cout.flush();
		}
	}
	std::cout << '\r' << total;
}

void AssociationRulesInterface::createItemSetMap(std::ifstream *ifs)
{
	unsigned int read_data = 0;
	char cacheWord[CACHESIZE];
	unsigned int supportCount;
	unsigned int cacheCount;

	std::cout << "Read Item Sets:" << std::endl;
	//read userid and productcount
	while(*ifs >> supportCount >> cacheCount)
	{
		//read product
		ITEM_LIST tempItemSet;

		tempItemSet.clear();
		for(unsigned int i = cacheCount; i > 0; --i)
		{
			*ifs >> cacheWord;

			std::string productID(cacheWord);
			tempItemSet.insert(productID);
		}

		frequentPattern[tempItemSet] = supportCount;
		if(cacheCount > 1)
			frequentItemSetMap[supportCount].insert(tempItemSet);

		if(++read_data % 100 == 0)
		{
			std::cout << '\r' << read_data;
			std::cout.flush();
		}
	}
	std::cout << '\r' << read_data << std::endl;
}

void AssociationRulesInterface::findAssociationRules(unsigned int topK, double confidence)
{
	for(FREQUENT_ITEM_SET_MAP::iterator itMap = frequentItemSetMap.begin(); countAssociationRules() < topK && itMap != frequentItemSetMap.end(); ++itMap)
	for(FREQUENT_ITEM_SET::iterator itSet = itMap->second.begin(); itSet != itMap->second.end(); ++itSet)
	{
		std::cout << "\rNow Generate: " << countAssociationRules();

		for(ITEM_LIST::iterator itItem = itSet->begin(); itItem != itSet->end(); ++itItem)
		{
			ITEM_LIST first, second;
			first = *itSet;
			second.clear();
			first.erase(*itItem);
			second.insert(*itItem);

			double thisConfidence = double(itMap->first) / double(frequentPattern[first]);
			/*
			std::cout << "\r" << confidence;
			std::cout << itMap->first << " " << confidence << " ";
			for(FREQUENT_ITEM::iterator itItems = first.begin(); itItems != first.end(); ++itItems)
			std::cout << itItems->c_str() << " ";
			std::cout << " -> ";
			for(FREQUENT_ITEM::iterator itItems = second.begin(); itItems != second.end(); ++itItems)
			std::cout << itItems->c_str() << " ";
			std::cout << std::endl;
			*/
			if(thisConfidence >= confidence)
				associationRules[itMap->first][thisConfidence].insert(CONFIDENCE(first, second));

			findAllPossibleRules(*itItem, first, second, itMap->first, confidence);
		}
	}
	std::cout << "\rNow Generate: " << countAssociationRules() << std::endl;
}

void AssociationRulesInterface::findAllPossibleRules(std::string remainID, ITEM_LIST first, ITEM_LIST second, unsigned int supportCount, double confidence)
{
	if(first.size() == 1)
		return;

	for(ITEM_LIST::iterator itItem = first.begin(); itItem != first.end(); ++itItem)
	{
		if(itItem->compare(remainID) > 0)
			continue;

		ITEM_LIST tempFirst = first, tempSecond = second;
		tempFirst.erase(*itItem);
		tempSecond.insert(*itItem);

		double thisConfidence = double(supportCount) / double(frequentPattern[tempFirst]);
		/*
		std::cout << "\r" << confidence;
		std::cout << supportCount << " " << confidence << " ";
		for(FREQUENT_ITEM::iterator itItems = tempFirst.begin(); itItems != tempFirst.end(); ++itItems)
		std::cout << itItems->c_str() << " ";
		std::cout << " -> ";
		for(FREQUENT_ITEM::iterator itItems = tempSecond.begin(); itItems != tempSecond.end(); ++itItems)
		std::cout << itItems->c_str() << " ";
		std::cout << std::endl;
		*/
		if(thisConfidence >= confidence)
			associationRules[supportCount][thisConfidence].insert(CONFIDENCE(tempFirst, tempSecond));

		findAllPossibleRules(remainID, tempFirst, tempSecond, supportCount, confidence);
	}
}

unsigned int AssociationRulesInterface::countAssociationRules()
{
	//typedef std::map<unsigned int, CONFIDENCE_MAP> ASSOCIATION_RULES;
	unsigned int total = 0;
	for(ASSOCIATION_RULES::iterator itRules = associationRules.begin(); itRules != associationRules.end(); ++itRules)
	for(CONFIDENCE_MAP::iterator itConfidence = itRules->second.begin(); itConfidence != itRules->second.end(); ++itConfidence)
		total += itConfidence->second.size();

	return total;
}

#endif