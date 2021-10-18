#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WLink>
#include <Wt/WText>
#include <Wt/WBreak>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>
#include <Wt/WSelectionBox>
#include <Wt/WProgressBar>
#include <Wt/WLabel>
#include <Wt/WTemplate>
#include <Wt/WTable>
#include <Wt/WImage>
#include <Wt/WPushButton>
#include <Wt/WResource>
#include <Wt/WCalendar>
#include <Wt/WDate>
#include <Wt/WDatePicker>
#include <Wt/WDateEdit>
#include <Wt/WLineEdit>
#include <Wt/WDialog>
#include <Wt/WEnvironment>
#include <Wt/WRegExpValidator>
#include <Wt/WAnimation>
#include <Wt/WToolBar>

/*
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/Render/WPdfRenderer>
#include <Wt/WApplication>
#include <hpdf.h>
*/
#include "hospital.h"

using namespace Wt;

bool notDate(string dia){
    int d=atoi(dia.substr(0,2).c_str());
    int m=atoi(dia.substr(3,2).c_str());
    int y=atoi(dia.substr(6,4).c_str());
	if(d>0 && d<32 && m>0 && m<13 && y>1900 && y<2100)
		return false;
	else
		return true;
}



class FormHospital: public WApplication {
private:
    std::string appName;
    WContainerWidget* _content;
    std::string frequencia="frequencia.txt";
    std::string evolucao="";
    std::string prescricao="";
    std::string medicamentos="produtos1.txt";
	std::string inicio="",fim="";
	Wt::WFileUpload *fu2,*fu3;


public:
	//Variavel usado para armazenar indice de selectionbox1 para ser usado em selectionbox2
	int escolha1;
    FormHospital(const WEnvironment &env): WApplication(env) {
		setLocale("pt_BR.utf-8");
        appName = "Hospital Brasilia";
        setTitle(appName);
        _content = 0;
        internalPathChanged().connect(this, &FormHospital::onInternalPathChange);
		this->instance()->styleSheet().addRule("html", "background-color: #ffffff; width: 100%; height: 100%;");
		this->instance()->styleSheet().addRule("body", "background-color: #ffffff; width: 100%; height: 100%;margin: 0 auto;");
		this->instance()->styleSheet().addRule("#header", "background-color: #ffffff; padding: 0.1em 0.5em;");
		this->instance()->styleSheet().addRule("#sidebar", "background-color: #ffffff; width: 20%; height: 100%; float: left; display: inline-block;");
		this->instance()->styleSheet().addRule("#content", "width: 80%; overflow: scroll;float: left; display: inline-block;height: 100%;");
		this->instance()->styleSheet().addRule("#footer", "background-color: #ffffff; color:#262626 ; width: 100%;");
		this->instance()->styleSheet().addRule("th", "data-position=fixed; background-color: #4CAF50; color: white; padding: 15px;text-align: left;font-size : 77%;font-family : Myriad Web,Verdana,Helvetica,Arial,sans-serif;");
		this->instance()->styleSheet().addRule("td", "background-color: #ffffff;padding: 15px;text-align: left;font-size : 77%;font-family : Myriad Web,Verdana,Helvetica,Arial,sans-serif;");
		this->instance()->styleSheet().addRule("h1", "padding: 15px;text-align: left;font-family : Myriad Web,Verdana,Helvetica,Arial,sans-serif;");
		this->instance()->styleSheet().addRule("h2", "padding: 15px;text-align: left;font-family : Myriad Web,Verdana,Helvetica,Arial,sans-serif;");
       	header();
        sidebar();
        home();
		//Wt::WApplication::instance()->useStyleSheet("style/estilo.css");
        //footer();
    }
	
    WContainerWidget* content() {
        if (_content == 0) {
            _content = new WContainerWidget(root());
            _content->setId("content");
        }
        return _content;
    }
	
    void onInternalPathChange() {
        content()->clear();
        if (internalPath() == "/" || internalPath() == "/page0" || internalPath() == "?_=/" || internalPath() == "") {
            home();//home();
			setInternalPath("/page0");
        }
        else if (internalPath() == "/page1") {
            page1();
        }
        else if (internalPath() == "/page2") {
            page2();
        }
        else if (internalPath() == "/page3") {
            page3();
        }
        else if (internalPath() == "/page4"){
            page4();
        }
    }

    void header() {
        WContainerWidget* header = new WContainerWidget(root());
        header->setId("header");
		header->addWidget(new Wt::WImage("LogoZoetec.JPG"));
		header->addWidget(new Wt::WImage("banner1.jpeg"));
        //header->addWidget(new WText("<h1>" + appName + "</h1>"));
    }

