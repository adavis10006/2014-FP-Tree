#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <map>
#include <vector>

#define BUFFER 102400

typedef std::set<std::string> ID_LIST;
typedef std::map<std::string, ID_LIST> TRANSACTION;
TRANSACTION transaction_user, transaction_product;

bool parse_movie_file(std::ifstream &ifs)
{
	std::string productId, userId;

	bool have_productId, have_userId;
	have_productId = have_userId = false;

	std::string line;
	char line_tmp[BUFFER];
	int line_count = 0;

	transaction_user.clear();
	transaction_product.clear();

	ifs.getline(line_tmp, BUFFER);
	line = line_tmp;

	while(ifs.eof() == false)
	{
		if(line == "")
		{
			if(have_productId && have_userId)
			{
				transaction_user[userId].insert(productId);
				transaction_product[productId].insert(userId);
			}

			have_productId = have_userId = false;
		}
		else if(line.find("product/productId: ") != std::string::npos)
		{
			productId = line.substr(line.find(":") + 2);
			have_productId = true;
		}
		else if(line.find("review/userId: ") != std::string::npos)
		{
			userId = line.substr(line.find(":") + 2);
			have_userId = true;
		}

		if(++line_count % 100000 == 0)
			std::cout << '\r' << line_count;

		ifs.getline(line_tmp, BUFFER);
		line = line_tmp;
	}

	std::cout << '\r' << ++line_count << std::endl;
	return true;
}

bool output_transaction()
{
	int line_count;
	std::ofstream output_user("D:\\Temp\\Temp\\trans_user.txt", std::ofstream::out);

	std::cout << transaction_user.size() << std::endl;
	line_count = 0;
	for(TRANSACTION::iterator itTransaction = transaction_user.begin(); itTransaction != transaction_user.end(); ++itTransaction)
	{

		output_user << itTransaction->first.c_str() << " " << itTransaction->second.size();
		for(ID_LIST::iterator itList = itTransaction->second.begin(); itList != itTransaction->second.end(); ++itList)
			output_user << " " << itList->c_str();
		output_user << std::endl;

		if(++line_count % 1000 == 0)
			std::cout << '\r' << line_count;
	}
	std::cout << '\r' << line_count << std::endl;
	output_user.close();

	std::ofstream output_product("D:\\Temp\\Temp\\trans_product.txt", std::ofstream::out);

	std::cout << transaction_product.size() << std::endl;
	line_count = 0;
	for(TRANSACTION::iterator itTransaction = transaction_product.begin(); itTransaction != transaction_product.end(); ++itTransaction)
	{
		output_product << itTransaction->first.c_str() << " " << itTransaction->second.size();
		for(ID_LIST::iterator itList = itTransaction->second.begin(); itList != itTransaction->second.end(); ++itList)
			output_product << " " << itList->c_str();
		output_product << std::endl;

		if(++line_count % 1000 == 0)
			std::cout << '\r' << line_count;
	}
	std::cout << '\r' << line_count << std::endl;
	output_product.close();

	return true;
}
