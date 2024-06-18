#include "md2Html.h"

int main() {
    VS input;
    input.resize(1);
    for (size_t i = 0; getline(cin, input.at(i), '\n'); i++) {
        input.resize(i + 2);
        //cout << input.at(i) << endl;
    }
    md2Html md2Html(input);
    return 0;
}