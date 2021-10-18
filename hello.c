#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include "hospital.h"

class HelloApplication : public Wt::WApplication
{
public:
    HelloApplication(const Wt::WEnvironment& env);

private:
    Wt::WLineEdit *nameEdit_;
    Wt::WText *greeting_;

    void greet();
};

HelloApplication::HelloApplication(const Wt::WEnvironment& env)
    : Wt::WApplication(env)
{
    setTitle("Fawaka Marlon");

    root()->addWidget(new Wt::WText("Schrijf je naam"));
    nameEdit_ = new Wt::WLineEdit(root());
    Wt::WPushButton *button = new Wt::WPushButton("Groet me.", root());
    root()->addWidget(new Wt::WBreak());
    greeting_ = new Wt::WText(root());
    button->clicked().connect(this, &HelloApplication::greet);
}

void HelloApplication::greet()
{
    greeting_->setText("Fawaka, " + nameEdit_->text() + " mi matie!");
}

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new HelloApplication(env);
}

int main(int argc, char **argv)
{
	construa_vetor_frequencia();
    construa_vetor_evolucao();
    construa_vetor_prescricao();
    carrega_base_de_sinonimos();
    verifica_torre_evolucao();

    return Wt::WRun(argc, argv, &createApplication);
}