#include "Time.h"
using namespace std;
Time::Time()
{
	this->h = this->m = this->s = this->ms = this->miliSecs= 0;
	this->sTime = "";
	
}

Time::Time(int hh, int mm, int ss, int mmss)
{
	this->h = hh;
	this->m = mm;
	this->s = ss;
	this->ms = mmss;
	this->sTime = setTimeString(hh, mm, ss, mmss);
	this->miliSecs = setMiliSecs(hh, mm, ss, mmss);
}

string Time::setTimeString(int h, int m, int s, int ms)
{
	string sTime="";
	if (h < 10) {
		sTime.append("0");
		sTime.append(to_string(h));
	}
	else
		sTime.append(to_string(h));
	sTime.append(":");
	if (m < 10) {
		sTime.append("0");
		sTime.append(to_string(m));
	}
	else
		sTime.append(to_string(m));
	sTime.append(":");
	if (s < 10) {
		sTime.append("0");
		sTime.append(to_string(s));
	}
	else
		sTime.append(to_string(s));
	sTime.append(",");
	if (ms < 100) {
		sTime.append("0");
		if (ms < 10) {
			sTime.append("0");
			sTime.append(to_string(ms));
		}
		else
			sTime.append(to_string(ms));
	}
	else
		sTime.append(to_string(ms));
	return sTime;
}

string Time::getTimeString()
{
	return this->sTime;
}

void Time::setH(int h)
{
	this->h = h;
}

void Time::setM(int m)
{
	this->m = m;
}

void Time::setS(int s)
{
	this->s = s;
}

void Time::setMS(int ms)
{
	this->ms = ms;
}

void Time::setMiliSecs(int miliSecs)
{
	this->miliSecs = miliSecs;
}

void Time::setByMiliSecs(int ms)
{
	this->miliSecs = ms;
	int a = ms / (60 * 60 * 1000);
	this->h = a;
	ms = ms - a * 60 * 60 * 1000;
	int b = ms / (60 * 1000);
	this->m = b;
	ms = ms - b * 60 * 1000;
	int c = ms / 1000;
	this->s = c;
	ms = ms - c * 1000;
	this->ms = ms;
	this->sTime = setTimeString(a, b, c, ms);
	
	
}

int Time::setMiliSecs(int h, int m, int s, int ms)
{
	return ms + s * 1000 + m * 1000 * 60 + h * 1000 * 60 * 60;
}

int Time::getH()
{
	return this->h;
}

int Time::getS()
{
	return this->s;
}

int Time::getM()
{
	return this->m;
}

int Time::getMS()
{
	return this->ms;
}

int Time::getMiliSecs()
{
	return this->miliSecs;
}
