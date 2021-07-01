#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Graphics.h"
#include "Parser.h"
using namespace std;

/* Creator - Vihan Patel */

const int fontSize = 12;
int incConst = 8;

int getWidth(const char * word) {
    TTF_Font * t = TTF_OpenFont("Times_New_Roman.ttf", fontSize);
    int width;
    int height;
    int answer = 0;
    if(TTF_SizeText(t,word,&width,&height)) {
      cout << "";
    }else{
      answer = width;
    }
    TTF_CloseFont(t);
    return answer;
}

int getHeight(const char * word) {
    TTF_Font * t = TTF_OpenFont("Times_New_Roman.ttf", fontSize);
    int width;
    int height;
    int answer = 0;
    if(TTF_SizeText(t,word,&width,&height)) {
        cout << "";
    }else{
        answer = height;
    }
    TTF_CloseFont(t);
    return answer;
}

int main(int argc, char * argv[]) {
    
    if(argc == 1) {
        return 0;
    }
    
    int screenWidth = 12;
    int screenHeight = 12;
                
    vector<string> files;
    
    for(int k = 1; k < argc; k++) {
        string file = argv[k];
        ifstream f;
        f.open(file);
        if(f) {
           if(file.length() - file.rfind(".py") == 3) {
              files.push_back(file);
           }
        }
        f.close();
    }
    
    // $$$$$$$$$$$$$$$$$
    for(int f = 0; f < files.size(); f++) {
        string tempCommand = "cp " + files[f] + " . > /dev/null 2>&1" ;
        const char* tc = tempCommand.c_str();
        int a = -1;
        for(int k = files[f].length() - 1; k >= 0; k--) {
            if(files[f][k] == '/') {
               a = k;
               break;
            }
        }
        files[f] = files[f].substr(a + 1);
        system(tc);
    }
        
    vector<string> modified;
    
    for(int g = 0; g < files.size(); g++) {
      int c = 0;
      for(int y = g + 1; y < files.size(); y++) {
        if(files[y] == files[g]) {
          c++;
        }
      }
      if(c == 0) {
        modified.push_back(files[g]);
      }
    }
    
    files = modified;
    
    char pngFile[strlen(argv[0]) - 2];
    int index = 0;
    for(int k = 2; k < strlen(argv[0]); k++) {
      pngFile[index] = argv[0][k];
      index++;
    }
    
    strcat(pngFile, ".png");
    
    const char * pngName = pngFile;
    Graphics screen(screenWidth, screenHeight, pngName, fontSize);
    Parser p;
    
    vector< vector<string> > UML = p.UMLdata(files);
    vector<int> heights;
    vector<int> widths;
    int maxHeight = 0;
    
    for(int i = 0; i < UML.size(); i++) {
        
        int y = fontSize;
        vector<string> contents = UML[i];
        vector<string> copy = contents;
        
        for(int k = 0; k < copy.size(); k++) {
            string blank = "";
            if(copy[k].find("\e[3m") != string::npos && copy[k].find("\e[4m") != string::npos) {
                if(copy[k].find("\e[3m") < copy[k].find("\e[4m")) {
                    copy[k] = copy[k].replace(0, copy[k].find("[3m") + 3, blank);
                    copy[k] = copy[k].substr(0, copy[k].find("\e[0m"));
                    copy[k] = copy[k].replace(0, copy[k].find("[4m") + 3, blank);
                    copy[k] = copy[k].substr(0, copy[k].find("\e[0m"));
                }else if(copy[k].find("\e[4m") < copy[k].find("\e[3m")) {
                    copy[k] = copy[k].replace(0, copy[k].find("[4m") + 3, blank);
                    copy[k] = copy[k].substr(0, copy[k].find("\e[0m"));
                    copy[k] = copy[k].replace(0, copy[k].find("[3m") + 3, blank);
                    copy[k] = copy[k].substr(0, copy[k].find("\e[0m"));
                }
            }else if(copy[k].find("\e[3m") != string::npos) {
                copy[k]= copy[k].replace(0, copy[k].find("[3m") + 3, blank);
                copy[k] = copy[k].substr(0, copy[k].find("\e[0m"));
            }else if(copy[k].find("\e[4m") != string::npos) {
                copy[k] = copy[k].replace(0, copy[k].find("[4m") + 3, blank);
                copy[k] = copy[k].substr(0, copy[k].find("\e[0m"));
            }
        }
        int maxWidth = 0;
        for(int k = 0; k < contents.size(); k++) {
            if(getWidth(contents[k].c_str()) > maxWidth) {
                maxWidth = getWidth(contents[k].c_str());
            }
            if(k == 0) {
              y += getHeight(contents[k].c_str());
              y += fontSize;
            }else if(k < contents.size() - 1 && copy[k].find("self.") != string::npos && copy[k+1].find("self.") == string::npos) {
              y += getHeight(contents[k].c_str());
              y += fontSize;
            }else{
              y += getHeight(contents[k].c_str());
            }
        }
        y += (fontSize * incConst);
        heights.push_back(y);
        widths.push_back(maxWidth);
        
        if(y > maxHeight) {
            maxHeight = y;
        }
        
    }
    
    cout << maxHeight << endl;
    
    int sum = 0;
    int start = 0;
    int end = heights.size() - 1;
    int whatIsReached = 0;
    vector<vector<int> > ranges;
    
    for(int k = 0; k < heights.size(); k++) {
        sum += heights[k];
        if(sum > maxHeight) {
            sum = heights[k];
            vector<int> range;
            range.push_back(start);
            range.push_back(k - 1);
            whatIsReached = k - 1;
            ranges.push_back(range);
            start = k;
        }
    }
    
    if(whatIsReached < end) {
        vector<int> range;
        int total = heights[whatIsReached + 1];
        for(int k = whatIsReached + 2; k <= end; k++) {
            total += heights[k];
        }
        if(total <= maxHeight) {
            range.push_back(whatIsReached + 1);
            range.push_back(end);
            ranges.push_back(range);
        }
    }
    
    int maxWidth = 0;
        
    for(int k = 0; k < ranges.size(); k++) {
        int maxNum = 0;
        for(int f = ranges[k][0]; f <= ranges[k][1]; f++) {
            if(widths[f] > maxNum) {
                maxNum = widths[f];
            }
        }
        maxWidth += maxNum;
    }
    
    maxWidth += ((ranges.size()) * (fontSize * incConst));

    cout << maxWidth << endl;
    
    // $$$$$$$$$$$$$$$$$
    system("rm *.py");
    
}
