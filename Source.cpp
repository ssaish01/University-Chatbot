#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <time.h>

using namespace std;

#define hashSize 1000
#define hashTableSize 1000





int hashFunc1(string s, int maxVal)
{
	s = capitalize(s);

	int ascii;
	int hash = 0;

	for (int i = 0; i < s.length(); i++)
	{
		ascii = s.at(i);

		hash = hash + (ascii * (i + 1));

	}


	return (hash % maxVal);
}


int hashFunc2(string s, int maxVal)
{
	s = capitalize(s);

	int ascii;
	int hash = 0;

	for (int i = 0; i < s.length(); i++)
	{
		ascii = s.at(i);

		hash = hash + ascii;

	}

	return (hash % maxVal);
}

int hashFunc3(string s, int maxVal)
{
	s = capitalize(s);

	int ascii;
	int hash = 0;

	for (int i = 0; i < s.length(); i++)
	{
		ascii = s.at(i);

		hash = ascii + (hash << 6) + (hash << 16) - hash;

		if (hash < 0)
		{
			hash = hash * -1;
		}

	}


	return (hash % maxVal);
}


string capitalize(string s)
{
	string s2;

	for (int i = 0; i < s.length(); i++)
	{
		s2.push_back(toupper(s.at(i)));
	}

	return s2;
}


class Word {
private:
	string key;
	string value;
public:
	Word()
	{
		key = "defaultNull";
		value = "defaultNull";
	}
	Word(string k, string val)
	{
		key = k;
		value = val;
	}
	void setKey(string k)
	{
		key = k;
	}
	string getKey()
	{
		return key;
	}

	void setValue(string val)
	{
		value = val;
	}
	string getValue()
	{
		return value;
	}
};

class HashTable {

private:
	int totalElements = 0;

public:

	int tableSize;
	list<Word> hashTable[hashTableSize];
	HashTable()
	{
		totalElements = 0;
		tableSize = hashTableSize;
	}
	HashTable(int size)
	{
		totalElements = 0;
		tableSize = size;
	}

	int getTotalElements()
	{
		return totalElements;
	}
	void insertElement(Word word)
	{
		int index = hashFunc1(word.getKey(), tableSize);
		hashTable[index].push_back(word);

		totalElements++;
	}

	int searchForIndex(string key)
	{
		int index = hashFunc1(key, hashSize);

		list<Word>::iterator p = hashTable[index].begin();

		while (p != hashTable[index].end())
		{
			if (p->getKey() == key)
			{
				return index;
			}
			p++;
		}

		return -1;
	}

	bool wordExists(string key)
	{
		int index = hashFunc1(key, hashSize);


		list<Word>::iterator p = hashTable[index].begin();


		while (p != hashTable[index].end())
		{
			if (p->getKey() == key)
			{
				return true;
			}
			p++;
		}

		return false;
	}

	string getVal(string key)
	{
		int index = hashFunc1(key, hashSize);

		list<Word>::iterator p = hashTable[index].begin();


		while (p != hashTable[index].end())
		{
			if (p->getKey() == key)
			{
				return p->getValue();
			}
			p++;
		}

		return "NOT FOUND";

	}

	Word getWord(string key)
	{
		int index = hashFunc1(key, hashSize);

		list<Word>::iterator p = hashTable[index].begin();


		while (p != hashTable[index].end())
		{
			if (p->getKey() == key)
			{
				return *p;
			}
			p++;
		}

	}

};

HashTable mappedStemming;


vector<string> tokenize(string sentence)
{
	vector<string> tokens;

	string word;

	for (int i = 0; i < sentence.length(); i++)
	{

		if (sentence[i] != ' ')
		{
			word.push_back(sentence[i]);
		}

		else if (sentence[i] == ' ')
		{
			tokens.push_back(word);
			word.clear();
		}

	}

	tokens.push_back(word);

	return tokens;

}


