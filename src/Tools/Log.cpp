#include "Log.h"
#include <fstream>
#include <string>

void	Log(const char *text)
{
	std::ofstream	logDat;

	logDat.open("log.txt", std::ios::app);

	logDat << text << std::endl;

	logDat.close();
}

void	Log(const char *text, int value)
{
	std::ofstream logDat;

	logDat.open("log.txt", std::ios::app);

	logDat << text << value << std::endl;
}

void	Log(char *fileName,const char *text)
{
	std::string tmpFileName(fileName);

	if (tmpFileName.rfind(".txt") == std::string::npos)
	{
		tmpFileName += ".txt";
	}

	std::ofstream	logDat;

	logDat.open(tmpFileName, std::ios::app);

	logDat << text << std::endl;

	logDat.close();
}

void	CleanLogFile()
{
	std::ofstream	logDat("log.txt");

	logDat.close();
}

void	CleanLogFile(char *fileName)
{
	std::string	tmpFileName(fileName);

	if (tmpFileName.rfind(".txt") == std::string::npos)
	{
		tmpFileName += ".txt";
	}

	std::ofstream	logDat(tmpFileName);

	logDat.close();
}
