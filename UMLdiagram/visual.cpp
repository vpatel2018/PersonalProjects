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

/* This serves as a procedural driver code for auto UML generator.
   The file uses the Parser and Graphics classes extensively.
   This program will output a PNG file with a UML diagram. */

/*
STEPS
1. Compile visual.cpp using the command "g++ visual.cpp -lSDL2 -lSDL2_image -lSDL2_ttf".
2. Suppose we want to use three python files that have classes.
Suppose the names of the python files are "a.py", "b.py" and "c.py".
Suppose the path to "a.py" is "/Users/vihanpatel/desktop/a.py".
Suppose the path to "b.py" is "/Users/vihanpatel/desktop/b.py".
Suppose the path to "c.py" is "/Users/vihanpatel/desktop/c.py".
If we want to see a UML diagram with information about those classes,
use the command "./a.out /Users/vihanpatel/desktop/a.py /Users/vihanpatel/desktop/b.py /Users/vihanpatel/desktop/c.py".
Make sure all the files you use have classes in them.
 
Suppose you want to see a UML diagram of ALL python files within a directory.*/
//Example command to use: " ./a.out /NLP-ML-Research/src/* "
   //NOTE: This example command is for /NLP-ML-Research/src directory

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

void link(string class1, string class2, vector<vector<string> >& storage, Graphics& screen, vector<vector<int> >& gates, string relationship) {
    
    int chosenOne = -1;
    
    if(class1.find("%") != string::npos) {
      chosenOne = 0;
      class1 = class1.substr(0, class1.find("%"));
    }else if(class2.find("%") != string::npos) {
      chosenOne = 1;
      class2 = class2.substr(0, class2.find("%"));
    }
    
    int c1X = -1;
    int c1Y = -1;
    int c1W = -1;
    int c1H = -1;
    int c1R = -1;
    
    for(int k = 0; k < storage.size(); k++) {
        if(storage[k][0] == class1) {
            c1X = stoi(storage[k][1]);
            c1Y = stoi(storage[k][2]);
            c1W = stoi(storage[k][3]);
            c1H = stoi(storage[k][4]);
            c1R = stoi(storage[k][5]);
            break;
        }
    }
    
    int randY = (rand() % (c1H + 1)) + c1Y;
    vector<int> maxes;
    
    for(int h = 0; h < storage.size(); h++) {
        if(stoi(storage[h][5]) == c1R) {
            maxes.push_back(stoi(storage[h][3]));
        }
    }
    
    int max = -1;
    
    for(int j = 0; j < maxes.size(); j++) {
        if(maxes[j] > max) {
            max = maxes[j];
        }
    }
    
    int extent = -1;
    int lastRow = 0;
    
    for(int k = 0; k < storage.size(); k++) {
        if(stoi(storage[k][5]) == c1R + 1) {
            extent = stoi(storage[k][1]);
        }
    }
    
    for(int k = 0; k < storage.size(); k++) {
        if(stoi(storage[k][5]) > lastRow) {
            lastRow = stoi(storage[k][5]);
        }
    }
    
    max = max + fontSize/2;
    
    int p = (c1X + max) + (rand() % ((extent - fontSize) - (c1X + max) + 1));
    
    int r = (rand() * rand() * rand() * rand() * rand()) % 128;
    int w = (rand() * rand() * rand() * rand() * rand()) % 128;
    int b = (rand() * rand() * rand() * rand() * rand()) % 128;
    
    int c2X = -1;
    int c2Y = -1;
    int c2W = -1;
    int c2H = -1;
    int c2R = -1;
    
    for(int k = 0; k < storage.size(); k++) {
        if(storage[k][0] == class2) {
            c2X = stoi(storage[k][1]);
            c2Y = stoi(storage[k][2]);
            c2W = stoi(storage[k][3]);
            c2H = stoi(storage[k][4]);
            c2R = stoi(storage[k][5]);
            break;
        }
    }
    
    if(c1R == lastRow && c2R - c1R == 0) {
        p = c1X + max + (rand() % fontSize);
    }
    
    screen.drawDashLn(c1X + c1W, randY, p, randY, r, w, b);
    
    int randY2 = (rand() % (c2H + 1)) + c2Y;
    
    if(c2R - c1R == 0) {
        screen.drawDashLn(p, randY, p, randY2, r, w, b);
        screen.drawDashLn(p, randY2, c2X + c2W, randY2, r, w, b);
        if(chosenOne == 0) {
          if(relationship == "IN") {
            screen.drawTriangle(c1X + c1W, randY, r, w, b);
          }else if(relationship == "CO") {
            screen.drawFillDiamond(c1X + c1W, randY, fontSize, r, w, b);
          }else if(relationship == "AG") {
            screen.drawDiamond(c1X + c1W, randY, r, w, b);
          }
        }else if(chosenOne == 1) {
          if(relationship == "IN") {
            screen.drawTriangle(c2X + c2W, randY2, r, w, b);
          }else if(relationship == "CO") {
            screen.drawFillDiamond(c2X + c2W, randY2, fontSize, r, w, b);
          }else if(relationship == "AG") {
            screen.drawDiamond(c2X + c2W, randY2, r, w, b);
          }
        }
    }else if(c2R - c1R == 1) {
        if(randY >= c2Y && randY <= c2Y + c2H) {
            screen.drawDashLn(p, randY, c2X, randY, r, w, b);
            if(chosenOne == 0) {
                if(relationship == "IN") {
                    screen.drawTriangle(c1X + c1W, randY, r, w, b);
                }else if(relationship == "CO") {
                    screen.drawFillDiamond(c1X + c1W, randY, fontSize, r, w, b);
                }else if(relationship == "AG") {
                    screen.drawDiamond(c1X + c1W, randY, r, w, b);
                }
            }else if(chosenOne == 1) {
                if(relationship == "IN") {
                    screen.drawBackTri(c2X, randY, r, w, b);
                }else if(relationship == "CO") {
                    screen.drawFillDiamond(c2X - (fontSize * 2), randY, fontSize, r, w, b);
                }else if(relationship == "AG") {
                    screen.drawDiamond(c2X - (fontSize * 2), randY, r, w, b);
                }
            }
        }else{
            screen.drawDashLn(p, randY, p, randY2, r, w, b);
            screen.drawDashLn(p, randY2, c2X, randY2, r, w, b);
            if(chosenOne == 0) {
                if(relationship == "IN") {
                    screen.drawTriangle(c1X + c1W, randY, r, w, b);
                }else if(relationship == "CO") {
                    screen.drawFillDiamond(c1X + c1W, randY, fontSize, r, w, b);
                }else if(relationship == "AG") {
                    screen.drawDiamond(c1X + c1W, randY, r, w, b);
                }
            }else if(chosenOne == 1) {
                if(relationship == "IN") {
                    screen.drawBackTri(c2X, randY2, r, w, b);
                }else if(relationship == "CO") {
                    screen.drawFillDiamond(c2X - (fontSize * 2), randY2, fontSize, r, w, b);
                }else if(relationship == "AG") {
                    screen.drawDiamond(c2X - (fontSize * 2), randY2, r, w, b);
                }
            }
        }
    }else if(c2R - c1R > 1) {
        
        int prevY = randY;
        int lastX = p;
        
        if(chosenOne == 0) {
            if(relationship == "IN") {
                screen.drawTriangle(c1X + c1W, randY, r, w, b);
            }else if(relationship == "CO") {
                screen.drawFillDiamond(c1X + c1W, randY, fontSize, r, w, b);
            }else if(relationship == "AG") {
                screen.drawDiamond(c1X + c1W, randY, r, w, b);
            }
        }
        
        for(int g = c1R; g < c2R - 1; g++) {
            
            vector<vector<int> > coords;
            int i = 0;
            
            for(int k = 0; k < gates.size(); k++) {
                if(gates[k][3] == g + 1) {
                    vector<int> yS;
                    yS.push_back(gates[k][1]);
                    yS.push_back(gates[k][2]);
                    coords.push_back(yS);
                    i++;
                }
            }
            
            int pos = rand() % i;
            int gate1 = coords[pos][0] + fontSize/4;
            int gate2 = coords[pos][1] - fontSize/4;
            int magicY = gate1 + (rand() % (gate2 - gate1 + 1));
            
            screen.drawDashLn(lastX, prevY, lastX, magicY, r, w, b);
            int maximum = 0;
            int xVal = 0;
            vector<int> lens;
            int end = -1;
            
            for(int k = 0; k < storage.size(); k++) {
                if(stoi(storage[k][5]) == g + 1) {
                    lens.push_back(stoi(storage[k][3]));
                    xVal = stoi(storage[k][1]);
                }else if(stoi(storage[k][5]) == g + 2) {
                    end = stoi(storage[k][1]);
                }
            }
            
            for(int x = 0; x < lens.size(); x++) {
                if(lens[x] > maximum) {
                    maximum = lens[x];
                }
            }
            
            maximum = maximum + fontSize/2;
            
            p = (maximum + xVal) + (rand() % (end - fontSize/2 - (maximum + xVal) + 1));
            
            screen.drawDashLn(lastX, magicY, p, magicY, r, w, b);
            
            prevY = magicY;
            lastX = p;
            
            
        }
        
        if(prevY >= c2Y && prevY <= c2Y + c2H) {
            screen.drawDashLn(lastX, prevY, c2X, prevY, r, w, b);
            if(chosenOne == 1) {
                if(relationship == "IN") {
                    screen.drawBackTri(c2X, prevY, r, w, b);
                }else if(relationship == "CO") {
                    screen.drawFillDiamond(c2X - (fontSize * 2), prevY, fontSize, r, w, b);
                }else if(relationship == "AG") {
                    screen.drawDiamond(c2X - (fontSize * 2), prevY, r, w, b);
                }
            }
        }else{
            screen.drawDashLn(lastX, prevY, lastX, randY2, r, w, b);
            screen.drawDashLn(lastX, randY2, c2X, randY2, r, w, b);
            if(chosenOne == 1) {
                if(relationship == "IN") {
                    screen.drawBackTri(c2X, randY2, r, w, b);
                }else if(relationship == "CO") {
                    screen.drawFillDiamond(c2X - (fontSize * 2), randY2, fontSize, r, w, b);
                }else if(relationship == "AG") {
                    screen.drawDiamond(c2X - (fontSize * 2), randY2, r, w, b);
                }
            }
        }
        
    }
    
}

