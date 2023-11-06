//
// Created by cksfa on 2023/10/18.
//
#include <map>
#ifndef MYCOMPAILER_KEYWORD_H
#define MYCOMPAILER_KEYWORD_H
// 定义种别码
const int WHITESPACE =-3; //空白字符
const int ERR = -2; // 错误码
const int END = -1; // # 作为一个变量的结束符号
const int RESUME = 0; // 继续分析
const int MAIN = 1;
const int INT = 2;
const int CHAR = 3;
const int IF = 4;
const int ELSE = 5;
const int FOR = 6;
const int WHILE = 7;
const int RETURN = 8;
const int VOID = 9;
const int STRING = 10;
const int ASSIGN = 11; // =
const int PLUS = 12; // +
const int SUB = 13; // -
const int MUL = 14; // *
const int DIVISION = 15; // /
const int LEFT_BRACKET = 16; // (
const int RIGHT_BRACKET = 17; // )
const int LEFT_SQUARE_BRACKET = 18; // [
const int RIGHT_SQUARE_BRACKET = 19; // ]
const int LEFT_BRACE = 20; // {
const int RIGHT_BRACE = 21; // }
const int COMMA = 22; // ,
const int COLON = 23; // :
const int SEMICOLON = 24; // ;
const int GT = 25; // >
const int LT = 26; //<
const int GE = 27; //>=
const int LE= 28; //<=
const int EQ = 29; // ==
const int NEQ = 30; // !=
const int VAR = 31; // 变量标识符
const int NUM = 32; // 数字
const int QUOTE = 33; // 引号
const int LINEFEED = 34; //换行
const int INPUTSTREAM = 35;
const int OUTPUTSTREAM = 36;
const int INTEGER = 37;
const int FLOAT = 38;
const int ESC = 39; //转义字符
const int INC = 40;
const int DEC = 41;
const int TRUE = 42;
const int FALSE = 43;


const std::map<int,std::string> alphaBet={{WHITESPACE,"WHITESPACE"},
                                    {TRUE,"true"},
                                    {LINEFEED,"linefeed"},
                                    {FALSE,"false"},
                                    {ERR,"ERR"},
                                    {END,"$"},
                                    {RESUME,"RESUME"},
                                   // {MAIN,"main"},
                                    {MAIN,"id"},
                                    {INT,"int"},
                                    //{CHAR,"CHAR"},
                                    {IF,"if"},
                                    {ELSE,"else"},
                                    //{FOR,"FOR"},
                                    {WHILE,"while"},
                                    //{RETURN,"return"},
                                    //{VOID,"VOID"},
                                    //{STRING,"STRING"},
                                    {ASSIGN,"="},
                                    {PLUS,"+"},
                                    {SUB,"-"},
                                    {MUL,"*"},
                                    {DIVISION,"/"},
                                    {LEFT_BRACKET,"("},
                                    {RIGHT_BRACKET,")"},
                                    {LEFT_SQUARE_BRACKET,"["},
                                    {RIGHT_SQUARE_BRACKET,"]"},
                                    {LEFT_BRACE,"{"},
                                    {RIGHT_BRACE,"}"},
                                    {COMMA,","},
                                    {COLON,":"},
                                    {SEMICOLON,";"},
                                    {GT,"gt"},
                                    {LT,"lt"},
                                    {GE,"ge"},
                                    {LE,"le"},
                                    {EQ,"eq"},
                                    {NEQ,"neq"},
                                    {VAR,"id"},
                                    //{NUM,"NUMBER"},
                                    //{QUOTE,"QUOTE"},
                                    //{COUT,"COUT"},
                                    //{INPUTSTREAM,"INPUTSTREAM"},
                                    //{OUTPUTSTREAM,"OUTPUTSTREAM"},
                                    {FLOAT,"float"},
                                    {INTEGER,"digit"},
                                    //{ESC,"ESC"},
                                    //{INC,"INC"},
                                    //{DEC,"DEC"},
                                    };

#endif //MYCOMPAILER_KEYWORD_H
