#include "StringWrapper.h"
#include <string>

void DD_Strcpy(char *des_, size_t size_, const char* src_)
{
#if defined(_MSC_VER)
	strcpy_s(des_, size_, src_);
#else 
	strcpy(des_, src_);
#endif
}

void	DD_Strncpy(char *des_, size_t size_, const char* src_, size_t count_)
{
#if defined(_MSC_VER)
	strncpy_s(des_, size_, src_, count_);
#else
	strncpy(des_, src_, count_);
	des_[count_] = '\0';
#endif
}

void	DD_Strncat(char *des_,int size_,const char* src_, size_t _count_)
{
#if defined(_MSC_VER)
	strncat_s(des_,size_ , src_, _count_);
#else 
	strncat(des_, src_,_count_);
#endif
}

void	DD_Strcat(char *des_, int size_,const char* src_)
{
#if defined(_MSC_VER)
	strcat_s(des_,size_, src_);
#else
	strcat(des_, src_);
#endif
}

char*	DD_Strtok(char *str_, const char* delim_, char **context_)
{
#if defined(_MSC_VER)
	return	strtok_s(str_, delim_, context_);
#else
	return	strtok(str_, delim_);
#endif
}

void	DD_Itoa(int value_, char *des_,int size_, int radix_)
{
#if defined(_MSC_VER)
	_itoa_s(value_, des_, size_, radix_); 
#else
	itoa(value_, des_, radix_);
#endif
}

void	DD_Sprintf(char *des_, int size_, const char *first_, const char *second_)
{
#if defined(_MSC_VER)
	sprintf_s(des_,size_, first_, second_);
#else
	sprintf(des_, first_, second_);
#endif
}