void connect(string class1, string class2, vector<vector<string> >& storage, Graphics& screen, vector<vector<int> >& gates, string relationship) {
    
    int chosenOne = -1;
    
    if(class1.find("%") != string::npos) {
      chosenOne = 0;
      class1 = class1.substr(0, class1.find("%"));
    }else if(class2.find("%") != string::npos) {
      chosenOne = 1;
      class2 = class2.substr(0, class2.find("%"));
    }
  
    int c1X = -1;
    int c1Y = -1;
    int c1W = -1;
    int c1H = -1;
    int c1R = -1;
    
    for(int k = 0; k < storage.size(); k++) {
      if(storage[k][0] == class1) {
        c1X = stoi(storage[k][1]);
        c1Y = stoi(storage[k][2]);
        c1W = stoi(storage[k][3]);
        c1H = stoi(storage[k][4]);
        c1R = stoi(storage[k][5]);
        break;
      }
    }
    
    int randY = (rand() % (c1H + 1)) + c1Y;
    vector<int> maxes;
    
    for(int h = 0; h < storage.size(); h++) {
      if(stoi(storage[h][5]) == c1R) {
        maxes.push_back(stoi(storage[h][3]));
      }
    }
    
    int max = -1;
    
    for(int j = 0; j < maxes.size(); j++) {
      if(maxes[j] > max) {
        max = maxes[j];
      }
    }
    
    int extent = -1;
    int lastRow = 0;
    
    for(int k = 0; k < storage.size(); k++) {
      if(stoi(storage[k][5]) == c1R + 1) {
        extent = stoi(storage[k][1]);
      }
    }
    
    for(int k = 0; k < storage.size(); k++) {
      if(stoi(storage[k][5]) > lastRow) {
        lastRow = stoi(storage[k][5]);
      }
    }
    
    max = max + fontSize/2;
    
    int p = (c1X + max) + (rand() % ((extent - fontSize) - (c1X + max) + 1));
    
    int r = (rand() * rand() * rand() * rand() * rand()) % 128;
    int w = (rand() * rand() * rand() * rand() * rand()) % 128;
    int b = (rand() * rand() * rand() * rand() * rand()) % 128;
    
    int c2X = -1;
    int c2Y = -1;
    int c2W = -1;
    int c2H = -1;
    int c2R = -1;
    
    for(int k = 0; k < storage.size(); k++) {
      if(storage[k][0] == class2) {
        c2X = stoi(storage[k][1]);
        c2Y = stoi(storage[k][2]);
        c2W = stoi(storage[k][3]);
        c2H = stoi(storage[k][4]);
        c2R = stoi(storage[k][5]);
        break;
      }
    }
    
    if(c1R == lastRow && c2R - c1R == 0) {
      p = c1X + max + (rand() % fontSize);
    }
    
    screen.drawLine(c1X + c1W, randY, p, randY, r, w, b);
    
    int randY2 = (rand() % (c2H + 1)) + c2Y;
    
    if(c2R - c1R == 0) {
      screen.drawLine(p, randY, p, randY2, r, w, b);
      screen.drawLine(p, randY2, c2X + c2W, randY2, r, w, b);
      if(chosenOne == 0) {
        if(relationship == "IN") {
          screen.drawTriangle(c1X + c1W, randY, r, w, b);
        }else if(relationship == "CO") {
          screen.drawFillDiamond(c1X + c1W, randY, fontSize, r, w, b);
        }else if(relationship == "AG") {
          screen.drawDiamond(c1X + c1W, randY, r, w, b);
        }
      }else if(chosenOne == 1) {
        if(relationship == "IN") {
          screen.drawTriangle(c2X + c2W, randY2, r, w, b);
        }else if(relationship == "CO") {
          screen.drawFillDiamond(c2X + c2W, randY2, fontSize, r, w, b);
        }else if(relationship == "AG") {
          screen.drawDiamond(c2X + c2W, randY2, r, w, b);
        }
      }
    }else if(c2R - c1R == 1) {
       if(randY >= c2Y && randY <= c2Y + c2H) {
         screen.drawLine(p, randY, c2X, randY, r, w, b);
         if(chosenOne == 0) {
           if(relationship == "IN") {
            screen.drawTriangle(c1X + c1W, randY, r, w, b);
           }else if(relationship == "CO") {
            screen.drawFillDiamond(c1X + c1W, randY, fontSize, r, w, b);
           }else if(relationship == "AG") {
            screen.drawDiamond(c1X + c1W, randY, r, w, b);
           }
         }else if(chosenOne == 1) {
           if(relationship == "IN") {
              screen.drawBackTri(c2X, randY, r, w, b);
           }else if(relationship == "CO") {
              screen.drawFillDiamond(c2X - (fontSize * 2), randY, fontSize, r, w, b);
           }else if(relationship == "AG") {
              screen.drawDiamond(c2X - (fontSize * 2), randY, r, w, b);
           }
         }
       }else{
         screen.drawLine(p, randY, p, randY2, r, w, b);
         screen.drawLine(p, randY2, c2X, randY2, r, w, b);
         if(chosenOne == 0) {
            if(relationship == "IN") {
              screen.drawTriangle(c1X + c1W, randY, r, w, b);
            }else if(relationship == "CO") {
              screen.drawFillDiamond(c1X + c1W, randY, fontSize, r, w, b);
            }else if(relationship == "AG") {
              screen.drawDiamond(c1X + c1W, randY, r, w, b);
            }
         }else if(chosenOne == 1) {
            if(relationship == "IN") {
              screen.drawBackTri(c2X, randY2, r, w, b);
            }else if(relationship == "CO") {
              screen.drawFillDiamond(c2X - (fontSize * 2), randY2, fontSize, r, w, b);
            }else if(relationship == "AG") {
              screen.drawDiamond(c2X - (fontSize * 2), randY2, r, w, b);
            }
         }
       }
    }else if(c2R - c1R > 1) {

       int prevY = randY;
       int lastX = p;
        
       if(chosenOne == 0) {
         if(relationship == "IN") {
           screen.drawTriangle(c1X + c1W, randY, r, w, b);
         }else if(relationship == "CO") {
           screen.drawFillDiamond(c1X + c1W, randY, fontSize, r, w, b);
         }else if(relationship == "AG") {
           screen.drawDiamond(c1X + c1W, randY, r, w, b);
         }
       }
       
       for(int g = c1R; g < c2R - 1; g++) {

           vector<vector<int> > coords;
           int i = 0;
           
           for(int k = 0; k < gates.size(); k++) {
               if(gates[k][3] == g + 1) {
                   vector<int> yS;
                   yS.push_back(gates[k][1]);
                   yS.push_back(gates[k][2]);
                   coords.push_back(yS);
                   i++;
               }
           }
           
           int pos = rand() % i;
           int gate1 = coords[pos][0] + fontSize/4;
           int gate2 = coords[pos][1] - fontSize/4;
           int magicY = gate1 + (rand() % (gate2 - gate1 + 1));
           
           screen.drawLine(lastX, prevY, lastX, magicY, r, w, b);
           int maximum = 0;
           int xVal = 0;
           vector<int> lens;
           int end = -1;
           
           for(int k = 0; k < storage.size(); k++) {
               if(stoi(storage[k][5]) == g + 1) {
                   lens.push_back(stoi(storage[k][3]));
                   xVal = stoi(storage[k][1]);
               }else if(stoi(storage[k][5]) == g + 2) {
                   end = stoi(storage[k][1]);
               }
           }
           
           for(int x = 0; x < lens.size(); x++) {
               if(lens[x] > maximum) {
                   maximum = lens[x];
               }
           }
           
           maximum = maximum + fontSize/2;
           
           p = (maximum + xVal) + (rand() % (end - fontSize/2 - (maximum + xVal) + 1));
           
           screen.drawLine(lastX, magicY, p, magicY, r, w, b);
                
           prevY = magicY;
           lastX = p;
           
       }
        
       if(prevY >= c2Y && prevY <= c2Y + c2H) {
         screen.drawLine(lastX, prevY, c2X, prevY, r, w, b);
         if(chosenOne == 1) {
           if(relationship == "IN") {
             screen.drawBackTri(c2X, prevY, r, w, b);
           }else if(relationship == "CO") {
             screen.drawFillDiamond(c2X - (fontSize * 2), prevY, fontSize, r, w, b);
           }else if(relationship == "AG") {
             screen.drawDiamond(c2X - (fontSize * 2), prevY, r, w, b);
           }
         }
       }else{
         screen.drawLine(lastX, prevY, lastX, randY2, r, w, b);
         screen.drawLine(lastX, randY2, c2X, randY2, r, w, b);
         if(chosenOne == 1) {
           if(relationship == "IN") {
             screen.drawBackTri(c2X, randY2, r, w, b);
           }else if(relationship == "CO") {
             screen.drawFillDiamond(c2X - (fontSize * 2), randY2, fontSize, r, w, b);
           }else if(relationship == "AG") {
             screen.drawDiamond(c2X - (fontSize * 2), randY2, r, w, b);
           }
         }
       }
        
    }

}

