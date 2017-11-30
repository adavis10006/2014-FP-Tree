#ifndef ASSOCIATIONRULES_HPP
#define ASSOCIATIONRULES_HPP

#include "typedef.hpp"

#include <fstream>

class AssociationRulesInterface
{
public:
	AssociationRulesInterface(std::ifstream *ifs, unsigned int topK, double confidence);

	void output(unsigned int topK, std::ofstream *ofs);
private:
	unsigned int findRules;
	FREQUENT_PATTERN frequentPattern;
	FREQUENT_ITEM_SET_MAP frequentItemSetMap;
	ASSOCIATION_RULES associationRules;

	void createItemSetMap(std::ifstream *ifs);
	void findAssociationRules(unsigned int topK, double confidence);
	void findAllPossibleRules(std::string remainID, ITEM_LIST first, ITEM_LIST second, unsigned int supportCount, double confidence);
	unsigned int countAssociationRules();
};

#endif
