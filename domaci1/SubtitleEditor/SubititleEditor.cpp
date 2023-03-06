#include "SubititleEditor.h"

SubtitleEditor::SubtitleEditor()
{
	this->subListFirst = this->subListLast = this->subListCurrent = nullptr;
	this->numOfSub = 0;
	this->error = -1;
	this->ifSelected = false;
	string* states = new string[10];
	this->previousState = states;
	this->helpCurrent = this->helpFirst = this->helpLast = nullptr;
	this->numActed = 0;
	//this->selListFirst = this->selListCurrent = this->selListLast = nullptr;
}

SubtitleEditor::~SubtitleEditor()//i help izbrisi?
{
	while (this->subListFirst != nullptr) {
		this->subListCurrent = this->subListFirst;
		this->subListFirst = this->subListFirst->next;
		delete subListCurrent;

	}
	delete subListFirst;
	delete[] previousState;
}

int SubtitleEditor::loadSubtitles(string subtitle)
{
	if (this->subListFirst != nullptr)
	{
		while (this->subListFirst != nullptr) {
			this->subListCurrent = this->subListFirst;
			this->subListFirst = this->subListFirst->next;
			delete subListCurrent;
		}
		delete subListFirst;
	}
	int pos = 0;
	int flag = 0;
	int numofsubs = 0;
	while (subtitle[pos] != '\0')
	{
		flag = createNewSubtitle(subtitle, pos, numofsubs);
		if (flag) {
			cout << numofsubs;
			return flag;
		}
		else {
			numofsubs++;
		}
	}
	if(!flag)
	this->numOfSub = numofsubs;
	this->previousState[numActed] = saveState();
	this->numActed++;
	return -1;
}

string SubtitleEditor::saveSubtitles()
{
	return previousState[numActed - 1];
}

int SubtitleEditor::createNewSubtitle(string str, int& pos, int numofsubs)
{
	int index = readNumber(str, pos);
	if (index == -1) {
		return numofsubs + 1;
	}
	int h1 = readNumber(str, pos);
	int m1 = readNumber(str, pos);
	int s1 = readNumber(str, pos);
	int ms1 = readNumber(str, pos);
	if (h1 == -1 || m1 == -1 || s1 == -1 || ms1 == -1 || h1 > 99 || m1 > 59 || s1 > 59 || ms1 > 999)
		return index;
	Time* timeStart = new Time(h1, m1, s1, ms1);
	pos += 4;
	int h2 = readNumber(str, pos);
	int m2 = readNumber(str, pos);
	int s2 = readNumber(str, pos);
	int ms2 = readNumber(str, pos);
	if (h2 == -1 || m2 == -1 || s2 == -1 || ms2 == -1 || h2 > 99 || m2 > 59 || s2 > 59 || ms2 > 999)
		return index;
	Time* timeEnd = new Time(h2, m2, s2, ms2);
	string text = readText(str, pos);
	Text* data = new Text(text);
	if (data->getTextError() == 1 || data->getStyleNumEnd() != data->getStyleNumStart())
	{
		delete data;
		return index;
	}
	Subtitle* subtitle = new Subtitle(index, timeStart, timeEnd, data);
	Subs* subtitlesList = new Subs();
	subtitlesList->sub = subtitle;
	subtitlesList->next = nullptr;
	subtitlesList->prev = nullptr;
	if (!this->subListFirst)
		this->subListFirst = this->subListLast = this->subListCurrent = subtitlesList;
	else
	{
		subtitlesList->prev = this->subListLast;
		this->subListLast->next = subtitlesList;
		this->subListLast = subtitlesList;
	}
	return 0;
}

void SubtitleEditor::addToSelection(int start, int end)
{
	this->ifSelected = true;
	int pos = 1;
	if (start <= 0)
		start = 1;
	if (end > this->numOfSub)
		end = this->numOfSub;
	this->subListCurrent = this->subListFirst;
	while (this->subListCurrent != nullptr)
	{
		if (pos >= start && pos <= end) {
			this->subListCurrent->sub->setSelected(true);
		}
		pos++;
		this->subListCurrent = this->subListCurrent->next;
	}

}

void SubtitleEditor::deselect()//zavrsen
{
	this->ifSelected = false;
	this->subListCurrent = this->subListFirst;
	while (this->subListCurrent != nullptr)
	{
		this->subListCurrent->sub->setSelected(false);
	}
}

