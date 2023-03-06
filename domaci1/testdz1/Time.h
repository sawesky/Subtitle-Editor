#ifndef TIME_H
#define TIME_H
#include<string>
using namespace std;
class Time {
public:
	Time();
	Time(int hh, int mm, int ss, int mmss);

	string setTimeString(int h, int m, int s, int ms);
	string getTimeString();

	void setH(int h);
	void setM(int m);
	void setS(int s);
	void setMS(int ms);
	void setMiliSecs(int miliSecs); 
	void setByMiliSecs(int ms); //namesta celo vreme na osnovu milisekundi;
	int setMiliSecs(int h, int m, int s, int ms);

	int getH();
	int getS();
	int getM();
	int getMS();
	int getMiliSecs();

private:
	int h, m, s, ms;
	string sTime;
	int miliSecs;
};

#endif 

