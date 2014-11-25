#include "Functions.h"
#include "StringWrapper.h"

void	DD::DeleteChar(char *src,int i)
{
	int l = strlen(src);

	for (; i < l - 1; i++)
	{
		src[i] = src[i + 1];
	}

	src[i] = '\0';
}

bool	DD::LineParse(char *src)
{
	std::string s(src);

	size_t pos = s.find('=');

	if (pos != std::string::npos)
	{
		// character found, search for the next one
		
		pos++;

		for (; pos < s.length(); pos++)
		{
			if (s[pos] != *" ")
			{
				break;
			}
		}

		s = s.substr(pos); // delete everything else

		DD_Strcpy(src, std::char_traits<char>::length(src), s.c_str());

		return	true;
	}
	
	return	false;
}

bool	DD::LineParse(char *after_, char *before_,const size_t size_)
{
	std::string after(after_);

	size_t	posAfter = after.find("=");
	int		posBefore = (int)posAfter;

	if (posAfter != std::string::npos) 
	{
		// character = found, now we need to update after_ and before_

		posBefore--;

		for (; posBefore >= 0; posBefore--)
		{
			if (after[posBefore] != *" ") // find the end of the string before = 
				break;					  // because of that increase posBefore by one later, because we also want that end character 
		}

		// get the string before =
		DD_Strncpy(before_, size_, after_, ++posBefore); 
		
		posAfter++;
		
		for (; posAfter < after.length(); posAfter++)
		{
			if (after[posAfter] != *" ")	// find the next character
				break;
		}

		after = after.substr(posAfter); // delete everything else

		// get the string after =
		DD_Strcpy(after_, std::char_traits<char>::length(after_), after.c_str());
		
		return	true;
	}
	return	false;
}

void DD::ReplaceString(char *string_, int size_, char *replaceThis_, char *replaceWith_)
{
	std::string s(string_);
	std::string replaceThis(replaceThis_);
	std::string replaceWith(replaceWith_);

	s.replace(s.find(replaceThis), replaceThis.length(), replaceWith);

	DD_Strcpy(string_, size_, s.c_str());
}

void	DD::GetMutableCString(char *str_, const int size_, const char *constStr_)
{
	DD_Strcpy(str_, size_, constStr_);
}

std::string	GetFloatStr(std::string str, size_t decimals)
{
	size_t found{ str.find(".") };

	if (found == std::string::npos)
		str += ".", found = str.length() - 1;

	size_t j{ 0 };

	for (size_t i = found + 1; j < decimals; ++i, ++j)
	{
		if (i >= str.length())
			str += '0';
	}

	size_t k{ 0 };

	if (j != 0)
		k = 1;

	return	str.substr(0, found + j + k);
}

template<class T>	std::string	DD::GetPreciseStr(T value, int decimals)
{
	return	GetFloatStr(std::to_string(value), decimals);
}

template<> std::string	DD::GetPreciseStr<const char*>(const char *value, int decimals)
{
	return	GetFloatStr(value, decimals);
}

template<> std::string	DD::GetPreciseStr<const std::string*>(const std::string *value, int decimals)
{
	return	GetFloatStr(*value, decimals);
}

template	std::string DD::GetPreciseStr<int>(int, int);
template	std::string DD::GetPreciseStr<float>(float, int);
template	std::string DD::GetPreciseStr<double>(double, int);
template	std::string DD::GetPreciseStr<const char*>(const char*, int);
template	std::string	DD::GetPreciseStr<const std::string*>(const std::string*, int);
