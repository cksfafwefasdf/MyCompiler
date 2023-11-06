#include "SynParser.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

//小写字符串表示一个终结符，此函数用于判断一个字符是否是小写字符
bool isCharacter(char c){return (c>='a'&&c<='z');}
//是否是非终结符
bool isVType(char c){return (c>='A'&&c<='Z');}

void SynParser::getFirst() {
    for(int i=0;i<extendGV.size()/2;i++){
        //初始化first集合
        firstSet[extendGV[2*i]]=vector<string>();
    }
    map<string,map<string,bool>> isInSet; // 用于标识一个元素是否在某个集合中,由于右部是唯一的，所以用右部索引左部
    bool is_change; //记录当前遍历是否使得集合发送变化
    do {
        is_change= false;
        //查看每一个产生式的右部
        for(int i=0;i<extendGV.size()/2;i++){
            int j=0;
            while(j<extendGV[2*i+1].size()){

                string token;
                //如果是一个类似于id这样的小写字母终结符
                if(isCharacter(extendGV[2*i+1][j])){
                    while(isCharacter(extendGV[2*i+1][j])){
                        token+=extendGV[2*i+1][j];
                        j++;
                    }
                    //如果这个token在集合中的个数是0（在集合中不存在），那么就加进去
                    if(!isInSet[token][extendGV[2*i]]){
                        firstSet[extendGV[2*i]].push_back(token);
                        isInSet[token][extendGV[2*i]]= true;
                        is_change= true;
                    }
                }else if(extendGV[2*i+1][j]<='Z'&&extendGV[2*i+1][j]>='A'){
                    //如果首字符是非终结符，那么就进行继承
                    //此时的token是一个大写字符
                    token+=extendGV[2*i+1][j];
                    j++;
                    for(auto item:firstSet[token]){
                        if(!isInSet[item][extendGV[2*i]]){
                            firstSet[extendGV[2*i]].push_back(item);
                            is_change= true;
                            isInSet[item][extendGV[2*i]]= true;
                            //如果说当前的是个空串，那么就要考虑空串后面紧随的那个字符是否是first了
                            if(item=="ept"){
                                for(int eptc=0;eptc<extendGV.size()/2;eptc++){

                                    //其实此处的token就是一个非终结符，只有一个字母,看看这个token是作为哪个右部的前缀
                                    //如果token是某个产生式的前缀，且token会成空，那么紧邻他的后一个字符的first也在其父first中
                                    int start =1;
                                    if(extendGV[eptc*2+1][0]==token[0]&&start<extendGV[eptc*2+1].size()){
                                        string right_token;
                                        do{
                                            right_token+=extendGV[eptc*2+1][start];
                                            start++;
                                        } while (start<extendGV[eptc*2+1].size()&&isCharacter(extendGV[eptc*2+1][start]));
                                        if(right_token[0]>='A'&&right_token[0]<='Z'){
                                            for(auto item1 : firstSet[right_token]){
                                                if(!isInSet[item1][extendGV[2*i]]){
                                                    firstSet[extendGV[2*i]].push_back(item1);
                                                    isInSet[item1][extendGV[2*i]]= true;
                                                }
                                            }
                                        }else{
                                            if(!isInSet[right_token][extendGV[2*i]]){
                                                firstSet[extendGV[2*i]].push_back(right_token);
                                                isInSet[right_token][extendGV[2*i]]= true;
                                            }
                                        }
                                    }
                                }
                            }

                        }
                    }
                }else{
                    //如果是其他非大写字符（大写字符是非终结符）
                    token+=extendGV[2*i+1][j];
                    //cout<<"1111"<<isInSet[token][extendGV[2*i]]<<endl;
                    if(!isInSet[token][extendGV[2*i]]){
                        firstSet[extendGV[2*i]].push_back(token);
                        isInSet[token][extendGV[2*i]]= true;
                        is_change= true;
                    }
                }
                break;
            }
        }
    } while (is_change);
}

