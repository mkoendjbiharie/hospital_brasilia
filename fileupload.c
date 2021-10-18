#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>
#include <Wt/WText>

Wt::WContainerWidget *container = new Wt::WContainerWidget();

Wt::WFileUpload *fu = new Wt::WFileUpload(container);
fu->setFileTextSize(500000); // Set the maximum file size to 500000 kB.
fu->setProgressBar(new Wt::WProgressBar());
fu->setMargin(10, Wt::Right);

// Provide a button to start uploading.
Wt::WPushButton *uploadButton = new Wt::WPushButton("Send", container);
uploadButton->setMargin(10, Wt::Left | Wt::Right);

Wt::WText *out = new Wt::WText(container);

// Upload when the button is clicked.
uploadButton->clicked().connect(std::bind([=] () {
    fu->upload();
    uploadButton->disable();
}));

// Upload automatically when the user entered a file.
fu->changed().connect(std::bind([=] () {
    fu->upload();
    uploadButton->disable();
    out->setText("File upload is changed.");
}));

// React to a succesfull upload.
fu->uploaded().connect(std::bind([=] () {
    out->setText("File upload is finished.");
}));

// React to a file upload problem.
fu->fileTooLarge().connect(std::bind([=] () {
    out->setText("File is too large.");
}));