#ifndef FREQUENTITEMMAP_HPP
#define FREQUENTITEMMAP_HPP

#include "typedef.hpp"

#include <fstream>

extern clock_t time_start;

class FrequentItemMapInterface
{
public:
	FrequentItemMapInterface(std::ifstream *ifs, double supportThreshold);

	ORDER_ITEM_MAP frequentItemMap;
	unsigned int supportCount;

	void debugItemMap();
	void debugItemCount();
	void debugFrequentItemMap();
private:
	TRANSACTION_MAP transactionMap;
	ITEM_COUNT itemCount;

	void createMap(std::ifstream *ifs, double supportThreshold);
	void orderMap(unsigned int supportCount);
};

#endif
