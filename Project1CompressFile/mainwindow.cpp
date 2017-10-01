/****************************************************************************
Modified: hunght
Date: 19-09-2017
Email: hunght.thaibinh@gmail.com
****************************************************************************/

#include <QtWidgets>
#include <QtWebEngineWidgets>
#include "mainwindow.h"

template<typename Arg, typename R, typename C>
struct InvokeWrapper {
    R *receiver;
    void (C::*memberFun)(Arg);
    void operator()(Arg result) {
        (receiver->*memberFun)(result);
    }
};

template<typename Arg, typename R, typename C>
InvokeWrapper<Arg, R, C> invoke(R *receiver, void (C::*memberFun)(Arg))
{
    InvokeWrapper<Arg, R, C> wrapper = {receiver, memberFun};
    return wrapper;
}

MainWindow::MainWindow(const QUrl& url)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    progress = 0;

    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    jQuery.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
    file.close();

    view = new QWebEngineView(this);
//    view->load(url);

    mngData = new ManageDataFromUrl(view, url);
    m_thread = new QThread();
//    mngData->moveToThread(m_thread);
    connect(m_thread, SIGNAL(started()), mngData, SLOT(loadTheFirstUrl()));
    m_thread->start();


    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebEnginePage::Back));
    toolBar->addAction(view->pageAction(QWebEnginePage::Forward));
    toolBar->addAction(view->pageAction(QWebEnginePage::Reload));
    toolBar->addAction(view->pageAction(QWebEnginePage::Stop));
    toolBar->addWidget(locationEdit);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QAction* viewSourceAction = new QAction("Page Source", this);
    connect(viewSourceAction, SIGNAL(triggered()), SLOT(viewSource()));
    viewMenu->addAction(viewSourceAction);

    QMenu *effectMenu = menuBar()->addMenu(tr("&Effect"));
    effectMenu->addAction("Highlight all links", this, SLOT(highlightAllLinks()));

    rotateAction = new QAction(this);
    rotateAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    rotateAction->setCheckable(true);
    rotateAction->setText(tr("Turn images upside down"));
    connect(rotateAction, SIGNAL(toggled(bool)), this, SLOT(rotateImages(bool)));
    effectMenu->addAction(rotateAction);

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(tr("Remove GIF images"), this, SLOT(removeGifImages()));
    toolsMenu->addAction(tr("Remove all inline frames"), this, SLOT(removeInlineFrames()));
    toolsMenu->addAction(tr("Remove all object elements"), this, SLOT(removeObjectElements()));
    toolsMenu->addAction(tr("Remove all embedded elements"), this, SLOT(removeEmbeddedElements()));
    toolsMenu->addAction(tr("Test"), this, SLOT(removeEmbeddedElements2()));

    setCentralWidget(view);
}

void MainWindow::viewSource()
{
    QTextEdit* textEdit = new QTextEdit(NULL);
    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    textEdit->adjustSize();
    textEdit->move(this->geometry().center() - textEdit->rect().center());
    textEdit->show();

    view->page()->toHtml(invoke(textEdit, &QTextEdit::setPlainText));
}

void MainWindow::adjustLocation()
{
    locationEdit->setText(view->url().toString());
}

void MainWindow::changeLocation()
{
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    view->load(url);
    view->setFocus();
}

void MainWindow::adjustTitle()
{
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

void MainWindow::finishLoading(bool)
{
    progress = 100;
    adjustTitle();
    view->page()->runJavaScript(jQuery);

    rotateImages(rotateAction->isChecked());
}

void MainWindow::highlightAllLinks()
{
    QString code = "qt.jQuery('a').each( function () { qt.jQuery(this).css('background-color', 'yellow') } ); undefined";
    view->page()->runJavaScript(code);
}

void MainWindow::rotateImages(bool invert)
{
    QString code;

    if (invert)
        code = "qt.jQuery('img').each( function () { qt.jQuery(this).css('-webkit-transition', '-webkit-transform 2s'); qt.jQuery(this).css('-webkit-transform', 'rotate(180deg)') } ); undefined";
    else
        code = "qt.jQuery('img').each( function () { qt.jQuery(this).css('-webkit-transition', '-webkit-transform 2s'); qt.jQuery(this).css('-webkit-transform', 'rotate(0deg)') } ); undefined";
    view->page()->runJavaScript(code);
}

void MainWindow::removeGifImages()
{
    QString code = "qt.jQuery('[src*=gif]').remove()";
    view->page()->runJavaScript(code);
}

void MainWindow::removeInlineFrames()
{
    QString code = "qt.jQuery('iframe').remove()";
    view->page()->runJavaScript(code);
}

void MainWindow::removeObjectElements()
{
    QString code = "qt.jQuery('object').remove()";
    view->page()->runJavaScript(code);
}

void MainWindow::removeEmbeddedElements()
{
    QString code = "qt.jQuery('embed').remove()";
    view->page()->runJavaScript(code);
}

void MainWindow::removeEmbeddedElements2()
{
    QString code = "jQuery('#productTitle').text().trim();";
    view->page()->runJavaScript(code,
                                [] (const QVariant &result) {
                                qDebug() << result;
                                });

//    runJavaScript("return getValue()", [&](const QVariant& result){ callback(result.toString()); });


//    webFrame->runJavaScript("function myFunction() {"
//    "var Row = document.getElementById('GridView1');var Cells = Row.getElementsByTagName('td');"
//    "return Cells[0].innerText;} myFunction();",
//    [] (const QVariant &result) {
//    qDebug()<<result.toString();
//    });
}
