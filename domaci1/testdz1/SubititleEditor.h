#ifndef SUBTITLEEDITOR_H
#define SUBTITLEEDITOR_H

#include"Subtitle.h"
#include<string>
#include<iostream>
using namespace std;

struct Subs {
	Subtitle* sub;
	Subs* next;
	Subs* prev;
};

class SubtitleEditor {
public:
	SubtitleEditor();
	~SubtitleEditor();
	int loadSubtitles(string subtitle);
	string saveSubtitles();
	int createNewSubtitle(string str, int& pos, int numofsubs);
	void addToSelection(int start, int end);
	void deselect();
	void shiftForward(int ms);
	void shiftBackward(int ms);
	void boldText(int start, int end);
	void italicText(int start, int end);
	void underlineText(int start, int end);
	void removeStyle();
	void insertSubtitle(int start, int length, string text);
	void deleteSubtitles();
	void merge(string subtitles);
	void fixLongLines(int max);
	void fixLongTime(int ms);
	void undo();

private:
	int numOfSub;
	int error;
	bool ifSelected;
	int numActed;
	Subs *subListFirst, *subListLast, *subListCurrent;
	Subs *helpFirst, *helpLast, *helpCurrent;
	string* previousState;
	int readNumber(string str, int& pos);
	string readText(string str, int& pos);
	void updateIndices();
	int createMerge(string str, int& pos, int numofsubs);
	void mergeLists();
	void deleteOne(Subs* current);
	string saveState();
	void stylize(char c, int start, int end);
};
#endif
