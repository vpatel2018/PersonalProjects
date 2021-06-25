#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

/* Creator - Vihan Patel */

/* This class is responsible for parsing python files.
 * The main goals of this class are to analyze class relationships
 * and give other important information needed to construct a UML diagram.
 * Although this class has been fully established, it can be subject to
 * additional updates from time to time. 
 */

class Parser {

  public: 
   
   vector< vector<string> > relations(vector<string> fileNames);
   vector< vector<string> > UMLdata(vector<string> fileNames);

  private:

   void noMLComments(string type, vector<string> &allLines);
   string trim(string word);
   int firstCharPlace(string& word);
   vector<string> parse(string& fileName);
   vector<string> pass(vector<string> &list);
   vector< vector<string> > UMLinfo(string& fileName);
   vector< vector<string> > inheritance(vector<string> list);
   vector< vector<string> > composition(vector<string> list);
   vector< vector<string> > magnitude(vector<string> list);
   vector< vector<string> > getInfo(vector<string> list, vector<string> files, vector<vector<string> >& classes, bool onlyInit, bool useOtherFiles);
   void modify(vector<string>& assignments, vector<vector<string> > pieces);
   string grammarCheck(string& line);
   vector< vector<string> > aggregation(vector<string> list, vector<string> files);
   vector< vector<string> > association(vector<string> list, vector<string> files);
   vector< string > topClasses(vector<string> files);
   
};

vector<vector<string> > Parser::inheritance(vector<string> list) {
  
    vector<string> info;
    vector<string> mainFiles;
    
    int pos = 0;
    
    for(int k = 0; k < list.size(); k++) {
        if(list[k].find("IMPORT ") != string::npos) {
            string ln = trim(list[k].substr(list[k].find("IMPORT ") + 6, list[k].length() - (list[k].find("IMPORT ") + 6)));
            vector<vector<string> > li = UMLinfo(ln);
            mainFiles.push_back(ln.substr(0, ln.find(".py")));
            for(int g = 0; g < li.size(); g++) {
                info.push_back(li[g][0]);
            }
        }else if(list[k].find("class ") != string::npos && trim(list[k]).find("class") == 0){
            pos = k;
            break;
        }
    }
    
    for(int k = 0; k < info.size(); k++) {
        string ln = info[k];
        if(ln.find("\e[3m") != string::npos) {
            ln = ln.replace(ln.find("\e[3m"), 4, "");
        }
        if(ln.find("\e[0m") != string::npos) {
            ln = ln.replace(ln.find("\e[0m"), 4, "");
        }
        if(ln.find("\e[4m") != string::npos) {
            ln = ln.replace(ln.find("\e[4m"), 4, "");
        }
        if(ln.find("(") != string::npos) {
            ln = ln.substr(0, ln.find("("));
        }else{
            ln = ln.substr(0, ln.find(":"));
        }
        ln = trim(ln);
        info[k] = ln;
    }
    
    vector<string> classes;
    
    for(int j = pos; j < list.size(); j++) {
        if(list[j].find("class ") != string::npos && trim(list[j]).find("class") == 0){
            classes.push_back(list[j]);
        }
    }
    
    vector<string> allNames;
    
    for(int j = 0; j < info.size(); j++) {
      allNames.push_back(info[j]);
    }
    
    for(int k = 0; k < classes.size(); k++) {
        string ln = classes[k];
        ln = ln.substr(ln.find("class ") + 6, ln.length() - (ln.find("class ") + 6));
        ln = ln.substr(0, ln.find("("));
        ln = ln.substr(0, ln.find(":"));
        ln = trim(ln);
        allNames.push_back(ln);
    }
    
    vector<vector<string> > inheritances;
    
    for(int r = 0; r < classes.size(); r++) {
      if(classes[r].find("(") == string::npos) {
        classes.erase(classes.begin() + r);
        r = r - 1;
      }
    }
    
    for(int z = 0; z < classes.size(); z++) {
      vector<string> inf;
      string cName = classes[z].substr(classes[z].find("class ") + 6, classes[z].length() - (classes[z].find("class ") + 6));
      cName = cName.substr(0, cName.find("("));
      string items = classes[z].substr(classes[z].find("("), classes[z].find(")") - classes[z].find("("));
      items = trim(items.substr(1, items.length() - 1));
      if(items.find(",") == string::npos) {
        int count = 0;
        int stop = -1;
        for(int d = items.length() - 1; d >= 0; d = d - 1) {
          if((items.at(d) >= 65 && items.at(d) <= 90) || (items.at(d) >= 97 && items.at(d) <= 122) || (items.at(d) >= 48 && items.at(d) <= 57) || items.at(d) == '_'){
            stop = d;
          }else{
            break;
          }
        }
        int j = 0;
        for(int o = 0; o < mainFiles.size(); o++) {
          if(items.substr(0, stop) == mainFiles[o]) {
            j++;
          }
        }
        if(stop != -1 && items.find("metaclass") == string::npos && j > 0) {
          items = items.substr(stop);
        }
        for(int c = 0; c < allNames.size(); c++) {
          if(items == allNames[c]) {
            count++;
          }
        }
        if(count > 0) {
          inf.push_back(items);
          inf.push_back(cName);
          inheritances.push_back(inf);
        }
      }else{
        vector<int> commas;
        commas.push_back(0);
        for(int j = 0; j < items.length(); j++) {
          if(items.at(j) == ',') {
            commas.push_back(j-1);
            commas.push_back(j+1);
          }
        }
        commas.push_back(items.length() - 1);
        for(int y = 0; y < commas.size(); y += 2) {
          int start = commas[y];
          int end = commas[y+1];
          string word = items.substr(start, end - start + 1);
          for(int g = 0; g < allNames.size(); g++) {
            int stop = -1;
            for(int d = word.length() - 1; d >= 0; d = d - 1) {
              if((word.at(d) >= 65 && word.at(d) <= 90) || (word.at(d) >= 97 && word.at(d) <= 122) || (word.at(d) >= 48 && word.at(d) <= 57) || word.at(d) == '_'){
                stop = d;
              }else{
                break;
              }
            }
            if(stop != -1 && word.find("metaclass") == string::npos) {
              word = word.substr(stop);
            }
            if(word == allNames[g]) {
              inf.push_back(word);
              inf.push_back(cName);
              inheritances.push_back(inf);
              inf.clear();
            }
          }
        }
      }
    }
    
    return inheritances;
    
}

vector<vector<string> > Parser::composition(vector<string> list) {
    
    vector<vector<string> > classes;
    vector<vector<string> > info;
    vector<string> mainFiles;
    
    int pos = 0;
    
    for(int k = 0; k < list.size(); k++) {
        if(list[k].find("IMPORT ") != string::npos) {
            string ln = trim(list[k].substr(list[k].find("IMPORT ") + 6, list[k].length() - (list[k].find("IMPORT ") + 6)));
            vector<vector<string> > li = UMLinfo(ln);
            mainFiles.push_back(ln.substr(0, ln.find(".py")));
            for(int g = 0; g < li.size(); g++) {
                info.push_back(li[g]);
            }
        }else if(list[k].find("class ") != string::npos && trim(list[k]).find("class") == 0){
            pos = k;
            break;
        }
    }
    
    for(int j = pos; j < list.size(); j++) {
        vector<string> subset;
        if(list[j].find("class ") != string::npos && trim(list[j]).find("class") == 0){
            subset.push_back(list[j]);
            for(int k = j + 1; k < list.size(); k++) {
                if(list[k].find("class ") != string::npos && trim(list[k]).find("class") == 0){
                    break;
                }else{
                    subset.push_back(list[k]);
                }
            }
        }
        classes.push_back(subset);
    }
    
    for(int c = 0; c < classes.size(); c++) {
        int p = -1;
        for(int k = 0; k < classes[c].size(); k++) {
            if(classes[c][k].find("__init__") != string::npos && classes[c][k].find("def ") != string::npos) {
                p = k;
                break;
            }
        }
        if(p == -1) {
            classes.erase(classes.begin() + c);
            c = c - 1;
        }else{
            vector<string> taker;
            taker.push_back(classes[c][0]);
            taker.push_back(classes[c][p]);
            for(int y = p + 1; y < classes[c].size(); y++) {
                if(firstCharPlace(classes[c][y]) <= firstCharPlace(classes[c][p])) {
                    break;
                }else if(trim(classes[c][y]).find("self.") == 0 && classes[c][y].find("=") != string::npos){
                    taker.push_back(classes[c][y]);
                }
            }
            classes[c] = taker;
        }
    }
    
    vector<string> peek;
    
    for(int k = 0; k < classes.size(); k++) {
        peek.push_back(classes[k][0]);
    }
    
    for(int y = 0; y < peek.size(); y++) {
        string ln = peek[y];
        ln = ln.substr(ln.find("class ") + 6, ln.length() - (ln.find("class ") + 6));
        if(ln.find("(") != string::npos) {
            ln = ln.substr(0, ln.find("("));
        }else{
            ln = ln.substr(0, ln.find(":"));
        }
        ln = trim(ln);
        peek[y] = ln;
    }
    
    for(int j = 0; j < info.size(); j++) {
        string ln = info[j][0];
        if(ln.find("\e[3m") != string::npos) {
            ln = ln.replace(ln.find("\e[3m"), 4, "");
        }
        if(ln.find("\e[0m") != string::npos) {
            ln = ln.replace(ln.find("\e[0m"), 4, "");
        }
        if(ln.find("\e[4m") != string::npos) {
            ln = ln.replace(ln.find("\e[4m"), 4, "");
        }
        if(ln.find("(") != string::npos) {
            ln = ln.substr(0, ln.find("("));
        }else{
            ln = ln.substr(0, ln.find(":"));
        }
        ln = trim(ln);
        peek.push_back(ln);
    }
    
    vector<vector<string> > compositions;
    
    for(int x = 0; x < classes.size(); x++) {
        string class1 = classes[x][0].substr(classes[x][0].find("class ") + 6, classes[x][0].length() - (classes[x][0].find("class ") + 6));
        if(class1.find("(") != string::npos) {
            class1 = class1.substr(0, class1.find("("));
        }
        class1 = class1.substr(0, class1.find(":"));
        for(int y = 1; y < classes[x].size(); y++) {
            if(classes[x][y].find("self.") != string::npos) {
                string line = classes[x][y].substr(classes[x][y].find("=") + 1, classes[x][y].length() - (classes[x][y].find("=") + 1));
                if(line.find("(") != string::npos) {
                    line = line.substr(0, line.find("("));
                }
                line = trim(line);
                int stop = -1;
                for(int d = line.length() - 1; d >= 0; d = d - 1) {
                  if((line.at(d) >= 65 && line.at(d) <= 90) || (line.at(d) >= 97 && line.at(d) <= 122) || (line.at(d) >= 48 && line.at(d) <= 57) || line.at(d) == '_'){
                    stop = d;
                  }else{
                    break;
                  }
                }
                int j = 0;
                for(int h = 0; h < mainFiles.size(); h++) {
                  if(line.substr(0,stop) == mainFiles[h]) {
                    j++;
                  }
                }
                if(stop != -1 && j > 0) {
                  line = line.substr(stop);
                }
                for(int k = 0; k < peek.size(); k++) {
                    if(peek[k] == line) {
                        vector<string> pair;
                        pair.push_back(peek[k]);
                        pair.push_back(class1);
                        compositions.push_back(pair);
                    }
                }
            }
        }
    }
    
    vector<vector<string> > inherits = inheritance(list);
    
    for(int k = 0; k < compositions.size(); k++) {
        string chas = compositions[k][0];
        string mainClass = compositions[k][1];
        int count = 0;
        for(int y = 0; y < inherits.size(); y++) {
          if(inherits[y][0] == chas && inherits[y][1] == mainClass) {
            count++;
          }
        }
        if(count > 0 || chas == mainClass) {
          compositions.erase(compositions.begin() + k);
          k = k - 1;
        }
    }
    
    return compositions;
    
}