void drawBox(vector<string> contents, Graphics& t, int& xPos, int& yPos, vector<int>& collection, vector<vector<string> >& storage, int column, vector<vector<int> >& spaces) {
    
    int x = xPos;
    int y = yPos;
    
    int maxLen = 0;
    string line = "";
    
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
        if(k > 0) {
            copy[k] = "+" + copy[k];
        }
        if(getWidth(copy[k].c_str()) > maxLen) {
          maxLen = getWidth(copy[k].c_str());
        }
    }
    
    bool addPlus = false;
    
    for(int k = 0; k < copy.size(); k++) {
        if(k > 0) {
          addPlus = true;
        }
        t.drawText(contents[k].c_str(), xPos, yPos, addPlus);
        if(k == 0) {
          yPos += fontSize;
          t.drawLine(xPos - (fontSize/2), yPos + (fontSize/2), maxLen + xPos, yPos + (fontSize/2));
        }else if(k < copy.size() - 1 && copy[k].find("self.") != string::npos && copy[k+1].find("self.") == string::npos) {
          yPos += fontSize;
          t.drawLine(xPos - (fontSize/2), yPos + (fontSize/2), maxLen + xPos, yPos + (fontSize/2));
        }
        yPos += getHeight(contents[k].c_str());
    }
    
    t.drawRect(x - fontSize/2, y, maxLen + fontSize/2, yPos - (y - fontSize/2));
    
    vector<int> top;
    top.push_back(x - fontSize/2);
    top.push_back(y);
    top.push_back(column);
    
    vector<int> down;
    down.push_back(x - fontSize/2);
    down.push_back(yPos + fontSize/2);
    down.push_back(column);
    
    spaces.push_back(top);
    spaces.push_back(down);
    
    vector<string> piece;
    
    piece.push_back(contents[0] + "\e[0m");
    piece.push_back(to_string(x - fontSize/2));
    piece.push_back(to_string(y));
    piece.push_back(to_string(maxLen + fontSize/2));
    piece.push_back(to_string(yPos - (y - fontSize/2)));
    piece.push_back(to_string(column));
    
    storage.push_back(piece);
    
    collection.push_back(maxLen);
    
    yPos += (fontSize * incConst);
    
}

