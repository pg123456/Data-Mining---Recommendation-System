/************************************************************
* Name: Patrick Guo
* ID: 11378369
************************************************************/
#ifndef STRINGMANIPULATOR_H
#define STRINGMANIPULATOR_H

#include <vector>
#include <string>

class StringManipulator
{
public:
	static std::vector<std::string> split(const std::string text, const std::string toCut)
	{
		std::vector<std::string> pieces;
		const int cutLen = toCut.size();

		int curStartLoc = 0; //location first letter of current word
		while (text.substr(curStartLoc, cutLen) == toCut)
		{
			curStartLoc += cutLen;
		}
		int curEndLoc = text.find(toCut, curStartLoc); //location of the first toCut after current word

		while (curEndLoc != std::string::npos)
		{
			std::string toAdd = text.substr(curStartLoc, curEndLoc - curStartLoc);
			pieces.push_back(toAdd);

			curStartLoc = curEndLoc + cutLen;
			while (text.substr(curStartLoc, cutLen) == toCut)
			{
				curStartLoc += cutLen;
			}
			curEndLoc = text.find(toCut, curStartLoc);
		}

		std::string lastPiece = text.substr(curStartLoc, std::string::npos);
		if (lastPiece != "")
		{
			pieces.push_back(lastPiece);
		}

		return pieces;
	}
};

#endif 
