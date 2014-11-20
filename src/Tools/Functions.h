#pragma once

#include <string>

namespace DD
{
	// delete character on position i
	// no checks done
	void	DeleteChar(char *src,int i);

	// searches for the first char after =, everything before that will be deleted
	// returns false if there's no = in src
	// For example, SCREEN WIDTH = 640 as src, will result into 640 as src
	bool	LineParse(char *src);

	// insert line in after_
	// pass a buffer c str for before_
	// size_ -> sizeof(before_)
	// updates before_ as part of line before =
	// updates after_ as part of line after =
	// returns false if there's no = in after_
	// For example, SCREEN WIDTH = 640 passed as after_ will result in after_ becoming SCREEN WIDTH and before 640
	bool	LineParse(char *after_, char *before_, const size_t size_);

	// updates string_ by replacing replaceThis_ part of the string with replaceWith_
	// no checks done ! replacing a string that's not there will cause an error 
	void	ReplaceString(char *string_, int size_, char *replaceThis_, char *replaceWith_);

	// updates stru with content from constStr 
	// used when one wants to get a mutable cstr from a std::string 
	// no checks done
	void	GetMutableCString(char *str_, const int size_, const char *constStr_);

	// example: 3.15 as value and 3 as decimals, result: std::string{3.150}
	// example: 3.15 as value and 0 as decimals, result: std::string{3}
	// example: 3 as value, 2 as decimals, result: std::string {3.00}
	template<class T>	std::string	GetPreciseStr(T value, int decimals);

	// example: 3.15 as value and 3 as decimals, result: std::string{3.150}
	// example: 3.15 as value and 0 as decimals, result: std::string{3}
	// example: 3 as value, 2 as decimals, result: std::string {3.00}
	template<> std::string	GetPreciseStr<const char*>(const char *value, int decimals);

	// example: 3.15 as value and 3 as decimals, result: std::string{3.150}
	// example: 3.15 as value and 0 as decimals, result: std::string{3}
	// example: 3 as value, 2 as decimals, result: std::string {3.00}
	template<> std::string	GetPreciseStr<const std::string*>(const std::string *value, int decimals);
};