string filterString(string s)
{
	string filteredString;
	int ch;

	int spaceCounter = 0;
	int wordFound = 0;

	for (int i = 0; i < s.length(); i++)
	{
		ch = s.at(i);

		if ((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122) || (ch == 32))
		{
			if (ch == 32)
			{
				if (i == s.length() - 1 || i == 0 || wordFound == 0)
				{
					spaceCounter++;
					continue;
				}
				if (spaceCounter < 1)
				{
					filteredString.push_back(s.at(i));
					spaceCounter++;
				}
				else
				{
					continue;
				}
			}
			else
			{
				filteredString.push_back(s.at(i));
				spaceCounter = 0;
				wordFound = 1;
			}


		}

	}

	if (filteredString.back() == 32)
	{
		filteredString.pop_back();
	}


	return capitalize(filteredString);;
}

string sortString(string s)
{

	string sortedString;

	vector<string> tokens;

	tokens = tokenize(s);

	sort(tokens.begin(), tokens.end());

	vector<string>::iterator p = tokens.begin();

	while (p != tokens.end())
	{
		sortedString.append(*p);

		sortedString.append(" ");

		p++;
	}

	sortedString.pop_back();

	return sortedString;

}


string stemmenizeString(string s)
{

	string stemmedString;
	vector<string> tokens;

	tokens = tokenize(s);

	vector<string>::iterator p = tokens.begin();
	while (p != tokens.end())
	{
		if (mappedStemming.wordExists(*p))
		{
			stemmedString.append(mappedStemming.getVal(*p));
		}
		else
		{
			stemmedString.append(*p);
		}

		stemmedString.append(" ");
		p++;
	}

	stemmedString.pop_back();

	if (stemmedString != s)
	{
		stemmedString = stemmenizeString(stemmedString);
	}

	return stemmedString;

}


string filterSortString(string s)
{
	s = filterString(s);
	s = stemmenizeString(s);
	s = sortString(s);

	return s;
}

class Question {

	string question;
	string ans = "defaultNullVal";

public:
	void setQuestion(string question1)
	{
		question = question1;
	}
	string getQuestion()
	{
		return question;
	}
	void setAns(string ans1)
	{
		ans = ans1;
	}
	string getAns()
	{
		return ans;
	}
	void setAnsException(string ex)
	{
		ansException = ex;
	}
	string getAnsException()
	{
		return ansException;
	}


};

class HashMap {

private:

	int totalElements = 0;

public:
	list<Question> hashTable[hashSize];

	int getTotalElements()
	{
		return totalElements;
	}
	void insertVal(Question q)
	{

		int index = hashFunc1(q.getQuestion(), hashSize);
		hashTable[index].push_back(q);
		totalElements++;
	}

	int searchForIndex(Question q)
	{
		int index = hashFunc1(q.getQuestion(), hashSize);

		list<Question>::iterator p = hashTable[index].begin();

		while (p != hashTable[index].end())
		{
			if (p->getQuestion() == q.getQuestion())
			{
				return index;
				break;
			}
			p++;
		} 

		return -1;
	}

	bool questionExists(Question q)
	{
		int index = hashFunc1(q.getQuestion(), hashSize);

		list<Question>::iterator p = hashTable[index].begin();

		while (p != hashTable[index].end())
		{
			if (p->getQuestion() == q.getQuestion())
			{
				return true;
				break;
			}
			p++;
		}

		return false;
	}

	string getAns(string q)
	{
		int index = hashFunc1(q, hashSize);

		int r = rand() % 5;

		list<Question>::iterator p = hashTable[index].begin();

		int reRandCounter = 0;

		while (p != hashTable[index].end())
		{
			if (p->getQuestion() == q)
			{
				if (reRandCounter < 5000)
				{
					
						if (p->getAns() == "defaultNullVal")
						{
							r = rand() % 5;
							reRandCounter++;
							continue;
						}

						return p->getAns();
					
					
				}
				else {

					return "No Answer :(";
				}

			}
			p++;
		}

		return "NOT FOUND";
	}



};

void loadStemmingWords(string sourceFile, char usedDelimiter)
{
	string line;
	string key;
	string value;

	char ch;

	int delimiterCounter = 0;


	ifstream srcFile(sourceFile);

	if (srcFile.is_open())
	{

		while (getline(srcFile, line))
		{
			delimiterCounter = 0;


			for (int i = 0; i < line.length(); i++)
			{
				ch = line.at(i);

				if (ch == usedDelimiter)
				{
					delimiterCounter++;
					continue;
				}

				if (delimiterCounter == 0)
				{
					value.push_back(ch);
				}

				else if (delimiterCounter == 1)
				{
					key.push_back(ch);
				}
			}


			value = filterString(value);
			key = filterString(key);

			Word* w = new Word(value, value);

			Word* word = new Word(key, value);


			if (!mappedStemming.wordExists(value))
			{
				mappedStemming.insertElement(*w);

			}

			if (!mappedStemming.wordExists(key))
			{
				mappedStemming.insertElement(*word);

			}
			key.clear();
			value.clear();

		}
	}

	srcFile.close();

}