void SubtitleEditor::shiftForward(int ms)//zavrsen
{
	this->subListCurrent = this->subListFirst;
	bool flag = false;
	if (this->ifSelected)
	{
		while (this->subListCurrent->next != nullptr) {
			if (this->subListCurrent->sub->getSelected() == true && this->subListCurrent->next->sub->getSelected() == false) {
				flag = this->subListCurrent->sub->findTimeEndError(this->subListCurrent->sub->getTimeEnd(), this->subListCurrent->next->sub->getTimeStart(), ms);
				if (flag)
					break;
			}
			this->subListCurrent = this->subListCurrent->next;
		}
		this->subListCurrent = this->subListFirst;
		if (!flag) {
			while (this->subListCurrent != nullptr)
			{
				if (this->subListCurrent->sub->getSelected()) {
					int time = this->subListCurrent->sub->getTimeStart()->getMiliSecs();
					time += ms;
					this->subListCurrent->sub->getTimeStart()->setByMiliSecs(time);
					time = this->subListCurrent->sub->getTimeEnd()->getMiliSecs();
					time += ms;
					this->subListCurrent->sub->getTimeStart()->setByMiliSecs(time);
				}
				this->subListCurrent = this->subListCurrent->next;
			}
		}
	}
	else
	{
		while (this->subListCurrent != nullptr)//dobar
		{
			int time = this->subListCurrent->sub->getTimeStart()->getMiliSecs();
			time += ms;
			this->subListCurrent->sub->getTimeStart()->setByMiliSecs(time);
			time = this->subListCurrent->sub->getTimeEnd()->getMiliSecs();
			time += ms;
			this->subListCurrent->sub->getTimeEnd()->setByMiliSecs(time);
			this->subListCurrent = this->subListCurrent->next;
		}
	}
	this->previousState[numActed] = saveState();
	this->numActed++;
}

void SubtitleEditor::shiftBackward(int ms)
{
	this->subListCurrent = this->subListLast;
	bool flag = false;
	if (this->ifSelected) {
		while (this->subListCurrent->prev != nullptr) {
			if (this->subListCurrent->sub->getSelected() == true && this->subListCurrent->prev->sub->getSelected() == false) {
				flag = this->subListCurrent->sub->findTimeStartError(this->subListCurrent->sub->getTimeStart(), this->subListCurrent->prev->sub->getTimeEnd(), ms);
				if (flag)
					break;
			}
			this->subListCurrent = this->subListCurrent->prev;
		}
		this->subListCurrent = this->subListLast;
		if (!flag) {
			while (this->subListCurrent != nullptr) {
				if (this->subListCurrent->sub->getSelected()) {
					int time = this->subListCurrent->sub->getTimeEnd()->getMiliSecs();
					time -= ms;
					this->subListCurrent->sub->getTimeEnd()->setByMiliSecs(time);
					time = this->subListCurrent->sub->getTimeStart()->getMiliSecs();
					time -= ms;
					this->subListCurrent->sub->getTimeStart()->setByMiliSecs(time);
				}
				this->subListCurrent = this->subListCurrent->prev;
			}
		}
	}
	else {
		while (this->subListCurrent != nullptr) {
			int time = this->subListCurrent->sub->getTimeEnd()->getMiliSecs();
			time -= ms;
			this->subListCurrent->sub->getTimeEnd()->setByMiliSecs(time);
			time = this->subListCurrent->sub->getTimeStart()->getMiliSecs();
			time -= ms;
			this->subListCurrent->sub->getTimeStart()->setByMiliSecs(time);
			this->subListCurrent = this->subListCurrent->prev;
		}
	}
	this->previousState[numActed] = saveState();
	this->numActed++;
}

void SubtitleEditor::boldText(int start, int end)//popravi
{
	if (this->ifSelected) {
		stylize('b', start, end);
		this->previousState[numActed] = saveState();
		this->numActed++;
	}
}

void SubtitleEditor::italicText(int start, int end)
{
	if (this->ifSelected) {
		stylize('i', start, end);
		this->previousState[numActed] = saveState();
		this->numActed++;
	}
}

