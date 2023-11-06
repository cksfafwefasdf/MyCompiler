#ifndef MYCOMPAILER_SLR1ANALYZER_H
#define MYCOMPAILER_SLR1ANALYZER_H
#include "SynParser.h"
class SLR1Analyzer: public SynParser{
public:
    void process() override;
    void getTable();
    void showTable() override;
    void parseCode(const std::vector<std::string> &readBuffer) override;
    int errorHandle(int &ip,const std::vector<std::string> &readBuffer);
};

#endif //MYCOMPAILER_SLR1ANALYZER_H