    void sidebar() {
        WContainerWidget* sidebar = new WContainerWidget(root());
        sidebar->setId("sidebar");
		
		int breedte=250;
		
		Wt::WTemplate *result = new Wt::WTemplate(Wt::WString::tr("pushButtonColor-template"));
		sidebar->addWidget(new Wt::WBreak());
		sidebar->addWidget(new Wt::WBreak());
		sidebar->addWidget(new Wt::WBreak());
		Wt::WPushButton *btnHome = new Wt::WPushButton("Home",sidebar);
		btnHome->setWidth(breedte);
		btnHome->setLink(Wt::WLink(Wt::WLink::InternalPath, "/page0"));

		sidebar->addWidget(new Wt::WBreak());

		Wt::WPushButton *btnSinonimos = new Wt::WPushButton("Verificar Sinonimos",sidebar);
		btnSinonimos->setWidth(breedte);
		btnSinonimos->setLink(Wt::WLink(Wt::WLink::InternalPath, "/page1"));

		sidebar->addWidget(new Wt::WBreak());

		Wt::WPushButton *btnVerificar = new Wt::WPushButton("Verificar Evolucao",sidebar);
		btnVerificar->setWidth(breedte);
		btnVerificar->setLink(Wt::WLink(Wt::WLink::InternalPath, "/page2"));

		sidebar->addWidget(new Wt::WBreak());

		Wt::WPushButton *btnInserir = new Wt::WPushButton("Inserir sinonimos",sidebar);
		btnInserir->setWidth(breedte);
		btnInserir->setLink(Wt::WLink(Wt::WLink::InternalPath, "/page4"));

    }

    void footer() {
        WContainerWidget* footer = new WContainerWidget(root());
        footer->setId("footer");
        footer->addWidget(new WText("m.koendjbiharie@gmail.com"));
    }

    void home() {
		WApplication::setInternalPath("/page0");
        WText *t = new WText("<h2>Home</h2>",content());
		content()->addWidget(new WText("<hr/>"));
		//Wt::WContainerWidget *container = new Wt::WContainerWidget();
		reg_frequencia.clear();
		reg_medicamento.clear();
		reg_evolucao.clear();
		reg_prescricao.clear();
		//frequencia="frequencia.txt";
	}

