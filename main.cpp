#include <iostream>
#include "HashSet.hpp"
#include <vector>
#include "Set.hpp"
#include <memory>
#include <string>
#include <algorithm> 
#include <fstream>
#include <cctype>


unsigned int hashFunction(const std::string&);
std::vector<std::string> findSuggestions(std::string& word, HashSet<std::string> words);
void addWords(HashSet<std::string>& words, std::string fName);
void suggestions(std::string, HashSet<std::string>&);

int main()
{
    HashSet<std::string> hSet{hashFunction};  
    addWords(hSet, "words.txt");

    std::string sent = "Howw arre yoo";
    std::cout<< "Enter a word or sentence: ";
    std::getline(std::cin, sent);

    suggestions(sent, hSet);

    return 0;
}

unsigned int hashFunction(const std::string& word)
{
    unsigned int hash = 0;

    for (unsigned int i = 0; i < word.length(); ++i)
    {
        hash *= 37;
        hash += static_cast<unsigned int>(word[i]);
    }

    return hash;
}


std::vector<std::string> findSuggestions(std::string& word, HashSet<std::string> words) 
{
    std::vector<std::string>::iterator p; 
    std::vector<std::string> suggestions = {};
    std::string testWord = word;
    
    //Swapping
    for (int i=0; i < word.size(); i ++)
    {
        if (i + 1 < word.size())
        {
            std::swap(testWord[i], testWord[i+1]);
            if (words.contains(testWord))
            {
                p = std::find (suggestions.begin(), suggestions.end(), testWord);
                if (p == suggestions.end())
                    suggestions.push_back(testWord);
            }
            testWord = word;
        }
    }

    //replacing with each letter
    testWord = word;
    for (int curChar = 97; curChar <= 122; curChar++)
    {
        for (int i=0; i <= word.size(); i++)
        {
            testWord.insert(i,1,curChar);
            if (words.contains(testWord))
            {
                p = std::find (suggestions.begin(), suggestions.end(), testWord);
                if (p == suggestions.end())
                    suggestions.push_back(testWord);
            }
            testWord = word;
        }
    } 

    //Deleting
    testWord = word;
    for (int i=0; i < word.size(); i++)
    {
        testWord.erase(i,1);
        if (words.contains(testWord))
        {
            p = std::find (suggestions.begin(), suggestions.end(), testWord);
            if (p == suggestions.end())
                suggestions.push_back(testWord);
        }
        testWord = word;
    }

    //replace 
    testWord = word; 
    if (testWord.size() != 0)
    {
        for (int curChar = 97; curChar <= 122; curChar++)
        {
            for (int i=0; i < word.size(); i++)
            {
                testWord.replace(i,1,1,curChar);

                if (words.contains(testWord))
                {
                    p = std::find (suggestions.begin(), suggestions.end(), testWord);
                    if (p == suggestions.end())
                        suggestions.push_back(testWord);
                }
                testWord = word;
            }
        }
    }

    //Splitting
    testWord = word;
    std::string firstPart;
    std::string lastPart;

    if (testWord.size() > 1)
    {
        for (int i=1; i < word.size(); i++)
        {
            lastPart = testWord.substr(i);
            firstPart = testWord.substr(0,i);
            if (words.contains(firstPart) && words.contains(lastPart))
            {
                testWord = firstPart + " " + lastPart;
                
                p = std::find (suggestions.begin(), suggestions.end(), testWord);
                if (p == suggestions.end())
                    suggestions.push_back(testWord);
            }
            testWord = word;
        }
    }


    return suggestions;
}

void addWords(HashSet<std::string>& words, std::string fName)
{
    std::string myText;
    try
    {
        std::ifstream MyReadFile(fName);

        while (std::getline (MyReadFile, myText)) 
        {
            words.add(myText);
        }   

        MyReadFile.close();
    }
    catch(...)
    {
        return;
    }

}

void suggestions(std::string sent, HashSet<std::string>& hMap) 
{
    std::vector<std::string> words;
    std::string temp = "";
	for(int i=0;i<sent.length();++i){
		
		if(sent[i]==' '){
			words.push_back(temp);
			temp = "";
		}
		else{
			temp.push_back(sent[i]);
		}
		
	}
	words.push_back(temp);    

    std::cout << "\nOriginal: " << sent;
    std::vector<std::string> incorrectWords = {};
    for (auto w : words)
    {
        std::string wd = w;
        std::transform(wd.begin(), wd.end(), wd.begin(),
            [](unsigned char c){ return std::tolower(c); });

        if (hMap.contains(wd) == false)
        {
            incorrectWords = findSuggestions(wd, hMap);

            if (incorrectWords.size() != 0)
            {   
                std::cout << "\nFor '" << w <<"' did you mean to write:" << std::endl << "    Suggestions: ";
                for (auto sug : incorrectWords)
                {
                    std::cout<< sug << " ";
                }
            }
        }
        incorrectWords = {};
    }



}

