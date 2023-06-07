#ifndef BITCOINEXCHANGE
#define BITCOINEXCHANGE

#include <iostream>
#include <map>
#include <cstring>
#include <fstream>
#include <string>
#include <locale>

class Btc
{
	private:
		std::string date;
		double value;
		std::map<std::string, double> map;
	public:
		Btc();
		Btc(const std::string name);
		Btc(Btc const & src);
		Btc &		operator=( Btc const & rhs );
		~Btc();
		void fill_map(std::string name);
		void set_values(std::string date, double value);
		void search_value();
		int parse_date(std::string date);
		int parse_value(long double value);
		void error();
		int parse_val(std::string val);
		int comma(std::string str);
		int isdigit_all(std::string str);
		int check_signs(std::string val);
};

#endif