	//Verificar torre evolucao
    void page2() {
        content()->addWidget(new WText("<h2>Verificar torre evolucao</h2>"));
        content()->addWidget(new WText("<hr/>"));
        content()->addWidget(new WBreak());

		//Solution: increase the parameter max-request-size within wt_config.xml
//        content()->addWidget(new WText("<strong>Arquivo de medicamentos</strong>"));
//        content()->addWidget(new WBreak());

		/*Wt::WFileUpload *fu1 = new Wt::WFileUpload(content());
		fu1->setFileTextSize(50000); // Set the maximum file size to 5000 kB.
		fu1->setProgressBar(new Wt::WProgressBar());
		fu1->setMargin(10, Wt::Right);
		Wt::WText *out1 = new Wt::WText(content());
		Wt::WPushButton *uploadButton1 = new Wt::WPushButton("Send", content());
		uploadButton1->setMargin(10, Wt::Left | Wt::Right);
        content()->addWidget(new WBreak());
        content()->addWidget(new WBreak());*/
		
        /*content()->addWidget(new WText("<strong>Arquivo de medicamentos</strong>"));
        content()->addWidget(new WBreak());
		Wt::WFileUpload *fu1 = new Wt::WFileUpload(content());
		fu1->setFileTextSize(500000000); 
		fu1->setProgressBar(new Wt::WProgressBar());
		fu1->setMargin(10, Wt::Right);
		Wt::WText *out1 = new Wt::WText(content());
		Wt::WPushButton *uploadButton1 = new Wt::WPushButton("Send", content());
		uploadButton1->setMargin(10, Wt::Left | Wt::Right);
        content()->addWidget(new WBreak());
        content()->addWidget(new WBreak());*/
		
        content()->addWidget(new WText("<strong>Arquivo de prescricoes</strong>"));
        content()->addWidget(new WBreak());
		//if(prescricao==""){
			fu2 = new Wt::WFileUpload(content());
			fu2->stealSpooledFile();
			fu2->setFileTextSize(500000000); 
			fu2->setProgressBar(new Wt::WProgressBar());
			fu2->setMargin(10, Wt::Right);
		//}
		Wt::WText *out2 = new Wt::WText(content());
		out2->setText(prescricao);
		Wt::WPushButton *uploadButton2 = new Wt::WPushButton("Send", content());
		uploadButton2->setMargin(10, Wt::Left | Wt::Right);
        content()->addWidget(new WBreak());
        content()->addWidget(new WBreak());
		
        content()->addWidget(new WText("<strong>Arquivo de anotacoes</strong>"));
        content()->addWidget(new WBreak());
		//if(evolucao==""){
			fu3 = new Wt::WFileUpload(content());
			fu3->stealSpooledFile();
			fu3->setFileTextSize(500000000); 
			fu3->setProgressBar(new Wt::WProgressBar());
			fu3->setMargin(10, Wt::Right);
		//}
		Wt::WText *out3 = new Wt::WText(content());
		out3->setText(evolucao);
		Wt::WPushButton *uploadButton3 = new Wt::WPushButton("Send", content());
		uploadButton3->setMargin(10, Wt::Left | Wt::Right);
        content()->addWidget(new WBreak());
        content()->addWidget(new WBreak());


        content()->addWidget(new WText("<hr/>"));
        content()->addWidget(new WBreak());

/*		Wt::WPushButton *processarButton = new Wt::WPushButton("Processar", content());
		//processarButton->setEnabled(false);
        content()->addWidget(new WBreak());*/

		Wt::WText *out = new Wt::WText("<strong>Escolhe datas validas</strong>",content());
        content()->addWidget(new WBreak());

		Wt::WDateEdit *de1 = new Wt::WDateEdit();
		content()->addWidget(de1);
		//form->bindWidget("from", de1);
		//de1->setDate(Wt::WDate::currentServerDate().addDays(1));

		Wt::WDateEdit *de2 = new Wt::WDateEdit();
		content()->addWidget(de2);
		//de2->setFormat("dd MM yyyy"); // Apply a different date format.
		//de2->calendar()->setHorizontalHeaderFormat(Wt::WCalendar::SingleLetterDayNames);
		//de2->setBottom(de1->date());        
		content()->addWidget(new WBreak());

		Wt::WPushButton *btnProcessar2 = new Wt::WPushButton("Processar",content());
		Wt::WPushButton *btnPdf = new Wt::WPushButton("Gera Pdf", content());
		Wt::WPushButton *btnExcel = new Wt::WPushButton("Gera Excel", content());
		btnPdf->setEnabled(false);
		//Wt::WResource *pdf = new ReportResource(content());
		//btnPdf->setLink(pdf);
        content()->addWidget(new WBreak());
        content()->addWidget(new WBreak());
		//form->bindWidget("processar", btnProcessar2);
		
		de1->setText(inicio);
		de2->setText(fim);

/*		de1->changed().connect(std::bind([=] () {
			if (de1->validate() == Wt::WValidator::Valid) {
				de2->setBottom(de1->date());
			//out->setText("Date picker 1 is changed.");
			}
		}));

		de2->changed().connect(std::bind([=] () {
			if (de1->validate() == Wt::WValidator::Valid) {
				de1->setTop(de2->date());
			//out->setText("Date picker 2 is changed.");
			}
		}));
*/
		btnProcessar2->setLink(Wt::WLink(Wt::WLink::InternalPath, "/page3"));
		btnProcessar2->clicked().connect(std::bind([=] () {
			if (de1->text().empty() || de2->text().empty())
			out->setText("<strong>Entre duas datas!</strong>");
			else {
			int days = de1->date().daysTo(de2->date()) + 1;
			if (days >= 1)
				out->setText(de1->text()+" ate "+de2->text());
//				out->setText(Wt::WString("So, you want to take holiday for a period of "
//							 "{1} days?...").arg(days));
			else
				out->setText("Periodo invalido!");
			}
			reg_frequencia.clear();
			reg_medicamento.clear();
			reg_prescricao.clear();
			reg_evolucao.clear();
			reg_horario.clear();
			//frequencia="frequencia.txt";
	    	/*evolucao="evolucao1.txt";
			prescricao="extracao_prescricao1.txt";*/
			//medicamentos="produtos1.txt";
			//if(evolucao=="")
				evolucao=fu3->spoolFileName();
			//if(prescricao=="")
				prescricao=fu2->spoolFileName();
			cout<<"Arquivo de evolucoes: "<<evolucao<<endl;
			cout<<"Arquivo de prescricoes: "<<prescricao<<endl;

			carrega_base_medicamentos(medicamentos);
			construa_vetor_frequencia(frequencia);
			construa_vetor_prescricao(prescricao);
			construa_vetor_evolucao(evolucao);
			verifica_torre_evolucao();

			content()->addWidget(new WText("<h2>Resultado verificacao evolucao</h2>"));
			content()->addWidget(new WText("<hr/>"));
			content()->addWidget(new WBreak());
			inicio=de1->text().toUTF8();
			fim=de2->text().toUTF8();
			//btnPdf->setEnabled(true);
		}));

		// Upload when the button is clicked.
/*		uploadButton1->clicked().connect(std::bind([=] () {
			fu1->upload();
		}));*/

		// Upload when the button is clicked.
		uploadButton2->clicked().connect(std::bind([=] () {
			fu2->upload();
		}));

		// Upload when the button is clicked.
		uploadButton3->clicked().connect(std::bind([=] () {
			fu3->upload();
		}));

		// React to a succesfull upload.+fu->spoolFileName()
/*		fu1->uploaded().connect(std::bind([=] () {
			uploadButton1->disable();
			out1->setText(" File uploaded: "+fu1->spoolFileName());
		}));*/

		// React to a succesfull upload.+fu->spoolFileName()
		fu2->uploaded().connect(std::bind([=] () {
			uploadButton2->disable();
			out2->setText(" File uploaded: "+fu2->spoolFileName());
		}));

		// React to a succesfull upload.+fu->spoolFileName()
		fu3->uploaded().connect(std::bind([=] () {
			uploadButton3->disable();
			out3->setText(" File uploaded: "+fu3->spoolFileName());
		}));

		btnExcel->clicked().connect(std::bind([=] () {
			if (de1->text().empty() || de2->text().empty())
			out->setText("<strong>Entre duas datas!</strong>");
			else {
			int days = de1->date().daysTo(de2->date()) + 1;
			if (days >= 1)
				out->setText(de1->text()+" ate "+de2->text());
//				out->setText(Wt::WString("So, you want to take holiday for a period of "
//							 "{1} days?...").arg(days));
			else
				out->setText("Periodo invalido!");
			}
			reg_frequencia.clear();
			reg_medicamento.clear();
			reg_evolucao.clear();
			reg_prescricao.clear();
			frequencia="frequencia.txt";
//	    	evolucao="evolucao.txt";
//			prescricao="extracao_prescricao.txt";
			medicamentos="produtos1.txt";
			evolucao=fu3->spoolFileName();
			prescricao=fu2->spoolFileName();
			//medicamentos=fu1->spoolFileName();

			carrega_base_medicamentos(medicamentos);
			construa_vetor_frequencia(frequencia);
			construa_vetor_evolucao(evolucao);
			construa_vetor_prescricao(prescricao);
			verifica_torre_evolucao();

			int n=0;
			inicio=de1->text().toUTF8();
			fim=de2->text().toUTF8();
			FILE *fp;
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
				if(reg_prescricao[i].cd_pro!=""&& reg_prescricao[i].dt_pre>=inicio && reg_prescricao[i].dt_pre<=fim){
					date dia_pre(reg_prescricao[i].dt_pre);
					date prox_dia_evo=dia_pre;
					prox_dia_evo++;
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
 		}));

	}

	
	
	//Radar por paciente
    void page3() {
		Wt::WTable *table = new Wt::WTable(content());
		table->setHeaderCount(2); 	
		table->elementAt(0,0)->addWidget(new Wt::WText("Data:"));
		table->elementAt(0,1)->addWidget(new Wt::WText(inicio));
		table->elementAt(0,2)->addWidget(new Wt::WText("ate"));
		table->elementAt(0,3)->addWidget(new Wt::WText(fim));
		table->elementAt(1, 0)->addWidget(new Wt::WText("STATUS"));
		table->elementAt(1, 1)->addWidget(new Wt::WText("PACIENTE"));
		table->elementAt(1, 2)->addWidget(new Wt::WText("DATA"));
		table->elementAt(1, 3)->addWidget(new Wt::WText("MEDICAMENTO"));
		table->elementAt(1, 4)->addWidget(new Wt::WText("FREQUENCIA"));
		table->elementAt(1, 5)->addWidget(new Wt::WText("DISPENSADO"));
		table->elementAt(1, 6)->addWidget(new Wt::WText("EVOLUIDO"));

		int n=-1;
/*		if(notDate(inicio) || notDate(fim)){
			for(int i=0;i<reg_prescricao.size();i++){
				cout<<reg_prescricao[i].ds_pro<<endl;
				if(reg_prescricao[i].cd_pro!=""){
					//date dia_pre(reg_prescricao[i].dt_pre);
					//date prox_dia_evo=dia_pre;
					//prox_dia_evo++;
					if(reg_prescricao[i].vezes==reg_prescricao[i].freq)
						table->elementAt(i+1-n, 0)->addWidget(new Wt::WImage("imgCheck.jpeg", content()));
					else
						if(reg_prescricao[i].vezes<reg_prescricao[i].freq)
							table->elementAt(i+1-n, 0)->addWidget(new Wt::WImage("imgCross.jpeg", content()));
					else
						table->elementAt(i+1-n, 0)->addWidget(new Wt::WImage("imgExclaim.jpeg", content()));

					table->elementAt(i+1-n, 1)->addWidget(new Wt::WText(reg_prescricao[i].cd_atd));
					table->elementAt(i+1-n, 2)->addWidget(new Wt::WText(reg_prescricao[i].dt_pre));
					table->elementAt(i+1-n, 3)->addWidget(new Wt::WText(reg_prescricao[i].ds_pro));
					table->elementAt(i+1-n, 4)->addWidget(new Wt::WText(reg_prescricao[i].ds_fre));
					table->elementAt(i+1-n, 5)->addWidget(new Wt::WText(to_string(reg_prescricao[i].freq)));
					table->elementAt(i+1-n, 6)->addWidget(new Wt::WText(to_string(reg_prescricao[i].vezes)));
				}
				else
					n++;
			}
		}
		else*/
			for(int i=0;i<reg_prescricao.size();i++){
				cout<<reg_prescricao[i].ds_pro<<endl;
				if(reg_prescricao[i].cd_pro!=""&& reg_prescricao[i].dt_pre>=inicio && reg_prescricao[i].dt_pre<=fim){
					/*date dia_pre(reg_prescricao[i].dt_pre);
					date prox_dia_evo=dia_pre;
					prox_dia_evo++;*/
					if(reg_prescricao[i].vezes==reg_prescricao[i].freq)
						table->elementAt(i+1-n, 0)->addWidget(new Wt::WImage("imgCheck.jpeg", content()));
					else
						if(reg_prescricao[i].vezes<reg_prescricao[i].freq)
							table->elementAt(i+1-n, 0)->addWidget(new Wt::WImage("imgCross.jpeg", content()));
					else
						table->elementAt(i+1-n, 0)->addWidget(new Wt::WImage("imgExclaim.jpeg", content()));

					table->elementAt(i+1-n, 1)->addWidget(new Wt::WText(reg_prescricao[i].cd_atd));
					table->elementAt(i+1-n, 2)->addWidget(new Wt::WText(reg_prescricao[i].dt_pre));
					table->elementAt(i+1-n, 3)->addWidget(new Wt::WText(reg_prescricao[i].ds_pro));
					table->elementAt(i+1-n, 4)->addWidget(new Wt::WText(reg_prescricao[i].ds_fre));
					table->elementAt(i+1-n, 5)->addWidget(new Wt::WText(to_string(reg_prescricao[i].freq)));
					table->elementAt(i+1-n, 6)->addWidget(new Wt::WText(to_string(reg_prescricao[i].vezes)));
				}
				else
					n++;
			}
    }

	//Inserir sinônimos
    void page4() {
        content()->addWidget(new WText("<h2>Inserir sinonimos</h2>"));
        content()->addWidget(new WText("<hr/>"));
        content()->addWidget(new WBreak());
		
		//Carrega medicamentos
		carrega_base_medicamentos("produtos1.txt");

		//Wt::WText *out4 = new Wt::WText(content());
		//content()->addWidget(new WBreak());
		Wt::WText *txt40 = new WText("<strong>Medicamentos&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Sinonimos</strong>",content());
		content()->addWidget(new WBreak());

		//Caixa de selecao1
		Wt::WSelectionBox *sb1 = new Wt::WSelectionBox(content());
		sb1->setWidth(400); 
		sb1->clear();
		
		//Caixa de selecao2
		Wt::WSelectionBox *sb2 = new Wt::WSelectionBox(content());
		sb2->setWidth(400); 
		sb2->clear();
		content()->addWidget(new WBreak());

		//escolha1=0;
		//cout<<sb2->currentText().toUTF8()<<endl;
		
		//Load selectionbox1
		for(int j=0;j<reg_medicamento.size();j++){
			if(reg_medicamento[j].codigo!="")
				sb1->addItem(reg_medicamento[j].nome);
		}
		sb1->setCurrentIndex(0);
		escolha1=sb1->currentIndex();
		
		//Load selectionbox2
		for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++)
		if(reg_medicamento[escolha1].nome_alternativo[j]!="\n"){
			sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
		}
		sb2->setCurrentIndex(0);

		// Provide an add button.
		Wt::WPushButton *addButton = new Wt::WPushButton("Adicionar novo sinonimo", content());
		//addButton->setEnabled(false);
		
		// Provide a remove button.
		Wt::WPushButton *removeButton= new Wt::WPushButton("Remover sinonimo", content());
		removeButton->setEnabled(false);
		content()->addWidget(new WBreak());

		// Provide a Save button.
		Wt::WPushButton *saveButton= new Wt::WPushButton("Salvar", content());
		//saveButton->setEnabled(false);

		//Ao selecionar item de selectionbox1
		sb1->activated().connect(std::bind([=] () {
			//out4->setText(Wt::WString::fromUTF8("Voce escolheu: {1}").arg(sb1->currentIndex()));
			escolha1=sb1->currentIndex();
			sb2->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++)
			if(reg_medicamento[escolha1].nome_alternativo[j]!="\n"){
				sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
			}
			removeButton->setEnabled(true);
			//addButton->setEnabled(true);
		}));

