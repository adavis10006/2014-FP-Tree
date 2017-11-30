#ifndef TESTUNIT_CPP
#define TESTUNIT_CPP

#include "typedef.hpp"
#include "Parser.hpp"
#include "FrequentItemMap.hpp"
#include "FPTree.hpp"
#include "FrequentItemSet.hpp"
#include "AssociationRules.hpp"

#include <cstdlib>

void ParserTest()
{
	std::ifstream input("D:\\Temp\\Temp\\movies.txt", std::ifstream::in | std::ifstream::binary);
	extern TRANSACTION transaction_user, transaction_product;

	parse_movie_file(input);
	input.close();
	
	output_transaction();
	
	exit(0);
}

void FrequentItemMapTest()
{
	std::ifstream ifs("D:\\Temp\\Temp\\trans_user.txt", std::ifstream::in | std::ifstream::binary);
	FrequentItemMapInterface frequentItemMap(&ifs, 0);
	ifs.close();

	frequentItemMap.debugItemMap();
	frequentItemMap.debugItemCount();
	frequentItemMap.debugFrequentItemMap();

	exit(0);
}

void FPTreeTest()
{
	std::ifstream ifs("D:\\Temp\\Temp\\trans_user.txt", std::ifstream::in | std::ifstream::binary);
	FrequentItemMapInterface frequentItemMap(&ifs, 3);
	ifs.close();
	FPTreeInterface FPTree(&frequentItemMap);
	
	FPTree.debugNodeMap();
	FPTree.debugTree();

	exit(0);
}

void FrequentItemSetTest(double supportThreshold)
{
	std::ifstream ifs("D:\\Temp\\Temp\\trans_user.txt", std::ifstream::in | std::ifstream::binary);
	FrequentItemMapInterface frequentItemMap(&ifs, supportThreshold);
	ifs.close();
	FPTreeInterface FPTree(&frequentItemMap);
	FrequentItemSetInterface frequentItemSet(&FPTree, frequentItemMap.supportCount);

	frequentItemSet.debugPatternCount();
	frequentItemSet.debugPattern();

	exit(0);
}

void AssociationRulesTest(unsigned int topK, double confidence)
{
	std::ifstream association_ifs("D:\\Temp\\Temp\\frequentItemSet.txt", std::ifstream::in | std::ifstream::binary);
	AssociationRulesInterface associationRules(&association_ifs, topK, confidence);
	association_ifs.close();

	std::ofstream association_ofs("D:\\Temp\\Temp\\TopKRules.txt", std::ofstream::out);
	associationRules.output(topK, &association_ofs);
	association_ofs.close();

	exit(0);
}

void FPGrowth(double supportThreshold, unsigned int topK, double confidence, char *outputFile)
{
	std::ifstream transaction_ifs("D:\\Temp\\Temp\\trans_user.txt", std::ifstream::in | std::ifstream::binary);
	FrequentItemMapInterface frequentItemMap(&transaction_ifs, supportThreshold);
	transaction_ifs.close();
	FPTreeInterface FPTree(&frequentItemMap);
	FrequentItemSetInterface frequentItemSet(&FPTree, frequentItemMap.supportCount);
	
	std::ofstream transaction_ofs("D:\\Temp\\Temp\\frequentItemSet.txt", std::ofstream::out);
	frequentItemSet.output(&transaction_ofs);
	transaction_ofs.close();

	extern clock_t time_start, time_end;
	std::cout << "Done! Cost " << double(time_end - time_start) / CLOCKS_PER_SEC << " second to generate Frequent Item Sets." << std::endl;
	/*
	frequentItemMap.~FrequentItemMapInterface();
	FPTree.~FPTreeInterface();
	frequentItemSet.~FrequentItemSetInterface();
	*/
	std::ifstream association_ifs("D:\\Temp\\Temp\\frequentItemSet.txt", std::ifstream::in | std::ifstream::binary);
	AssociationRulesInterface associationRules(&association_ifs, topK, confidence);
	association_ifs.close();

	std::ofstream association_ofs("D:\\Temp\\Temp\\TopKRules.txt", std::ofstream::out);
	associationRules.output(topK, &association_ofs);
	association_ofs.close();

	exit(0);
}

int main()
{
	//ParserTest();
	//FrequentItemMapTest();
	//FPTreeTest();
	//FrequentItemSetTest(0.001);
	//AssociationRulesTest(10000, 0.9999);
	//FPGrowth(1, 1000, "D:\\Temp\\Temp\\TopKRules_0.01.txt");
	//FPGrowth(0.001, 1000, "D:\\Temp\\Temp\\TopKRules_0.005.txt");
	//FPGrowth(0.001, 1000, "D:\\Temp\\Temp\\TopKRules_0.004.txt");
	//FPGrowth(0.00095, 1000, "D:\\Temp\\Temp\\TopKRules_0.003.txt");
	//FPGrowth(0.0009, 1000, "D:\\Temp\\Temp\\TopKRules_0.002.txt");
	//FPGrowth(0.00085, 10000, "D:\\Temp\\Temp\\TopKRules_0.001.txt");
	int topk;
	double support, confidence;
	char path[1024] = "D:\\Temp\\Temp\\TopKRules.txt";

	std::cin >> support >> confidence >> topk;

	FPGrowth(support, topk, confidence, path);
	return 0;
}

#endif
