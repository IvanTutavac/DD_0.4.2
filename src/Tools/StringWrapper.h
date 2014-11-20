#pragma once

void	DD_Strcpy(char *des_, size_t size_, const char* src_);
void	DD_Strncpy(char *des_, size_t size_, const char* src_, size_t count_);
void	DD_Strncat(char *des_,int size_, const char* src_, size_t _count_);
void	DD_Strcat(char *des_,int size_, const char* src_);
char*	DD_Strtok(char *str_, const char* delim_, char **context_);
void	DD_Itoa(int value_, char *des_,int size_, int radix_);
void	DD_Sprintf(char *des_, int size_,const char *first_,const char *second_);