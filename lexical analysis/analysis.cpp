//
// Created by cksfa on 2023/10/18.
//
#include <fstream>
#include "analysis.h"
using namespace std;
// + ++ +++ - -- --- === >> << < > >>> <<< != = ==#
void Analyzer::readText()
{
    char buffer[2048];
    string buffer_str;
    do {
        cin.getline(buffer,2048);
        buffer_str = buffer;
        this->stringHold+=buffer_str+'\n';
    }while(buffer_str[buffer_str.size()-1]!='#');
}

bool Analyzer::isChar(char c) {
    return (c>='a'&&c<='z')||(c>='A'&&c<='Z');
}

bool Analyzer::isDigit(char c) {
    return (c>='0'&&c<='9');
}

int Analyzer::checkAlphabet(string token) {
    //直接检查最后一个字符
    char to = token[token.size()-1];
    switch (to) {
        case '+':
            if(token=="++")
                return INC;
            else if(token=="+")
                return PLUS;
            break;
        case '-':
            if(token=="--")
                return DEC;
            else if(token=="-")
                return SUB;
            break;
        case '=':
            if(token==">=")
                return GE;
            else if(token=="<=")
                return LE;
            else if(token=="==")
                return EQ;
            else if(token=="!=")
                return NEQ;
            else if(token=="=")
                return ASSIGN;
            break;
        case ';':
            return SEMICOLON;
        case ',':
            return COMMA;
        case '(':
            return LEFT_BRACKET;
        case ')':
            return RIGHT_BRACKET;
        case '{':
            return LEFT_BRACE;
        case '}':
            return RIGHT_BRACE;
        case '>':
            if(token==">>")
                return OUTPUTSTREAM;
            else if(token==">")
                return GT;
            break;
        case '<':
            if(token=="<<")
                return INPUTSTREAM;
            else if(token=="<")
                return LT;
            break;
        case '\"':
            return STRING;
        case '#':
            return END;
        // \0和\n和空格都是空白字符！！
        case ' ':
        case '\n':
        case '\0':
            return WHITESPACE;
        default:
            if(isChar(to)){
                if(token=="int")
                    return INT;
                else if(token=="char")
                    return CHAR;
                else if(token=="while")
                    return WHILE;
                else if(token=="if")
                    return IF;
                else if(token=="else")
                    return ELSE;
                else if(token=="main")
                    return MAIN;
                else if(token=="return")
                    return RETURN;
                else if(token=="false")
                    return FALSE;
                else if(token=="true")
                    return TRUE;
                else if(token.size()-2>=0&&token[token.size()-2]=='\\')
                    return ESC;
                else
                    return VAR;
            }else if(isDigit(to)){
                int dotNum=0; //一个数字最多只能有一个小数点
                for(int i=0;i<token.size();i++){
                    if(isChar(token[i]))
                        return VAR;
                    if(token[i]=='.')
                        dotNum++;
                }
                if(dotNum==0)
                    return INTEGER;
                else if(dotNum==1)
                    return FLOAT;
            }
            return ERR;
    }
    return ERR;
}

void Analyzer::analyze() {
    string token="";
    int index = 0;
    while(index<this->stringHold.size())
    {
        string token="";
        if(this->stringHold[index]==' ')
            index++;
        if(this->stringHold[index]=='\n')
            this->res.push_back({"\n",LINEFEED});
        if(isChar(this->stringHold[index])){
            while(isChar(this->stringHold[index])|| isDigit(this->stringHold[index])){
                token = token+this->stringHold[index];
                index++;
            }
            int sync = checkAlphabet(token);
            if(sync!=WHITESPACE)
                this->res.push_back({token,sync});
            token="";
        }else if(isDigit(this->stringHold[index])){
            while(isDigit(this->stringHold[index])||this->stringHold[index]=='.'){
                token = token+this->stringHold[index];
                index++;
            }
            int sync = checkAlphabet(token);
            if(sync!=WHITESPACE)
                this->res.push_back({token,sync});
            token="";
        }else if(this->stringHold[index]=='='||this->stringHold[index]=='<'||this->stringHold[index]=='>'||this->stringHold[index]=='!'){
            token += this->stringHold[index];
            while(index+1<this->stringHold.size()&&(this->stringHold[index+1]=='='||this->stringHold[index+1]=='>'||this->stringHold[index+1]=='<')){
                index++;
                token=token+stringHold[index];
            }
            int sync=checkAlphabet(token);
            if(sync!=WHITESPACE)
                this->res.push_back({token,sync});
            token="";
            index++;
        }else if(this->stringHold[index]=='\"'){
            do{
               token+=this->stringHold[index];
               index++;
            }while(this->stringHold[index]!='\"');
            token+=this->stringHold[index]; //把最后那个引号加上
            index++;
            int sync = checkAlphabet(token);
            if(sync!=ERR&&sync!=WHITESPACE)
                this->res.push_back({token,sync});
            token="";
        }else if(this->stringHold[index]=='\\'){
            token+=this->stringHold[index];
            index++;
            token+=this->stringHold[index];
            int sync=checkAlphabet(token);
            this->res.push_back({token,sync});
            index++;
        }else if(this->stringHold[index]=='+'){
            token+=this->stringHold[index];
            index++;
            while(this->stringHold[index]=='+'){
                token+=this->stringHold[index];
                index++;
            }
            int sync=checkAlphabet(token);
            this->res.push_back({token,sync});
        }else if(this->stringHold[index]=='-'){
            token+=this->stringHold[index];
            index++;
            while(this->stringHold[index]=='-'){
                token+=this->stringHold[index];
                index++;
            }
            int sync=checkAlphabet(token);
            this->res.push_back({token,sync});
        }else{
            token+=this->stringHold[index];
            int sync=checkAlphabet(token);
            if(sync!=WHITESPACE)
                this->res.push_back({token,sync});
            index++;
        }
    }
    for(pair<string,int> item:this->res){
        tokens.push_back(alphaBet.at(item.second));
    }
}

void Analyzer::printResult(){
    if(this->stringHold.size()>0){
        for(pair<string,int> item:this->res){
            cout<<"( "<< item.first <<" , "<<alphaBet.at(item.second)<<" )"<<endl;
        }
    }else{
        cout<<"err in print result!"<<endl;
    }
}

void Analyzer::readTextFromFile(string path){
    fstream fs;
    fs.open(path);
    char buffer[2048];
    string buffer_str;
    do {
        fs.getline(buffer,2048);
        buffer_str = buffer;
        this->stringHold+=buffer_str+'\n';
    }while(buffer_str[buffer_str.size()-1]!='#');
}

