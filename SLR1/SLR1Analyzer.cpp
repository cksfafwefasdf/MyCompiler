#include "SLR1Analyzer.h"
#include "iostream"
#include <algorithm>

using namespace std;

//小写字符串表示一个终结符，此函数用于判断一个字符是否是小写字符
bool is_Character(char c){return (c>='a'&&c<='z');}
//是否是非终结符
bool is_VType(char c){return (c>='A'&&c<='Z');}

int readDigit(string token){
    string res;
    int i=0;
    for(i=0;i<token.size();i++){
        if(token[i]>=0&&token[i]<='9'){
            break;
        }
    }
    while(i<token.size()&&token[i]>=0&&token[i]<='9'){
        res+=token[i];
        i++;
    }
    //cout<<"res is -------"<<res<<endl;
    return stoi(res);
}

int checkLength(string token){
    int len=0;
    int i =0;
    while(i<token.size()){
        if(is_Character(token[i])){
            while(is_Character(token[i])){
                i++;
            }
            len++;
            continue;
        }else{
            i++;
            len++;
        }
    }
    return len;
}

void SLR1Analyzer::process(){
    extendV();
    getItems();
    computeClosure();
    getFirst();
    getFollow();
    getTable();
}

void SLR1Analyzer::parseCode(const vector<string> &readBuffer){
    vector<string> status_stack;
    vector<string> token_stack;
    vector<int> error_rows;
    token_stack.push_back("$");
    status_stack.push_back("s0");
    int ip=0;
    //遇到换行符直接跳过，换行符用于定位错误
    for(string item:readBuffer){
        cout<<"item--------"<<item<<endl;
    }
    while (!status_stack.empty()){
        cout<<"status stack :{ ";
        for(auto item:status_stack){
            cout<<item<<" ";
        }
        cout<<"}"<<endl;
        cout<<"token stack :{ ";
        for(auto item:token_stack){
            cout<<item<<" ";
        }
        cout<<"}"<<endl;

        if(ip>=readBuffer.size()){
            break;
        }
        string token = readBuffer[ip];
        if(token=="linefeed"){
            ip++;
            continue;
        }

        //if(status_stack.top())
        string action = actionTable[readDigit(status_stack.back())][token];
        cout<<"token is "<<token<<" ---action is "<<action<<endl;
        if(action[0]=='s'){
            //移进操作
           token_stack.push_back(token);
           status_stack.push_back(action);
           ip++;
        }else if(action[0]=='r'){
            //用第num个产生式规约
            int num = readDigit(action);
            //查看产生式右部符号个数
            //第0个产生是拓广文法的产生式，直接跳过他
            int len = checkLength(extendGV[2*(num)+1]);
            //cout<<"len "<<"--------"<<len<<endl;
            for(int i=0;i<len;i++){
                token_stack.pop_back();
                status_stack.pop_back();
            }
            //将产生式左部放入符号栈
            token_stack.push_back(extendGV[2*(num)]);
            string go = gotoTable[readDigit(status_stack.back())][token_stack.back()];
            //cout<<"gototable-----"<<readDigit(status_stack.back())<<token_stack.back()<<endl;
            status_stack.push_back("s"+go);
            cout<<"use "<<extendGV[2*(num)]<<"->"<<extendGV[2*(num)+1]<<endl;
        }else if(action=="acc"){
            cout<<"access!"<<endl;
            break;
        }else{
            int err_row = errorHandle(ip,readBuffer);
            //如果处理两次还不能把这个问题解决了，说明他是个难以解决的问题，终止分析
            if(std::count(error_rows.begin(), error_rows.end(),err_row)>1){
                error_rows.pop_back(); //把多余的那个弹出去
                break;
            }
            error_rows.push_back(err_row);
        }
    }

    for(auto item:error_rows){
        cerr<<"err in "<<item<<" row!"<<endl;
    }

}