void SubtitleEditor::underlineText(int start, int end)
{
	if (this->ifSelected) {
		stylize('u', start, end);
		this->previousState[numActed] = saveState();
		this->numActed++;
	}
}

void SubtitleEditor::removeStyle()
{
	if (this->ifSelected) {
		this->subListCurrent = this->subListFirst;
		while (this->subListCurrent != nullptr) {
			if (this->subListCurrent->sub->getSelected()) {
				int* startStyle = this->subListCurrent->sub->getData()->getStartStyle();
				delete[] startStyle;
				int* sStyle = new int[5];
				this->subListCurrent->sub->getData()->setStartStyle(sStyle);

				int* endStyle = this->subListCurrent->sub->getData()->getEndStyle();
				delete[] endStyle;
				int* eStyle = new int[5];
				this->subListCurrent->sub->getData()->setEndStyle(eStyle);

				char* typeStyle = this->subListCurrent->sub->getData()->getTypeStyle();
				delete[] typeStyle;
				char* tStyle = new char[5];
				this->subListCurrent->sub->getData()->setTypeStyle(tStyle);

				this->subListCurrent->sub->getData()->setStyleNumStart(0);
				this->subListCurrent->sub->getData()->setStyleNumEnd(0);
				this->subListCurrent->sub->getData()->setSText(this->subListCurrent->sub->getData()->getText());
			}
			this->subListCurrent = this->subListCurrent->next;
		}
		this->previousState[numActed] = saveState();
		this->numActed++;
	}
}

void SubtitleEditor::insertSubtitle(int start, int length, string text)
{
	this->subListCurrent = this->subListFirst->next;
	Time* timeStart = new Time();
	timeStart->setByMiliSecs(start);
	Time* timeEnd = new Time();
	timeEnd->setByMiliSecs(start + length);
	int index = 1;
	if (text[text.length() - 1] == '\n')
		text = text.substr(0, text.length() - 1);
	Text* data = new Text(text);
	Subtitle* subtitle = new Subtitle(index, timeStart, timeEnd, data);
	Subs* newSub = new Subs();
	newSub->next = nullptr;
	newSub->prev = nullptr;
	newSub->sub = subtitle;
	if (this->subListFirst->sub->getTimeStart()->getMiliSecs() > timeEnd->getMiliSecs())
	{
		this->subListFirst->prev = newSub;
		newSub->next = this->subListFirst;
		this->subListFirst = newSub;
	}
	else if (this->subListLast->sub->getTimeEnd()->getMiliSecs() < timeStart->getMiliSecs()) {
		this->subListLast->next = newSub;
		newSub->prev = this->subListLast;
		this->subListLast = newSub;
	}
	else {
		while (this->subListCurrent != nullptr) {
			if (this->subListCurrent->prev->sub->getTimeEnd()->getMiliSecs() < timeStart->getMiliSecs() && this->subListCurrent->sub->getTimeStart()->getMiliSecs() > timeEnd->getMiliSecs())
			{
				newSub->prev = this->subListCurrent->prev;
				newSub->next = this->subListCurrent;
				this->subListCurrent->prev->next = newSub;
				this->subListCurrent->prev = newSub;
				break;
			}
			this->subListCurrent = this->subListCurrent->next;
		}
	}
	int numSubs = this->numOfSub;
	numSubs++;
	this->numOfSub = numSubs;
	updateIndices();
	this->previousState[numActed] = saveState();
	this->numActed++;

}

void SubtitleEditor::deleteSubtitles()
{
	if (this->ifSelected) {
		while (this->subListFirst->sub->getSelected()) {
			this->subListCurrent = this->subListFirst;
			this->subListFirst = this->subListFirst->next;
			delete this->subListCurrent;
		}
		while (this->subListLast->sub->getSelected()) {
			this->subListCurrent = this->subListLast;
			this->subListLast = this->subListLast->prev;
			delete this->subListCurrent;
		}
		this->subListCurrent = this->subListFirst;
		while (this->subListCurrent != nullptr)//mozda next next posle ovoga
		{
			if (this->subListCurrent->sub->getSelected()) {
				Subs* dummy = this->subListCurrent;
				this->subListCurrent = this->subListCurrent->next;
				dummy->prev->next = this->subListCurrent;
				this->subListCurrent->prev = dummy->prev;
				delete dummy;
			}
			else {
				this->subListCurrent = this->subListCurrent->next;
			}
		}


		updateIndices();
		this->previousState[numActed] = saveState();
		this->numActed++;
		this->ifSelected = false;
	}
}

