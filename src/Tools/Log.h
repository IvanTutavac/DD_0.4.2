#pragma once

void	Log(const char *text);
void	Log(const char *text, int value);
void	Log(char *fileName,const char *text);

void	CleanLogFile();
void	CleanLogFile(char *fileName);
