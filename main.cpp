#include "lexical analysis/analysis.h"
#include "SLR1/SLR1Analyzer.h"
#include <iostream>
using namespace std;

const string grammar_path="../grammar/test5.txt";
const string code_path="../testcode/test1.txt";

int main() {
    Analyzer an;
    an.readTextFromFile(code_path);
    an.analyze();
    SynParser* s1 = new SLR1Analyzer();
    s1->readGrammar(grammar_path);
    s1->process();
    s1->showTable();
    s1->parseCode(an.tokens);
    return 0;
}
