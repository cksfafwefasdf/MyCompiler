#ifndef MYCOMPAILER_SLR1ANALYZER_H
#define MYCOMPAILER_SLR1ANALYZER_H
#include "SynParser.h"
class SLR1Analyzer: public SynParser{
public:
    void process() override;
    void getTable();
    void showTable() override;
    void parseCode(const std::vector<std::string> &readBuffer) override;
    std::pair<int,std::string>  errorHandle(int &ip,const std::vector<std::string> &readBuffer,const std::vector<std::string> &status_stack);
};

#endif //MYCOMPAILER_SLR1ANALYZER_H