void SynParser::getFollow() {
    //初始化follow
    for(int i=0;i<extendGV.size()/2;i++){
        followSet[extendGV[2*i]] = vector<string>();
    }
    //健壮性处理，如果没求first集的话就直接退出
    if(firstSet.empty()){
        return;
    }
    map<string,map<string,bool>> isInSet; //一个符号是否在这个集合中
    //先将结束符号加入到开始符号的follow集中
    followSet[extendGV[2*0]].push_back("$");
    isInSet["$"][extendGV[2*0]]= true;
    vector<string> vSet;
    bool is_change;

    do{
        is_change= false;
        for(int left=0;left<extendGV.size()/2;left++){
            for(int right=0;right<extendGV.size()/2;right++){
                int v_pos=-1;
                v_pos=extendGV[2*right+1].find(extendGV[left*2],v_pos+1);
                while(v_pos!=-1){
                    //如果不是最后一个字符
                    if(v_pos+1<extendGV[2*right+1].size()){
                        //如果是非终结符，那么就将后一个字符的first集合给前一个字符
                        if(isVType(extendGV[2*right+1][v_pos+1])){
                            for(auto item:firstSet[string (1,extendGV[2*right+1][v_pos+1])]){
                                if(item!=EMPTY_STRING&&!isInSet[item][string(1,extendGV[2*right+1][v_pos])]){
                                    isInSet[item][string(1,extendGV[2*right+1][v_pos])]= true;
                                    followSet[string(1,extendGV[2*right+1][v_pos])].push_back(item);
                                    is_change=true;
                                }
                            }
                            string token = string (1,extendGV[2*right+1][v_pos+1]);
                            //查看当前的token是否有可能会推出empty
                            for(int eptc=0;eptc<extendGV.size()/2;eptc++){
                                //如果当前字符会空，那么就把当前左部的元素也继承给这个ept左边的那个元素
                                //此外，还要求当前ept的前一个字符是一个非终结符
                                if(extendGV[2*eptc]==token&&extendGV[2*eptc+1]==EMPTY_STRING){
                                    for(auto item1 : followSet[extendGV[2*eptc]]){
                                        if(!isInSet[item1][token]&&item1!=EMPTY_STRING){
                                            followSet[token].push_back(item1);
                                            is_change = true;
                                            isInSet[item1][token]= true;
                                        }
                                    }
                                }
                            }
                        }else{
                            //如果是终结符
                            string token;
                            int start = v_pos+1;
                            if(isCharacter(extendGV[2*right+1][start])){
                                while(start<extendGV[2*right+1].size()&&isCharacter(extendGV[2*right+1][start])){
                                     token+=extendGV[2*right+1][start];
                                     start++;
                                }
                            }else{
                                token+=extendGV[2*right+1][start];
                            }

                            if(token!=EMPTY_STRING&&!isInSet[token][string(1,extendGV[2*right+1][v_pos])]){
                                isInSet[token][string(1,extendGV[2*right+1][v_pos])]= true;
                                followSet[string(1,extendGV[2*right+1][v_pos])].push_back(token);
                                is_change=true;
                            }
                        }
                    }else{
                        //如果是最后一个字符,且不与左部字符相同
                        if(extendGV[2*right+1][v_pos]!=extendGV[2*right][0]){
                            for(auto item:followSet[string(1,extendGV[2*right][0])]){
                                if(item!=EMPTY_STRING&&!isInSet[item][string(1,extendGV[2*right+1][v_pos])]){
                                    followSet[string (1,extendGV[2*right+1][v_pos])].push_back(item);
                                    isInSet[item][string(1,extendGV[2*right+1][v_pos])]= true;
                                    is_change= true;
                                }
                            }
                        }
                    }
                    v_pos=extendGV[2*right+1].find(extendGV[left*2],v_pos+1);
                }
            }

        }
    } while (is_change);

}

//若extendG是string类型的那么就用这个处理
// cout<<extendGV[2*i]<<"->"<<extendGV[2*i+1]<<endl; 2*i是左部 2*i+1是右部
//vector i是产生式左部，i+1是产生式右部，所以左部只存放在偶数位中 0 2 4... 所以第n个产生式的左部脚标是2n
//vector i是产生式左部，i+1是产生式右部，所以左部只存放在偶数位中
//vector i是产生式左部，i+1是产生式右部，所以左部只存放在偶数位中
void SynParser::extendV() {
    string leftPart,current;

    for(int i=0;i<originalG.size();i++){
        if(originalG[i]=='-')
            break;
        current+=originalG[i];
    }
    extendGV.push_back("X");
    extendGV.push_back(current);
    current="";
    for(int i=0;i<originalG.size();i++){
        //左箭头代表左部结束
        if(originalG[i]=='>'){
            current.erase(current.end()-1); //擦去末尾的-
            leftPart=current;
            current="";
            continue;
        }
        //originalG中不同的产生式使用空格分离的
        if(originalG[i]=='|'){
            //cout<<leftPart+current<<endl;
            extendGV.push_back(leftPart);
            extendGV.push_back(current);
            current="";
            continue;
        }
        if(originalG[i]==' '){
            //cout<<leftPart+current<<endl;
            extendGV.push_back(leftPart);
            extendGV.push_back(current);
            current="";
            leftPart="";
            continue;
        }

        current+=originalG[i];
        //cout<<current<<endl;
    }
    //最后一个字母后面没有空格，所以要push以下最后一个
    if(!(leftPart+current).empty()){
        extendGV.push_back(leftPart);
        extendGV.push_back(current);
    }
//    cout<<"----------1111111"<<endl;
    //选出非终结符和终结符
    for(int i=0;i<extendGV.size()/2;i++){
        if(std::count(vChSet.begin(), vChSet.end(),extendGV[2*i])==0){
            vChSet.push_back(extendGV[2*i]);
        }
    }
    for(int i=0;i<extendGV.size()/2;i++){
        for(int j=0;j<extendGV[i*2+1].size();j++){
            if(isVType(extendGV[i*2+1][j])){
                continue;
            }
            string token;
            if(isCharacter(extendGV[i*2+1][j])){
                while(isCharacter(extendGV[i*2+1][j])){
                    token+=extendGV[i*2+1][j];
                    j++;
                }
                j--;
            }else{
                token+=extendGV[i*2+1][j];
            }
            if(std::count(tChSet.begin(), tChSet.end(),token)==0){
                tChSet.push_back(token);
            }
        }
    }
//    for(auto item:vChSet){
//        cout<<item<<endl;
//    }
//    cout<<"----------1111111"<<endl;
//    for(auto item:tChSet){
//        cout<<item<<endl;
//    }
}

