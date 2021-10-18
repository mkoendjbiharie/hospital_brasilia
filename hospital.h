#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <utility>
//#include <ctime>
#include "date.h"
#include "soundex.h"
#include "tempo.h"

using namespace std;

struct prescricao_med{
    string cd_pre;
    string dt_pre;
    string cd_atd;
    string dt_prm;
    string cd_pro;
    string ds_pro;
    string cd_fat;
    string ds_uni;
    string ds_pre;
    string cd_fre;
    string ds_fre;
    string dh_reg;
    int ds_evo;
    int vezes;
    int freq;
};

struct evolucao_enf{
    string cd_evo;
    string cd_atd;
    string dt_evo;
    string ds_evo;
    string hr_evo;
    bool nao_existe_prescricao_dia_anterior;
    bool entre_0700_e_1600;
    bool entre_1900_e_2359;
};        

struct horario_evo{
    string cd_evo;
    string cd_atd;
    string dt_evo;
    string hr_evo;
    string hr_anot;
    string ds_evo;
    bool nao_existe_prescricao_dia_anterior;
    bool entre_0700_e_1600;
    bool entre_1900_e_2359;
};        

struct frequencia_pre{
    string cd_fre;
    string ds_fre;
    int nr_apl;
};        

struct movimento_farm{
    string cd_emp;
    string cd_mov;
    string tp_mov;
    string cd_pro;
    string cd_fat;
    string ds_fat;
    string cd_atd;
    string cd_pre;
    string cd_sol;
};        

struct registro{
    string codigo;
    string nome;
    string sinonimo;
    //string soundex;
};

struct sinonimo_med{
    string codigo;
    string nome;
    string sinonimo;
    vector<string> nome_alternativo;
    vector<string> nome_alternativo_out;
};

/*registro medicamento;
vector<registro> reg_medicamento;*/

prescricao_med prescricao;
vector<prescricao_med> reg_prescricao;

sinonimo_med sinonimo,medicamento;
vector<sinonimo_med> reg_sinonimo,reg_medicamento;

movimento_farm movimento;
vector<movimento_farm> reg_movimento;

frequencia_pre frequencia;
vector<frequencia_pre> reg_frequencia;

horario_evo horario;
vector <horario_evo> reg_horario;

evolucao_enf evolucao;
vector<evolucao_enf> reg_evolucao;

//Substituir um caractere por outro em um string
void replace_all(char s[], char chr, char repl_chr)
{
     int i=0;
     while(s[i]!='\0')
     {
           if(s[i]==chr)
           {
               s[i]=repl_chr;
           }  
           i++; 
     }
}
/*
//Substituir um caractere por outro em um string
//Nao permite substituir : se representa um horario no formato xx:xx
void replace_all(char s[], char chr, char repl_chr)
{
    int i=0;
    bool hora=false;
    while(s[i]!='\0'){
        if(s[i]==':'){
            hora=isdigit(s[i-2])&&isdigit(s[i-1])&&isdigit(s[i+1])&&isdigit(s[i+2]);
            if(!hora)
                s[i]=' ';
        }
        else
            if(s[i]==chr){
                s[i]=repl_chr;
            } 
        hora=false;
        i++; 
    }
}
*/

int tijd(string hora){
    int hour=atoi(hora.substr(1,2).c_str());
    int minute=atoi(hora.substr(4,2).c_str());
    int total=(hour*60)+minute;
    //cout<<hora.substr(0,2)<<": "<<hora.substr(3,2)<<endl;
    //cout<<hour<<"-"<<minute<<": "<<total<<endl;
    return (total);
}

//Ordenar prescricao por cd_atendimento, cd_pre e data
bool sortBy_cliente_data(const prescricao_med &a, const prescricao_med &b){
   if (a.cd_atd < b.cd_atd) return true;
   if (b.cd_atd < a.cd_atd) return false;

   if (a.dt_pre < b.dt_pre) return true;
   if (b.dt_pre < a.dt_pre) return false;
   
   if (a.cd_pre < b.cd_pre) return true;
   if (b.cd_pre < a.cd_pre) return false;
}

//Ordenar evolucao por cd_atendimento, cd_pre e data
bool sortEvoBy_cliente_data(const evolucao_enf &a, const evolucao_enf &b){
   if (a.cd_atd < b.cd_atd) return true;
   if (b.cd_atd < a.cd_atd) return false;

   if (a.dt_evo < b.dt_evo) return true;
   if (b.dt_evo < a.dt_evo) return false;
   }

//Ordenar reg_sinonimo por nome
bool sortBy_nome_medicamento(const sinonimo_med &a, const sinonimo_med &b){
   if (a.nome < b.nome) return true;
   if (b.nome < a.nome) return false;
}

//Ordenar horario por cd_atd, dt_evo e hr_evo
bool sortBy_cliente_horario(const horario_evo &a, const horario_evo &b){
   if (a.cd_atd < b.cd_atd) return true;
   if (b.cd_atd < a.cd_atd) return false;

   if (a.dt_evo < b.dt_evo) return true;
   if (b.dt_evo < a.dt_evo) return false;
  

   if (a.hr_evo < b.hr_evo) return true;
   if (b.hr_evo < a.hr_evo) return false;
}

