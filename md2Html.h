#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

#define VS vector<string>

class md2Html {
public:
    md2Html(VS input);
    ~md2Html();
private:
    VS cInput;
    VS init(VS input);
    VS headerInit(VS input);
    VS headers(VS input);
    VS paragraph(VS input);
    VS Bold(VS input);
};