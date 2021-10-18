#include <stdio.h>
#include <string>
#include <iostream>
#include <time.h>

using namespace std;

class tempo
{
private:
    string horario;
public:
    tempo(string hora);
    bool entre(string h1,string h2);
};

bool tempo::entre(string h1,string h2){
    int x,y;
    sscanf(h1.c_str(),"%d:%d",&x,&y);
    int v1=x*60+y;
    sscanf(h2.c_str(),"%d:%d",&x,&y);
    int v2=x*60+y;
    sscanf(horario.c_str(),"%d:%d",&x,&y);
    int v3=x*60+y;
    return (v3<=v2 && v3>=v1);
}

tempo::tempo(string hora){
    horario=hora;
}