vector<vector<string> > Parser::getInfo(vector<string> list, vector<string> files, vector<vector<string> >& classes, bool onlyInit=false, bool useOtherFiles=false) {
  
    vector<vector<string> > info;
    vector<string> everyLine;
    
    int pos = 0;
    
    //read all import statements and extract all UML information about each file in the import statements
    //when import statements are not present anymore, stop at pos
    for(int k = 0; k < list.size(); k++) {
        if(list[k].find("IMPORT ") != string::npos) {
            string ln = trim(list[k].substr(list[k].find("IMPORT ") + 6, list[k].length() - (list[k].find("IMPORT ") + 6)));
            vector<vector<string> > li = UMLinfo(ln);
            for(int g = 0; g < li.size(); g++) {
                info.push_back(li[g]);
            }
            if(useOtherFiles == true) {
                vector<string> writing = parse(ln);
                for(int j = 0; j < writing.size(); j++) {
                  everyLine.push_back(writing[j]);
                }
            }
        }else if(list[k].find("class ") != string::npos && trim(list[k]).find("class") == 0){
            pos = k;
            break;
        }
    }
    
    //modify all the UML information of files in import statements
    for(int j = 0; j < info.size(); j++) {
        for(int k = 0; k < info[j].size(); k++) {
            string ln = info[j][k];
            if(ln.find("\e[3m") != string::npos) {
                ln = ln.replace(ln.find("\e[3m"), 4, "");
            }
            if(ln.find("\e[0m") != string::npos) {
                ln = ln.replace(ln.find("\e[0m"), 4, "");
            }
            if(ln.find("\e[4m") != string::npos) {
                ln = ln.replace(ln.find("\e[4m"), 4, "");
            }
            if(ln.find("(") != string::npos) {
                ln = ln.substr(0, ln.find("("));
            }else{
                ln = ln.substr(0, ln.find(":"));
            }
            if(trim(ln).find("self.") == 0) {
                ln = "";
            }
            ln = trim(ln);
            info[j][k] = ln;
        }
    }
    
    //get rid of all instance variable information
    for(int x = 0; x < info.size(); x++) {
        for(int y = 0; y < info[x].size(); y++) {
            if(info[x][y] == "") {
                info[x].erase(info[x].begin() + y);
                y = y - 1;
            }
        }
    }
    
    //break down each part of the list parameter into separate classes
    for(int j = pos; j < list.size(); j++) {
        vector<string> subset;
        if(list[j].find("class ") != string::npos && trim(list[j]).find("class") == 0){
            subset.push_back(list.at(j));
            for(int k = j + 1; k < list.size(); k++) {
                if(list[k].find("class ") != string::npos && trim(list[k]).find("class") == 0){
                    break;
                }else{
                    subset.push_back(list.at(k));
                }
            }
            classes.push_back(subset);
        }
    }
    
    if(useOtherFiles == true) {
      for(int k = 0; k < everyLine.size(); k++) {
        vector<string> subset;
        if(everyLine[k].find("class ") != string::npos && trim(everyLine[k]).find("class") == 0){
          subset.push_back(everyLine.at(k));
          for(int j = k + 1; j < everyLine.size(); j++) {
            if(everyLine[j].find("class ") != string::npos && trim(everyLine[j]).find("class") == 0){
              break;
            }else{
              subset.push_back(everyLine.at(j));
            }
          }
          classes.push_back(subset);
        }
      }
    }
    
    vector<string> names;
    
    for(int g = 0; g < classes.size(); g++) {
        string name = trim(classes[g][0]);
        name = name.substr(name.find("class") + 5, name.length() - (name.find("class") + 5));
        if(name.find("(") != string::npos) {
            name = name.substr(0, name.find("("));
        }
        name = name.substr(0, name.find(":"));
        names.push_back(name);
    }
    
    //for each separate class, push expressions with parentheses and brackets together
    string closers[3][2] = {{"(",")"},{"[","]"},{"{","}"}};
    
    for(int h = 0; h < 3; h++) {
      for(int k = 0; k < classes.size(); k++) {
        for(int j = 0; j < classes[k].size(); j++) {
          if(classes[k][j].find(closers[h][0]) != string::npos) {
            int position = -1;
            int totalFront = 0;
            int totalBack = 0;
            string line = "";
            for(int r = j; r < classes[k].size(); r++) {
              string str = classes[k][r];
              for(int g = 0; g < str.length(); g++) {
                if(str.substr(g,1) == closers[h][0]) {
                  totalFront++;
                }else if(str.substr(g,1) == closers[h][1]) {
                  totalBack++;
                }
              }
              if(totalFront == totalBack) {
                position = r;
                break;
              }
            }
            if(position != -1) {
              line += classes[k][j];
              for(int e = j + 1; e <= position; e++) {
                line += classes[k][e].substr(firstCharPlace(classes[k][e]), classes[k][e].length() - firstCharPlace(classes[k][e]));
              }
              classes[k][j] = line;
              for(int u = j + 1; u <= position; u++) {
                classes[k].erase(classes[k].begin() + (j + 1));
              }
            }
          }
        }
      }
    }
    
    //delete every string in each class
    string quotes[2] = {"'", "\""};
    
    for(int r = 0; r < 2; r++) {
      for(int k = 0; k < classes.size(); k++) {
        for(int j = 0; j < classes[k].size(); j++) {
          if(classes[k][j].find(quotes[r]) != string::npos) {
            string str = classes[k][j];
            vector<int> locations;
            for(int f = 0; f < str.length(); f++) {
              if(str.substr(f,1) == quotes[r]) {
                locations.push_back(f);
              }
            }
            string cpy = "";
            for(int f = 0; f < locations.size(); f += 2) {
              int start = locations[f];
              int end = locations[f+1];
              for(int g = start; g <= end; g++) {
                str = str.replace(g,1,quotes[r]);
              }
            }
            for(int c = 0; c < str.length(); c++) {
              if(str.substr(c,1) != quotes[r]) {
                cpy += str.substr(c,1);
              }
            }
            classes[k][j] = cpy;
           }
        }
      }
    }
    
    for(int k = 0; k < classes.size(); k++) {
      for(int j = 0; j < classes[k].size(); j++) {
        string line = trim(classes[k][j]);
        if(line.find("\\") == line.length() - 1) {
          classes[k][j] = classes[k][j] + classes[k][j+1];
          int location = classes[k][j].find("\\");
          string word = classes[k][j].substr(location + 1);
          word = word.substr(firstCharPlace(word));
          classes[k][j] = classes[k][j].substr(0, location) + word;
          classes[k].erase(classes[k].begin() + (j+1));
        }
      }
    }
        
    vector<vector<string> > classInfo;
    
    //extract the UML info of all files given in parameter
    for(int w = 0; w < files.size(); w++) {
        vector<vector<string> > inf = UMLinfo(files[w]);
        for(int i = 0; i < inf.size(); i++) {
            classInfo.push_back(inf[i]);
        }
    }
    
    //modify all UML info
    for(int t = 0; t < classInfo.size(); t++) {
        for(int u = 0; u < classInfo[t].size(); u++) {
            string ln = classInfo[t][u];
            if(ln.find("\e[3m") != string::npos) {
                ln = ln.replace(ln.find("\e[3m"), 4, "");
            }
            if(ln.find("\e[0m") != string::npos) {
                ln = ln.replace(ln.find("\e[0m"), 4, "");
            }
            if(ln.find("\e[4m") != string::npos) {
                ln = ln.replace(ln.find("\e[4m"), 4, "");
            }
            if(ln.find("(") != string::npos) {
                ln = ln.substr(0, ln.find("("));
            }else{
                ln = ln.substr(0, ln.find(":"));
            }
            if(trim(ln).find("self.") == 0) {
                ln = "";
            }
            ln = trim(ln);
            classInfo[t][u] = ln;
        }
    }
    
    //delete classes from list parameter that do not have def __init__
    if(onlyInit == true) {
        for(int y = 0; y < classes.size(); y++) {
            int place = -1;
            for(int j = 0; j < classes[y].size(); j++) {
                if(classes[y][j].find("__init__") != string::npos && classes[y][j].find("def ") != string::npos) {
                    place = j;
                    break;
                }
            }
            if(place == -1) {
                classes.erase(classes.begin() + y);
                y = y - 1;
            }
        }
    }
    
    for(int j = 0; j < classInfo.size(); j++) {
        string title = classInfo[j][0];
        int c = 0;
        for(int x = 0; x < names.size(); x++) {
            if(names[x] == title) {
                c++;
            }
        }
        if(c == 0) {
            classInfo.erase(classInfo.begin() + j);
            j = j - 1;
        }
    }
    
    for(int u = 0; u < info.size(); u++) {
        classInfo.push_back(info[u]);
    }
    
    for(int h = 0; h < classInfo.size(); h++) {
        for(int w = 0; w < classInfo[h].size(); w++) {
            if(classInfo[h][w] == "" || classInfo[h][w] == "__init__") {
                classInfo[h].erase(classInfo[h].begin() + w);
                w = w - 1;
            }
        }
    }
    
    return classInfo;
    
}

void Parser::modify(vector<string>& assignments, vector<vector<string> > pieces) {
    
    vector<string> copy;
    for(int f = 0; f < assignments.size(); f++) {
      string line = assignments[f];
      if(line.find("=") != string::npos && line.find("=") < line.find("(")) {
        bool isEqual = line.find("==") == string::npos && line.find("!=") == string::npos && line.find("+=") == string::npos && line.find("-=") == string::npos;
        bool noComparisons = line.find(">") == string::npos && line.find("<") == string::npos;
        int count = 0;
        if(isEqual == true && noComparisons == true) {
          copy.push_back(line);
        }
      }
    }
    
    vector<vector<string> > groups;
    vector<string> theNames;
    
    for(int c = 0; c < copy.size(); c++) {
      string line = copy[c];
      line = line.substr(0, line.find("="));
      line = trim(line);
      theNames.push_back(line);
    }
    
    for(int c = 0; c < theNames.size(); c++) {
      string ln = theNames[c];
      int count = 0;
      for(int k = c + 1; k < theNames.size(); k++) {
        if(theNames[k] == ln) {
          count++;
        }
      }
      if(count > 0 || ln.find("self.") == 0) {
        theNames.erase(theNames.begin() + c);
        c = c - 1;
      }
    }
    
    for(int c = 0; c < theNames.size(); c++) {
      string name = theNames[c];
      vector<string> subset;
      for(int d = 0; d < copy.size(); d++) {
        string ln = trim(copy[d]);
        if(ln.find(name) == 0) {
          subset.push_back(copy[d]);
        }
      }
      groups.push_back(subset);
    }
    
    for(int y = 0; y < groups.size(); y++) {
      int count = 0;
      for(int u = 0; u < groups[y].size(); u++) {
        string ln = groups[y][u];
        int times = 0;
        for(int z = 0; z < pieces.size(); z++) {
          for(int q = 0; q < pieces[z].size(); q++) {
            if(ln.find(pieces[z][q]) != string::npos) {
              times++;
            }
          }
        }
        if(times > 0) {
          count++;
        }
      }
      if(count > 0) {
        for(int u = 0; u < groups[y].size(); u++) {
          string ln = groups[y][u];
          int times = 0;
          for(int z = 0; z < pieces.size(); z++) {
            for(int q = 0; q < pieces[z].size(); q++) {
              if(ln.find(pieces[z][q]) != string::npos) {
                times++;
              }
            }
          }
          if(times == 0) {
            groups[y].erase(groups[y].begin() + u);
            u = u - 1;
          }
        }
      }
    }
    
    theNames.clear();
    
    for(int c = 0; c < groups.size(); c++) {
      for(int d = 0; d < groups[c].size(); d++) {
        theNames.push_back(groups[c][d]);
      }
    }
    
    for(int d = 0; d < copy.size(); d++) {
      string ln = copy[d];
      int count = 0;
      for(int c = 0; c < theNames.size(); c++) {
        if(ln == theNames[c]) {
          count++;
        }
      }
      if(count == 0) {
        copy.erase(copy.begin() + d);
        d = d - 1;
      }
    }
    
    //group all variables with same name
    //if any of them have keywords relating to UML class diagrams, keep those otherwise delete
    //if none have class relationships, just keep them all
    
    reverse(copy.begin(), copy.end());
    
    for(int k = 0; k < copy.size(); k++) {
        vector<string> elements;
        for(int g = k; g < copy.size(); g++) {
          elements.push_back(copy[g]);
        }
        for(int c = 1; c < elements.size(); c++) {
          elements[c] = trim(elements[c]);
          string statement = trim(elements[0]);
          string name = statement.substr(0, statement.find("="));
          string answer = statement.substr(statement.find("=") + 1);
            
          string varname = elements[c].substr(0, elements[c].find("="));
          string value = elements[c].substr(elements[c].find("=") + 1);
            
          if(answer.find(varname) != string::npos) {
            int len = varname.length() + 2;
            vector<int> locations;
            answer = "?" + answer + "?";
            for(int u = 0; u < answer.length() - len + 1; u++) {
              string substring = answer.substr(u, len);
              if(substring.substr(1,substring.length() - 2) == varname) {
                int nums[2] = {0, int(substring.length() - 1)};
                int count = 0;
                for(int y = 0; y < 2; y++) {
                  bool hasCaps = substring.at(nums[y]) >= 65 && substring.at(nums[y]) <= 90;
                  bool hasLows = substring.at(nums[y]) >= 97 && substring.at(nums[y]) <= 122;
                  bool hasDigit = substring.at(nums[y]) >= 48 && substring.at(nums[y]) <= 57;
                  bool hasUnderScore = substring.at(nums[y]) == '_';
                  bool hasMisleads = substring.at(nums[0]) == '.';
                  bool hasForbids = substring.at(nums[1]) == '=' || substring.at(nums[1]) == '(' || substring.at(nums[1]) == '[' ||substring.at(nums[1]) == '{';
                  if(hasCaps == true || hasLows == true || hasDigit == true || hasMisleads == true || hasForbids == true || hasUnderScore == true) {
                    count++;
                  }
                }
                if(count == 0) {
                  locations.push_back(u+1);
                }
              }
            }
            for(int f = 0; f < locations.size(); f++) {
              answer = answer.replace(locations[f], len - 2, value);
            }
            answer = answer.substr(1, answer.length() - 2);
          }
          elements[0] = name + " = " + answer;
        }
        copy[k] = elements[0];
    }
    
    reverse(copy.begin(), copy.end());
    
    assignments = copy;
    
}

