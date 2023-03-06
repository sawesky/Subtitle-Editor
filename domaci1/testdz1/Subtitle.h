#ifndef SUBTITLE_H
#define SUBTITLE_H

#include<string>
#include "Time.h"
#include "Text.h"
using namespace std;


class Subtitle {
public:
	Subtitle();
	~Subtitle();
	Subtitle(int num, Time* timeStart, Time* timeEnd, Text* data);

	bool findTimeEndError(Time* timeL, Time* timeR, int ms);
	bool findTimeStartError(Time* timeR, Time* timeL, int ms);


	void setNum(int num);
	void setData(Text* data);
	void setSelected(bool selected);
	void setTimeStart(Time* time);
	void setTimeEnd(Time* time);

	int getNum();
	Text* getData();
	Time* getTimeStart();
	Time* getTimeEnd();
	bool getSelected();

private:
	bool selected;
	int num;
	Time* timeStart;
	Time* timeEnd;
	Text* data;
	
};

#endif 