		//Remove item when the button is clicked.
		removeButton->clicked().connect(std::bind([=] () {
			escolha1=sb1->currentIndex();
			cout<<sb2->currentText().toUTF8()<<endl;
			retiraSinonimo(escolha1,sb2->currentText().toUTF8());
			
			sb2->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++){
				sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
			}
			sb2->setCurrentIndex(0);
		}));

		//Add item when the button is clicked.
		addButton->clicked().connect(std::bind([=] () {
			Wt::WDialog *dialog = new Wt::WDialog("Adicionar novo sinonimo");

			//Wt::WLabel *label = new Wt::WLabel("Novo sinonimo)",dialog->contents());
			Wt::WLineEdit *edit = new Wt::WLineEdit(dialog->contents());
			//label->setBuddy(edit);

			//dialog->contents()->addStyleClass("form-group");
			//dialog->setStyleClass("background-color: #ffffff;padding: 15px;text-align: left;font-size : 77%;font-family : Myriad Web,Verdana,Helvetica,Arial,sans-serif;");
			dialog->contents()->setStyleClass("background-color: #ffffff;padding: 15px;text-align: left;font-size : 77%;font-family : Myriad Web,Verdana,Helvetica,Arial,sans-serif;");

			Wt::WPushButton *ok = new Wt::WPushButton("OK", dialog->footer());
			ok->setDefault(true);

			Wt::WPushButton *cancel = new Wt::WPushButton("Cancela", dialog->footer());
			dialog->rejectWhenEscapePressed();

			ok->clicked().connect(std::bind([=] () {
				dialog->accept();
				escolha1=sb1->currentIndex();
				insiraSinonimoDireto(escolha1,edit->text().toUTF8());
				sb2->clear();
				for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++){
					sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
				}
				sb2->setCurrentIndex(0);
			}));

			cancel->clicked().connect(dialog, &Wt::WDialog::reject);

			dialog->show();
		}));
			
		//Ao selecionar item de selectionbox1