string Parser::grammarCheck(string& line) {
 
  string t = trim(line);
  string word = "";
    
  if(line.length() == 0) {
    return line;
  }
    
  for(int k = 0; k < t.length(); k++) {
    if((t.at(k) >= 65 && t.at(k) <= 90) || (t.at(k) >= 97 && t.at(k) <= 122) || (t.at(k) >= 48 && t.at(k) <= 57) || (t.at(k) == '_' || t.at(k) == '.')) {
      word += t.at(k);
    }else if(t.at(k) == '(' || t.at(k) == '[' || t.at(k) == '{') {
      char front = t.at(k);
      char back = '0';
      if(front == '(') {
        back = ')';
      }else if(front == '[') {
        back = ']';
      }else if(front == '{') {
        back = '}';
      }
      int totalFront = 0;
      int totalBack = 0;
      int place = -1;
      for(int y = k; y < t.length(); y++) {
        if(t.at(y) == front) {
          totalFront++;
        }else if(t.at(y) == back) {
          totalBack++;
        }
        if(totalFront == totalBack) {
          word += t.at(y);
          break;
        }else{
          word += t.at(y);
          k++;
        }
      }
    }else if(t.at(k) == '=' || t.at(k) == '*' || t.at(k) == '/' || t.at(k) == '+' || t.at(k) == '-') {
      for(int r = k; r < t.length(); r++) {
        if(t.at(r) == '=' || t.at(r) == '*' || t.at(r) == '/' || t.at(r) == '+' || t.at(r) == '-') {
          word += t.at(r);
        }else{
          break;
        }
      }
    }else{
      break;
    }
  }
    
  return word;
 
}

vector<vector<string> > Parser::magnitude(vector<string> list) {
    
    vector<string> files;
    vector<vector<string> > classes;
    vector<vector<string> > filler = getInfo(list, files, classes, false, true);
  
    vector<vector<string> > magnitudes;
    
    for(int k = 0; k < classes.size(); k++) {
      vector<string> subset;
      for(int q = 1; q < classes[k].size(); q++) {
        if(classes[k][q].find("def ") != string::npos && trim(classes[k][q]).find("def") == 0) {
          if(classes[k][q].find("(") != string::npos) {
            string line = classes[k][q].substr(classes[k][q].find("("), classes[k][q].find(":") - classes[k][q].find("("));
            vector<int> commas;
            commas.push_back(line.find("(") + 1);
            for(int r = 0; r < line.length(); r++) {
              if(line.at(r) == ',') {
                commas.push_back(r-1);
                commas.push_back(r+1);
              }
            }
            commas.push_back(line.find_last_of(")") - 1);
            vector<string> stuff;
            int count = 0;
            if(commas.size() > 2) {
              for(int r = 0; r < commas.size(); r += 2) {
                int start = commas[r];
                int end = commas[r+1];
                string word = line.substr(start, end - start + 1);
                if(word != "self") {
                  stuff.push_back(trim(word));
                }
              }
              for(int r = 1; r < stuff.size(); r++) {
                if(stuff[r].find("=") != string::npos) {
                  count++;
                }
              }
            }else{
              string phrase = line.substr(line.find("(") + 1, line.find_last_of(")") - 1);
              stuff.push_back(phrase);
              if(phrase.find("=") != string::npos) {
                count++;
              }
            }
            int min = stuff.size() - count;
            int max = stuff.size();
            string substring = classes[k][q].substr(0, classes[k][q].find("("));
            substring = trim(substring);
            substring = substring.substr(substring.find("def") + 3);
            if(substring == "__init__") {
               substring = classes[k][0].substr(classes[k][0].find("class "));
               substring = substring.substr(0, substring.find("("));
               substring = substring.substr(0, substring.find(":"));
               substring = "ISCLASS" + trim(substring);
            }
            subset.push_back(substring + " " + to_string(min) + "-" + to_string(max));
          }
        }
      }
      int counts = 0;
      for(int l = 0; l < magnitudes.size(); l++) {
        if(magnitudes[l][0] == classes[k][0]) {
          counts++;
        }
      }
      if(counts == 0) {
        subset.insert(subset.begin(), classes[k][0]);
        magnitudes.push_back(subset);
      }
    }
    
    return magnitudes;
    
}

