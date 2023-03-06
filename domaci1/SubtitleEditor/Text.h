#ifndef TEXT_H
#define TEXT_H

#include<string>
using namespace std;
class Text {

public:
	Text();
	~Text();
	Text(string input);
	string getRaw(string input);
	int* getEStyle(string input, int& pos);
	int* getSStyle(string input, int& pos);
	char* getTStyle(string input);
	int getBNum(string input);
	string getStylized(string input, int* startStyle, int* endStyle, char* typeStyle);

	void insertAndUpdate(string text, int* startStyle, int* endStyle, char* typeStyle, int numStyle, char c, int start, int end);

	void setText(string text);
	void setStartStyle(int* startStyle);
	void setEndStyle(int* endStyle);
	void setTypeStyle(char* typeStyle);
	void setTextSize(int textSize);
	void setBlankNum(int blankNum);
	void setSText(string sText);
	void setStyleNumStart(int styleNumStart);
	void setStyleNumEnd(int styleNumEnd);
	void setTextError(int textError);

	string getText();
	int* getStartStyle();
	int* getEndStyle();
	char* getTypeStyle();
	int getTextSize();
	int getBlankNum();
	string getSText();
	int getStyleNumStart();
	int getStyleNumEnd();
	int getTextError();




private:
	string text;
	int* startStyle;
	int* endStyle;
	char* typeStyle;
	int textSize;
	int blankNum;
	string sText;
	int styleNumStart;
	int styleNumEnd;
	int textError;
};

#endif 