/*		sb1->activated().connect(std::bind([=] () {
			escolha1=sb1->currentIndex();
			sb2->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++){
				sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
			}
		}));*/

		//Botao Save quando clicar botao.
		saveButton->clicked().connect(std::bind([=] () {
			//GRAVAR ARQUIVO
			FILE *fp;
			fp=fopen("produtos1.txt","w");
			for(int i=0;i<reg_medicamento.size();i++){
				fprintf(fp,"%s;%s;\n",reg_medicamento[i].codigo.c_str(),reg_medicamento[i].nome.c_str());
				for(int j=1;j<reg_medicamento[i].nome_alternativo.size();j++){
//					int s=reg_medicamento[i].nome_alternativo[j].length()-1;
//					if(reg_medicamento[i].nome_alternativo[j].at(s)!='\n')
					if(reg_medicamento[i].nome_alternativo[j].empty()==false){
						int s=reg_medicamento[i].nome_alternativo[j].length()-1;
						if(reg_medicamento[i].nome_alternativo[j].at(s)!='\n')
							fprintf(fp,"%s\n",reg_medicamento[i].nome_alternativo[j].c_str());
						else
							fprintf(fp,"%s",reg_medicamento[i].nome_alternativo[j].c_str());	
					}
				}
			}
			fclose(fp);
 		}));
    }


	
	//Verificar sinônimos
    void page1() {
		
        content()->addWidget(new WText("<h2>Verificar sinonimos</h2>"));
        content()->addWidget(new WText("<hr/>"));
        content()->addWidget(new WBreak());

		Wt::WContainerWidget *container1 = new Wt::WContainerWidget();
		Wt::WContainerWidget *container2 = new Wt::WContainerWidget();
		Wt::WContainerWidget *container3 = new Wt::WContainerWidget();
		Wt::WContainerWidget *container4 = new Wt::WContainerWidget();

		//Solution: increase the parameter max-request-size within wt_config.xml
/*		Wt::WFileUpload *fu1 = new Wt::WFileUpload(container1);
		fu1->setFileTextSize(500000000); 
		fu1->setProgressBar(new Wt::WProgressBar());
		fu1->setMargin(10, Wt::Right);*/
		
		fu2 = new Wt::WFileUpload(container2);
		fu2->stealSpooledFile();
		fu2->setFileTextSize(500000000); 
		fu2->setProgressBar(new Wt::WProgressBar());
		fu2->setMargin(10, Wt::Right);
		
		fu3 = new Wt::WFileUpload(container3);
		fu3->stealSpooledFile();
		fu3->setFileTextSize(500000000); 
		fu3->setProgressBar(new Wt::WProgressBar());
		fu3->setMargin(10, Wt::Right);

		// Provide a button to start uploading.
/*		Wt::WPushButton *uploadButton1 = new Wt::WPushButton("Send", container1);
		uploadButton1->setMargin(10, Wt::Left | Wt::Right);*/

		// Provide a button to start uploading.
		Wt::WPushButton *uploadButton2 = new Wt::WPushButton("Send", container2);
		uploadButton2->setMargin(10, Wt::Left | Wt::Right);

		// Provide a button to start uploading.
		Wt::WPushButton *uploadButton3 = new Wt::WPushButton("Send", container3);
		uploadButton3->setMargin(10, Wt::Left | Wt::Right);

		// Provide a button to process.
		container4->addWidget(new WBreak());
		Wt::WPushButton *processarButton = new Wt::WPushButton("Processar", container4);

		//Mostrar barra de progresso para acompanhar progresso de processamento
		Wt::WProgressBar *bar = new Wt::WProgressBar(container4);
		bar->setRange(0, 10);
		container4->addWidget(new WBreak());
		processarButton->setMargin(10, Wt::Left | Wt::Right);

		
		Wt::WText *txt4 = new WText("<strong>Lista de medicamentos</strong>",container4);
		container4->addWidget(new WBreak());

//		Wt::WText *out1 = new Wt::WText(container1);
		Wt::WText *out2 = new Wt::WText(container2);
		Wt::WText *out3 = new Wt::WText(container3);

		// Upload when the button is clicked.
/*		uploadButton1->clicked().connect(std::bind([=] () {
			fu1->upload();
		}));*/

		// Upload when the button is clicked.
		uploadButton2->clicked().connect(std::bind([=] () {
			fu2->upload();
		}));

		// Upload when the button is clicked.
		uploadButton3->clicked().connect(std::bind([=] () {
			fu3->upload();
		}));
		
		//Caixa de selecao1
		Wt::WSelectionBox *sb1 = new Wt::WSelectionBox(container4);
		sb1->setWidth(600); 
		container4->addWidget(new WBreak());
		Wt::WText *out4 = new Wt::WText(container4);
		container4->addWidget(new WBreak());
		Wt::WText *txt40 = new WText("<strong>Sinonimos&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Sinonimos candidatos</strong>",container4);
		container4->addWidget(new WBreak());

		//Caixa de selecao2
		Wt::WSelectionBox *sb2 = new Wt::WSelectionBox(container4);

		// Provide a remove button.
		Wt::WPushButton *removeButton= new Wt::WPushButton(">>", container4);
		removeButton->setEnabled(true);

		// Provide an add button.
		Wt::WPushButton *addButton = new Wt::WPushButton("<<", container4);
		addButton->setEnabled(true);

		//Caixa de selecao3
		Wt::WSelectionBox *sb3 = new Wt::WSelectionBox(container4);
		Wt::WBreak *brk45= new WBreak(container4);

		sb2->setWidth(255);
		sb3->setWidth(255); 

		//Remove item >> when the button is clicked.
		removeButton->clicked().connect(std::bind([=] () {
			escolha1=sb1->currentIndex();
			cout<<sb2->currentText().toUTF8()<<endl;
			retiraSinonimo(escolha1,sb2->currentText().toUTF8());
			
			sb2->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++){
				sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
			}
			sb3->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo_out.size();j++){
				sb3->addItem(reg_medicamento[escolha1].nome_alternativo_out[j]);
			}
			sb2->setCurrentIndex(0);
		}));

		//Add item << when the button is clicked.
		addButton->clicked().connect(std::bind([=] () {
			escolha1=sb1->currentIndex();
			cout<<sb3->currentText().toUTF8()<<endl;
			insiraSinonimo(escolha1,sb3->currentText().toUTF8());
			
			sb2->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++){
				sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
			}
			sb3->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo_out.size();j++){
				sb3->addItem(reg_medicamento[escolha1].nome_alternativo_out[j]);
			}
			sb2->setCurrentIndex(0);
		}));

		//Ao selecionar item de selectionbox1
		sb1->activated().connect(std::bind([=] () {
			out4->setText(Wt::WString::fromUTF8("Voce escolheu: {1}").arg(sb1->currentIndex()));
			escolha1=sb1->currentIndex();
			sb3->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo_out.size();j++){
				sb3->addItem(reg_medicamento[escolha1].nome_alternativo_out[j]);
			}
			sb2->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++){
				sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
			}
			removeButton->setEnabled(true);
			addButton->setEnabled(true);
		}));

		// React to a succesfull upload.+fu->spoolFileName()