vector<vector<string> > Parser::aggregation(vector<string> list, vector<string> files) {
    
  vector<vector<string> > classes;
  vector<vector<string> > pieces = getInfo(list, files, classes, true);
  vector<vector<string> > components;
  vector<vector<int> > defs;
        
  //store all the statements regarding the instance variables in each class
  for(int k = 0; k < classes.size(); k++) {
    vector<string> instances;
    vector<string> names;
    vector<string> params;
      
    int pos = 0;
    for(int j = 0; j < classes[k].size(); j++) {
      if(classes[k][j].find("__init__") != string::npos && classes[k][j].find("def ") != string::npos) {
         pos = j;
         break;
      }
    }
    int last = -1;
    for(int y = pos + 1; y < classes[k].size(); y++) {
      if(firstCharPlace(classes[k][y]) == classes[k][y].find("def ")) {
        last = y;
        break;
      }else if(trim(classes[k][y]).find("self.") == 0 && classes[k][y].find("=") != string::npos){
        instances.push_back(to_string(y) + " " + classes[k][y]);
        string line = classes[k][y].substr(0, classes[k][y].find("="));
        names.push_back(trim(line));
      }else if(trim(classes[k][y]).find("self.") != string::npos) {
        instances.push_back(to_string(y) + " " + classes[k][y]);
      }
    }
      
    for(int u = last + 1; u < classes[k].size(); u++) {
      int count = 0;
      for(int j = 0; j < names.size(); j++) {
        if(classes[k][u].find(names[j]) != string::npos || classes[k][u].find("self.") != string::npos) {
          count++;
        }
      }
      if(count > 0) {
        instances.push_back(to_string(u) + " " + classes[k][u]);
      }
    }
    components.push_back(instances);
  }
        
  //calculate the positions of each def statement for each class
  for(int u = 0; u < classes.size(); u++) {
    vector<int> def;
    for(int k = 0; k < classes[u].size(); k++) {
      if(trim(classes[u][k]).find("def") == 0 && classes[u][k].find("def ") != string::npos) {
        def.push_back(k + 1);
      }
    }
    def.push_back(classes[u].size() - 1);
    defs.push_back(def);
  }

  //calculate the range of each instance variables position and its nearest def
  vector<vector<string> > extracts;
    
  for(int u = 0; u < components.size(); u++) {
    for(int k = 0; k < components[u].size(); k++) {
      vector<int> locs = defs[u];
      int place = stoi(components[u][k].substr(0, components[u][k].find(" ")));
      int number = -1;
      for(int q = 0; q < locs.size(); q++) {
        if(place < locs[q]) {
          if(q == 0) {
            number = locs[q];
          }else{
            number = locs[q - 1];
          }
          components[u][k] = to_string(number) + "-" + components[u][k];
          break;
        }else if(place == locs[locs.size() - 1]) {
          components[u][k] = to_string(locs[locs.size()-2]) + "-" + components[u][k];
          break;
        }
      }
    }
  }
            
  //between a nearest def location and instance variable's position, capture all assignment statements that are relevant
  for(int x = 0; x < components.size(); x++) {
    for(int y = 0; y < components[x].size(); y++) {
      string range = components[x][y].substr(0, components[x][y].find(" "));
      int low = stoi(range.substr(0, range.find("-")));
      int high = stoi(range.substr(range.find("-") + 1, range.length() - (range.find("-") + 1)));
      vector<string> assignments;
      vector<string> parameters;
      for(int q = low; q < high; q++) {
        assignments.push_back(classes[x][q]);
      }
      modify(assignments, pieces);
      for(int g = 0; g < assignments.size(); g++) {
        int count = 0;
        for(int p = 0; p < pieces.size(); p++) {
          for(int c = 0; c < pieces[p].size(); c++) {
            if(assignments[g].find(pieces[p][c]) != string::npos) {
              count++;
            }
          }
        }
        if(count == 0) {
          assignments.erase(assignments.begin() + g);
          g = g - 1;
        }
      }
      vector<string> copy;
      for(int f = 0; f < assignments.size(); f++) {
        string line = assignments[f];
        if(line.find("=") != string::npos && line.find("=") < line.find("(")) {
          bool isEqual = line.find("==") == string::npos && line.find("!=") == string::npos && line.find("+=") == string::npos && line.find("-=") == string::npos;
          bool noComparisons = line.find(">") == string::npos && line.find("<") == string::npos;
          if(isEqual == true && noComparisons == true) {
            copy.push_back(line);
          }
        }
      }
      for(int a = 0; a < copy.size(); a++) {
        string line = trim(copy[a]).substr(0, trim(copy[a]).find("="));
        int count = 0;
        for(int d = 0; d < components[x][y].size(); d++) {
          if(components[x][y].find(line) != string::npos) {
            count++;
          }
        }
        if(count == 0) {
          copy.erase(copy.begin() + a);
          a = a - 1;
        }
      }
      assignments.clear();
      assignments.push_back(classes[x][0]);
      assignments.push_back(components[x][y]);
      for(int x = 0; x < copy.size(); x++) {
        assignments.push_back(copy[x]);
      }
      extracts.push_back(assignments);
    }
  }

  //get rid of extracts that are size 2 and do not have any relationships anyhow
  for(int k = 0; k < extracts.size(); k++) {
    if(extracts[k].size() == 2) {
      int count = 0;
      string statement = extracts[k][1];
      for(int w = 0; w < pieces.size(); w++) {
        for(int z = 0; z < pieces[w].size(); z++) {
          if(statement.find(pieces[w][z]) != string::npos) {
            count++;
          }
        }
      }
      if(count == 0) {
        extracts.erase(extracts.begin() + k);
        k = k - 1;
      }
    }
  }

  string reserved[33] = {"False","class","finally","is","return","None","continue","for","lambda","try",
      "True","def","from","nonlocal" ,"while","and","del","global","not","with","as","elif",
      "if","or","yield","assert","else","import","pass","break","except","in","raise"};

  //eliminate all reserved words
  for(int u = 0; u < extracts.size(); u++) {
    string line = extracts[u][1];
    for(int c = 0; c < line.length(); c++) {
      if(line.substr(c,1) == " ") {
        int b = c + 1;
        int e = -1;
        for(int k = c + 1; k < line.length(); k++) {
            if(line.substr(k,1) == " ") {
                e = k;
                break;
            }
        }
        if(e == -1) {
           e = line.length();
        }
        if(e != -1) {
           string temp = line.substr(b, e - b);
           if(temp.length() > 0) {
             temp = line.substr(b - 1, e + 2 - b);
             temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
             int isReservedWord = 0;
             for(int i = 0; i < 33; i++) {
                if(temp == reserved[i]) {
                   isReservedWord = 1;
                   break;
                }
             }
             if(isReservedWord == 1) {
                int numHashes = e - b;
                string rep = "";
                for(int b = 0; b < numHashes; b++) {
                    rep += "#";
                }
                line = line.replace(b, e - b, rep);
             }
           }
        }
      }
    }
    while(line.find("#") != string::npos) {
        for(int f = 0; f < line.length(); f++) {
            if(line.substr(f, 1) == "#") {
                line = line.replace(f, 1, "");
            }
        }
    }
    extracts[u][1] = line;
  }
        
  for(int j = 0; j < extracts.size(); j++) {
    if(extracts[j].size() > 2) {
      string statement = extracts[j][1].substr(extracts[j][1].find(" "), extracts[j][1].length() - extracts[j][1].find(" "));
      statement = trim(statement);
      for(int z = 2; z < extracts[j].size(); z++) {
        string varname = trim(extracts[j][z]);
        string value = varname.substr(varname.find("=") + 1);
        varname = varname.substr(0, varname.find("="));
        int len = int(varname.length() + 2);
        vector<int> locations;
        for(int r = 0; r < statement.length() - len + 1; r++) {
          string substring = statement.substr(r, len);
          if(substring.substr(1,substring.length() - 2) == varname) {
            int nums[2] = {0, int(substring.length() - 1)};
            int count = 0;
            for(int y = 0; y < 2; y++) {
              bool hasCaps = substring.at(nums[y]) >= 65 && substring.at(nums[y]) <= 90;
              bool hasLows = substring.at(nums[y]) >= 97 && substring.at(nums[y]) <= 122;
              bool hasDigit = substring.at(nums[y]) >= 48 && substring.at(nums[y]) <= 57;
              bool hasUnderScore = substring.at(nums[y]) == '_';
              bool hasMisleads = substring.at(nums[0]) == '.';
              bool hasForbids = substring.at(nums[1]) == '=' || substring.at(nums[1]) == '(' || substring.at(nums[1]) == '[' ||substring.at(nums[1]) == '{';
              if(hasCaps == true || hasLows == true || hasDigit == true || hasMisleads == true || hasForbids == true || hasUnderScore == true) {
                count++;
              }
            }
            if(count == 0) {
              locations.push_back(r+1);
            }
          }
        }
        for(int y = 0; y < locations.size(); y++) {
          statement = statement.replace(locations[y], len - 2, value);
        }
        if(statement.find(varname) == 0) {
          statement = statement.replace(0, varname.length(), value);
        }
      }
      extracts[j][1] = statement;
    }
  }
    
  for(int j = 0; j < extracts.size(); j++) {
    if(extracts[j].size() > 2) {
      int times = extracts[j].size() - 2;
      for(int k = 0; k < times; k++) {
        extracts[j].erase(extracts[j].begin() + 2);
      }
    }
  }
    
  for(int j = 0; j < extracts.size(); j++) {
    if(extracts[j][1].find(" ") != string::npos) {
      extracts[j][1] = extracts[j][1].substr(extracts[j][1].find(" "));
      extracts[j][1] = extracts[j][1].substr(firstCharPlace(extracts[j][1]));
    }
  }
    
  for(int j = 0; j < extracts.size(); j++) {
    vector<int> locations;
    int len = 6;
    string word = extracts[j][1];
    if(word.substr(0, 5) == "self.") {
      locations.push_back(0);
    }
    for(int q = 0; q < word.length() - len + 1; q++) {
      string term = word.substr(q, len);
      char start = term.at(0);
      char end = term.at(len - 1);
      string middle = term.substr(1, term.length() - 2);
      if(middle == "self" && end == '.') {
        bool hasCaps = start >= 65 && start <= 90;
        bool hasLows = start >= 97 && start <= 122;
        bool hasDigit = start >= 48 && start <= 57;
        bool hasOthers = start == '_' || start == '.';
        if(hasCaps == false && hasLows == false && hasDigit == false && hasOthers == false) {
          locations.push_back(q + 1);
        }
      }
    }
    for(int w = 0; w < locations.size(); w++) {
      string substring = word.substr(locations[w]);
      int count = 0;
      for(int x = 0; x < pieces.size(); x++) {
        for(int y = 0; y < pieces[x].size(); y++) {
          if(substring.find(pieces[x][y]) != string::npos) {
            count++;
          }
        }
      }
      if(count > 0) {
        extracts[j].push_back(substring);
      }
    }
    extracts[j].erase(extracts[j].begin() + 1);
  }
    
  for(int q = 0; q < extracts.size(); q++) {
    for(int y = 1; y < extracts[q].size(); y++) {
      extracts[q][y] = grammarCheck(extracts[q][y]);
    }
    if(extracts[q].size() == 1) {
      extracts.erase(extracts.begin() + q);
      q = q - 1;
    }
  }
    
  for(int q = 0; q < extracts.size(); q++) {
    for(int r = 1; r < extracts[q].size(); r++) {
      string line = extracts[q][r];
      for(int j = r + 1; j < extracts[q].size(); j++) {
        string ln = extracts[q][j];
        if(line.find(ln) != string::npos) {
          for(int u = 0; u < line.length() - ln.length() + 1; u++) {
            if(line.substr(u,ln.length()) == ln) {
              string whitespace = "";
              string comment = "";
              for(int k = 0; k < ln.length(); k++) {
                whitespace += " ";
                comment += "#";
              }
              if(line.find("(") != u - 1) {
                extracts[q][r] = extracts[q][r].replace(u, ln.length(), whitespace);
              }else{
                extracts[q][r] = extracts[q][r].replace(u, ln.length(), comment);
              }
            }
          }
        }
      }
      extracts[q][r] = trim(extracts[q][r]);
      int count = 0;
      for(int v = 0; v < pieces.size(); v++) {
        for(int w = 0; w < pieces[v].size(); w++) {
          if(extracts[q][r].find(pieces[v][w]) != string::npos) {
            count++;
          }
        }
      }
      if(count == 0) {
        extracts[q].erase(extracts[q].begin() + r);
        r = r - 1;
      }
    }
  }
    
  for(int o = 0; o < extracts.size(); o++) {
    for(int r = 1; r < extracts[o].size(); r++) {
      string t = extracts[o][r];
      string word = "";
      int place = -1;
      for(int k = 0; k < t.length(); k++) {
        if((t.at(k) >= 65 && t.at(k) <= 90) || (t.at(k) >= 97 && t.at(k) <= 122) || (t.at(k) >= 48 && t.at(k) <= 57) || (t.at(k) == '_' || t.at(k) == '.')) {
          word += t.at(k);
        }else if(t.at(k) == '(' || t.at(k) == '[' || t.at(k) == '{') {
          char front = t.at(k);
          char back = '0';
          if(front == '(') {
            back = ')';
          }else if(front == '[') {
            back = ']';
          }else if(front == '{') {
            back = '}';
          }
          int totalFront = 0;
          int totalBack = 0;
          for(int y = k; y < t.length(); y++) {
            if(t.at(y) == front) {
              totalFront++;
            }else if(t.at(y) == back) {
              totalBack++;
            }
            if(totalFront == totalBack) {
              word += t.at(y);
              break;
            }else{
              word += t.at(y);
              k++;
            }
          }
        }else if(t.at(k) == '=') {
          place = k;
          break;
        }
      }
      if(place != -1) {
        extracts[o][r] = word + "BREAKPOINT" + extracts[o][r].substr(place + 1);
      }
    }
  }
    
  string lists[] = {"append", "insert", "remove", "index", "count"};
    
  vector<string> mainClasses;
  vector<int> theLens;
    
  for(int q = 0; q < pieces.size(); q++) {
    theLens.push_back(pieces[q][0].length());
  }
    
  sort(theLens.begin(), theLens.end());
    
  for(int c = 0; c < theLens.size(); c++) {
    int count = 0;
    for(int f = c + 1; f < theLens.size(); f++) {
      if(theLens[f] == theLens[c]) {
        count++;
      }
    }
    if(count > 0) {
      theLens.erase(theLens.begin() + c);
    }
  }
    
  reverse(theLens.begin(), theLens.end());
    
  for(int c = 0; c < theLens.size(); c++) {
    for(int j = 0; j < pieces.size(); j++) {
      if(pieces[j][0].length() == theLens[c]) {
        mainClasses.push_back(pieces[j][0]);
      }
    }
  }
        
  for(int x = 0; x < extracts.size(); x++) {
    vector<string> extras;
    for(int y = 1; y < extracts[x].size(); y++) {
      int position = -1;
      for(int p = 0; p < classes.size(); p++) {
        if(trim(classes[p][0]) == trim(extracts[x][0])) {
          position = p;
        }
      }
      vector<string> variables;
      int pos = 0;
      for(int j = 0; j < classes[position].size(); j++) {
        if(classes[position][j].find("__init__") != string::npos && classes[position][j].find("def ") != string::npos) {
          pos = j;
          break;
        }
      }
      for(int z = pos + 1; z < classes[position].size(); z++) {
        if(firstCharPlace(classes[position][z]) == classes[position][z].find("def ")) {
          break;
        }else if(trim(classes[position][z]).find("self.") == 0 && classes[position][z].find("=") != string::npos){
          string line = classes[position][z].substr(0, classes[position][z].find("="));
          variables.push_back(trim(line));
        }
      }

      if(extracts[x][y].find("BREAKPOINT") != string::npos) {
        string title = extracts[x][y].substr(0, extracts[x][y].find("BREAKPOINT"));
        string value = extracts[x][y].substr(extracts[x][y].find("BREAKPOINT") + 10);
        string word = "";
        for(int k = 0; k < title.length(); k++) {
          if((title.at(k) >= 65 && title.at(k) <= 90) || (title.at(k) >= 97 && title.at(k) <= 122) || (title.at(k) >= 48 && title.at(k) <= 57) || (title.at(k) == '_' || title.at(k) == '.')) {
            word += title.at(k);
          }else if(title.at(k) == '[') {
            char front = title.at(k);
            char back = ']';
            int totalFront = 0;
            int totalBack = 0;
            for(int y = k; y < title.length(); y++) {
              if(title.at(y) == front) {
                totalFront++;
              }else if(title.at(y) == back) {
                totalBack++;
              }
              if(totalFront == totalBack) {
                word += title.at(y);
                break;
              }else{
                word += title.at(y);
                k++;
              }
            }
          }
        }
                                        
        //checking if there is an instance variable in Exact front for title/varname
        //if so, trimming the instance variable name
        //then seeing if there are any list methods being invoked or not
        int amt = 0;
        bool acceptValue = false;
        for(int f = 0; f < variables.size(); f++) {
          if(trim(title).find(variables[f]) == 0 && title.length() > variables[f].length()) {
            title = title.substr(variables[f].length());
            if(title != "") {
              for(int q = 0; q < 5; q++) {
                if(trim(title).find("." + lists[q] + "(") == 0) {
                  amt++;
                }
              }
            }
            break;
          }else if(trim(title) == variables[f]) {
            acceptValue = true;
          }
        }
                    
        //if there is no instance variable, then checking if self. keyword is there
        //if it is, removing it
        //if it is not, then making title a null thing
        if(trim(title).find("self.") == 0) {
          title = title.substr(4);
        }else{
          title = "NULL";
        }
          
        //checking if title or varname invokes any methods
        for(int w = 0; w < pieces.size(); w++) {
          for(int i = 1; i < pieces[w].size(); i++) {
            if(trim(title).find("." + pieces[w][i] + "(") != string::npos) {
              amt++;
            }
          }
        }
         
        //if no invocations are made, then making title a null thing
        if(amt == 0) {
          title = "NULL";
        }
          
        amt = 0;
          
        //now checking the value to see whether it has an instance variable in EXACT front or not
        //if it does, removing it, checking it if has list invocations, exiting for loop
        for(int f = 0; f < variables.size(); f++) {
          if(trim(value).find(variables[f]) == 0 && value.length() > variables[f].length()) {
            value = value.substr(variables[f].length());
            for(int q = 0; q < 5; q++) {
              if(trim(value).find("." + lists[q] + "(") == 0) {
                amt++;
              }
            }
            break;
          }
        }
        
        //if value has self., removing the self. keyword
        if(trim(value).find("self.") == 0) {
          value = value.substr(4);
        }else if(acceptValue == true) {
          int position = -1;
          string subVal = value.substr(0, value.find("("));
          subVal = trim(subVal);
          for(int d = 0; d < pieces.size(); d++) {
            for(int f = 1; f < pieces[d].size(); f++) {
              if(subVal.find("." + pieces[d][f]) != string::npos) {
                position = subVal.find("." + pieces[d][f]);
                break;
              }
            }
            if(position != -1) {
              break;
            }
          }
          if(position != -1) {
            string before = value.substr(0, position);
            int stop = -1;
            for(int d = before.length() - 1; d >= 0; d = d - 1) {
              if((before.at(d) >= 65 && before.at(d) <= 90) || (before.at(d) >= 97 && before.at(d) <= 122) || (before.at(d) >= 48 && before.at(d) <= 57) || before.at(d) == '_'){
                stop = d;
              }else{
                break;
              }
            }
            if(stop != -1 && stop <= before.length() - theLens[theLens.size() - 1]) {
              before = before.substr(stop);
              int point = -1;
              for(int b = 0; b < mainClasses.size(); b++) {
                if(before == mainClasses[b]) {
                  before = mainClasses[b] + "()";
                  value = before + value.substr(position);
                  point = b;
                  break;
                }
              }
              if(point == -1) {
                value = value.substr(position);
              }
            }else{
              value = value.substr(position);
            }
          }
        }
          
        //checking to see if value has method invocations
        for(int w = 0; w < pieces.size(); w++) {
          for(int i = 1; i < pieces[w].size(); i++) {
            if(trim(value).find("." + pieces[w][i] + "(") != string::npos) {
              amt++;
            }
          }
        }
         
        //if there are no method or list invocations, checking to see if value is a class Object
        //if it is not, then making value a null thing
        if(amt == 0) {
          int times = 0;
          for(int q = 0; q < pieces.size(); q++) {
            if(trim(value).find(trim(pieces[q][0]) + "(") == 0) {
              times++;
            }
          }
          //check if tuple or list has the class objects
          if(times == 0) {
            value = "NULL";
          }
        }
          
        if(title == "NULL" && value == "NULL") {
          extracts[x][y] = "";
        }else if(value == "NULL") {
          extracts[x][y] = title;
        }else if(title == "NULL") {
          extracts[x][y] = value;
        }else{
          extracts[x][y] = "";
          extras.push_back(title);
          extras.push_back(value);
        }
        
      }else{
          
        string expr = extracts[x][y];
        int amt = 0;
          
        for(int f = 0; f < variables.size(); f++) {
          if(trim(expr).find(variables[f]) == 0 && expr.length() > variables[f].length()) {
            expr = expr.substr(variables[f].length());
            for(int q = 0; q < 5; q++) {
              if(trim(expr).find("." + lists[q] + "(") == 0) {
                amt++;
              }
            }
            break;
          }
        }
          
        if(trim(expr).find("self.") == 0) {
          expr = expr.substr(4);
        }
          
        for(int w = 0; w < pieces.size(); w++) {
          for(int i = 1; i < pieces[w].size(); i++) {
            if(trim(expr).find("." + pieces[w][i] + "(") != string::npos) {
              amt++;
            }
          }
        }

        if(amt == 0) {
          int times = 0;
          for(int q = 0; q < pieces.size(); q++) {
            if(trim(expr).find(trim(pieces[q][0]) + "(") == 0) {
              times++;
            }
          }
          //check if a tuple or list has class Objects
          if(times == 0) {
            expr = "";
          }
        }
        extracts[x][y] = expr;
      }
    }
    for(int l = 0; l < extras.size(); l++) {
      extracts[x].push_back(extras[l]);
    }
  }
        
  vector<vector<string> > magnitudes = magnitude(list);

  //we create a set of substrings relating to method and list invocations and class Objects --> then we analyze their context
  //we get rid of internal parentheses within the necessary parentheses
  for(int q = 0; q < extracts.size(); q++) {
    for(int r = 1; r < extracts[q].size(); r++) {
      string line = extracts[q][r];
      vector<int> locations;
      vector<string> parens;
      for(int g = 0; g < line.length(); g++) {
        if(line.at(g) == '(') {
          locations.push_back(g);
          int fronts = 0;
          int backs = 0;
          for(int h = g; h < line.length(); h++) {
            if(line.at(h) == '(') {
              fronts++;
            }else if(line.at(h) == ')') {
              backs++;
            }
            if(fronts == backs) {
              locations.push_back(h);
              g++;
              break;
            }else{
              g++;
            }
          }
          parens.push_back(line.substr(locations[0], locations[1] - locations[0] + 1));
          locations.clear();
        }
      }
      for(int g = 0; g < parens.size(); g++) {
        string ln = parens[g].substr(1, parens[g].length() - 2);
        vector<int> places;
        vector<string> express;
        for(int k = 0; k < ln.length(); k++) {
          if(ln.at(k) == '(') {
            places.push_back(k);
            int totalfronts = 0;
            int totalbacks = 0;
            for(int h = k; h < ln.length(); h++) {
              if(ln.at(h) == '(') {
                totalfronts++;
              }else if(ln.at(h) == ')') {
                totalbacks++;
              }
              if(totalfronts == totalbacks) {
                places.push_back(h);
                k++;
                break;
              }else{
                k++;
              }
            }
            express.push_back(parens[g].substr(places[0] + 1, places[1] - places[0] + 1));
            places.clear();
          }
        }
        for(int y = 0; y < express.size(); y++) {
          int location = extracts[q][r].find(express[y]);
          int len = express[y].length() - 1;
          extracts[q][r] = extracts[q][r].replace(location + 1, len - 1, "#");
        }
        express.clear();
      }
    }
  }
    
  for(int j = 0; j < extracts.size(); j++) {
    for(int k = 1; k < extracts[j].size(); k++) {
      if(extracts[j][k].find(".") == 0) {
        string line = extracts[j][k];
        vector<int> locations;
        for(int i = 0; i < line.length(); i++) {
          if(line.at(i) == '.') {
            locations.push_back(i);
          }else if(line.at(i) == '(' || line.at(i) == '[' || line.at(i) == '{') {
            char front = line.at(i);
            char back = '0';
            if(front == '(') {
              back = ')';
            }else if(front == '[') {
              back = ']';
            }else if(front == '{') {
              back = '}';
            }
            int totalFront = 0;
            int totalBack = 0;
            for(int y = i; y < line.length(); y++) {
              if(line.at(y) == front) {
                totalFront++;
              }else if(line.at(y) == back) {
                totalBack++;
              }
              if(totalFront == totalBack) {
                break;
              }else{
                i++;
              }
            }
          }
        }
        if(locations.size() > 1) {
          locations.push_back(line.length());
          string word = extracts[j][k];
          extracts[j].erase(extracts[j].begin() + k);
          for(int u = 0; u < locations.size() - 1; u++) {
            extracts[j].insert(extracts[j].begin() + 1, word.substr(locations[u], locations[u+1] - locations[u]));
          }
        }
      }
    }
  }
    
  vector<vector<string> > aggregates;
    
  for(int u = 0; u < extracts.size(); u++) {
    for(int i = 0; i < extracts[u].size(); i++) {
      int count = 0;
      for(int q = 0; q < 5; q++) {
        if(trim(extracts[u][i]).find("." + lists[q] + "(") == 0) {
          count++;
        }
      }
      if(count > 0) {
        string sub = extracts[u][i].substr(extracts[u][i].find("(") + 1, extracts[u][i].find_last_of(")") - extracts[u][i].find("(") - 1);
        extracts[u][i] = trim(sub);
        sub = "";
        for(int d = 0; d < pieces.size(); d++) {
          if(extracts[u][i].find(pieces[d][0] + "(") != string::npos) {
            vector<string> twos;
            twos.push_back(extracts[u][0]);
            twos.push_back(pieces[d][0]);
            aggregates.push_back(twos);
          }
        }
      }else if(extracts[u][i].find(".") == 0){
        int times = 0;
        string defName = extracts[u][i].substr(1, extracts[u][i].find("(") - 1);
        string params = extracts[u][i].substr(extracts[u][i].find("(") + 1, extracts[u][i].find_last_of(")"));
        params = params.substr(0, params.length() - 1);
        for(int d = 0; d < params.length(); d++) {
          if(params.at(d) == ',') {
            times++;
          }else if(params.at(d) == '[' || params.at(d) == '{' || params.at(d) == '(') {
            char front = params.at(d);
            char back = '0';
            if(front == '(') {
              back = ')';
            }else if(front == '[') {
              back = ']';
            }else if(front == '{') {
              back = '}';
            }
            int totalFront = 0;
            int totalBack = 0;
            for(int y = d; y < params.length(); y++) {
              if(params.at(y) == front) {
                totalFront++;
              }else if(params.at(y) == back) {
                totalBack++;
              }
              if(totalFront == totalBack) {
                break;
              }else{
                d++;
              }
            }
          }
        }
        if(params != "") {
          times++;
        }
        for(int s = 0; s < magnitudes.size(); s++) {
          for(int p = 1; p < magnitudes[s].size(); p++) {
            string definition = magnitudes[s][p].substr(0, magnitudes[s][p].find(" "));
            definition = trim(definition);
            string range = trim(magnitudes[s][p].substr(magnitudes[s][p].find(" ")));
            int min = stoi(range.substr(0, range.find("-")));
            int max = stoi(range.substr(range.find("-") + 1));
            if(definition == defName) {
              if(times >= min && times <= max) {
                int index = -1;
                for(int h = 0; h < magnitudes.size(); h++) {
                  if(extracts[u][0] == magnitudes[h][0]) {
                    for(int a = 0; a < magnitudes[h].size(); a++) {
                      if(magnitudes[h][a].substr(0, magnitudes[h][a].find(" ")) == definition) {
                        index = h;
                        break;
                      }
                    }
                    break;
                  }
                }
                if(index == -1) {
                  vector<string> pairs;
                  pairs.push_back(extracts[u][0]);
                  pairs.push_back(magnitudes[s][0]);
                  aggregates.push_back(pairs);
                }
                
              }else{
                //could add case about when self is the only parameter here
              }
            }
          }
        }
      }else{
        for(int d = 0; d < pieces.size(); d++) {
          if(extracts[u][i].find(pieces[d][0] + "(") != string::npos) {
            vector<string> twos;
            twos.push_back(extracts[u][0]);
            twos.push_back(pieces[d][0]);
            aggregates.push_back(twos);
          }
        }
      }
    }
  }
    
  vector<vector<string> > inherits = inheritance(list);
  vector<vector<string> > composites = composition(list);
    
  for(int q = 0; q < aggregates.size(); q++) {
    string className = trim(aggregates[q][0]);
    string usedClass = trim(aggregates[q][1]);
    if(className.find(":") != string::npos) {
      className = className.substr(0, className.find(":"));
    }
    if(className.find("(") != string::npos) {
      className = className.substr(0, className.find("("));
    }
    if(className.find("class") != string::npos) {
      className = className.substr(className.find("class") + 5);
    }
    if(usedClass.find(":") != string::npos) {
      usedClass = usedClass.substr(0, usedClass.find(":"));
    }
    if(usedClass.find("(") != string::npos) {
      usedClass = usedClass.substr(0, usedClass.find("("));
    }
    if(usedClass.find("class") != string::npos) {
      usedClass = usedClass.substr(usedClass.find("class") + 5);
    }
    aggregates[q][0] = usedClass;
    aggregates[q][1] = className;
  }
    
  for(int q = 0; q < aggregates.size(); q++) {
    int count = 0;
    for(int u = 0; u < composites.size(); u++) {
      if(aggregates[q][0] == composites[u][0] && aggregates[q][1] == composites[u][1]) {
        count++;
      }
    }
    if(count > 0) {
      aggregates.erase(aggregates.begin() + q);
      q = q - 1;
    }
  }
    
  for(int q = 0; q < aggregates.size(); q++) {
    int count = 0;
    for(int u = 0; u < inherits.size(); u++) {
      if(aggregates[q][0] == inherits[u][0] && aggregates[q][1] == inherits[u][1]) {
        count++;
      }else if(aggregates[q][0] == inherits[u][1] && aggregates[q][1] == inherits[u][0]) {
        count++;
      }
    }
    if(count > 0 || aggregates[q][0] == aggregates[q][1]) {
      aggregates.erase(aggregates.begin() + q);
      q = q - 1;
    }
  }
               
  for(int q = 0; q < aggregates.size(); q++) {
    int count = 0;
    for(int u = q + 1; u < aggregates.size(); u++) {
      if(aggregates[q][0] == aggregates[u][0] && aggregates[q][1] == aggregates[u][1]) {
        count++;
      }
    }
    if(count > 0) {
      aggregates.erase(aggregates.begin() + q);
      q = q - 1;
    }
  }
    
  return aggregates;
    
}