void SubtitleEditor::merge(string subtitles)
{
	int pos = 0;
	int flag = 0;
	int numofsubs = 0;
	while (subtitles[pos] != '\0')
	{
		flag = createMerge(subtitles, pos, numofsubs);
		if (flag) {
			this->error = 1;
		}
		else {
			numofsubs++;
		}
	}
	if (!flag) {
		this->numOfSub += numofsubs;
		mergeLists();
		updateIndices();
		this->previousState[numActed] = saveState();
		this->numActed++;
	}
}

void SubtitleEditor::fixLongLines(int max)
{
	this->subListCurrent = this->subListFirst;
	if (ifSelected) {
		while (this->subListCurrent != nullptr) {
			if (this->subListCurrent->sub->getSelected()) {
				string textS = this->subListCurrent->sub->getData()->getText();
				int sLength = textS.length();
				int cnt = 0;
				int carry = 0;
				for (int i = 0; i < sLength; i++) {
					if (textS[i] == '\n')
						cnt = 0;
					if (textS[i] != ' ' && textS[i] != '\n') {
						cnt++;
						if (cnt > max) {
							int j = i;
							while (1)
							{
								if (textS[j] == ' ') {
									break;
								}
								j--;
							}
							textS[j] = '\n';
							i = j;
							cnt = 0;
						}
					}
				}
				int* start = this->subListCurrent->sub->getData()->getStartStyle();
				int* end = this->subListCurrent->sub->getData()->getEndStyle();
				char* type = this->subListCurrent->sub->getData()->getTypeStyle();
				this->subListCurrent->sub->getData()->setSText(this->subListCurrent->sub->getData()->getStylized(textS, start, end, type));
			}
			this->subListCurrent = this->subListCurrent->next;
		}
	}
	else {
		while (this->subListCurrent != nullptr) { // pazi na duzinu reci kad prelamas da se ne menja
			string textS = this->subListCurrent->sub->getData()->getText();
			int sLength = textS.length();
			int cnt = 0;
			int carry = 0;
			for (int i = 0; i < sLength; i++) {
				if (textS[i] == '\n')
					cnt = 0;
				if (textS[i] != ' ' && textS[i] != '\n') {
					cnt++;
					if (cnt > max) {
						int j = i;
						while (1)
						{
							if (textS[j] == ' ') {
								break;
							}
							j--;
						}
						textS[j] ='\n';
						i = j;
						cnt = 0;
					}
				}
			}
			int* start = this->subListCurrent->sub->getData()->getStartStyle();
			int* end = this->subListCurrent->sub->getData()->getEndStyle();
			char* type = this->subListCurrent->sub->getData()->getTypeStyle();
			this->subListCurrent->sub->getData()->setSText(this->subListCurrent->sub->getData()->getStylized(textS, start, end, type));
			this->subListCurrent = this->subListCurrent->next;
		}
	}
	this->previousState[numActed] = saveState();
	this->numActed++;
}

