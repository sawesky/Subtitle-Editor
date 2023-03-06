#include "Text.h"

Text::Text()
{
	this->text = "";
	this->startStyle = nullptr;
	this->endStyle = nullptr;
	this->typeStyle = nullptr;
	this->textSize = 0;
	this->blankNum = 0;
	this->sText = "";
	this->styleNumStart = 0;
	this->styleNumEnd = 0;
	this->textError = 0;
}

Text::~Text()
{
	delete[] startStyle;
	delete[] endStyle;
	delete[] typeStyle;
}

Text::Text(string input)
{
	this->textError = 0;
	string text = getRaw(input);
	this->text = text;
	this->textSize = text.length();
	int bNum = getBNum(text);
	this->blankNum = bNum;
	int styleNumStart = 0;
	int styleNumEnd = 0;
	int* sStyle = getSStyle(input, styleNumStart);
	int* eStyle = getEStyle(input, styleNumEnd);
	char* tStyle = getTStyle(input);
	this->startStyle = sStyle;
	this->endStyle = eStyle;
	this->typeStyle = tStyle;
	this->sText = getStylized(text, sStyle, eStyle, tStyle);
	this->styleNumStart = styleNumStart;
	this->styleNumEnd = styleNumEnd;
}

string Text::getRaw(string input)
{
	string output = "";
	int l = input.length();
	for (int i = 0; i < l ;) {
		if (input[i] == '<' && input[i + 2] == '>') {
			if (input[i + 1] != 'i' && input[i + 1] != 'u' && input[i + 1] != 'b')
				setTextError(1);
			i += 3;
		}
		else if (input[i] == '<' && input[i + 1] == '/') {
			if (input[i + 2] != 'i' && input[i + 2] != 'u' && input[i + 2] != 'b')
				setTextError(1);
			i += 4;
		}
		else {
			output += input[i];
			i++;
		}
	}
	return output;
}

int* Text::getEStyle(string input, int& pos)
{
	int* eStyle = new int[5];
	int l = input.length();
	for (int i = 0; i < l;) {
		if (input[i] == '<' && input[i + 1] == '/')
		{
			eStyle[pos] = i;
			i += 4;
			pos++;
		}
		else {
			i++;
		}
	}
	return eStyle;
}

int* Text::getSStyle(string input, int& pos)
{
	int* sStyle = new int[5];
	int l = input.length();
	for (int i = 0; i < l;) {
		if (input[i] == '<' && input[i + 2] == '>')
		{
			sStyle[pos] = i;
			i += 3;
			pos++;
		}
		else {
			i++;
		}
	}
	return sStyle;
}

char* Text::getTStyle(string input)
{
	char* tStyle = new char[5];
	int pos = 0;
	int l = input.length();
	for (int i = 0; i < l;) {
		if (input[i] == '<' && input[i + 2] == '>')
		{
			tStyle[pos] = input[i + 1];
			i += 3;
			pos++;
		}
		else {
			i++;
		}
	}
	return tStyle;
}

int Text::getBNum(string input)
{
	int bNum = 0;
	int l = input.length();
	for (int i = 0; i < l; i++) {
		if (input[i] == ' ' || input[i] == '\n')
			bNum++;
	}
	return bNum;
}

string Text::getStylized(string input, int* startStyle, int* endStyle, char* typeStyle)
{
	string output = "";
	int posStart = 0;
	int posEnd = 0;
	int l = input.length();
	for (int i = 0; i < l; i++)
	{
		if (startStyle[posStart] == i)
		{
			output += '<';
			output += typeStyle[posEnd];
			output += '>';
			posStart++;
			i--;
		}
		else if (endStyle[posEnd] == i)
		{
			output += '<';
			output += '/';
			output += typeStyle[posEnd];
			output += '>';
			posEnd++;
			i--;
		}
		else {
			output += input[i];
		}
	}
	if (endStyle[posEnd] >= l) {
		output += '<';
		output += '/';
		output += typeStyle[posEnd];
		output += '>';
	}
	return output;
}

void Text::insertAndUpdate(string text, int* startStyle, int* endStyle, char* typeStyle, int numStyle, char c, int start, int end)
{
	if (numStyle == 1)
	{
		startStyle[0] = start;
		endStyle[0] = end;
		typeStyle[0] = c;
	}
	else
	{
		int i = numStyle - 2;
		while (startStyle[i] > start && i >= 0) {
			startStyle[i + 1] = startStyle[i];
			endStyle[i + 1] = endStyle[i];
			typeStyle[i + 1] = typeStyle[i];
			i--;
		}
		startStyle[i] = start;
		endStyle[i] = end;
		typeStyle[i] = c;
		
	}
	setStartStyle(startStyle);
	setEndStyle(endStyle);
	setTypeStyle(typeStyle);
	setStyleNumStart(numStyle);
	setStyleNumEnd(numStyle);
	setSText(getStylized(text, startStyle, endStyle, typeStyle));
}

void Text::setText(string textS)
{
	this->text = textS;
}

void Text::setStartStyle(int* startStyle)
{
	this->startStyle = startStyle;
}

void Text::setEndStyle(int* endStyle)
{
	this->endStyle = endStyle;
}

void Text::setTypeStyle(char* typeStyle)
{
	this->typeStyle = typeStyle;
}

void Text::setTextSize(int textSize)
{
	this->textSize = textSize;
}

void Text::setBlankNum(int blankNum)
{
	this->blankNum = blankNum;
}

void Text::setSText(string sText)
{
	this->sText = sText;
}

void Text::setStyleNumStart(int styleNumStart)
{
	this->styleNumStart = styleNumStart;
}

void Text::setStyleNumEnd(int styleNumEnd)
{
	this->styleNumEnd = styleNumEnd;
}

void Text::setTextError(int textError)
{
	this->textError = textError;
}

string Text::getText()
{
	return this->text;
}

int* Text::getStartStyle()
{
	return this->startStyle;
}

int* Text::getEndStyle()
{
	return this->endStyle;
}

char* Text::getTypeStyle()
{
	return this->typeStyle;
}

int Text::getTextSize()
{
	return this->textSize;
}

int Text::getBlankNum()
{
	return this->blankNum;
}

string Text::getSText()
{
	return this->sText;
}

int Text::getStyleNumStart()
{
	return this->styleNumStart;
}

int Text::getStyleNumEnd()
{
	return this->styleNumEnd;
}

int Text::getTextError()
{
	return this->textError;
}