vector<vector<string> > Parser::association(vector<string> list, vector<string> files) {
        
  vector<vector<string> > classes;
  vector<vector<string> > pieces = getInfo(list, files, classes);
  vector<vector<string> > magnitudes = magnitude(list);
  vector<vector<string> > components;
        
  for(int q = 0; q < classes.size(); q++) {
    for(int e = 1; e < classes[q].size(); e++) {
      if(!(classes[q][e].find("def ") != string::npos && trim(classes[q][e]).find("def") == 0)) {
        int count = 0;
        for(int j = 0; j < pieces.size(); j++) {
          for(int i = 0; i < pieces[j].size(); i++) {
            if(classes[q][e].find(pieces[j][i]) != string::npos) {
              count++;
            }
          }
        }
        if(count > 0) {
          classes[q][e] = trim(classes[q][e]);
        }else{
          classes[q].erase(classes[q].begin() + e);
          e = e - 1;
        }
      }else{
        classes[q].erase(classes[q].begin() + e);
        e = e - 1;
      }
    }
  }

  vector<string> mainClasses;
  vector<int> theLens;
    
  for(int q = 0; q < pieces.size(); q++) {
    theLens.push_back(pieces[q][0].length());
  }
        
  sort(theLens.begin(), theLens.end());
    
  for(int c = 0; c < theLens.size(); c++) {
    int count = 0;
    for(int f = c + 1; f < theLens.size(); f++) {
      if(theLens[f] == theLens[c]) {
        count++;
      }
    }
    if(count > 0) {
      theLens.erase(theLens.begin() + c);
    }
  }
    
  reverse(theLens.begin(), theLens.end());
    
  for(int c = 0; c < theLens.size(); c++) {
    for(int j = 0; j < pieces.size(); j++) {
      if(pieces[j][0].length() == theLens[c]) {
        mainClasses.push_back(pieces[j][0]);
      }
    }
  }
  
  for(int x = 0; x < classes.size(); x++) {
    vector<string> subset;
    subset.push_back(classes[x][0]);
    for(int y = 1; y < classes[x].size(); y++) {
      vector<int> locations;
      string line = classes[x][y];
      for(int h = 0; h < pieces.size(); h++) {
        for(int f = 0; f < pieces[h].size(); f++) {
          string target = "";
          if(f == 0) {
            target = pieces[h][0] + "(";
          }else{
            target = "." + pieces[h][f] + "(";
          }
          if(line.length() > target.length()) {
            for(int z = 0; z < line.length() - target.length() + 1; z++) {
              if(line.substr(z,target.length()) == target) {
                if(f > 0) {
                  string before = line.substr(0, z);
                  string classTerm = "";
                  int stop = -1;
                  for(int d = before.length() - 1; d >= 0; d = d - 1) {
                    if((before.at(d) >= 65 && before.at(d) <= 90) || (before.at(d) >= 97 && before.at(d) <= 122) || (before.at(d) >= 48 && before.at(d) <= 57) || (before.at(d) == '_')) {
                      stop = d;
                    }else{
                      break;
                    }
                  }
                  if(stop != -1 && stop <= before.length() - theLens[theLens.size() - 1]) {
                    before = before.substr(stop);
                    for(int p = 0; p < mainClasses.size(); p++) {
                      if(before == mainClasses[p]) {
                        classTerm = mainClasses[p];
                        break;
                      }
                    }
                  }
                  if(classTerm != "") {
                    locations.push_back(z - classTerm.length());
                    if(z + 1 < line.length() - target.length() + 1) {
                      z++;
                    }
                  }else{
                    locations.push_back(z);
                  }
                }else if(f == 0){
                  locations.push_back(z);
                }
              }
            }
          }
        }
      }
      for(int d = 1; d < locations.size(); d++) {
        string substring = classes[x][y].substr(locations[d]);
        substring = trim(substring);
        substring = grammarCheck(substring);
        subset.push_back(substring);
      }
    }
    components.push_back(subset);
  }
    
  vector<vector<string> > associates;
    
  for(int d = 0; d < components.size(); d++) {
    for(int f = 1; f < components[d].size(); f++) {
      int times = 0;
      if(components[d][f].at(0) == '.') {
        string defName = components[d][f].substr(1, components[d][f].find("(") - 1);
        defName = trim(defName);
        string params = components[d][f].substr(components[d][f].find("(") + 1, components[d][f].find_last_of(")"));
        params = params.substr(0, params.length() - 1);
        params = trim(params);
        for(int d = 0; d < params.length(); d++) {
          if(params.at(d) == ',') {
            times++;
          }else if(params.at(d) == '[' || params.at(d) == '{' || params.at(d) == '(') {
            char front = params.at(d);
            char back = '0';
            if(front == '(') {
              back = ')';
            }else if(front == '[') {
              back = ']';
            }else if(front == '{') {
              back = '}';
            }
            int totalFront = 0;
            int totalBack = 0;
            for(int y = d; y < params.length(); y++) {
              if(params.at(y) == front) {
                totalFront++;
              }else if(params.at(y) == back) {
                totalBack++;
              }
              if(totalFront == totalBack) {
                break;
              }else{
                d++;
              }
            }
          }
        }
        if(params != "") {
          times++;
        }
        params = to_string(times);
        components[d][f] = defName + "->" + params;
      }else{
        vector<string> pairs;
        string beforeParen = components[d][f].substr(0, components[d][f].find("("));
        if(beforeParen.find(".") != string::npos) {
          beforeParen = beforeParen.substr(0, beforeParen.find("."));
        }
        components[d][f] = beforeParen;
      }
    }
  }
    
  for(int x = 0; x < components.size(); x++) {
    for(int y = 1; y < components[x].size(); y++) {
      if(components[x][y].find("->") != string::npos) {
        int numParams = stoi(components[x][y].substr(components[x][y].find("->") + 2));
        string method = components[x][y].substr(0, components[x][y].find("->"));
        for(int s = 0; s < magnitudes.size(); s++) {
          for(int p = 1; p < magnitudes[s].size(); p++) {
            string definition = magnitudes[s][p].substr(0, magnitudes[s][p].find(" "));
            definition = trim(definition);
            string range = trim(magnitudes[s][p].substr(magnitudes[s][p].find(" ")));
            int min = stoi(range.substr(0, range.find("-")));
            int max = stoi(range.substr(range.find("-") + 1));
            if(definition == method) {
              if(numParams >= min && numParams <= max) {
                int index = -1;
                for(int h = 0; h < magnitudes.size(); h++) {
                  if(components[x][0] == magnitudes[h][0]) {
                    for(int a = 0; a < magnitudes[h].size(); a++) {
                      if(magnitudes[h][a].substr(0, magnitudes[h][a].find(" ")) == definition) {
                        index = h;
                        break;
                      }
                    }
                    break;
                  }
                }
                if(index == -1) {
                  vector<string> pairs;
                  pairs.push_back(magnitudes[s][0]);
                  pairs.push_back(components[x][0]);
                  associates.push_back(pairs);
                }
              }
            }
          }
        }
      }else{
        vector<string> pairs;
        pairs.push_back(components[x][y]);
        pairs.push_back(components[x][0]);
        associates.push_back(pairs);
      }
    }
  }
    
  for(int q = 0; q < associates.size(); q++) {
    string usedClass = trim(associates[q][0]);
    string className = trim(associates[q][1]);
    if(className.find(":") != string::npos) {
      className = className.substr(0, className.find(":"));
    }
    if(className.find("(") != string::npos) {
      className = className.substr(0, className.find("("));
    }
    if(className.find("class") != string::npos) {
      className = className.substr(className.find("class") + 5);
    }
    if(usedClass.find(":") != string::npos) {
      usedClass = usedClass.substr(0, usedClass.find(":"));
    }
    if(usedClass.find("(") != string::npos) {
      usedClass = usedClass.substr(0, usedClass.find("("));
    }
    if(usedClass.find("class") != string::npos) {
      usedClass = usedClass.substr(usedClass.find("class") + 5);
    }
    associates[q][0] = usedClass;
    associates[q][1] = className;
  }
        
  vector<vector<string> > composites = composition(list);
  vector<vector<string> > aggregates = aggregation(list, files);
  vector<vector<string> > inherits = inheritance(list);
    
  for(int q = 0; q < associates.size(); q++) {
    int count = 0;
    for(int u = 0; u < composites.size(); u++) {
      if(associates[q][0] == composites[u][0] && associates[q][1] == composites[u][1]) {
        count++;
      }else if(associates[q][1] == composites[u][0] && associates[q][0] == composites[u][1]) {
        count++;
      }
    }
    if(count > 0) {
      associates.erase(associates.begin() + q);
      q = q - 1;
    }
  }
    
  for(int q = 0; q < associates.size(); q++) {
    int count = 0;
    for(int u = 0; u < inherits.size(); u++) {
      if(associates[q][0] == inherits[u][0] && associates[q][1] == inherits[u][1]) {
        count++;
      }else if(associates[q][0] == inherits[u][1] && associates[q][1] == inherits[u][0]) {
        count++;
      }
    }
    if(count > 0) {
      associates.erase(associates.begin() + q);
      q = q - 1;
    }
  }
    
  for(int q = 0; q < associates.size(); q++) {
    int count = 0;
    for(int u = 0; u < aggregates.size(); u++) {
      if(associates[q][0] == aggregates[u][0] && associates[q][1] == aggregates[u][1]) {
        count++;
      }else if(associates[q][0] == aggregates[u][1] && associates[q][1] == aggregates[u][0]) {
        count++;
      }
    }
    if(count > 0) {
      associates.erase(associates.begin() + q);
      q = q - 1;
    }
  }
    
  for(int q = 0; q < associates.size(); q++) {
    int count = 0;
    for(int u = q + 1; u < associates.size(); u++) {
      if(associates[q][0] == associates[u][0] && associates[q][1] == associates[u][1]) {
        count++;
      }
    }
    if(count > 0) {
      associates.erase(associates.begin() + q);
      q = q - 1;
    }
  }
  
  return associates;
    
}