void SubtitleEditor::fixLongTime(int ms)
{
	this->subListCurrent = this->subListFirst;
	if (ifSelected) {
		while (this->subListCurrent->next != nullptr) {
			if (this->subListCurrent->sub->getSelected()) {
				Time* timeS = this->subListCurrent->sub->getTimeStart();
				Time* timeE = this->subListCurrent->sub->getTimeEnd();
				string text = this->subListCurrent->sub->getData()->getText();
				int textLength = this->subListCurrent->sub->getData()->getTextSize();
				int numWords = 0;
				int words[20] = {};
				for (int i = 0; i < textLength; i++) {
					if (text[i] == ' ' || text[i] == '\n')
					{
						words[numWords] = i;
						numWords++;

					}
				}
				words[numWords] = 0;
				numWords++;
				int start = timeS->getMiliSecs();
				int end = timeE->getMiliSecs();
				int flag = 0;
				if (end-start > ms) {
					int parts = (timeE->getMiliSecs() - timeS->getMiliSecs()) / ms + ((timeE->getMiliSecs() - timeS->getMiliSecs()) % ms ? 1 : 0);
					int equalMS = (end - start) / parts;
					int equalLine = numWords / parts + (numWords % parts ? 1 : 0);
					int oldTimeMS = timeS->getMiliSecs();
					
					for (int i = 1; i <= parts; i++) {
						Time* timeStart = new Time();
						Time* timeEnd = new Time();
						string textSubstr;
						if (i == 1) {
							timeStart->setByMiliSecs(timeS->getMiliSecs());
							oldTimeMS = timeStart->getMiliSecs() + equalMS;
							timeEnd->setByMiliSecs(oldTimeMS);
							textSubstr = text.substr(0, words[equalLine - 1]);
						}
						else if (i == parts) {
							timeStart->setByMiliSecs(oldTimeMS);
							timeEnd->setByMiliSecs(timeE->getMiliSecs());
							textSubstr = text.substr(words[(i - 1) * equalLine - 1] + 1);

						}
						else {
							timeStart->setByMiliSecs(oldTimeMS);
							oldTimeMS += equalMS;
							timeEnd->setByMiliSecs(oldTimeMS);
							textSubstr = text.substr(words[(i - 1) * equalLine - 1] + 1, words[i * equalLine - 1] - (words[(i - 1) * equalLine - 1] + 1));
						}
						Text* newData = new Text(textSubstr);
						Subtitle* subtitle = new Subtitle(0, timeStart, timeEnd, newData);
						Subs* newSub = new Subs();
						newSub->sub = subtitle;
						newSub->next = nullptr;
						newSub->prev = nullptr;
						if (!this->helpFirst)
							this->helpFirst = this->helpCurrent = this->helpLast = newSub;
						else
						{
							newSub->prev = this->helpLast;
							this->helpLast->next = newSub;
							this->helpLast = newSub;
						}
					}
					flag = 1;
				}
				if (flag) {
					Subs* dummyPrev = this->subListCurrent;
					Subs* dummyNext = this->subListCurrent->next;
					dummyPrev->prev->next = this->helpFirst;
					this->helpFirst->prev = dummyPrev->prev;
					this->helpLast->next = dummyNext;
					dummyNext->prev = helpLast;
					this->subListCurrent = this->helpLast;
					delete dummyPrev;
					flag = 0;
				}
			}
			this->subListCurrent = this->subListCurrent->next;
		}
		
	}
	else {
		while (this->subListCurrent->next != nullptr) {
			Time* timeS = this->subListCurrent->sub->getTimeStart();
			Time* timeE = this->subListCurrent->sub->getTimeEnd();
			string text = this->subListCurrent->sub->getData()->getText();
			int textLength = this->subListCurrent->sub->getData()->getTextSize();
			int numWords = 0;
			int words[20] = {};
			for (int i = 0; i < textLength; i++) {
				if (text[i] == ' ' || text[i] == '\n')
				{
					words[numWords] = i;
					numWords++;

				}
			}
			words[numWords] = 0;
			numWords++;
			int start = timeS->getMiliSecs();
			int end = timeE->getMiliSecs();
			int flag = 0;
			if (end - start > ms) {
				int parts = (timeE->getMiliSecs() - timeS->getMiliSecs()) / ms + ((timeE->getMiliSecs() - timeS->getMiliSecs()) % ms ? 1 : 0);
				int equalMS = (end - start) / parts;
				int equalLine = numWords / parts + (numWords % parts ? 1 : 0);
				int oldTimeMS = timeS->getMiliSecs();

				for (int i = 1; i <= parts; i++) {
					Time* timeStart = new Time();
					Time* timeEnd = new Time();
					string textSubstr;
					if (i == 1) {
						timeStart->setByMiliSecs(timeS->getMiliSecs());
						oldTimeMS = timeStart->getMiliSecs() + equalMS;
						timeEnd->setByMiliSecs(oldTimeMS);
						textSubstr = text.substr(0, words[equalLine - 1]);
					}
					else if (i == parts) {
						timeStart->setByMiliSecs(oldTimeMS);
						timeEnd->setByMiliSecs(timeE->getMiliSecs());
						textSubstr = text.substr(words[(i - 1) * equalLine - 1] + 1);

					}
					else {
						timeStart->setByMiliSecs(oldTimeMS);
						oldTimeMS += equalMS;
						timeEnd->setByMiliSecs(oldTimeMS);
						textSubstr = text.substr(words[(i - 1) * equalLine - 1] + 1, words[i * equalLine - 1] - (words[(i - 1) * equalLine - 1] + 1));
					}
					Text* newData = new Text(textSubstr);
					Subtitle* subtitle = new Subtitle(0, timeStart, timeEnd, newData);
					Subs* newSub = new Subs();
					newSub->sub = subtitle;
					newSub->next = nullptr;
					newSub->prev = nullptr;
					if (!this->helpFirst)
						this->helpFirst = this->helpCurrent = this->helpLast = newSub;
					else
					{
						newSub->prev = this->helpLast;
						this->helpLast->next = newSub;
						this->helpLast = newSub;
					}
				}
				flag = 1;
			}
			if (flag) {
				Subs* dummyPrev = this->subListCurrent;
				Subs* dummyNext = this->subListCurrent->next;
				dummyPrev->prev->next = this->helpFirst;
				this->helpFirst->prev = dummyPrev->prev;
				this->helpLast->next = dummyNext;
				dummyNext->prev = helpLast;
				this->subListCurrent = this->helpLast;
				delete dummyPrev;
				flag = 0;
			}
			this->subListCurrent = this->subListCurrent->next;
		}
	}
	updateIndices();
	this->previousState[numActed] = saveState();
	this->numActed++;
}