/*		fu1->uploaded().connect(std::bind([=] () {
			uploadButton1->disable();
			out1->setText("File uploaded: "+fu1->spoolFileName());
		}));*/

		// React to a succesfull upload.+fu->spoolFileName()
		fu2->uploaded().connect(std::bind([=] () {
			uploadButton2->disable();
			out2->setText("File uploaded: "+fu2->spoolFileName());
		}));

		// React to a succesfull upload.+fu->spoolFileName()
		fu3->uploaded().connect(std::bind([=] () {
			uploadButton3->disable();
			out3->setText("File uploaded: "+fu3->spoolFileName());
		}));

		// React to a file upload problem.
/*		fu1->fileTooLarge().connect(std::bind([=] () {
			out1->setText("File is too large.");
		}));*/
		
		// React to a file upload problem.
		fu2->fileTooLarge().connect(std::bind([=] () {
			out2->setText("File is too large.");
		}));
		
		// React to a file upload problem.
		fu3->fileTooLarge().connect(std::bind([=] () {
			out3->setText("File is too large.");
		}));
		
/*		content()->addWidget(new WText("<strong>Arquivo de medicamentos:</strong>"));
		content()->addWidget(container1);
		content()->addWidget(new WBreak());*/
		content()->addWidget(new WText("<strong>Arquivo de prescricoes:</strong>"));
		content()->addWidget(container2);
		content()->addWidget(new WBreak());
		content()->addWidget(new WText("<strong>Arquivo de anotacoes:</strong>"));
		content()->addWidget(container3);
		content()->addWidget(new WBreak());
		content()->addWidget(container4);
		container4->addWidget(new WBreak());

		// Provide a Save button.
		Wt::WPushButton *saveButton= new Wt::WPushButton("Salvar", container4);
		saveButton->setEnabled(false);

		//Botao Save quando clicar botao.
		saveButton->clicked().connect(std::bind([=] () {
			FILE *fp;
			fp=fopen("produtos1.txt","w");
			for(int i=0;i<reg_medicamento.size();i++){
				fprintf(fp,"%s;%s;\n",reg_medicamento[i].codigo.c_str(),reg_medicamento[i].nome.c_str());
				for(int j=0;j<reg_medicamento[i].nome_alternativo.size();j++){
//					int s=reg_medicamento[i].nome_alternativo[j].length()-1;
//					if(reg_medicamento[i].nome_alternativo[j].at(s)!='\n')
					if(reg_medicamento[i].nome_alternativo[j].empty()==false){
						int s=reg_medicamento[i].nome_alternativo[j].length()-1;
						if(reg_medicamento[i].nome_alternativo[j].at(s)!='\n')
							fprintf(fp,"%s\n",reg_medicamento[i].nome_alternativo[j].c_str());
						else
							fprintf(fp,"%s",reg_medicamento[i].nome_alternativo[j].c_str());	
					}
				}
			}
			fclose(fp);
 		}));

    	frequencia="frequencia.txt";
