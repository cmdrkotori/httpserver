#include <QFile>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), http(nullptr)
{
    ui->setupUi(this);
    http.route("/test-image.png", [](HttpRequest &req, HttpResponse &res) {
        (void)req;
        res.serveFile(":/test-image.png");
    });
    http.route("/postsend", [](HttpRequest &req, HttpResponse &res) {
        (void)req;
        res.content = "\
              <form action=\"/varprint\" method=\"POST\">\
                <div>\
                  <label for=\"say\">What greeting do you want to say?</label>\
                  <input name=\"say\" id=\"say\" value=\"Hi\">\
                </div>\
                <div>\
                  <label for=\"to\">Who do you want to say it to?</label>\
                  <input name=\"to\" id=\"to\" value=\"Mom\">\
                </div>\
                <div>\
                  <button>Send my greetings</button>\
                </div>\
              </form>";
    });
    http.route("/varprint", [](HttpRequest &req, HttpResponse& res) {
        res.content = QString("%1 received").arg(req.method).toUtf8();

        if (!req.getVars.isEmpty())
            res.content.append("<p>Getvars follow:");
        for (auto i = req.getVars.constBegin(); i != req.getVars.constEnd(); i++)
            res.content.append(QString("<br>%1 = %2").arg(i.key(), i.value()).toUtf8());

        if (!req.postVars.isEmpty())
            res.content.append("<p>Postvars follow:");
        for (auto i = req.postVars.constBegin(); i != req.postVars.constEnd(); i++)
            res.content.append(QString("<br>%1 = %2").arg(i.key(), i.value()).toUtf8());
    });
    http.route("/", [](HttpRequest &req, HttpResponse &res) {
        (void)req;
        res.content = "Hello, World!";
    });
    http.listen(QHostAddress::Any, 8080);
}

MainWindow::~MainWindow()
{
    delete ui;
}