void Parser::noMLComments(string type, vector<string> &allLines) {

  string character;
  vector<int> locations;

  if(type == "docString") {
    character = "\"";
  }else if(type == "regML") {
    character = "'";
  }

  for(int j = 0; j < allLines.size(); j++) {
    int count = 0;
    if(allLines[j].length() > 1 && allLines[j].find(character) != string::npos) {
      for(int k = 0; k < allLines[j].length() - 2; k++) {
        if(allLines[j].substr(k,1) == character && allLines[j].substr(k+1,1) == character && allLines[j].substr(k+2,1) == character) {
          count++;
        }
      }
    }
    if(count == 2) {
      allLines.erase(allLines.begin() + j);
      j = j - 1;
    }
  }

  for(int j = 0; j < allLines.size(); j++) {
    int count = 0;
    if(allLines[j].length() > 1 && allLines[j].find(character) != string::npos) {
      for(int k = 0; k < allLines[j].length() - 2; k++) {
        if(allLines[j].substr(k,1) == character && allLines[j].substr(k+1,1) == character && allLines[j].substr(k+2,1) == character) {
          count++;
        }
      }
    }
    if(count == 1) {
      locations.push_back(j);
    }
  }

  for(int k = 0; k < locations.size(); k += 2) {
    for(int g = locations[k]; g <= locations[k+1]; g++) {
      allLines[g] = "";
    }
  }

  for(int h = 0; h < allLines.size(); h++) {
    if(allLines[h] == "") {
      allLines.erase(allLines.begin() + h);
      h = h - 1;
    }
  }

}


