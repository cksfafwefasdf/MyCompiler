#ifndef MYCOMPAILER_SYNPARSER_H
#define MYCOMPAILER_SYNPARSER_H
#include <string>
#include <map>
#include <vector>

class SynParser {
public:
    void readGrammar(std::string path);
    virtual void process(){};
    virtual void showTable(){};
    virtual void parseCode(const std::vector<std::string> &readBuffer){};
// 所有与产生式有关的集合中 2i都是左部，2i+1是右部 （i从0计数） 集合中产生式的个数是size/2
protected:
    const std::string EMPTY_STRING = "ept";
    std::string originalG; // 初始的文法串
    std::vector<std::string> extendGV; // 拓广文法串,此vector中，奇数是产生式的左部，偶数是产生式的右部
    std::vector<std::string> items;
    std::vector<std::vector<std::string>> closures; //所有的项目集闭包,项目集规范组
    std::map<int,std::map<std::string,int>> status; //在 i 号状态下遇到token符号要跳转到那个状态
    std::map<std::string,int> transfer; //检查闭包是否重复
    std::map<std::string,std::vector<std::string>> firstSet;
    std::map<std::string,std::vector<std::string>> followSet;
    std::map<int,std::map<std::string,std::string>> actionTable;
    std::map<int,std::map<std::string,std::string>> gotoTable;
    std::vector<std::string> vChSet; //非终结符集合
    std::vector<std::string> tChSet; //终结符集合

    std::vector<std::string> gotoNext(int from_closure,const std::string& check); //转移到下一个项目集闭包,返回值是闭包的编号
    void extendV(); // 生成拓广文法
    void getItems(); // 根据拓广文法生成项目
    void computeClosure();
    std::vector<std::string> closure(const std::vector<std::string> &iSet);
    void getFirst();
    void getFollow();
};


#endif //MYCOMPAILER_SYNPARSER_H