void SynParser::readGrammar(string path){
    fstream f;
    f.open(path,ios::in);
    char buffer;
    while (f.get(buffer)){
        if(buffer==' ') //去除空白字符
            continue;
        if(buffer=='\n'){
            originalG+=" ";
            continue;
        }
        originalG+=buffer;
    }
    //cout<<originalG<<endl;
    f.close();
}

void SynParser::getItems(){
    for(int i=0;i<extendGV.size()/2;i++){
        int j=0;
        //先插入第一个逗号
        do{

            string temp = extendGV[2*i+1];
            items.push_back(extendGV[2*i]);
            items.push_back(temp.insert(j,"."));
            if(isCharacter(extendGV[2*i+1][j])){
                while(isCharacter(extendGV[2*i+1][j]))
                    j++;
                j--;
            }
            j++;
            if(isCharacter(extendGV[2*i+1][j])){
                continue;
            }
        } while (j<=extendGV[2*i+1].size());
    }
//    for(int i=0;i<items.size()/2;i++){
//        cout<<items[2*i]<<"->"<<items[2*i+1]<<endl;
//    }
}


vector<string> SynParser::closure(const vector<string> &iSet){
    bool is_change;
    vector<string> closure=iSet;
    do{
        is_change= false;
        for(int item_num=0;item_num<closure.size()/2;item_num++){
            int dot = closure[item_num*2+1].find('.');
            if(dot+1<closure[item_num*2+1].size()&& isVType(closure[item_num*2+1][dot+1])){
                string token = string (1,closure[item_num*2+1][dot+1]);
                for(int production=0;production<extendGV.size()/2;production++){
                    if(extendGV[production*2]==token){
                        string newItem = extendGV[production*2+1];
                        //在首部插入dot
                        newItem.insert(0,".");
                        //cout<<"new --------- "<<newItem<<endl;
                        // if newItem is not in closure
                        if(std::count(closure.begin(), closure.end(),newItem)==0){
                            closure.push_back(extendGV[production*2]);
                            closure.push_back(newItem);
                            is_change= true;
                        }
                    }
                }

            }
        }
    } while (is_change);
    return closure;
}

vector<string> SynParser::gotoNext(int from_closure,const string& check){
    vector<string> ret;
    for(int item_num=0;item_num<closures[from_closure].size()/2;item_num++){
        int pos=-1;
        do{
            pos=closures[from_closure][item_num*2+1].find(check,pos+1);
            //cout<<closures[from_closure][item_num*2+1]<<" --find-- "<<check<<" "<<pos<<endl;
            if(pos!=-1&&closures[from_closure][item_num*2+1][pos-1]=='.'){
                string temp = closures[from_closure][item_num*2+1];
                temp.erase(pos-1,1);
                int temp_pos = pos-1;//因为擦去了一个dot，所以pos会向前动一下
                temp.insert(temp_pos+check.size(),".");
                ret.push_back(closures[from_closure][item_num*2]);
                ret.push_back(temp);
                //cout<<"temp-----------"<<closures[from_closure][item_num*2]<<"->"<<temp<<endl;
            }
        }while(pos!=-1);
    }
    vector<string> res = closure(ret);
    //cout<<"size---"<<res.size()<<endl;
    return res;
}

void SynParser::computeClosure() {
    //构造I0
    closures.push_back(vector<string>());
    closures[0].clear();
    for(int i=0;i<extendGV.size()/2;i++){
        string temp = extendGV[2*i+1];
        temp.insert(0,".");
        this->closures[0].push_back(extendGV[2*i]);
        this->closures[0].push_back(temp);
    }
    vector<string> charSet;
    for(auto item:vChSet){
        charSet.push_back(item);
    }
    for(auto item:tChSet){
        charSet.push_back(item);
    }
    bool is_change;
    do{
        is_change = false;
        for(int closure_num=0;closure_num<closures.size();closure_num++){
            for(auto str:charSet){
                vector<string> goto_i = gotoNext(closure_num,str);
                //如果goto_i不为空且不再closures中
                //填写status表
                if(!goto_i.empty()){
                    bool lock= true;
                    for(int i=0;i<closures.size();i++){
                        if (closures[i]==goto_i){
                            lock= false;
                            status[closure_num][str]=i;
                            break;
                        }
                    }
                    if(lock){
                        status[closure_num][str]=closures.size();
                        closures.push_back(goto_i);
                        is_change= true;
                    }
                }
            }
            //cout<<"closure_num ---------"<<closure_num<<endl;
        }
    } while (is_change);
}