void SubtitleEditor::undo()
{
	if (this->numActed)
		this->numActed--;
	if (this->numActed > 0) {
		int pos = 0;
		int flag = 0;
		int numofsubs = 0;
		while (this->previousState[numActed - 1][pos] != '\0') {
			flag = createMerge(previousState[numActed - 1], pos, numofsubs);
			numofsubs++;
		}
		while (this->subListFirst != nullptr) {
			this->subListCurrent = this->subListFirst;
			this->subListFirst = this->subListFirst->next;
			delete subListCurrent;
		}
		this->subListCurrent = this->helpCurrent;
		this->subListFirst = this->helpFirst;
		this->subListLast = this->helpLast;
		this->helpCurrent = nullptr;
		this->helpFirst = nullptr;
		this->helpLast = nullptr;
	}
}

int SubtitleEditor::readNumber(string str, int& pos)
{
	int number = 0;
	while (str[pos] != '\n' && str[pos] != ':' && str[pos] != ' ' && str[pos] != ',')
	{
		if (str[pos] >= '0' && str[pos] <= '9') {
			number = number * 10 + (str[pos] - '0');
			pos++;
		}
		else 
			return -1;
		
	}
	pos++;
	return number;
}

string SubtitleEditor::readText(string str, int& pos)
{
	string text = "";
	while (1) {
		if (str[pos] == '\n' && str[pos + 1] == '\n')
			break;
		else {
			text += str[pos];
			pos++;
		}
		
	}
	pos++; pos++;
	return text;
}

void SubtitleEditor::updateIndices()
{
	int pos = 1;
	this->subListCurrent = this->subListFirst;
	while (this->subListCurrent != nullptr)
	{
		this->subListCurrent->sub->setNum(pos);
		pos++;
		this->subListCurrent = this->subListCurrent->next;
	}
	this->numOfSub = pos - 1;
}

int SubtitleEditor::createMerge(string str, int& pos, int numofsubs)
{
	int index = readNumber(str, pos);
	if (index == -1) {
		return numofsubs + 1;
	}
	int h1 = readNumber(str, pos);
	int m1 = readNumber(str, pos);
	int s1 = readNumber(str, pos);
	int ms1 = readNumber(str, pos);
	if (h1 == -1 || m1 == -1 || s1 == -1 || ms1 == -1 || h1 > 99 || m1 > 59 || s1 > 59 || ms1 > 999)
		return index;
	Time* timeStart = new Time(h1, m1, s1, ms1);
	pos += 4;
	int h2 = readNumber(str, pos);
	int m2 = readNumber(str, pos);
	int s2 = readNumber(str, pos);
	int ms2 = readNumber(str, pos);
	if (h2 == -1 || m2 == -1 || s2 == -1 || ms2 == -1 || h2 > 99 || m2 > 59 || s2 > 59 || ms2 > 999)
		return index;
	Time* timeEnd = new Time(h2, m2, s2, ms2);
	string text = readText(str, pos);
	Text* data = new Text(text);
	if (data->getTextError() == 1 || data->getStyleNumEnd() != data->getStyleNumStart())
	{
		delete data;
		return index;
	}
	Subtitle* subtitle = new Subtitle(index, timeStart, timeEnd, data);
	Subs* subtitlesList = new Subs();
	subtitlesList->sub = subtitle;
	subtitlesList->next = nullptr;
	subtitlesList->prev = nullptr;
	if (!this->helpFirst)
		this->helpFirst = this->helpLast = this->helpLast = subtitlesList;
	else
	{
		subtitlesList->prev = this->helpLast;
		this->helpLast->next = subtitlesList;
		this->helpLast = subtitlesList;
	}
	return 0;
}