int main(int argc, char * argv[]) {
    
    if(argc == 1) {
        return 0;
    }
    
    system("g++ getWidthHeightOfPngFile.cpp -lSDL2 -lSDL2_ttf -lSDL2_image -o temp");
    string command = "./temp";
    
    for(int f = 1; f < argc; f++) {
        string s(argv[f]);
        command += (" " + s);
    }
    
    command += " > temp.txt";
    
    const char* comm = command.c_str();
    system(comm);
    
    string number;
    fstream tempFile;
    tempFile.open("temp.txt");
    getline(tempFile, number);
    
    int screenHeight = stoi(number);
    
    getline(tempFile, number);
    
    int screenWidth = stoi(number);
        
    tempFile.close();
    
    system("rm temp.txt");
    system("rm temp");
    
    if(screenHeight == 0 || screenWidth == 0) {
        return 0;
    }
    
    vector<string> files;
    vector<string> messages;
    
    for(int k = 1; k < argc; k++) {
        string file = argv[k];
        ifstream f;
        f.open(file);
        if(!f) {
           string temp = file + " does not exist";
           messages.push_back(temp);
        }else{
           if(file.length() - file.rfind(".py") == 3) {
              files.push_back(file);
           }else{
              string temp = file + " is not a python file";
              messages.push_back(temp);
           }
        }
        f.close();
    }
    
    cout << endl;
    
    if(messages.size() == argc - 1) {
        cout << "None of the files you entered are valid." << endl;
        cout << endl;
        return 0;
    }
        
    for(int k = 0; k < messages.size(); k++) {
        cout << messages[k] << endl;
    }
    
    cout << endl;
    
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
   
    vector< vector<string> > UML = p.UMLdata(files);
    vector<int> collector;
    
    vector<vector<string> > storage;
    vector<vector<int> > spaces;
    
    int xPos = fontSize;
    int yPos = fontSize;
    int i = 0;
    int column = 0;
    
    while(yPos <= screenHeight && i < UML.size()) {
        int y = yPos;
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
        
        for(int k = 0; k < contents.size(); k++) {
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
        if(y > screenHeight) {
          int max = 0;
          for(int k = 0; k < collector.size(); k++) {
            if(collector[k] > max) {
              max = collector[k];
            }
          }
            
          vector<int> addThis;
          addThis.push_back(xPos - fontSize/2);
          addThis.push_back(screenHeight);
          addThis.push_back(column);
          spaces.push_back(addThis);
            
          column++;
          collector.clear();
            
          if(xPos + max + fontSize <= screenWidth) {
            xPos = xPos + max + (fontSize * incConst);
            yPos = fontSize;
          }else{
            break;
          }
        }else{
          drawBox(UML[i], screen, xPos, yPos, collector, storage, column, spaces);
          i++;
        }
    }
    
    vector<int> addThis;
    addThis.push_back(xPos - fontSize/2);
    addThis.push_back(screenHeight);
    addThis.push_back(column);
    spaces.push_back(addThis);
    
    for(int k = 0; k < spaces.size(); k++) {
        if(spaces[k][2] == 0) {
           spaces.erase(spaces.begin() + k);
           k = k - 1;
        }else if(spaces[k][1] == fontSize) {
           spaces.erase(spaces.begin() + k);
           k = k - 1;
        }
    }
    
    vector<vector<int> > gates;
    
    for(int h = 0; h < spaces.size(); h += 2) {
       vector<int> gt;
       int x = (spaces[h][0] + spaces[h+1][0])/2;
       int r = spaces[h][2];
       gt.push_back(x);
       gt.push_back(spaces[h][1]);
       gt.push_back(spaces[h+1][1]);
       gt.push_back(r);
       if(spaces[h][1] != screenHeight && spaces[h][2] != screenHeight) {
          gates.push_back(gt);
       }
    }
    
    spaces = gates;
    
    srand(time(0));
    
    vector<vector<string> > connections = p.relations(files);
    
    for(int q = 0; q < connections.size(); q++) {
        
        string className = connections[q][1];
        string arrowTo = connections[q][0];
        string relation = connections[q][2];
        
        int place1 = -1;
        int place2 = -1;
        
        for(int g = 0; g < storage.size(); g++) {
            
          string part = storage[g][0];
            
          for(int d = 0; d < storage[g][0].length() - 3; d++) {
            if(storage[g][0].substr(d, 4) == "\e[3m" || storage[g][0].substr(d, 4) == "\e[4m" || storage[g][0].substr(d, 4) == "\e[0m") {
              part = part.replace(d,4,"    ");
            }
          }
            
          string spare = "";
            
          for(int j = 0; j < part.size(); j++) {
            if(part.substr(j,1) != " ") {
              spare += part.at(j);
            }
          }
            
          if(spare == className) {
            place1 = g;
            break;
          }
            
        }
        
        for(int g = 0; g < storage.size(); g++) {
            
          string part = storage[g][0];
            
          for(int d = 0; d < storage[g][0].length() - 3; d++) {
            if(storage[g][0].substr(d, 4) == "\e[3m" || storage[g][0].substr(d, 4) == "\e[4m" || storage[g][0].substr(d, 4) == "\e[0m") {
              part = part.replace(d,4,"    ");
            }
          }
            
          string spare = "";
        
          for(int j = 0; j < part.size(); j++) {
            if(part.substr(j,1) != " ") {
              spare += part.at(j);
            }
          }
            
          if(spare == arrowTo) {
            place2 = g;
            break;
          }
            
        }
        
        if(place1 != -1 && place2 != -1) {
          if(place1 < place2) {
            connect(storage[place1][0], storage[place2][0] + "%", storage, screen, gates, relation);
          }else if(place1 > place2) {
            connect(storage[place2][0] + "%", storage[place1][0], storage, screen, gates, relation);
          }
        }
        
    }
    
}
