#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


using namespace std;

//Replacing substring with other substring in a string
std::string replaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

//Replacing substring with other substring in a string a partir de segunda caractere
std::string replaceSoundexString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 1;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

string toUpper(string &value)
{
    string str=value;
    for(int i=0;i<str.length();i++)
        str[i]=toupper(str[i]);
    return str;
}

// -> Preenche com 0 nos casos menor que quatro,
//      ou pega os 4 primeiros caracteres.
string fixLength(string &result)
{
    int length = result.length(),n=4-length,i=0;
    if(length<4)
        while (i < n){
            result.append("0");
            i++;
        }
    else
        result.resize(4);
    return result;
}

// -> Algoritmo de substituição padrão Soudex.
string getSoundexDigit(char item)
{
    string s1="BFPV",s2="CGJKQSXZ",s3="DT",s4="L",s5="MN",s6="R";
    if(s1.find(item,0)!=s1.npos)
        return "1";
    else
        if(s2.find(item,0)!=s2.npos)
            return "2";
        else
            if(s3.find(item,0)!=s3.npos)
                return "3";
            else
                if(s4.find(item,0)!=s4.npos)
                    return "4";
                else
                    if(s5.find(item,0)!=s5.npos)
                        return "5";
                    else
                        if(s6.find(item,0)!=s6.npos)
                            return "6";
                        else
                            return "0";
 }

// -> Adiciona o caractere soudex, cuidando para não repetir.
void addCharacter(string &result, char item)
{
    if (result.length() == 0)
        result.append(1,item);
    else
    {
        string code = getSoundexDigit(item);
        if (code[0]!=result[result.length()- 1])
            result=result.append(code);
    }
}

// -> Obtem o soudex de uma string
string getSoundex(string &value)
{
    string result="";
    string val=value;
    for(int i=0;i<val.length();i++)
        if(isalpha(val[i])){
            addCharacter(result,val[i]);
        }
    fixLength(result);
    value=result;
    return result;
}

string getSoundexBR(string value)
{
    string str=value;
    str=toUpper(str);
    str=replaceSoundexString(str,"LL","L");
    str=replaceSoundexString(str,"LH","L");
    str=replaceSoundexString(str,"AL","W");
    str=replaceSoundexString(str,"AU","W");
    str=replaceSoundexString(str,"PH","F");
    //str=replaceSoundexString(str,"C","K");
    str=replaceSoundexString(str,"ç","S");
    str=replaceSoundexString(str,"Ç","S");
    str=replaceSoundexString(str,"W","V");
    //str=replaceSoundexString(str,"C","K");
    //str=replaceSoundexString(str,"L","W");
    str=replaceSoundexString(str,"Z","S");
    str=replaceSoundexString(str,"á","");
    str=replaceSoundexString(str,"à","");
    str=replaceSoundexString(str,"ã","");
    str=replaceSoundexString(str,"â","");
    str=replaceSoundexString(str,"ä","");
    str=replaceSoundexString(str,"é","");
    str=replaceSoundexString(str,"è","");
    str=replaceSoundexString(str,"ê","");
    str=replaceSoundexString(str,"ë","");
    str=replaceSoundexString(str,"í","");
    str=replaceSoundexString(str,"ì","");
    str=replaceSoundexString(str,"î","");
    str=replaceSoundexString(str,"ï","");
    str=replaceSoundexString(str,"ó","");
    str=replaceSoundexString(str,"ò","");
    str=replaceSoundexString(str,"ö","");
    str=replaceSoundexString(str,"õ","");
    str=replaceSoundexString(str,"ô","");
    str=replaceSoundexString(str,"A","");
    str=replaceSoundexString(str,"E","");
    str=replaceSoundexString(str,"I","");
    str=replaceSoundexString(str,"O","");
    str=replaceSoundexString(str,"U","");
    str=replaceSoundexString(str,"Ú","");
    str=replaceSoundexString(str,"Ù","");
    str=replaceSoundexString(str,"Ü","");
    str=replaceSoundexString(str,"Û","");
    str=replaceSoundexString(str,"Á","");
    str=replaceSoundexString(str,"À","");
    str=replaceSoundexString(str,"Ã","");
    str=replaceSoundexString(str,"Â","");
    str=replaceSoundexString(str,"Ä","");
    str=replaceSoundexString(str,"É","");
    str=replaceSoundexString(str,"È","");
    str=replaceSoundexString(str,"Ê","");
    str=replaceSoundexString(str,"Ë","");
    str=replaceSoundexString(str,"Í","");
    str=replaceSoundexString(str,"Ì","");
    str=replaceSoundexString(str,"Î","");
    str=replaceSoundexString(str,"Ï","");
    str=replaceSoundexString(str,"Ó","");
    str=replaceSoundexString(str,"Ò","");
    str=replaceSoundexString(str,"Ö","");
    str=replaceSoundexString(str,"Õ","");
    str=replaceSoundexString(str,"Ô","");
    str=replaceSoundexString(str,"ú","");
    str=replaceSoundexString(str,"ù","");
    str=replaceSoundexString(str,"ü","");
    str=replaceSoundexString(str,"û","");
    str=replaceSoundexString(str,"Y","");
    str=replaceSoundexString(str,"y","");
    str=replaceSoundexString(str,".","");
    str=replaceSoundexString(str,"-","");
    str=replaceSoundexString(str,"0","");
    str=replaceSoundexString(str,"1","");
    str=replaceSoundexString(str,"2","");
    str=replaceSoundexString(str,"3","");
    str=replaceSoundexString(str,"4","");
    str=replaceSoundexString(str,"5","");
    str=replaceSoundexString(str,"6","");
    str=replaceSoundexString(str,"7","");
    str=replaceSoundexString(str,"8","");
    str=replaceSoundexString(str,"9","");
//    cout<<str<<endl;
    str=getSoundex(str);
    return str;
}