string Parser::trim(string word) {

  string term;

  for(int k = 0; k < word.length(); k++) {
     if(!isspace(word.at(k))) {
       term.push_back(word.at(k));
     }
  }

  return term;

}


int Parser::firstCharPlace(string& word) {

   int index = 0;

   while(isspace(word.at(index))) {
     index++;
   }

   return index;
}


vector<string> Parser::parse(string& fileName) {

  fstream document;
  string wholeLine;
  vector<string> allLines;

  document.open(fileName);

  while(!document.eof()) {
    getline(document, wholeLine);

    int whitespaces = 0;

    for(int k = 0; k < wholeLine.length(); k++) {
      if(isspace(wholeLine.at(k))) {
        whitespaces++;
	  }
    }    

    if(whitespaces != wholeLine.length()) {
      if(wholeLine.find("#") != string::npos) {
        int k = 0;
        while(isspace(wholeLine.at(k))) {
          k++;
        }
        if(wholeLine.at(k) != '#') {
          wholeLine = wholeLine.substr(0, wholeLine.find("#"));
      	  allLines.push_back(wholeLine);
        }
      }else{
        allLines.push_back(wholeLine);
      }
    }
  }

  noMLComments("docString", allLines);
  noMLComments("regML", allLines);

  document.close();  
  
  return allLines;  

}

vector<string> Parser::pass(vector<string> &list) {

   vector<string> acceptable;

   for(int k = 0; k < list.size(); k++) {

     if(list[k].find("class ") != string::npos && trim(list[k]).find("class") == 0) {
	   acceptable.push_back(list[k]);

   	   int position = -1;

	   for(int j = k + 1; j < list.size(); j++) {
	    if(trim(list[j]).find("def") == 0 && list[j].find("def ") != string::npos) {
	      position = int(list[j].find("def "));
	      break;
	    }
	   }

	   for(int e = k + 1; e < list.size(); e++) {
	    if(list[e].find(":") == string::npos && list[e].find("=") != string::npos) {
	      if(firstCharPlace(list[e]) == position) {
	        acceptable.push_back(list[e]);
	      }
	    }else if(firstCharPlace(list[e]) <= list[k].find("class")) {
	      break;
	    }
	   }

     }else if(list[k].find("def ") != string::npos && trim(list[k]).find("def") == 0) {

	   if(trim(list[k]).find("_init_") != string::npos) {
          for(int j = k + 1; j < list.size(); j++) {
            if(list[j].find("self.") != string::npos && trim(list[j]).find("self.") == 0 && list[j].find("=") != string::npos) {
              string instanceVar = list[j].substr(0, list[j].find("="));
              instanceVar = trim(instanceVar);
              acceptable.push_back(instanceVar);
            }else if(firstCharPlace(list[j]) <= firstCharPlace(list[k])) {
              break;
            }
          }
        }
         
       acceptable.push_back(list[k]);

     }else if(list[k].find("@") != string::npos && trim(list[k]).find("@") == 0) {
	   acceptable.push_back(list[k]);
     }else if(list[k].find("__metaclass__") != string::npos && list[k].find("=") != string::npos && list[k].find("ABCMeta") != string::npos) {
	   acceptable.push_back(list[k]);
     }else if(trim(list[k]) == "pass;" || trim(list[k]) == "pass") {
	   acceptable.push_back(list[k]);
     }
   }

   return acceptable;
}

vector< vector<string> > Parser::UMLinfo(string& fileName) {

    vector<string> list = parse(fileName);
    vector<string> acceptable = pass(list);
    vector<vector<string> > classes;
    
    for(int j = 0; j < acceptable.size(); j++) {
      vector<string> subset;
      if(acceptable[j].find("class ") != string::npos && trim(acceptable[j]).find("class") == 0){
        subset.push_back(acceptable.at(j));
        for(int k = j + 1; k < acceptable.size(); k++) {
          if(acceptable[k].find("class ") != string::npos && trim(acceptable[k]).find("class") == 0){
            break;
          }else{
            subset.push_back(acceptable.at(k));
          }
        }
        classes.push_back(subset);
      }
    }
    
    for(int k = 0; k < classes.size(); k++) {
      classes[k][0] = classes[k][0].substr(0, classes[k][0].find("("));
      classes[k][0] = classes[k][0].substr(classes[k][0].find("class") + 5);
      classes[k][0] = classes[k][0].substr(0, classes[k][0].find(":"));
      classes[k][0] = trim(classes[k][0]);
      for(int v = 1; v < classes[k].size(); v++) {
        string line = classes[k][v];
        if(line.find("def ") != string::npos) {
            if(classes[k][v].find("def ") != string::npos && trim(classes[k][v]).find("def") == 0 && classes[k][v].find("__init__") == string::npos) {
               classes[k][v] = classes[k][v].substr(classes[k][v].find("def ") + 4);
               classes[k][v] = classes[k][v].substr(0, classes[k][v].find("("));
               classes[k][v] = classes[k][v] + "():";
               classes[k][v] = trim(classes[k][v]);
               if(classes[k][v].find("__") == 0) {
                 classes[k][v] = "";
               }
            }
            if(classes[k][v] != "") {
               for(int d = v - 1; d >= 0; d = d - 1) {
                  if(classes[k][d].find("@") == string::npos) {
                    break;
                  }else{
                    if(classes[k][d].find("@static") != string::npos) {
                      if(classes[k][v].find("\e[4m") == string::npos) {
                        classes[k][v] = "\e[4m" + classes[k][v] + "\e[0m";
                      }
                    }else if(classes[k][d].find("@") != string::npos && classes[k][d].find("abstractstaticmethod") != string::npos) {
                      if(classes[k][v].find("\e[3m") == string::npos) {
                        classes[k][v] = "\e[3m" + classes[k][v] + "\e[0m";
                      }
                      if(classes[k][v].find("\e[4m") == string::npos) {
                        classes[k][v] = "\e[4m" + classes[k][v] + "\e[0m";
                      }
                      if(classes[k][0].find("\e[3m") == string::npos) {
                        classes[k][0] = "\e[3m" + classes[k][0] + "\e[0m";
                      }
                    }else if(classes[k][d].find("@") != string::npos && classes[k][d].find("abstract") != string::npos) {
                      if(classes[k][v].find("\e[3m") == string::npos) {
                        classes[k][v] = "\e[3m" + classes[k][v] + "\e[0m";
                      }
                      if(classes[k][0].find("\e[3m") == string::npos) {
                        classes[k][0] = "\e[3m" + classes[k][0] + "\e[0m";
                      }
                    }
                  }
               }
               for(int h = v + 1; h < classes[k].size(); h++) {
                 string ln = trim(classes[k][h]);
                 bool isDef = ln.find("def") == 0 && classes[k][h].find("def ") == line.find("def ");
                 bool isClass = ln.find("class") == 0 && classes[k][h].find("class ") != string::npos;
                 if(isDef || isClass) {
                  break;
                 }else if(ln == "pass;" || ln == "pass") {
                   if(classes[k][0].find("\e[3m") == string::npos) {
                     classes[k][0] = "\e[3m" + classes[k][0] + "\e[0m";
                   }
                   if(classes[k][h].find("pass") > line.find("def")) {
                     if(classes[k][v].find("\e[3m") == string::npos) {
                       classes[k][v] = "\e[3m" + classes[k][v] + "\e[0m";
                     }
                   }
                 }
               }
            }else{
               classes[k].erase(classes[k].begin() + v);
               v = v - 1;
            }
        }else if(line.find("class ") != string::npos && trim(line).find("class") == 0) {
          int index = int(line.find("ABCMeta"));
          int place = int(line.find("(ABC)"));
          if(index != string::npos || place != string::npos) {
            //modify classes[k][v]
            if(classes[k][v].find("\e[3m") == string::npos) {
              classes[k][v] = "\e[3m" + classes[k][v] + "\e[0m";
            }
          }
        }else if(line.find("__metaclass__") != string::npos && line.find("=") != string::npos && line.find("ABCMeta") != string::npos) {
          if(classes[k][0].find("\e[3m") == string::npos) {
            classes[k][0] = "\e[3m" + classes[k][0] + "\e[0m";
          }
        }else if(trim(line) == "pass" || trim(line) == "pass;") {
          int position = -1;
          for(int e = 0; e < classes[k].size(); e++) {
            if(classes[k][e].find("def ") != string::npos) {
              position = firstCharPlace(classes[k][e]);
            }
          }
          if(position == -1 || position == firstCharPlace(line)) {
            if(classes[k][0].find("\e[3m") == string::npos) {
              classes[k][0] = "\e[3m" + classes[k][0] + "\e[0m";
            }
          }
        }
      }
    }
    
    for(int q = 0; q < classes.size(); q++) {
      for(int d = 0; d < classes[q].size(); d++) {
        classes[q][d] = classes[q][d].substr(firstCharPlace(classes[q][d]));
      }
    }
    
    for(int d = 0; d < classes.size(); d++) {
      for(int f = 0; f < classes[d].size(); f++) {
        if(classes[d][f].find("@") != string::npos) {
          classes[d].erase(classes[d].begin() + f);
          f = f - 1;
        }else if(classes[d][f].find("__metaclass__") != string::npos && classes[d][f].find("=") != string::npos && classes[d][f].find("ABCMeta") != string::npos) {
          classes[d].erase(classes[d].begin() + f);
          f = f - 1;
        }else if(trim(classes[d][f]) == "pass" || trim(classes[d][f]) == "pass;") {
          classes[d].erase(classes[d].begin() + f);
          f = f - 1;
        }else if(classes[d][f].find("__init__") != string::npos) {
          classes[d].erase(classes[d].begin() + f);
          f = f - 1;
        }
      }
    }
    
    for(int q = 0; q < classes.size(); q++) {
      for(int d = 0; d < classes[q].size(); d++) {
        if(classes[q][d].find("self.") == 0) {
          int count = 0;
          for(int r = d + 1; r < classes[q].size(); r++) {
            if(classes[q][r] == classes[q][d]) {
              count++;
            }else if(classes[q][r].find("self.") != 0) {
              break;
            }
          }
          if(count > 0) {
            classes[q].erase(classes[q].begin() + d);
            d = d - 1;
          }
        }
      }
    }
        
    return classes;

}