static inline std::string trim(std::string &str){
    str.erase(0,str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ')+1);
    return str;
}

static inline std::string trim_newline(std::string &s){
    string str=s;
    if(!str.empty())
        if(str[str.length()-1]=='\n')
            str.erase(str.length()-1);
    return str;
}

void construa_vetor_evolucao(string str) {
    cout<<"Construindo vetor evolução..."<<endl;
    ifstream is(str.c_str());
    char c;
    char campo[20000]="";
    int ncampos=0;
    while(is.get(c)){          
        if(c=='|'){
            ncampos++;
            switch(ncampos){
                case 1:evolucao.cd_atd=campo;break;
                case 2:evolucao.cd_evo=campo;break;
                case 3:evolucao.dt_evo=campo;break;
                case 4:evolucao.hr_evo=campo;break;
                case 5:evolucao.ds_evo=campo;/*cout<<evolucao.cd_evo<<endl<<evolucao.ds_evo<<endl;*/reg_evolucao.push_back(evolucao);ncampos=0;break;
            }
            campo[0]='\0';
        }
        else{
            strncat(campo,&c,1);
        }
    }
    reg_evolucao.push_back(evolucao);
    is.close();                
    for(int i=0;i<reg_evolucao.size();i++){
        reg_evolucao[i].dt_evo=reg_evolucao[i].dt_evo.substr(0,10);
        replace_all((char*)reg_evolucao[i].dt_evo.c_str(),'.','/');
        replace_all((char*)reg_evolucao[i].ds_evo.c_str(),'/',' ');
        replace_all((char*)reg_evolucao[i].ds_evo.c_str(),'(',' ');
        replace_all((char*)reg_evolucao[i].ds_evo.c_str(),')',' ');
        replace_all((char*)reg_evolucao[i].ds_evo.c_str(),'.',' ');
        replace_all((char*)reg_evolucao[i].ds_evo.c_str(),',',' ');
        replace_all((char*)reg_evolucao[i].ds_evo.c_str(),'-',' ');
//        replace_all((char*)reg_evolucao[i].ds_evo.c_str(),':',' ');//esta linha tambem afeta horarios
        replace_all((char*)reg_evolucao[i].ds_evo.c_str(),';',' ');
        //reg_evolucao[i].ds_evo=trim(reg_evolucao[i].ds_evo);
        reg_evolucao[i].hr_evo=reg_evolucao[i].hr_evo.substr(10,6);
	//cout<<reg_evolucao[i].cd_evo<<endl<<reg_evolucao[i].ds_evo<<endl;
    }
    reg_evolucao.pop_back();
    cout<<"Número de evoluções:"<<reg_evolucao.size()<<endl;
    //para cada registro de evulucao
    for(int i=0;i<reg_evolucao.size();i++){
        bool entre_0700_e_1600=false,negou1=false;
        bool entre_1900_e_2359=false,negou2=false;
        int n=reg_evolucao[i].ds_evo.size(),j=0;
        cout<<reg_evolucao[i].ds_evo<<endl<<"--------------------------------------------------------"<<endl;
        //separar os registros de horario
        while(j<n){
            int pos=reg_evolucao[i].ds_evo.find_first_of(":",j);
            bool hora=(reg_evolucao[i].ds_evo[pos-3]=='\n')&&isdigit(reg_evolucao[i].ds_evo[pos-2])&&isdigit(reg_evolucao[i].ds_evo[pos-1])&&isdigit(reg_evolucao[i].ds_evo[pos+1])&&isdigit(reg_evolucao[i].ds_evo[pos+2]);
            //cout<<"teste 1..............."<<endl;
            //cout<<pos<<endl;
            //se encontrar um horario
            if(pos!=reg_evolucao[i].ds_evo.npos&&hora){
                horario.cd_atd=reg_evolucao[i].cd_atd;
                horario.dt_evo=reg_evolucao[i].dt_evo;
                horario.hr_evo=reg_evolucao[i].hr_evo;
                horario.cd_evo=reg_evolucao[i].cd_evo;
                horario.hr_anot=reg_evolucao[i].ds_evo.substr(pos-2,5);
                //Falta aqui tratar casos em que um horario se encontra no final do registro
                //um registro de horario se inicia a partir do primeiro espaco apos o horario
                //e termina antes do proximo horario
                int inicio=reg_evolucao[i].ds_evo.find_first_of(" ",pos);
                int posicoes=reg_evolucao[i].ds_evo.find_first_of(":",inicio+1)-inicio-2;
                hora=isdigit(reg_evolucao[i].ds_evo[posicoes-2])&&isdigit(reg_evolucao[i].ds_evo[posicoes-1])&&isdigit(reg_evolucao[i].ds_evo[posicoes+1])&&isdigit(reg_evolucao[i].ds_evo[posicoes+2]);
                horario.ds_evo=reg_evolucao[i].ds_evo.substr(inicio,posicoes);
//    cout<<"teste 4..............."<<endl;
                //cout<<horario.cd_atd<<endl;
/*                if(horario.hr_anot>="07:00" && horario.hr_anot <="16:00")//bool entre_0700_e_1600;
                    entre_0700_e_1600=true;
                else
                    negou1=true;
                if(horario.hr_anot>="19:00" && horario.hr_anot <="23:59")//bool entre_1900_e_2359;
                    entre_1900_e_2359=true;
                else
                    negou2=true;*/
                reg_horario.push_back(horario);
            }
            else
            //se nao encontrar um horario e nao chegou no final do registro de evolucao 
                if(pos!=reg_evolucao[i].ds_evo.npos&&!hora){
                    int inicio=reg_evolucao[i].ds_evo.find_first_of(":",pos);
                    int posicoes=reg_evolucao[i].ds_evo.find_first_of(":",inicio+1)-inicio-2;
                    int posicao=reg_horario.size()-1;
                    reg_horario[posicao].ds_evo=reg_horario[posicao].ds_evo+reg_evolucao[i].ds_evo.substr(inicio,posicoes);
                    //reg_horario[posicao].ds_evo="";
                }
                else 
                    break;
            j=pos+1;
        }
/*        if(entre_0700_e_1600==true && !negou1){
            //reg_horario[reg_horario.size()-1].entre_0700_e_1600=true;
            reg_evolucao[i].entre_0700_e_1600=true;
        }
        else{
            //reg_horario[reg_horario.size()-1].entre_0700_e_1600=false;
            reg_evolucao[i].entre_0700_e_1600=false;
        }
        if(entre_1900_e_2359==true && !negou2){
            //reg_horario[reg_horario.size()-1].entre_1900_e_2359=true;
            reg_evolucao[i].entre_1900_e_2359=true;
        }
        else{
            //reg_horario[reg_horario.size()-1].entre_1900_e_2359=false;
            reg_evolucao[i].entre_1900_e_2359=false;
        }*/
    }
/*    for(int i=0;i<reg_evolucao.size();i++){
        for(int j=0;j<reg_horario.size();j++){
            if(reg_horario[j].cd_evo==reg_evolucao[i].cd_evo){
                reg_horario[j].entre_0700_e_1600=reg_evolucao[i].entre_0700_e_1600;
                reg_horario[j].entre_1900_e_2359=reg_evolucao[i].entre_1900_e_2359;
            }
        }
        //cout<<reg_evolucao[i].cd_evo<<endl;
    }*/
    //cout<<reg_evolucao.back().ds_evo<<endl;
    cout<<"Ordenando vetor de horários..."<<endl;
    sort(reg_horario.begin(),reg_horario.end(),sortBy_cliente_horario);
    cout<<"Número de horários:"<<reg_horario.size()<<endl;
//    cout<<"Número de evoluções:"<<reg_evolucao.size()<<endl;
    cout<<"Médio de horários por evolução:"<<reg_horario.size()/reg_evolucao.size()<<endl;
}


void construa_vetor_prescricao(string arquivo){
    char palavra[400];
    string str,cd_pre,dt_pre,cd_atd,dt_prm,cd_pro,ds_pro,cd_fat,ds_uni,ds_pre,cd_fre,ds_fre,dh_reg;
    FILE *fp;
    fp=fopen(arquivo.c_str(),"r");
    //printf("Abriu file pointer...\n");
    if (fp == NULL){
        printf("Arquivo de prescrições não disponível. Abortando operação...\n");
        exit(1);
    }
    //printf("Preparando para criar tabela de prescrições...\n");
    while(fgets(palavra,400,fp)){ 
        str=palavra;
        int index,offset;
        //cout<<str<<"."<<endl;
        index=0;
        offset=str.find(';',index);
        cd_pre=str.substr(index,(offset-index));
        //cout<<"cd_pre:"<<cd_pre<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index);
        dt_pre=str.substr(index,(offset-index));
        //cout<<"dt_pre:"<<dt_pre<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;

        index=offset+1;
        offset=str.find(';',index);
        cd_atd=str.substr(index,(offset-index)); 
        //cout<<"cd_atd:"<<cd_atd<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index);
        dt_prm=str.substr(index,(offset-index)); 
        //cout<<"dt_pre:"<<dt_pre<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index);
        cd_pro=str.substr(index,(offset-index)); 
        //cout<<"cd_pro:"<<cd_pro<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        if(cd_pro=="")
            continue;
            
        index=offset+1;
        offset=str.find(';',index);
        ds_pro=str.substr(index,(offset-index)); 
        //cout<<"ds_pro:"<<ds_pro<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index);
        cd_fat=str.substr(index,(offset-index)); 
        //cout<<"cd_fat:"<<cd_fat<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index);
        ds_uni=str.substr(index,(offset-index)); 
        //cout<<"ds_uni:"<<ds_uni<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index);
        ds_pre=str.substr(index,(offset-index)); 
        //cout<<"ds_pre:"<<ds_pre<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index);
        cd_fre=str.substr(index,(offset-index)); 
        //cout<<"cd_fre:"<<cd_fre<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index);
        ds_fre=str.substr(index,(offset-index)); 
        //cout<<"ds_fre:"<<ds_fre<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find('\n',index);
        dh_reg=str.substr(index,(offset-index-1)); 
        //cout<<"dh_reg:"<<dh_reg<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        prescricao.cd_atd=cd_atd;
        prescricao.cd_fat=cd_fat;
        prescricao.cd_fre=cd_fre;
        prescricao.cd_pre=cd_pre;
        prescricao.cd_pro=cd_pro;
        prescricao.ds_pro=ds_pro;
        prescricao.dh_reg=dh_reg;
        prescricao.ds_fre=ds_fre;
        prescricao.ds_pre=ds_pre;
        prescricao.ds_uni=ds_uni;
        prescricao.dt_pre=dt_pre;
        prescricao.dt_prm=dt_prm;
        reg_prescricao.push_back(prescricao);
    }    
    for(int i=0;i<reg_prescricao.size();i++){
        for(int j=0;j<reg_frequencia.size();j++){
            if(reg_prescricao[i].cd_fre==reg_frequencia[j].cd_fre){
                reg_prescricao[i].freq=reg_frequencia[j].nr_apl;
                break;
            }
        }
    }
//    cout<<"Ordenando vetor prescrição-evolução..."<<endl;
    sort(reg_prescricao.begin(),reg_prescricao.end(), sortBy_cliente_data);
    cout<<"Número de prescrições:"<<reg_prescricao.size()<<endl;
    fclose(fp);
}

int countSubstring(const std::string& str, const std::string& sub){   
    string sub1;
    string str1=str;
	int s=sub.length()-1;
	if(sub.at(s)=='\n')
		sub1=sub.substr(0,s-1);
	else
		sub1=sub.substr(0,s);
    transform(sub1.begin(), sub1.end(), sub1.begin(), ::tolower);
    transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    if (sub1.length() == 0) return 0;
    int count = 0;
    for (size_t offset = str1.find(sub1); offset != std::string::npos;
	 offset = str1.find(sub1, offset + sub1.length()))
    {
        ++count;
    }
    return count;
}

//Uso de algoritmo soundex
int countSimilarSubstring(const std::string& str, const std::string sub){   
    string sub1=getSoundexBR(sub);
    int inicio,fim,lengte=str.length();
    if (sub1.length() == 0) return 0;
    int count = 0,n=0;
//    cout<<sub<<"-"<<sub1<<endl;
    while(n<lengte)
    {
        inicio=str.find_first_not_of(" ",n);
        fim=str.find_first_of(" ",inicio);
        if(fim!=-1){
            if(getSoundexBR(str.substr(inicio,fim-inicio))==sub1){
                count++;
                //cout<<sub<<str.substr(inicio,fim-inicio)<<endl;
            }
        }
        else{
            fim=lengte;   
            if(getSoundexBR(str.substr(inicio,fim-inicio))==getSoundexBR(sub1)){
                //cout<<sub<<str.substr(inicio,fim-inicio)<<endl;
                count++;
            }
            break;
        }
        n=fim;
    }
//    cout<<"---------------------------------------------------------------------"<<endl;
    return count;
}

void construa_vetor_frequencia(string arquivo){
    char palavra[400];
    string str,cod,desc;
    int apl;
    FILE *fp;
    fp=fopen(arquivo.c_str(),"r");
    printf("Carregando vetor freqüência aplicação medicamentos...\n");
    //printf("Abriu file pointer...\n");
    if (fp == NULL){
        printf("Erro ao ler o arquivo. Abortando operação...\n");
        exit(1);
    }
    reg_frequencia.clear();
    while(fgets(palavra,256,fp)){ 
        str=palavra;
        int index,offset;
        //cout<<str<<"."<<endl;
        index=0;
        offset=str.find(';',index);
        cod=str.substr(index,offset);
        //cout<<"cod:"<<cod<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;
        
        index=offset+1;
        offset=str.find(';',index)-index;
        desc=str.substr(index,offset);
        //cout<<"nome:"<<nom<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;

        index=offset+index+1;
        offset=str.find(';',index);
        apl=atoi(str.substr(index,(offset-index)).c_str()); 
        //cout<<"sinônimo:"<<sin<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl<<endl;
        frequencia.cd_fre=cod;
        frequencia.ds_fre=desc;
        frequencia.nr_apl=apl;
        //medicamento.soundex=getSoundexBR(sin);
        reg_frequencia.push_back(frequencia);
    }    
//    cout<<"Número de sinônimos:"<<reg.size()<<endl;
    fclose(fp);
}

void carrega_base_medicamentos(string arquivo){
	reg_medicamento.clear();
    char palavra[400];
    string str,cod,nom,sin,aux="";
    FILE *fp;
    fp=fopen(arquivo.c_str(),"r");
    printf("Carregando base de medicamentos...\n");
    //printf("Abriu file pointer...\n");
    if (fp == NULL){
        printf("Erro ao ler o arquivo. Abortando operação...\n");
        exit(1);
    }
    reg_medicamento.clear();
    while(fgets(palavra,256,fp)){ 
        str=palavra;
        int index,offset;
        //cout<<str<<"."<<endl;
        if(str.find(";")!=str.npos){
            index=0;
            offset=str.find(';',index);
            cod=str.substr(index,offset);
            //cout<<"cod:"<<cod<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;

            index=offset+1;
            offset=str.find(';',index)-index;
            nom=str.substr(index,offset);
            //cout<<"nome:"<<nom<<"-"<<"index:"<<index<<"-"<<"offset:"<<offset<<endl;

            //sin=nom.substr(0,nom.find_first_of("/")); 
            //sin=nom.substr(0,nom.find_first_of("-")); 
            sin=nom.substr(0,nom.find_first_of(" ")); 
            //cout<<"sinônimo:"<<sin<<endl;
            medicamento.codigo=cod;
            medicamento.nome=nom;
            medicamento.sinonimo=sin;
			//aux=sin;
			//medicamento.nome_alternativo.push_back(sin);
            //medicamento.soundex=getSoundexBR(sin);
            reg_medicamento.push_back(medicamento);
            reg_medicamento[reg_medicamento.size()-1].nome_alternativo.push_back(sin);
        }
        else{
            if(find(reg_medicamento[reg_medicamento.size()-1].nome_alternativo.begin(),reg_medicamento[reg_medicamento.size()-1].nome_alternativo.end(),str)!=reg_medicamento[reg_medicamento.size()-1].nome_alternativo.end());
                reg_medicamento[reg_medicamento.size()-1].nome_alternativo.push_back(str);
       	}
    }    
    sort(reg_medicamento.begin(),reg_medicamento.end(),sortBy_nome_medicamento);
/*    for(int i=0;i<reg_medicamento.size();i++){
        sort(reg_medicamento[i].nome_alternativo.begin(), reg_medicamento[i].nome_alternativo.end());
        reg_medicamento[i].nome_alternativo.erase(unique(reg_medicamento[i].nome_alternativo.begin(),reg_medicamento[i].nome_alternativo.end()),reg_medicamento[i].nome_alternativo.end());        
    }*/
    cout<<"Número produtos na base de medicamentos:"<<reg_medicamento.size()<<endl;
    fclose(fp);
}

void verifica_torre_evolucao(){
    registro sin;
    sinonimo_med med;
    cout<<"Verificando torre evolução..."<<endl;
    sort(reg_prescricao.begin(),reg_prescricao.end(), sortBy_cliente_data);
    FILE *fp_horario=fopen("teste_horario.txt","w");
    FILE *fp_evolucao=fopen("teste_evolucao.txt","w");
    
    //Verificar se nao existe prescricao para o dia anterior de cada prescricao
    bool existe_pre_dia_ant;
    //date dia_pre,dia_ant_pre,dia_evo;
    for(int i=0;i<reg_horario.size();i++){
        existe_pre_dia_ant=false;
        date dia_evo(reg_horario[i].dt_evo);
        date dia_ant_evo=dia_evo;
        dia_ant_evo--;
        for(int j=0;j<reg_prescricao.size();j++){
            date dia_pre(reg_prescricao[j].dt_pre);
            //se existe prescricao para o dia anterior da evolucao
            if(reg_horario[i].cd_atd==reg_prescricao[j].cd_atd && 
               dia_pre==dia_ant_evo){
                existe_pre_dia_ant=true;
                //reg_horario[i].nao_existe_prescricao_dia_anterior=false;
                break;
            }
        }
        if(!existe_pre_dia_ant)
            reg_horario[i].nao_existe_prescricao_dia_anterior=true;
        else
            reg_horario[i].nao_existe_prescricao_dia_anterior=false;
    }

//Se for comparar apenas com o sinonimo:
//  reg_prescricao[i].vezes=reg_prescricao[i].vezes+countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].sinonimo);
    
    for(int i=0;i<reg_prescricao.size();i++){
        if(reg_prescricao[i].cd_pro!="" //&& reg_prescricao[i].cd_pre=="5408274"
                /* && reg_prescricao[i].cd_pro=="2596" && reg_prescricao[i].cd_atd=="1005942" &&
                reg_prescricao[i].dt_pre=="12/06/2016"*/){
            //cout<<reg_prescricao[i].ds_pro<<endl;
            reg_prescricao[i].vezes=0;
            date dt_pre(reg_prescricao[i].dt_pre);
            int n=0;
            for(int j=0;j<reg_horario.size();j++){
                if(reg_prescricao[i].cd_atd==reg_horario[j].cd_atd 
                        //&& reg_horario[j].cd_evo=="1230846"
                        ){
                    date dt_evo(reg_horario[j].dt_evo );
                    date dt_evo_menos_1=dt_evo;
                    dt_evo_menos_1--;
                    //Se nao existe prescricao para o paciente no dia anterior da evolucao
                    //comparar evolucao com as prescricoes do mesmo dia da evolucao
                    if(reg_horario[j].nao_existe_prescricao_dia_anterior ){
                    //cout<<reg_prescricao[i].dt_pre<<" - "<<reg_horario[j].dt_evo<<" - "<<reg_horario[j].hr_evo<<" - "<<reg_horario[j].hr_anot<<" - "<<reg_horario[j].nao_existe_prescricao_dia_anterior<<endl;
                        for(int k=0;k<reg_medicamento.size();k++){
                            //cout<<reg_prescricao[i].ds_pro<<endl;
                            if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro && dt_evo==dt_pre){
                                //int n=0;
                                //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                    n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                    //cout<<n<<endl;
                                    reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                    //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                }
                                //cout<<"---------------------------------------------------------"<<endl;
                            }
                        }
                    }
                    else
                        //Evolucao entre 0 e 7 horas
                        //Paciente ja estava internado no hospital
                        if(tempo(reg_horario[j].hr_evo).entre("00:00","06:59")){
                            //XXXXXXXXX nao esta entrando aqui mas deveria
                    //cout<<reg_prescricao[i].dt_pre<<" - "<<reg_horario[j].dt_evo<<" - "<<reg_horario[j].hr_evo<<" - "<<reg_horario[j].hr_anot<<" - "<<reg_horario[j].nao_existe_prescricao_dia_anterior<<endl;
                            for(int k=0;k<reg_medicamento.size();k++){
                                //cout<<reg_prescricao[i].ds_pro<<endl;
                                if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro && dt_pre==dt_evo_menos_1){
                                    //int n=0;
                                    //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                    for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                        n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                        //cout<<n<<endl;
                                        reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                        //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                    }
                                    //cout<<"---------------------------------------------------------"<<endl;
                                }
                            }
                        }
                        else{
                            //Evolucao entre 7 e 19 horas
                            if(tempo(reg_horario[j].hr_evo).entre("07:00","18:59")){
                                //Se horarios entre 7 e 16 horas
                                if(tempo(reg_horario[j].hr_anot).entre("07:00","15:59")){
                    //cout<<reg_prescricao[i].dt_pre<<" - "<<reg_horario[j].dt_evo<<" - "<<reg_horario[j].hr_evo<<" - "<<reg_horario[j].hr_anot<<" - "<<reg_horario[j].nao_existe_prescricao_dia_anterior<<endl;
                                    for(int k=0;k<reg_medicamento.size();k++){
                                        //cout<<reg_prescricao[i].ds_pro<<endl;
                                        if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro && dt_pre==dt_evo_menos_1){
                                            //int n=0;
                                            //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                            for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                                n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                                reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                                //cout<<n<<endl;
                                                //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                            }
                                            //cout<<"---------------------------------------------------------"<<endl;
                                        }
                                    }
                                }
                                else{
                                    if(dt_evo==dt_pre){
                    //cout<<reg_prescricao[i].dt_pre<<" - "<<reg_horario[j].dt_evo<<" - "<<reg_horario[j].hr_evo<<" - "<<reg_horario[j].hr_anot<<" - "<<reg_horario[j].nao_existe_prescricao_dia_anterior<<endl;
                                        for(int k=0;k<reg_medicamento.size();k++){
                                            //cout<<reg_prescricao[i].ds_pro<<endl;
                                            if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro){
                                                //int n=0;
                                                //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                                for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                                    n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                                    reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                                    //cout<<n<<endl;
                                                    //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                                }
                                                //cout<<"---------------------------------------------------------"<<endl;
                                            }
                                        }
                                    }
                                }
                            }
                            else{
                                //Se hr_evo entre 19 e 2359
                                if(tempo(reg_horario[j].hr_evo).entre("19:00","23:59")){
                                    //Se horarios entre 19 e 2359
                                    if(tempo(reg_horario[j].hr_anot).entre("19:00","23:59 &&")){
                    //cout<<reg_prescricao[i].dt_pre<<" - "<<reg_horario[j].dt_evo<<" - "<<reg_horario[j].hr_evo<<" - "<<reg_horario[j].hr_anot<<" - "<<reg_horario[j].nao_existe_prescricao_dia_anterior<<endl;
                                        for(int k=0;k<reg_medicamento.size();k++){
                                            //cout<<reg_prescricao[i].ds_pro<<endl;
                                            if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro && dt_evo==dt_pre){
                                                //int n=0;
                                                //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                                for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                                    n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                                    reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                                    //cout<<n<<endl;
                                                    //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                                }
                                                //cout<<"---------------------------------------------------------"<<endl;
                                            }
                                        }
                                    }
                                    else{
                                        if(tempo(reg_horario[j].hr_evo).entre("19:00","23:59")) {
                    //cout<<reg_prescricao[i].dt_pre<<" - "<<reg_horario[j].dt_evo<<" - "<<reg_horario[j].hr_evo<<" - "<<reg_horario[j].hr_anot<<" - "<<reg_horario[j].nao_existe_prescricao_dia_anterior<<endl;
                                            for(int k=0;k<reg_medicamento.size();k++){
                                                //cout<<reg_prescricao[i].ds_pro<<endl;
                                                if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro && dt_evo==dt_pre){
                                                    //int n=0;
                                                    //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                                    for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                                        n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                                        reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                                        //cout<<n<<endl;
                                                        //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                                    }
                                                    //cout<<"---------------------------------------------------------"<<endl;
                                                }
                                            }
                                        }

                                    }
                                }

                            }

                        }
                }
            }
        }
    }
    
    for(int i=0;i<reg_horario.size();i++)
        //if(reg_horario[i].cd_atd=="1005942")
        fprintf(fp_horario,"%s - %s - %s - %s\n%s\nhorario: %s\nnao existe presc dia ant: %d\nevolucoes entre_0700_e_1600: %d\nevolucoes entre_1900_e_2359: %d\n-------------------------------------------------------------------------------\n",reg_horario[i].cd_evo.c_str(),reg_horario[i].cd_atd.c_str(),reg_horario[i].dt_evo.c_str(),reg_horario[i].hr_evo.c_str(),reg_horario[i].ds_evo.c_str(),reg_horario[i].hr_anot.c_str(),reg_horario[i].nao_existe_prescricao_dia_anterior,reg_horario[i].entre_0700_e_1600,reg_horario[i].entre_1900_e_2359);
    for(int i=0;i<reg_evolucao.size();i++)
        //if(reg_horario[i].cd_atd=="1005942")
        fprintf(fp_evolucao,"%s - %s - %s - %s\n%s\nnao existe presc dia ant: %d\nevolucoes entre_0700_e_1600: %d\nevolucoes entre_1900_e_2359: %d\n------------------------------------------------------------------------------\n",reg_evolucao[i].cd_evo.c_str(),reg_evolucao[i].cd_atd.c_str(),reg_evolucao[i].dt_evo.c_str(),reg_evolucao[i].hr_evo.c_str(),reg_evolucao[i].ds_evo.c_str(),reg_evolucao[i].nao_existe_prescricao_dia_anterior,reg_evolucao[i].entre_0700_e_1600,reg_evolucao[i].entre_1900_e_2359);
    fclose(fp_horario);
    fclose(fp_evolucao);

                
    FILE *fp_vermelho,*fp_amarelo,*fp_verde,*fp;
    cout<<"Escrevendo arquivo de saída..."<<endl;
    fp=fopen("saida_evolucao.txt","w");
    fp_vermelho=fopen("saida_evolucao_vermelho.txt","w");
    fp_amarelo=fopen("saida_evolucao_amarelo.txt","w");
    fp_verde=fopen("saida_evolucao_verde.txt","w");
    fprintf(fp,"paciente   data       medicamento                                                 freqüência   dispensado  evoluído dt_evo             hr_evo\n");
    fprintf(fp,"---------------------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(fp_vermelho,"paciente   data       medicamento                                                 freqüência   dispensado  evoluído\n");
    fprintf(fp_vermelho,"-------------------------------------------------------------------------------------------------------------------\n");
    fprintf(fp_verde,"paciente   data       medicamento                                                 freqüência   dispensado  evoluído\n");
    fprintf(fp_verde,"-------------------------------------------------------------------------------------------------------------------\n");
    fprintf(fp_amarelo,"paciente   data       medicamento                                                 freqüência   dispensado  evoluído\n");
    fprintf(fp_amarelo,"-------------------------------------------------------------------------------------------------------------------\n");
    for(int i=0;i<reg_prescricao.size();i++){
        if(reg_prescricao[i].cd_pro!=""){
            date dia_pre(reg_prescricao[i].dt_pre);
            date prox_dia_evo=dia_pre;
            prox_dia_evo++;
            if(reg_prescricao[i].vezes<reg_prescricao[i].freq){
                fprintf(fp_vermelho,"%-11s%-11s%-60s%-11s%11d%8d\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
                fprintf(fp_vermelho,"-------------------------------------------------------------------------------------------------------------------\n");
            }
            if(reg_prescricao[i].vezes>reg_prescricao[i].freq){
                fprintf(fp_amarelo,"%-11s%-11s%-60s%-11s%11d%8d\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
                fprintf(fp_amarelo,"-------------------------------------------------------------------------------------------------------------------\n");
            }
            if(reg_prescricao[i].vezes==reg_prescricao[i].freq){
                fprintf(fp_verde,"%-11s%-11s%-60s%-11s%11d%8d\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
                fprintf(fp_verde,"-------------------------------------------------------------------------------------------------------------------\n");
            }
			fprintf(fp,"%-11s%-11s%-60s%-11s%11d%8d\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
			fprintf(fp,"---------------------------------------------------------------------------------------------------------------------------------------------\n");
//            for(int j=0;j<reg_horario.size();j++){
//                date dia_evo(reg_horario[j].dt_evo);
//                if((reg_prescricao[i].cd_atd==reg_horario[j].cd_atd&&
//                   reg_prescricao[i].dt_pre==reg_horario[j].dt_evo&&
 //                  reg_horario[j].hr_evo>="16:00")||
//                   (reg_prescricao[i].cd_atd==reg_horario[j].cd_atd&&
//                   prox_dia_evo==dia_evo&&
 //                  reg_horario[j].hr_evo<"16:00")){
 //                       if(reg_prescricao[i].vezes<reg_prescricao[i].freq)
 //                           fprintf(fp_vermelho,"Eventos %-12s%-6s%-8s\n%-80s\n",reg_horario[j].dt_evo.c_str(),reg_horario[j].hr_evo.c_str(),reg_horario[j].cd_atd.c_str(),reg_horario[j].ds_evo.c_str());
 //                       if(reg_prescricao[i].vezes>reg_prescricao[i].freq)
 //                           fprintf(fp_amarelo,"Eventos %-12s%-6s%-8s\n%-80s\n",reg_horario[j].dt_evo.c_str(),reg_horario[j].hr_evo.c_str(),reg_horario[j].cd_atd.c_str(),reg_horario[j].ds_evo.c_str());
 //                       if(reg_prescricao[i].vezes==reg_prescricao[i].freq)
 //                           fprintf(fp_verde,"Eventos %-12s%-6s%-8s\n%-80s\n",reg_horario[j].dt_evo.c_str(),reg_horario[j].hr_evo.c_str(),reg_horario[j].cd_atd.c_str(),reg_horario[j].ds_evo.c_str());
//                }
//            }
        }
    }
    fclose(fp);
    fclose(fp_verde);
    fclose(fp_amarelo);
    fclose(fp_vermelho);

    fp=fopen("saida.xls","w");
    if(fp==NULL){
        printf("Arquivo nao disponivel...");
        exit(1);
    }
    fprintf(fp,"<\?xml version=\"1.0\"\?>\n");
    fprintf(fp,"<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\
    xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n\
    xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n\
    xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\
    xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n");
    fprintf(fp,"<Worksheet ss:Name=\"Sheet1\">\n");
    fprintf(fp,"<Table>\n");
    fprintf(fp,"<Row>\n\
    <Cell><Data ss:Type=\"String\">Cliente</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Data</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Medicamento</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Frequencia</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Dispensado</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Evoluido</Data></Cell>\n");
    fprintf(fp,"</Row>\n");

    for(int i=0;i<reg_prescricao.size();i++){
        if(reg_prescricao[i].cd_pro!=""){
            fprintf(fp,"<Row>\n\
            <Cell><Data ss:Type=\"String\">%s</Data></Cell>\n\
            <Cell><Data ss:Type=\"String\">%s</Data></Cell>\n\
            <Cell><Data ss:Type=\"String\">%s</Data></Cell>\n\
            <Cell><Data ss:Type=\"String\">%s</Data></Cell>\n\
            <Cell><Data ss:Type=\"Number\">%d</Data></Cell>\n\
            <Cell><Data ss:Type=\"Number\">%d</Data></Cell>\n\
            </Row>\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
        }
    }
    fprintf(fp,"</Table>\n");
    fprintf(fp,"</Worksheet>\n");
    fprintf(fp,"</Workbook>");
    fclose(fp);
}
/*
void verifica_torre_evolucao(){
    FILE *fp,*fp1;
    fp=fopen("teste_horario.txt","w");
    fp1=fopen("teste_evolucao.txt","w");
    registro sin;
    sinonimo_med med;
    cout<<"Verificando torre evolução..."<<endl;
    sort(reg_prescricao.begin(),reg_prescricao.end(), sortBy_cliente_data);
    sort(reg_evolucao.begin(),reg_evolucao.end(), sortEvoBy_cliente_data);
    
    //Verificar se nao existe prescricao para o dia anterior de cada prescricao
    bool existe_pre_dia_ant;
    //date dia_pre,dia_ant_pre,dia_evo;
    for(int i=0;i<reg_horario.size();i++){
        existe_pre_dia_ant=false;
        date dia_evo(reg_horario[i].dt_evo);
        date dia_ant_evo=dia_evo;
        dia_ant_evo--;
        for(int j=0;j<reg_prescricao.size();j++){
            date dia_pre(reg_prescricao[j].dt_pre);
            //se existe prescricao para o dia anterior da evolucao
            if(reg_horario[i].cd_atd==reg_prescricao[j].cd_atd && 
               dia_pre==dia_ant_evo){
                existe_pre_dia_ant=true;
                //reg_horario[i].nao_existe_prescricao_dia_anterior=false;
                break;
            }
        }
        if(!existe_pre_dia_ant)
            reg_horario[i].nao_existe_prescricao_dia_anterior=true;
        else
            reg_horario[i].nao_existe_prescricao_dia_anterior=false;
    }

    for(int i=0;i<reg_evolucao.size();i++){
        existe_pre_dia_ant=false;
        date dia_evo(reg_evolucao[i].dt_evo);
        date dia_ant_evo=dia_evo;
        dia_ant_evo--;
        for(int j=0;j<reg_prescricao.size();j++){
            date dia_pre(reg_prescricao[j].dt_pre);
            //se existe prescricao para o dia anterior da evolucao
            if(reg_evolucao[i].cd_atd==reg_prescricao[j].cd_atd && 
               dia_pre==dia_ant_evo){
                existe_pre_dia_ant=true;
                //reg_evolucao[i].nao_existe_prescricao_dia_anterior=false;
                break;
            }
        }
        if(!existe_pre_dia_ant)
            reg_evolucao[i].nao_existe_prescricao_dia_anterior=true;
        else
            reg_evolucao[i].nao_existe_prescricao_dia_anterior=false;
    }

    
    for(int i=0;i<reg_horario.size();i++)
        fprintf(fp,"%s - %s - %s - %s\n%s\nhorario: %s\nnao existe presc dia ant: %d\nevolucoes entre_0700_e_1600: %d\nevolucoes entre_1900_e_2359: %d\n-------------------------------------------------------------------------------\n",reg_horario[i].cd_evo.c_str(),reg_horario[i].cd_atd.c_str(),reg_horario[i].dt_evo.c_str(),reg_horario[i].hr_evo.c_str(),reg_horario[i].ds_evo.c_str(),reg_horario[i].hr_anot.c_str(),reg_horario[i].nao_existe_prescricao_dia_anterior,reg_horario[i].entre_0700_e_1600,reg_horario[i].entre_1900_e_2359);
    for(int i=0;i<reg_evolucao.size();i++)
        fprintf(fp1,"%s - %s - %s - %s\n%s\nnao existe presc dia ant: %d\nevolucoes entre_0700_e_1600: %d\nevolucoes entre_1900_e_2359: %d\n------------------------------------------------------------------------------\n",reg_evolucao[i].cd_evo.c_str(),reg_evolucao[i].cd_atd.c_str(),reg_evolucao[i].dt_evo.c_str(),reg_evolucao[i].hr_evo.c_str(),reg_evolucao[i].ds_evo.c_str(),reg_evolucao[i].nao_existe_prescricao_dia_anterior,reg_evolucao[i].entre_0700_e_1600,reg_evolucao[i].entre_1900_e_2359);
    fclose(fp);
    fclose(fp1);
    
    for(int i=0;i<reg_prescricao.size();i++){
	//cout<<reg_prescricao[i].ds_pro<<endl;
        reg_prescricao[i].vezes=0;
        if(reg_prescricao[i].cd_pro!=""){
            date dt_pre(reg_prescricao[i].dt_pre);
            date dt_pre_menos_1=dt_pre;
            dt_pre_menos_1--;
            for(int j=0;j<reg_horario.size();j++){
                //Se nao existe prescricao no dia anterior
                if(reg_horario[j].cd_atd==reg_prescricao[i].cd_atd &&
                   date(reg_horario[j].dt_evo)==dt_pre &&
                   reg_horario[j].nao_existe_prescricao_dia_anterior){
                    for(int k=0;k<reg_medicamento.size();k++){
                        //cout<<reg_prescricao[i].ds_pro<<endl;
                        if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro){
//                          reg_prescricao[i].vezes=reg_prescricao[i].vezes+countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].sinonimo);
                            int n=0;
                            //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                            for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                            }
                            //cout<<"---------------------------------------------------------"<<endl;
                        }
                    }
                }
                else
                    //paciente ja estava internado no hospital
                    if(reg_horario[j].cd_atd==reg_prescricao[i].cd_atd &&
                       date(reg_horario[j].dt_evo)==dt_pre_menos_1 &&
                            reg_horario[j].entre_0700_e_1600 && //erbij gezet als correctie
                       (reg_horario[j].hr_evo >= "00:00" && reg_horario[j].hr_evo < "07:00")){
                        for(int k=0;k<reg_medicamento.size();k++){
                            //cout<<reg_prescricao[i].ds_pro<<endl;
                            if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro){
    //                          reg_prescricao[i].vezes=reg_prescricao[i].vezes+countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].sinonimo);
                                int n=0;
                                //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                    n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                    reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                    //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                }
                                //cout<<"---------------------------------------------------------"<<endl;
                            }
                        }
                    }
                    else
                        if (reg_horario[j].cd_atd==reg_prescricao[i].cd_atd &&
                            reg_horario[j].hr_evo >="07:00" && reg_horario[j].hr_evo< "19:00"){
                            if(reg_horario[j].entre_0700_e_1600 &&
                               date(reg_horario[j].dt_evo)==dt_pre_menos_1){//horário das evoluções descritas estiver entre 7:00 e 16:00)
                                //Então comparar todos os horários da evolução com prescrição dt_pre - 1
                                for(int k=0;k<reg_medicamento.size();k++){
                                    //cout<<reg_prescricao[i].ds_pro<<endl;
                                    if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro){
            //                          reg_prescricao[i].vezes=reg_prescricao[i].vezes+countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].sinonimo);
                                        int n=0;
                                        //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                        for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                            n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                            reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                            //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                        }
                                        //cout<<"---------------------------------------------------------"<<endl;
                                    }
                                }
                            }
                            else{
                                //comparar todos os horários da evolução com prescrição do dia
                                if(reg_horario[j].cd_atd==reg_prescricao[i].cd_atd &&
                                   date(reg_horario[j].dt_evo)==dt_pre){
                                    for(int k=0;k<reg_medicamento.size();k++){
                                        //cout<<reg_prescricao[i].ds_pro<<endl;
                                        if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro){
                //                          reg_prescricao[i].vezes=reg_prescricao[i].vezes+countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].sinonimo);
                                            int n=0;
                                            //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                            for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                                n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                                reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                                //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                            }
                                            //cout<<"---------------------------------------------------------"<<endl;
                                        }
                                    }
                                }
                            }
                        }
                        else{      
                            if(reg_horario[j].cd_atd==reg_prescricao[i].cd_atd &&
                               reg_horario[j].hr_evo>="19:00" && reg_horario[j].hr_evo<="23:59"){
                                if(reg_horario[j].entre_1900_e_2359 &&
                                   date(reg_horario[j].dt_evo)==dt_pre){//horário das evoluções descritas estiver entre 19:00 e 23:59)
                                    for(int k=0;k<reg_medicamento.size();k++){
                                        //cout<<reg_prescricao[i].ds_pro<<endl;
                                        if(reg_medicamento[k].codigo==reg_prescricao[i].cd_pro){
                //                          reg_prescricao[i].vezes=reg_prescricao[i].vezes+countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].sinonimo);
                                            int n=0;
                                            //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                            for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                                n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                                reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                                //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                            }
                                            //cout<<"---------------------------------------------------------"<<endl;
                                        }
                                    }
                                }
                                else{ 
                                    for(int k=0;k<reg_medicamento.size();k++){
                                        //cout<<reg_prescricao[i].ds_pro<<endl;
                                        if(reg_horario[j].cd_atd==reg_prescricao[i].cd_atd &&
                                           reg_medicamento[k].codigo==reg_prescricao[i].cd_pro &&
                                           date(reg_horario[j].dt_evo)==dt_pre_menos_1){
                //                          reg_prescricao[i].vezes=reg_prescricao[i].vezes+countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].sinonimo);
                                            int n=0;
                                            //cout<<"nome: "<<reg_medicamento[k].nome<<endl;								
                                            for(int p=0;p<reg_medicamento[k].nome_alternativo.size();p++){
                                                n=countSubstring(reg_horario[j].ds_evo,reg_medicamento[k].nome_alternativo[p]);
                                                reg_prescricao[i].vezes=n+reg_prescricao[i].vezes;
                                                //cout<<"sinonimo: "<<reg_medicamento[k].nome_alternativo[p];
                                            }
                                            //cout<<"---------------------------------------------------------"<<endl;
                                        }
                                    }
                                }
                            }
                        }
            }
        }
    }
              
                
    FILE *fp_vermelho,*fp_amarelo,*fp_verde,*fp_all;
    cout<<"Escrevendo arquivo de saída..."<<endl;
    fp_all=fopen("saida_evolucao.txt","w");
    fp_vermelho=fopen("saida_evolucao_vermelho.txt","w");
    fp_amarelo=fopen("saida_evolucao_amarelo.txt","w");
    fp_verde=fopen("saida_evolucao_verde.txt","w");
    fprintf(fp_all,"paciente   data       medicamento                                                 freqüência   dispensado  evoluído \n");
    fprintf(fp_all,"--------------------------------------------------------------------------------------------------------------------\n");
    fprintf(fp_vermelho,"paciente   data       medicamento                                                 freqüência   dispensado  evoluído\n");
    fprintf(fp_vermelho,"-------------------------------------------------------------------------------------------------------------------\n");
    fprintf(fp_verde,"paciente   data       medicamento                                                 freqüência   dispensado  evoluído\n");
    fprintf(fp_verde,"-------------------------------------------------------------------------------------------------------------------\n");
    fprintf(fp_amarelo,"paciente   data       medicamento                                                 freqüência   dispensado  evoluído\n");
    fprintf(fp_amarelo,"-------------------------------------------------------------------------------------------------------------------\n");
    for(int i=0;i<reg_prescricao.size();i++){
        if(reg_prescricao[i].cd_pro!=""){
            date dia_pre(reg_prescricao[i].dt_pre);
            date prox_dia_evo=dia_pre;
            prox_dia_evo++;
            fprintf(fp_all,"%-11s%-11s%-60s%-11s%11d%8d\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
            fprintf(fp_all,"-------------------------------------------------------------------------------------------------------------------\n");
            if(reg_prescricao[i].vezes<reg_prescricao[i].freq){
                fprintf(fp_vermelho,"%-11s%-11s%-60s%-11s%11d%8d\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
                fprintf(fp_vermelho,"-------------------------------------------------------------------------------------------------------------------\n");
            }
            if(reg_prescricao[i].vezes>reg_prescricao[i].freq){
                fprintf(fp_amarelo,"%-11s%-11s%-60s%-11s%11d%8d\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
                fprintf(fp_amarelo,"-------------------------------------------------------------------------------------------------------------------\n");
            }
            if(reg_prescricao[i].vezes==reg_prescricao[i].freq){
                fprintf(fp_verde,"%-11s%-11s%-60s%-11s%11d%8d\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
                fprintf(fp_verde,"-------------------------------------------------------------------------------------------------------------------\n");
            }
        }
    }
    fclose(fp_all);
    fclose(fp_verde);
    fclose(fp_amarelo);
    fclose(fp_vermelho);
    
    fp=fopen("saida.xls","w");
    if(fp==NULL){
        printf("Arquivo nao disponivel...");
        exit(1);
    }
    fprintf(fp,"<\?xml version=\"1.0\"\?>\n");
    fprintf(fp,"<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\
    xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n\
    xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n\
    xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\
    xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n");
    fprintf(fp,"<Worksheet ss:Name=\"Sheet1\">\n");
    fprintf(fp,"<Table>\n");
    fprintf(fp,"<Row>\n\
    <Cell><Data ss:Type=\"String\">Cliente</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Data</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Medicamento</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Frequencia</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Dispensado</Data></Cell>\n\
    <Cell><Data ss:Type=\"String\">Evoluido</Data></Cell>\n");
    fprintf(fp,"</Row>\n");

    for(int i=0;i<reg_prescricao.size();i++){
        if(reg_prescricao[i].cd_pro!=""){
            fprintf(fp,"<Row>\n\
            <Cell><Data ss:Type=\"String\">%s</Data></Cell>\n\
            <Cell><Data ss:Type=\"String\">%s</Data></Cell>\n\
            <Cell><Data ss:Type=\"String\">%s</Data></Cell>\n\
            <Cell><Data ss:Type=\"String\">%s</Data></Cell>\n\
            <Cell><Data ss:Type=\"Number\">%d</Data></Cell>\n\
            <Cell><Data ss:Type=\"Number\">%d</Data></Cell>\n\
            </Row>\n",reg_prescricao[i].cd_atd.c_str(),reg_prescricao[i].dt_pre.c_str(),reg_prescricao[i].ds_pro.c_str(),reg_prescricao[i].ds_fre.c_str(),reg_prescricao[i].freq,reg_prescricao[i].vezes);
        }
    }
    fprintf(fp,"</Table>\n");
    fprintf(fp,"</Worksheet>\n");
    fprintf(fp,"</Workbook>");
    fclose(fp);

}
*/

//Varredura de evolucao com vetor de sinonimos, criando estrutura
//de medicamentos e possiveis sinonimos
//Construir lista de medicamentos com possiveis nomes alternativos
void construa_vetor_sinonimos(string arquivo){
    vector <string> prescricoes;
    FILE *fp1;
    fp1=fopen("medicamentos_prescritos.txt","w");
	prescricoes.clear();
	//carrega_base_medicamentos(arquivo);
    for(int i=0;i<reg_prescricao.size();i++){
        if(find(prescricoes.begin(),prescricoes.end(),reg_prescricao[i].cd_pro)==prescricoes.end()){
            prescricoes.push_back(reg_prescricao[i].cd_pro);
            fprintf(fp1,"%s -",prescricoes.back().c_str());
            fprintf(fp1,"%s - %s\n",reg_prescricao[i].cd_pro.c_str(),reg_prescricao[i].ds_pro.c_str());
        }
    }
	reg_sinonimo.clear();
    for(int i=0;i<reg_medicamento.size();i++){
        if(find(prescricoes.begin(),prescricoes.end(),reg_medicamento[i].codigo)!=prescricoes.end()){
//            reg_medicamento.erase(find(prescricoes.begin(),prescricoes.end(),reg_medicamento[i].codigo)==prescricoes.end());
            //reg_medicamento.erase(reg_medicamento.begin()+i);
            reg_sinonimo.push_back(reg_medicamento[i]);
        }
    }
    //for(int i=0;i<reg_sinonimo.size();i++)
        //if(reg_sinonimo[i].sinonimo== "NOVALGINA")
            //cout<<reg_sinonimo[i].codigo<<" - "<<reg_sinonimo[i].nome<<endl;
    cout<<reg_sinonimo.size()<<endl;
    fclose(fp1);
    //exit(0);
    reg_medicamento=reg_sinonimo;
    cout<<"Medicamentos prescritos: "<<prescricoes.size()<<endl;
    cout<<"Construindo vetor de sinonimos..."<<endl;
    int count=0,total=prescricoes.size(),atual=0,n=0;
    FILE *fp2;
    fp2=fopen("palavras_evolucao.txt","w");
    int cnt=0;
    for(int i=0;i<reg_medicamento.size();i++){
        cnt++;
        if(cnt==reg_medicamento.size()+1)break;
		for(int j=0;j<reg_medicamento[i].nome_alternativo.size();j++){
			string nome_soundex=getSoundexBR(reg_medicamento[i].nome_alternativo[j]);
			cout<<cnt<<" de "<<reg_medicamento.size()<<endl;
			//cout<<reg_medicamento[i].nome_alternativo[j]<<endl;
			for(int j=0;j<reg_horario.size();j++){
	//          if(reg_prescricao[a].cd_atd==reg_horario[j].cd_atd&&reg_prescricao[a].dt_pre==reg_horario[j].dt_evo){
				string str=reg_horario[j].ds_evo;
				//cout<<str<<endl;
				int n=0;
				int inicio,fim,lengte=str.length();
				//identificar palavras em cada descricao de evolucao e comparar
				//com nome_soundex para achar coincidencias
				while(n<lengte)
				{
					inicio=str.find_first_not_of(" ",n);
					fim=str.find_first_of(" ",inicio);
					int k=0;
                    //cout<<lengte<<" - "<<n<<" - "<<inicio<<" - "<<fim<<endl;
					string nome_sinonimo=str.substr(inicio,fim-inicio);
					//cout<<nome_sinonimo<<endl;
					if(fim!=-1){
						if(getSoundexBR(nome_sinonimo)==nome_soundex){
							//cout<<reg_medicamento[i].sinonimo<<" - "<<nome_sinonimo<<endl;
							count++;
							//Inserir os nomes sinonimos achados
							//if(find(reg_medicamento[i].nome_alternativo.begin(),reg_medicamento[i].nome_alternativo.end(),nome_sinonimo)==reg_medicamento[i].nome_alternativo.end())
								reg_medicamento[i].nome_alternativo_out.push_back(nome_sinonimo);
						}
					}
					else
						break;
					n=fim;
                    //cout<<n<<endl;
				}
	 //       }
	 //       break;
			}
		}
    }
    FILE *fp;
    fp=fopen(arquivo.c_str(),"w");
    //COLOCAR TUDO EM MAIUSCULA E TIRAR ACENTOS
    for(int i=0;i<reg_medicamento.size();i++){
        //fprintf(fp,"\n%s---------------------------------------------------------\n",reg_sinonimo[i].nome.c_str());
        for(int j=0;j<reg_medicamento[i].nome_alternativo_out.size();j++){
            //fprintf(fp,"%s\n",reg_sinonimo[i].nome_alternativo[j].c_str());
            reg_medicamento[i].nome_alternativo_out[j]=normalizarString(reg_medicamento[i].nome_alternativo_out[j]);
        }
    }
    //ORDENAR LISTA E TIRAR DUPLAS
    sort(reg_medicamento.begin(),reg_medicamento.end(), sortBy_nome_medicamento);
    for(int i=0;i<reg_medicamento.size();i++){
        sort( reg_medicamento[i].nome_alternativo_out.begin(), reg_medicamento[i].nome_alternativo_out.end() );
        reg_medicamento[i].nome_alternativo_out.erase(unique(reg_medicamento[i].nome_alternativo_out.begin(),reg_medicamento[i].nome_alternativo_out.end() ),reg_medicamento[i].nome_alternativo_out.end());        
    }
    //GRAVAR ARQUIVO
/*    for(int i=0;i<reg_medicamento.size();i++){
        fprintf(fp,"%s;%s;\n",reg_medicamento[i].codigo.c_str(),reg_medicamento[i].nome.c_str());
        for(int j=0;j<reg_medicamento[i].nome_alternativo_out.size();j++){
			if(reg_medicamento[i].nome_alternativo_out[j].c_str()!="\n")
            	fprintf(fp,"%s\n",reg_medicamento[i].nome_alternativo_out[j].c_str());
        }
    }*/
    cout<<"Sinonimos achados: "<<count<<endl;
    fclose(fp);
    fclose(fp2);
}

vector<string> retornaSinonimos(string codigo){
    for(int i=0;i<reg_medicamento.size();i++)
        if(codigo==reg_medicamento[i].codigo)
            return reg_medicamento[i].nome_alternativo;
}

void insiraSinonimo(int cod,string str){
    int index=-1;
    cout<<"inserting "<<str<<endl;
    if(reg_medicamento.size()>=cod){
        for(int i=0;i<reg_medicamento[cod].nome_alternativo_out.size();i++){
            if(reg_medicamento[cod].nome_alternativo_out[i]==str)
                index=i;
        }
        if(index!=-1){
            reg_medicamento[cod].nome_alternativo_out.erase(reg_medicamento[cod].nome_alternativo_out.begin()+index);
            reg_medicamento[cod].nome_alternativo.push_back(str);
            cout<<str<<" inserted..."<<endl;
        }
        else
            cout<<str<<" not inserted..."<<endl;
    }
}

void insiraSinonimoDireto(int cod,string str){
    int index=-1;
    cout<<"inserting "<<str<<endl;
    if(reg_medicamento.size()>=cod){
            reg_medicamento[cod].nome_alternativo.push_back(str);
            cout<<str<<" inserted..."<<endl;
    }
    else
            cout<<str<<" not inserted..."<<endl;
}

void retiraSinonimo(int cod,string str){
    int index=-1;
    if(reg_medicamento.size()>=cod){
        for(int i=0;i<reg_medicamento[cod].nome_alternativo.size();i++){
            if(reg_medicamento[cod].nome_alternativo[i]==str)
                index=i;
        }
        if(index!=-1){
            reg_medicamento[cod].nome_alternativo.erase(reg_medicamento[cod].nome_alternativo.begin()+index);
            reg_medicamento[cod].nome_alternativo_out.push_back(str);
            cout<<str<<" deleted..."<<endl;
        }
        else
            cout<<str<<" not deleted..."<<endl;
    }
}