int SLR1Analyzer::errorHandle(int &ip,const vector<string> &readBuffer) {
    int linefeed_pos=0,row_num=0;
    for(linefeed_pos=ip;linefeed_pos<readBuffer.size();linefeed_pos++){
        if(readBuffer[linefeed_pos]=="linefeed"){
            break;
        }
    }
    for(int i=0;i<linefeed_pos;i++){
        if(readBuffer[i]=="linefeed"){
            row_num++;
        }
    }
    row_num++;
    ip++;
    //cerr<<"err in "<<row_num<<" row!"<<endl;
    return row_num;
}

void SLR1Analyzer::getTable() {
    for(int closure_num=0;closure_num<closures.size();closure_num++){
        for(int item_num=0;item_num<closures[closure_num].size()/2;item_num++){
            //第0个产生式是X->.P 第一个才是X->P.
            //cout<<"---"<<closures[closure_num][item_num*2+1]<<endl;
            if(closures[closure_num][item_num*2+1]==items[1*2+1]){
                actionTable[closure_num]["$"]="acc";
                continue;
            }
            int dot = closures[closure_num][item_num*2+1].find('.');
            string right_part = closures[closure_num][item_num*2+1];
            if(dot+1<right_part.size()){
                //如果不是最后一个字符
                int start = dot+1;
                if(is_VType(right_part[start])){
                    string token = string (1,right_part[start]);
                    //如果是非终结符
                    int go = status[closure_num][token];
                    gotoTable[closure_num][token]= to_string(go);

                }else{
                    //如果是终结符
                    string token;
                    if(is_Character(right_part[start])){
                        while(start<right_part.size()&&is_Character(right_part[start])){
                            token+=right_part[start];
                            start++;
                        }
                    }else{
                        token+=right_part[start];
                    }
                    int go = status[closure_num][token];
                    actionTable[closure_num][token]="s"+ to_string(go);
                }
            }else{
                right_part.erase(dot);
                int search=0;
                for(search=0;search<extendGV.size()/2;search++){
                    if(extendGV[search*2+1]==right_part){
                        break;
                    }
                }
                for(auto item:followSet[extendGV[search*2]]){
                    actionTable[closure_num][item]="r"+ to_string(search);
                }
            }
        }
    }
}


void SLR1Analyzer::showTable() {

    cout<<"-----------first---------"<<endl;
    for(auto item : firstSet){
        cout<<item.first<<": ";
        for(auto it : item.second){
            cout<<it<<" ";
        }
        cout<<endl;
    }
    cout<<"-----------first---------"<<endl;

    cout<<"---------- follow------"<<endl;
    for(auto item : followSet){
        cout<<item.first<<": ";
        for(auto it : item.second){
            cout<<it<<" ";
        }
        cout<<endl;
    }
    cout<<"--------- follow------"<<endl;

    cout<<"------------- I ----------"<<endl;
    for(int i=0;i<closures.size();i++){
        cout<<"<I is "<<i<<">"<<endl;
        for(int j=0;j<closures[i].size()/2;j++){
            cout<<closures[i][j*2]<<"->"<<closures[i][j*2+1]<<endl;
        }
        cout<<"</I is "<<i<<" >"<<endl;
    }
    cout<<"------------- I ----------"<<endl;

    cout<<"------------action--------"<<endl;
    for(auto item:actionTable){
        cout<<"|status: "<<item.first<<endl<<"| ";
        for(auto item1:item.second){
            cout<<"read "<<item1.first<<" do "<<item1.second<<" | ";
        }
        cout<<endl;
    }
    cout<<"------------action--------"<<endl;

    cout<<"------------goto--------"<<endl;
    for(auto item:gotoTable){
        cout<<"| status: "<<item.first<<endl<<"| ";
        for(auto item1:item.second){
            cout<<"read "<<item1.first<<" goto "<<item1.second<<" | ";
        }
        cout<<endl;
    }
    cout<<"------------goto--------"<<endl;
}