//    	evolucao="evolucao.txt";
//     	prescricao="extracao_prescricao.txt";
//     	medicamentos="produtos.txt";
		

		//Botao Processar quando clicar botao.
		processarButton->clicked().connect(std::bind([=] () {
			frequencia="frequencia.txt";
			evolucao=fu3->spoolFileName();
			prescricao=fu2->spoolFileName();
			medicamentos="produtos1.txt";
			//medicamentos=fu1->spoolFileName();
			reg_medicamento.clear();
			reg_prescricao.clear();
			reg_evolucao.clear();
			reg_sinonimo.clear();
			reg_frequencia.clear();

			carrega_base_medicamentos(medicamentos);
			construa_vetor_frequencia(frequencia);
			construa_vetor_evolucao(evolucao);
			construa_vetor_prescricao(prescricao);
			construa_vetor_sinonimos("soundex_igualdades.txt");
			//Posteriormente usar somente reg_medicamento
			
			//Populate selectionbox1
			for(int i=0;i<reg_medicamento.size();i++){
				sb1->addItem(reg_medicamento[i].nome);
			}
			
			sb1->setCurrentIndex(0); 
			sb1->setSelectionMode(Wt::SingleSelection);
			escolha1=sb1->currentIndex();
			
			//Populate selectionbox2
			sb2->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo.size();j++){
				sb2->addItem(reg_medicamento[escolha1].nome_alternativo[j]);
			}

			//Populate selectionbox3
			sb3->clear();
			for(int j=0;j<reg_medicamento[escolha1].nome_alternativo_out.size();j++){
				sb3->addItem(reg_medicamento[escolha1].nome_alternativo_out[j]);
			}
			saveButton->setEnabled(true);
 		}));
	}
};

WApplication* createApplication(const WEnvironment &env) {
    return new FormHospital(env);
}

int main(int argc, char** argv) {
	
    return WRun(argc, argv, &createApplication);
}