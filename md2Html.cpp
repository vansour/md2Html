#include "md2Html.h"

using namespace std;

md2Html::md2Html(VS input) {
    VS tInput = move(input);
    tInput = init(tInput);
    tInput = headers(tInput);
    tInput = paragraph(tInput);
    tInput = Bold(tInput);
    cInput = move(tInput);
}

md2Html::~md2Html() {
    for (const auto& line : cInput) {
        cout << line << endl;
    }
}

VS md2Html::headers(VS input) {
    VS tInput = move(input);
    const vector<regex> regexes = { regex("# (.+)"),regex("## (.+)"),regex("### (.+)"),regex("#### (.+)"),regex("##### (.+)"),regex("###### (.+)") };
    const vector<string> tagStart = { "<h1>", "<h2>", "<h3>", "<h4>", "<h5>", "<h6>" };
    const vector<string> tagEnd = { "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>" };

    for (size_t lineIndex = 0; lineIndex < tInput.size(); ++lineIndex) {
        for (size_t i = 0; i < regexes.size(); ++i) {
            smatch match;
            match._Resize(0);
            if (regex_match(tInput.at(lineIndex), match, regexes.at(i))) {
                if (match.size() > 1) {
                    string content = match._At(1).str();
                    tInput.at(lineIndex) = tagStart.at(i) + content + tagEnd.at(i);
                }
                break;
            }
        }
    }
    return tInput;
}

VS md2Html::init(VS input) {
    return headerInit(move(input));
}

VS md2Html::headerInit(VS input) {
    VS tInput = move(input);
    for (size_t i = 0; i < tInput.size(); ++i) {
        if (tInput.at(i).empty()) continue;

        bool header2 = all_of(tInput.at(i).begin(), tInput.at(i).end(), [](char ch) { return ch == '-'; });
        if (header2 && i > 0 && tInput.at(i).size() >= 2) {
            if (!tInput.at(i - 1).empty()) {
                tInput.at(i - 1) = "## " + tInput.at(i - 1);
                tInput.erase(tInput.begin() + i);
                --i;
                continue;
            }
            continue;
        }

        bool header1 = all_of(tInput[i].begin(), tInput[i].end(), [](char ch) { return ch == '='; });
        if (header1 && i > 0) {
            tInput.at(i - 1) = "# " + tInput.at(i - 1);
            tInput.erase(tInput.begin() + i);
            --i;
        }
    }
    return tInput;
}

VS md2Html::paragraph(VS input) {
    VS tInput;
    string currentParagraph;
    regex headingRegex("<h[1-6]>.*</h[1-6]>");

    for (size_t i = 0; i < input.size(); ++i) {
        if (regex_search(input.at(i), headingRegex)) {
            // Line contains <h1> to <h6> tags, add it directly to output
            tInput.push_back(input.at(i));
            continue;
        }
        if (input.at(i).empty()) {
            if (!currentParagraph.empty()) {
                // Close current paragraph
                currentParagraph += "</p>";
                tInput.push_back(currentParagraph);
                currentParagraph.clear();
            }
        }
        else {
            if (currentParagraph.empty()) {
                // Start a new paragraph
                currentParagraph = "<p>" + input[i];
            }
            else {
                // Continue the current paragraph with a line break
                currentParagraph += "<br>" + input[i];
            }
        }
    }
    // Add the last paragraph if not empty
    if (!currentParagraph.empty()) {
        currentParagraph += "</p>";
        tInput.push_back(currentParagraph);
    }
    return tInput;
}

VS md2Html::Bold(VS input) {
    VS tInput = move(input);
    const vector<regex> regexes = { regex("\\*\\*(.+?)\\*\\*"), regex("__(.+?)__") };
    const string tagStart = "<strong>";
    const string tagEnd = "</strong>";

    for (size_t i = 0; i < tInput.size(); ++i) {
        for (size_t j = 0; j < regexes.size(); ++j) {
            smatch match;
            if (regex_search(tInput.at(i), match, regexes.at(i))) {
                if (match.size() > 1) {
                    string content = match[1].str();
                    string replacement = tagStart + content + tagEnd;
                    tInput.at(i).replace(match.position(0), match.length(0), replacement);
                }
            }
        }
    }
    return tInput;
}