HashMap h1;

void loadQuestionsData(string sourceFile, char usedDelimiter)
{
	string line;
	string question;
	string answer;



	char ch;

	int delimiterCounter = 0;

	Question q;

	ifstream srcFile(sourceFile);

	if (srcFile.is_open())
	{

		while (getline(srcFile, line))
		{
			delimiterCounter = 0;


			for (int i = 0; i < line.length(); i++)
			{
				ch = line.at(i);

				if (ch == usedDelimiter)
				{
					delimiterCounter++;
					continue;
				}
				
				question.push_back(ch);
				

				
				else
				{
					ansException.push_back(ch);

				}


			}
			question = filterSortString(question);
			q.setQuestion(question);

			if (!h1.questionExists(q))
			{
				h1.insertVal(q);

				int index = h1.searchForIndex(q);


				if (index != -1)
				{
					list<Question>::iterator p = h1.hashTable[index].begin();

					while (p != h1.hashTable[index].end())
					{
						if (p->getQuestion() == q.getQuestion())
						{
							p->setAns(ans);

							

							break;
						}
						p++;
					}
				}

			}



			question.clear();
			answer.clear();
			ansException.clear();

		}
	}

	srcFile.close();

}

void insertQuestion(string sourceFile, char usedDelimiter, string que, string ans)
{
	char ch;

	int delimiterCounter = 0;

	string question = que;
	string answer = ans;
	Question q;

	q.setQuestion(question);
	q.setAns(answer);

	ofstream srcFile;

	srcFile.open(sourceFile, std::ios_base::app); 
	srcFile << question << usedDelimiter << ans << endl;


	srcFile.close();

}

int main()
{
	srand((unsigned)time(0));

	string questionAnswersFilePath = "D:/questionAnswers.txt";

	string lemmatizationMappingFilePath = "D:/lemmatizationData.txt";

	cout << endl << "WAIT ! Loading ChatBot.." << endl;

	loadStemmingWords(lemmatizationMappingFilePath, '\t');
	loadQuestionsData(questionAnswersFilePath, '\t');

	cout << endl << "ChatBot Loaded succesfully !!" << endl;

	cout << endl << "Total Questions in hash table: " << h1.getTotalElements() << endl;
	cout << endl << "Total Words in 2nd hash table: " << mappedStemming.getTotalElements() << endl;

	string askedQuestion = "1";
	string filteredQuestion;
	string suggestedAnswer;

	cout << endl << "Hello Human ! How may I help you?" << endl;

	Question qUser;
	Question q;

	getline(cin, askedQuestion);

	while (askedQuestion != "0")
	{


		filteredQuestion = filterSortString(askedQuestion);

		q.setQuestion(filteredQuestion);

		if (h1.getAns(filteredQuestion) == "NOT FOUND")
		{
			cout << endl << "I'm sorry i do not know the answer to this question" << endl;
			cout << "But if you tell me it's answer i'll remember it next time" << endl;
			getline(cin, suggestedAnswer);

			insertQuestion(questionAnswersFilePath, '\t', askedQuestion, suggestedAnswer);

			if (!h1.questionExists(q))
			{
				h1.insertVal(q);

				int index = h1.searchForIndex(q);

				if (index != -1)
				{
					list<Question>::iterator p = h1.hashTable[index].begin();

					while (p != h1.hashTable[index].end())
					{
						if (p->getQuestion() == q.getQuestion())
						{
							p->setAns(suggestedAnswer);
							break;

						}
						p++;
					}
				}

			}

			cout << endl << "Thank You! I'll remember it next time :)" << endl;

		}
		else
		{
			cout << endl << h1.getAns(filteredQuestion) << endl;
		}

		getline(cin, askedQuestion);

	}


	cout << endl;

}
