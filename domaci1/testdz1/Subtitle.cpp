#include "Subtitle.h"

Subtitle::Subtitle()
{
	this->num = 0;
	this->timeStart = nullptr;
	this->timeEnd = nullptr;
	this->data = nullptr;
	this->selected = false;
}

Subtitle::~Subtitle()
{
	delete timeStart;
	delete timeEnd;
	delete data;
}

Subtitle::Subtitle(int num, Time* timeStart, Time* timeEnd, Text* data)
{
	this->num = num;
	this->timeStart = timeStart;
	this->timeEnd = timeEnd;
	this->data = data;
	this->selected = false;
}

bool Subtitle::findTimeEndError(Time* timeL, Time* timeR, int ms)
{
	if (timeL->getMiliSecs() + ms >= timeR->getMiliSecs())
		return true;
	return false;
}

bool Subtitle::findTimeStartError(Time* timeR, Time* timeL, int ms)
{
	if (timeR->getMiliSecs() - ms <= timeL->getMiliSecs())
		return true;
	return false;
}

void Subtitle::setNum(int num)
{
	this->num = num;
}

void Subtitle::setData(Text* data)
{
	this->data = data;
}

void Subtitle::setSelected(bool selected)
{
	this->selected = selected;
}

void Subtitle::setTimeStart(Time* time)
{
	this->timeStart = time;
}

void Subtitle::setTimeEnd(Time* time)
{
	this->timeEnd = time;
}

int Subtitle::getNum()
{
	return this->num;
}

Text* Subtitle::getData()
{
	return this->data;
}

Time* Subtitle::getTimeStart()
{
	return this->timeStart;
}

Time* Subtitle::getTimeEnd()
{
	return this->timeEnd;
}

bool Subtitle::getSelected()
{
	return this->selected;
}

