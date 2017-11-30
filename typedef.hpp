#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <ctime>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <string>

typedef std::set<std::string> TRANSACTION_LIST;
typedef std::map<std::string, TRANSACTION_LIST> TRANSACTION_MAP;
typedef std::map<std::string, unsigned int> ITEM_COUNT;
typedef std::set<std::string> ITEM_LIST;
typedef std::map<unsigned int, ITEM_LIST, std::greater<unsigned int> > ORDER_ITEM_LIST;
typedef std::map<std::string, ORDER_ITEM_LIST> ORDER_ITEM_MAP;

struct FPTreeNode;

typedef std::list<FPTreeNode> FPTreeNodeList;
typedef std::map<std::string, FPTreeNodeList> FPTreeNodeMap;
typedef std::list<FPTreeNode*> FPTreeNodeLink;

struct FPTreeNode
{
	std::string ID;
	unsigned int value;
	FPTreeNode *parent;
	FPTreeNodeLink child;

	FPTreeNode(std::string ID, unsigned int value, FPTreeNode *parent) :
		ID(ID),
		value(value),
		parent(parent)
	{
		child.clear();
	}
};

typedef std::map<std::string, ITEM_COUNT> FREQUENT_PATTERN_COUNT;
typedef std::map<ITEM_LIST, unsigned int> FREQUENT_PATTERN;

typedef std::set<ITEM_LIST> FREQUENT_ITEM_SET;
typedef std::map<unsigned int, FREQUENT_ITEM_SET, std::greater<unsigned int> > FREQUENT_ITEM_SET_MAP;

typedef std::pair<ITEM_LIST, ITEM_LIST> CONFIDENCE;
typedef std::set<CONFIDENCE> CONFIDENCE_SET;
typedef std::map<double, CONFIDENCE_SET, std::greater<double> > CONFIDENCE_MAP;
typedef std::map<unsigned int, CONFIDENCE_MAP, std::greater<unsigned int> > ASSOCIATION_RULES;

#endif