void SubtitleEditor::mergeLists()
{
	if (this->subListFirst->sub->getTimeStart()->getMiliSecs() > this->helpLast->sub->getTimeEnd()->getMiliSecs()) {
		this->helpLast->next = this->subListFirst;
		this->subListFirst->prev = this->helpLast;
		this->subListFirst = this->helpFirst;
	}
	else if (this->subListLast->sub->getTimeEnd()->getMiliSecs() < this->helpFirst->sub->getTimeEnd()->getMiliSecs()) {
		this->helpFirst->prev = this->subListLast;
		this->subListLast->next = this->helpFirst;
		this->subListLast = this->helpLast;
	}
	else {
		this->subListCurrent = this->subListFirst;
		while (this->subListCurrent->next != nullptr) {
			if (this->subListCurrent->sub->getTimeEnd()->getMiliSecs() < this->helpFirst->sub->getTimeStart()->getMiliSecs() && this->subListCurrent->next->sub->getTimeStart()->getMiliSecs() > this->helpLast->sub->getTimeEnd()->getMiliSecs()) {
				this->helpFirst->prev = this->subListCurrent;
				this->helpLast->next = this->subListCurrent->next;
				this->subListCurrent->next->prev = this->helpLast;
				this->subListCurrent->next = this->helpLast;
			}
			this->subListCurrent = this->subListCurrent->next;
		}
	}
}

void SubtitleEditor::deleteOne(Subs* current)
{
	if (current == this->subListFirst) {
		this->subListFirst = this->subListFirst->next;
		delete current;
	}
	else if (current == this->subListLast) {
		this->subListLast = this->subListLast->prev;
		delete current;
	}
	else {
		Subs* dummy = current;
		current = current->next;
		dummy->prev->next = current;
		current->prev = dummy->prev;
		delete dummy;
	}
}

string SubtitleEditor::saveState()
{
	this->subListCurrent = this->subListFirst;
	string saved = "";
	while (this->subListCurrent != nullptr) {
		saved.append(to_string(this->subListCurrent->sub->getNum()));
		saved.append("\n");
		saved.append(this->subListCurrent->sub->getTimeStart()->getTimeString());
		saved.append(" --> ");
		saved.append(this->subListCurrent->sub->getTimeEnd()->getTimeString());
		saved.append("\n");
		saved.append(this->subListCurrent->sub->getData()->getSText());
		saved.append("\n\n");
		this->subListCurrent = this->subListCurrent->next;
	}
	return saved;
}

void SubtitleEditor::stylize(char c, int start, int end)
{
	this->subListCurrent = this->subListFirst;
	while (this->subListCurrent != nullptr)
	{
		int starting = start;
		int ending = end;
		if (this->subListCurrent->sub->getSelected())
		{
			if (starting < 0)
				starting = 0;
			if (ending >= this->subListCurrent->sub->getData()->getTextSize())
				ending = this->subListCurrent->sub->getData()->getTextSize();
			int* sStart = this->subListCurrent->sub->getData()->getStartStyle();
			int* sEnd = this->subListCurrent->sub->getData()->getEndStyle();
			char* sType = this->subListCurrent->sub->getData()->getTypeStyle();
			int sNum = this->subListCurrent->sub->getData()->getStyleNumStart();
			sNum++;
			string text = this->subListCurrent->sub->getData()->getText();
			this->subListCurrent->sub->getData()->insertAndUpdate(text, sStart, sEnd, sType, sNum, c, starting, ending);
		}
		this->subListCurrent = this->subListCurrent->next;
	}
}