vector<vector<string> > Parser::relations(vector<string> fileNames) {
    
    string blank = "";
    
    vector<vector<string> > list;
    vector<vector<string> > UMLpieces;
    
    for(int d = 0; d < fileNames.size(); d++) {
      if(UMLinfo(fileNames[d]).size() == 0) {
        fileNames.erase(fileNames.begin() + d);
        d = d - 1;
      }else{
        vector<string> part;
        part.push_back(fileNames[d]);
        vector<vector<string> > fileUML = UMLinfo(fileNames[d]);
        for(int f = 0; f < fileUML.size(); f++) {
          if(fileUML[f][0].find("\e[3m") != string::npos) {
            fileUML[f][0] = fileUML[f][0].substr(fileUML[f][0].find("\e[3m") + 4);
          }
          if(fileUML[f][0].find("\e[4m") != string::npos) {
            fileUML[f][0] = fileUML[f][0].substr(fileUML[f][0].find("\e[4m") + 4);
          }
          if(fileUML[f][0].find("\e[0m") != string::npos) {
            fileUML[f][0] = fileUML[f][0].substr(0, fileUML[f][0].find("\e[0m"));
          }
          part.push_back(fileUML[f][0]);
        }
        UMLpieces.push_back(part);
      }
    }
        
    for(int k = 0; k < fileNames.size(); k++) {
      vector<string> parsedList = parse(fileNames[k]);
      list.push_back(parsedList);
    }
        
    vector<string> files = fileNames;
    
    for(int k = 0; k < files.size(); k++) {
      files[k] = files[k].substr(0,files[k].find(".py"));
    }
        
    for(int e = 0; e < list.size(); e++) {
      vector<string> sub = list[e];
      vector<string> extraFiles;
      for(int k = 0; k < sub.size(); k++) {
        if(sub[k].find("import ") != string::npos && trim(sub[k]).find("import") == 0) {
          string line = trim(sub[k].substr(sub[k].find("import ") + 6, sub[k].length()));
          int i = -1;
          for(int d = 0; d < files.size(); d++) {
            if(line == files[d]) {
              list[e][k] = "IMPORT " + line + ".py";
              i = k;
              break;
            }
          }
          if(i == -1) {
            list[e][k] = "";
          }
        }else if(sub[k].find("from ") != string::npos && trim(sub[k]).find("from") == 0 && sub[k].find("import ") != string::npos) {
          string extras = trim(sub[k]).substr(trim(sub[k]).find("import") + 6);
          sub[k] = sub[k].substr(0, sub[k].find("import "));
          string line = trim(sub[k]);
          line = line.substr(line.find("from") + 4);
          if(line.find_last_of(".") != string::npos) {
            string portion = line.substr(line.find("."));
            portion = trim(portion);
            int dots = 0;
            for(int n = 0; n < portion.size(); n++) {
              if(portion.at(n) == '.') {
                dots++;
              }
            }
            if(dots == portion.size()) {
              vector<int> commas;
              commas.push_back(0);
              for(int j = 0; j < extras.length(); j++) {
                if(extras.at(j) == ',') {
                  commas.push_back(j-1);
                  commas.push_back(j+1);
                }
              }
              commas.push_back(extras.length() - 1);
              vector<string> stuff;
              for(int r = 0; r < commas.size(); r += 2) {
                int start = commas[r];
                int end = commas[r+1];
                string word = extras.substr(start, end - start + 1);
                stuff.push_back(trim(word));
              }
              for(int m = 0; m < stuff.size(); m++) {
                for(int x = 0; x < UMLpieces.size(); x++) {
                  for(int o = 1; o < UMLpieces[x].size(); o++) {
                    if(trim(UMLpieces[x][o]) == trim(stuff[m])) {
                      int num = 0;
                      for(int t = 0; t < files.size(); t++) {
                        if(UMLpieces[x][0] == files[t]) {
                          num++;
                        }
                      }
                      if(num == 0) {
                        extraFiles.push_back(trim(UMLpieces[x][0]));
                      }
                    }
                  }
                }
              }
            }else{
              line = line.substr(line.find_last_of(".") + 1);
            }
          }
          int i = -1;
          for(int d = 0; d < files.size(); d++) {
            if(line == files[d]) {
              list[e][k] = "IMPORT " + line + ".py";
              i = d;
              break;
            }
          }
          if(i == -1) {
            list[e][k] = "";
          }
          for(int f = 0; f < extraFiles.size(); f++) {
            list[e].insert(list[e].begin(), "IMPORT " + extraFiles[f]);
          }
        }
      }
    }
        
    for(int e = 0; e < list.size(); e++) {
      for(int f = 0; f < list[e].size(); f++) {
        if(list[e][f] == "") {
          list[e].erase(list[e].begin() + f);
          f = f - 1;
        }
      }
    }
    
    for(int f = 0; f < list.size(); f++) {
        int levelC = -1;
        int pos = 0;
        int placeC = -1;
        for(int h = 0; h < list[f].size(); h++) {
            if(list[f][h].find("class ") != string::npos && trim(list[f][h]).find("class") == 0) {
              levelC = firstCharPlace(list[f][h]);
              placeC = h;
              break;
            }else if(list[f][h].find("IMPORT ") != string::npos && trim(list[f][h]).find("IMPORT") == 0) {
              pos = h + 1;
            }
        }
        if(placeC == -1) {
           list.erase(list.begin() + f);
           f = f - 1;
        }else{
           int start = 0;
           if(pos > 0) {
             start = pos;
           }
           for(int y = start; y < list[f].size(); y++) {
             if(firstCharPlace(list[f][y]) == levelC) {
               if(list[f][y].find("class ") == string::npos) {
                 for(int r = y; r < list[f].size(); r++) {
                   if(list[f][r].find("class ") == levelC) {
                     break;
                   }else{
                     list[f][r] = "no";
                   }
                 }
               }
             }
           }
        }
        for(int y = 0; y < list[f].size(); y++) {
          if(list[f][y] == "no") {
            list[f].erase(list[f].begin() + y);
            y = y - 1;
          }
        }
    }
        
    vector<vector<string> > relationships;
    
    for(int k = 0; k < list.size(); k++) {
      vector<vector<string> > er = association(list[k], fileNames);
      for(int g = 0; g < er.size(); g++) {
            er[g].push_back("AC");
      }
      for(int y = 0; y < er.size(); y++) {
        relationships.push_back(er[y]);
      }
    }
        
    for(int k = 0; k < list.size(); k++) {
      vector<vector<string> > er = composition(list[k]);
      for(int g = 0; g < er.size(); g++) {
        er[g].push_back("CO");
      }
      for(int y = 0; y < er.size(); y++) {
        relationships.push_back(er[y]);
      }
    }
    
    for(int k = 0; k < list.size(); k++) {
        vector<vector<string> > ag = aggregation(list[k], fileNames);
        for(int g = 0; g < ag.size(); g++) {
          ag[g].push_back("AG");
        }
        for(int y = 0; y < ag.size(); y++) {
          relationships.push_back(ag[y]);
        }
    }
    
    for(int k = 0; k < list.size(); k++) {
      vector<vector<string> > ir = inheritance(list[k]);
      for(int g = 0; g < ir.size(); g++) {
        ir[g].push_back("IN");
      }
      for(int y = 0; y < ir.size(); y++) {
        relationships.push_back(ir[y]);
      }
    }
    
    //now keep all the stuff in the list[w] that has keywords in UMLinfo as well as information in IMPORT
    //if init and instance or class variables exist, then keep them    
    
    return relationships;
   
}

vector<string> Parser::topClasses(vector<string> files) {
    
    vector<vector<string> > r = relations(files);
    
    vector<string> bosses;
    
    for(int x = 0; x < r.size(); x++) {
        for(int y = 0; y < r[x].size(); y++) {
            bosses.push_back(r[x][1]);
        }
    }
    
    for(int f = 0; f < bosses.size(); f++) {
        int count = 0;
        for(int d = f + 1; d < bosses.size(); d++) {
            if(bosses[d] == bosses[f]) {
                count++;
            }
        }
        if(count > 0) {
            bosses.erase(bosses.begin() + f);
            f = f - 1;
        }
    }
    
    vector<vector<string> > groups;
    
    for(int x = 0; x < bosses.size(); x++) {
        vector<string> relaters;
        relaters.push_back(bosses[x]);
        for(int d = 0; d < r.size(); d++) {
            if(r[d][1] == bosses[x]) {
                relaters.push_back(r[d][0]);
            }
        }
        groups.push_back(relaters);
    }
    
    vector<string> relatedClasses;
    
    for(int h = 0; h < groups.size(); h++) {
        for(int f = 0; f < groups[h].size(); f++) {
            relatedClasses.push_back(groups[h][f]);
        }
        relatedClasses.push_back("--");
    }
    
    for(int f = 0; f < relatedClasses.size(); f++) {
        int position = -1;
        for(int d = 0; d < f - 1; d++) {
            if(relatedClasses[d] == relatedClasses[f]) {
                position = d;
                break;
            }
        }
        if(position >= 0 && relatedClasses[f] != "--") {;
            string target = relatedClasses[f];
            string owner = "";
            for(int j = f; j >= 0; j--) {
                if(relatedClasses[j] == "--") {
                    owner = relatedClasses[j+1];
                    break;
                }
            }
            if(owner != target) {
                relatedClasses.erase(relatedClasses.begin() + f);
                relatedClasses.insert(relatedClasses.begin() + position, owner);
            }
        }
    }
    
    for(int j = 0; j < relatedClasses.size(); j++) {
        int count = 0;
        for(int q = 0; q < j - 1; q++) {
            if(relatedClasses[j] == relatedClasses[q]) {
                count++;
            }
        }
        if(count > 0 || relatedClasses[j] == "--") {
            relatedClasses.erase(relatedClasses.begin() + j);
            j = j - 1;
        }
    }
    
    return relatedClasses;
    
}

vector< vector<string> > Parser::UMLdata(vector<string> fileNames) {
    
    vector< string > firstClasses = topClasses(fileNames);
        
    //rearranging this UMLinfo structure to make sure firstClasses always comes first
    
    vector< vector<string> > allInfo;
        
    for(int k = 0; k < fileNames.size(); k++) {
      vector< vector<string> > info = UMLinfo(fileNames[k]);
      for(int y = 0; y < info.size(); y++) {
        allInfo.push_back(info[y]);
      }
    }
            
    vector<int> lens;
    
    for(int h = 0; h < allInfo.size(); h++) {
      lens.push_back(allInfo[h].size());
    }
    
    sort(lens.begin(), lens.end());
    vector<int> copy;
    
    for(int f = 0; f < lens.size(); f++) {
        int c = 0;
        for(int h = f + 1; h < lens.size(); h++) {
          if(lens[h] == lens[f]) {
            c++;
          }
        }
        if(c == 0) {
          copy.push_back(lens[f]);
        }
    }
    
    lens = copy;
        
    vector< vector<string> > information;
    
    for(int h = 0; h < lens.size(); h++) {
      int num = lens[h];
      for(int g = 0; g < allInfo.size(); g++) {
        if(allInfo[g].size() == num) {
          information.push_back(allInfo[g]);
        }
      }
    }
        
    allInfo = information;
    
    vector<vector<string> > stores;
    
    for(int t = 0; t < firstClasses.size(); t++) {
      string regClass = firstClasses[t];
      string abClass = "\e[3m" + regClass;
      string stClass = "\e[4m" + regClass;
      for(int q = 0; q < allInfo.size(); q++) {
        if(allInfo[q][0].find(regClass) != string::npos || allInfo[q][0].find(abClass) != string::npos || allInfo[q][0].find(stClass) != string::npos) {
          stores.push_back(allInfo[q]);
          allInfo.erase(allInfo.begin() + q);
          q = q - 1;
        }
      }
    }
    
    for(int x = 0; x < allInfo.size(); x++) {
        stores.push_back(allInfo[x]);
    }
    
    return stores;
    
}
