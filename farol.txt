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
