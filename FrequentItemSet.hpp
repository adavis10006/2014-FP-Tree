#ifndef FREQUENTITEMSET_HPP
#define FREQUENTITEMSET_HPP

#include "typedef.hpp"
#include "FPTree.hpp"

extern clock_t time_end;

class FrequentItemSetInterface
{
public:
	FrequentItemSetInterface(FPTreeInterface *FPTree, unsigned int supportCount);

	void output(std::ofstream *ofs);

	void debugPatternCount();
	void debugPattern();
private:
	FPTreeInterface *FPTree;
	unsigned int supportCount;

	FREQUENT_PATTERN_COUNT frequentPatternCount;
	FREQUENT_PATTERN frequentPattern;

	void findPatternCount();
	void findPattern();
};

#endif
