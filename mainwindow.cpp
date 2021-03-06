#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QToolButton>
#include <QColorDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QCheckBox>
#include <QTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidgetAction>
#include <QClipboard>
#include <QImage>
#include <QScrollArea>
#include <QTableWidget>
#include <QLabel>
#include <QMimeData>
#include <QTextBrowser>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("QScrollArea{ background-color:#cccccc; }");
    //状态栏
    LSB1 = new QLabel("欢迎使用海天鹰画图！");
    LSB1->setMinimumSize(500,20);
    LSB1->setStyleSheet("padding:0px 3px;");
    //LSB1->setFrameShape(QFrame::WinPanel);
    //LSB1->setFrameShadow(QFrame::Sunken);
    LSB2 = new QLabel("");
    LSB2->setMinimumSize(150,20);
    LSB2->setStyleSheet("padding:0px 3px;");
    LSB2->setAlignment(Qt::AlignCenter);
    //LSB2->setFrameShape(QFrame::WinPanel);
    //LSB2->setFrameShadow(QFrame::Sunken);
    ui->statusBar->addWidget(LSB1);
    ui->statusBar->addWidget(LSB2);

    text = "文字内容";
    path = "";
    move((QApplication::desktop()->width() - width())/2, (QApplication::desktop()->height() - height())/2);
    imageWidget = new ImageWidget;
    connect(imageWidget, SIGNAL(statusbar2Message(QString)), LSB2, SLOT(setText(QString)));
    connect(imageWidget, SIGNAL(pick(QColor)), this, SLOT(setPicker(QColor)));
    scrollArea = new QScrollArea;
    scrollArea->setWidget(imageWidget);
    scrollArea->widget()->setMinimumSize(600,500);
    setCentralWidget(scrollArea);

    ui->action_new->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->action_open->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->action_save->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->action_quit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->action_undo->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    ui->action_quit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->action_setWallpaper->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    ui->action_about->setIcon(style()->standardIcon(QStyle::SP_DialogHelpButton));

    btnColorBorder = new QToolButton(this);
    btnColorBorder->setText("□");
    btnColorBorder->setToolTip("边框");
    ui->mainToolBar->addWidget(btnColorBorder);
    checkBorder = new QCheckBox(this);
    checkBorder->setCheckState(Qt::Checked);
    ui->mainToolBar->addWidget(checkBorder);
    btnColorFill = new QToolButton(this);
    btnColorFill->setText("■");
    btnColorFill->setToolTip("填充");
    ui->mainToolBar->addWidget(btnColorFill);
    checkFill = new QCheckBox(this);
    ui->mainToolBar->addWidget(checkFill);
    spinbox = new QSpinBox(this);
    spinbox->setSingleStep(1);
    spinbox->setRange(1,100);
    spinbox->setValue(1);
    ui->mainToolBar->addWidget(spinbox);
    labelFont = new QLabel;
    ui->actionFont->setText(labelFont->font().family() + "," + QString::number(labelFont->font().pointSize()));
    lineEdit = new QLineEdit(text,this);
    lineEdit->setFixedWidth(60);
    ui->mainToolBar->addWidget(lineEdit);
    imageWidget->text = text;

    connect(ui->action_new,SIGNAL(triggered(bool)),imageWidget,SLOT(newfile()));
    connect(ui->action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->action_selectAll,SIGNAL(triggered(bool)),imageWidget,SLOT(selectAll()));
    connect(ui->action_delete,SIGNAL(triggered(bool)),imageWidget,SLOT(delSelect()));
    connect(ui->actionPencil,SIGNAL(triggered(bool)),imageWidget,SLOT(drawPoint()));
    connect(ui->actionLine,SIGNAL(triggered(bool)),imageWidget,SLOT(drawLine()));
    connect(ui->actionArrow,SIGNAL(triggered(bool)),imageWidget,SLOT(drawArrow()));
    connect(ui->actionRect,SIGNAL(triggered(bool)),imageWidget,SLOT(drawRect()));
    connect(ui->actionEllipse,SIGNAL(triggered(bool)),imageWidget,SLOT(drawEllipse()));
    connect(ui->actionText,SIGNAL(triggered(bool)),imageWidget,SLOT(drawText()));
    connect(ui->actionFill,SIGNAL(triggered(bool)),imageWidget,SLOT(drawFill()));
    connect(ui->actionLine,SIGNAL(triggered(bool)),imageWidget,SLOT(drawLine()));
    connect(ui->actionErase,SIGNAL(triggered(bool)),imageWidget,SLOT(drawErase()));
    connect(ui->actionMove,SIGNAL(triggered(bool)),imageWidget,SLOT(drawMove()));
    connect(ui->actionRectselect,SIGNAL(triggered(bool)),imageWidget,SLOT(drawRectselect()));
    connect(ui->actionCutSelect,SIGNAL(triggered(bool)),imageWidget,SLOT(cutSelect()));
    connect(ui->actionColorPicker,SIGNAL(triggered(bool)),imageWidget,SLOT(colorPicker()));
    connect(ui->actionZoomin,SIGNAL(triggered(bool)),imageWidget,SLOT(zoomin()));
    connect(ui->actionZoomout,SIGNAL(triggered(bool)),imageWidget,SLOT(zoomout()));
    connect(ui->actionZoom1,SIGNAL(triggered(bool)),imageWidget,SLOT(zoom1()));

    connect(btnColorFill, SIGNAL(clicked()), this, SLOT(setColorFill()));
    connect(btnColorBorder, SIGNAL(clicked()), this, SLOT(setColorBorder()));
    connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(spinValueChange(int)));
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChange(QString)));
    connect(checkBorder, SIGNAL(stateChanged(int)), this, SLOT(checkBorderChanged(int)));
    connect(checkFill, SIGNAL(stateChanged(int)), this, SLOT(checkFillChanged(int)));

    QStringList Largs=QApplication::arguments();
    qDebug() << Largs;
    if(Largs.length()>1){
        path=Largs.at(1);
        open(path);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    //QRect childRect = scrollArea->childrenRect();
    //imageWidget->resize(childRect.size());
}

void MainWindow::on_action_changelog_triggered()
{
    QString s = "1.7\n(2017-11)\n颜色透明工具，取色后在边框色、填充色显示，超出边界清空鼠标位置信息。\n增加灰色背景，凸显绘图区域。\n优化代码。\n(2017-10)\n简化工具信号，简化setCursor()。\n(2017-09)\n增加箭头工具。\n增加抗锯齿。\n\n1.6\n(2017-07)\n更新日志消息窗口写不下了，改成带滚动条的文本框。\n自定义信号结合事件过滤器，把鼠标移动位置发送到主窗体信息栏。\n增加拖放打开文件。\n(2017-06)\n使用自定义信号解决子类发信息给主窗体状态栏问题，致谢rekols。\n(2017-05)\n右键打开文件记忆文件路径。\n\n1.5 (2017-04)\n透明色反色不改变。\n增加取色工具，橡皮擦颜色不再固定为白色，而是填充色。\n\n1.4 (2017-03)\n支持命令行打开文件和打开方式打开文件。\n修复鼠标指针引用本地文件，没引用资源文件，引起启动path参数改变不能加载图标的问题。\n菜单的SIGNAL-SLOT改为on_action_triggered()\n修复PNG图片裁剪丢失透明度问题。\n新建图像为透明图像。\n\n1.3 (2017-03)\n实现选区模糊。\n加入模糊滤镜。\n\n1.2 (2017-02)\n文件名显示在窗口标题栏。\n区别保存和另存为。\n增加导入图片。\n\n1.1 (2017-01)\n新增灰度、反色。\n\n1.0 (2017-01)\n解决删除选区后画不出选框的问题。\n恢复撤销。\n增加全选。\n实现选区或剪贴板移动！\n保存时自动获取打开文件的路径。\n增加按像素、比例缩放。\n实现在属性窗口设置画布大小。\n2016-12\n增加快捷键控制选框及其边框移动。\n绘图代码从MainWindow向imageWidget迁移。\n实现水平镜像、垂直镜像。\n实现放大、缩小、原始大小。\n为了增加滚动条，增加自定义imageWidget。\n状态栏显示绘图详情。\n复制选区到系统剪贴板，从系统剪贴板获取图像粘贴。\n优化颜色选择交互。\n增加撤销、重做功能，有BUG。\n设为壁纸。\n画选区，剪裁选区。\n新建图片，打开图片，保存图片。\n实现画点、线、框、圆、字。";
    QDialog *dialog = new QDialog;
    dialog->setWindowTitle("更新历史");
    dialog->setFixedSize(400,300);
    QVBoxLayout *vbox = new QVBoxLayout;
    QTextBrowser *textBrowser = new QTextBrowser;
    textBrowser->setText(s);
    textBrowser->zoomIn();
    vbox->addWidget(textBrowser);
    QHBoxLayout *hbox = new QHBoxLayout;
    QPushButton *btnConfirm = new QPushButton("确定");
    hbox->addStretch();
    hbox->addWidget(btnConfirm);
    hbox->addStretch();
    vbox->addLayout(hbox);
    dialog->setLayout(vbox);
    dialog->show();
    connect(btnConfirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    if(dialog->exec() == QDialog::Accepted){
        dialog->close();
    }
}

void MainWindow::on_action_aboutQt_triggered()
{
    QMessageBox::aboutQt(NULL, "关于 Qt");
}

void MainWindow::on_action_about_triggered()
{
    QMessageBox aboutMB(QMessageBox::NoIcon, "关于", "海天鹰画图 1.7\n一款基于Qt的画图程序。\n作者：黄颖\nE-mail: sonichy@163.com\n主页：sonichy.96.lt\n参考文献：\n绘图：\nhttp://tzc.is-programmer.com/posts/534.html\nhttp://lizhigg.love.blog.163.com/blog/static/62611781201222105550184/\n滚动条：http://down.51cto.com/data/233634\n灰度图：http://www.cnblogs.com/xianglan/archive/2010/12/24/1915905.html\n拖放打开文件：http://blog.csdn.net/rl529014/article/details/53057577");
    aboutMB.setIconPixmap(QPixmap(":/icon.png"));
    aboutMB.exec();
}

void MainWindow::setColorBorder()
{
    QPalette plt = btnColorBorder->palette();
    QBrush brushb = plt.color(QPalette::ButtonText);
    QColor color = QColorDialog::getColor(brushb.color(), this);
    imageWidget->pen.setColor(color);
    plt.setColor(QPalette::ButtonText, color);
    btnColorBorder->setPalette(plt);
}

void MainWindow::setColorFill()
{
    QPalette plt = btnColorFill->palette();
    QBrush brush = plt.color(QPalette::ButtonText);
    colorf = QColorDialog::getColor(brush.color(), this);    
    imageWidget->brush.setColor(colorf);
    plt.setColor(QPalette::ButtonText, colorf);
    btnColorFill->setPalette(plt);
}

void MainWindow::spinValueChange(int i)
{
    imageWidget->pen.setWidth(i);
    imageWidget->pen.setJoinStyle(Qt::MiterJoin);
}

void MainWindow::textChange(QString s)
{
    imageWidget->text=s;
}

void MainWindow::on_action_open_triggered()
{
    if(path==""){
        path = QFileDialog::getOpenFileName(this,"打开图片", ".", "图片文件(*.jpg *.jpeg *.png *.bmp)");
    }else{
        path = QFileDialog::getOpenFileName(this,"打开图片", path, "图片文件(*.jpg *.jpeg *.png *.bmp)");
    }    
    if(path.length() != 0){
        open(path);
    }
}

void MainWindow::open(QString path)
{
    imageWidget->load(path);
    filename = QFileInfo(path).fileName();
    setWindowTitle(filename+" - 海天鹰画图");
    LSB1->setText("打开 "+path);
    qDebug() << imageWidget->size();
}

void MainWindow::on_action_import_triggered(){
    QString filenamei="";
    if(path==""){
        filenamei = QFileDialog::getOpenFileName(this,"导入图片", ".", "图片文件(*.jpg *.png *.bmp)");
    }else{
        filenamei = QFileDialog::getOpenFileName(this,"导入图片", path, "图片文件(*.jpg *.png *.bmp)");
    }
    LSB1->setText("导入 "+filenamei);
    if(path.length()!=0){
        imageWidget->image=imageWidget->imgtemp;
        QImage imgImport(filenamei);
        QApplication::clipboard()->setImage(imgImport, QClipboard::Clipboard);
        imageWidget->paste();
        imageWidget->imgmove=imgImport;
        //imageWidget->imgtemp=imgImport;
        //imageWidget->imgbuf[0]=imgImport;
    }
}

void MainWindow::on_action_save_triggered(){
    qDebug() << "save path=" << path;
    if(path.length() != 0){
        LSB1->setText("保存 "+path);
        imageWidget->image=imageWidget->imgtemp;
        imageWidget->save(path);
    }else{
        on_action_saveas_triggered();
    }
}

void MainWindow::on_action_saveas_triggered()
{
    if(path==""){
        path = QFileDialog::getSaveFileName(this,"保存图片","./未命名.jpg","图片文件(*.jpg *.png *.bmp)");
    }else{
        path = QFileDialog::getSaveFileName(this,"保存图片",path,"图片文件(*.jpg *.png *.bmp)");
    }
    if(path.length() != 0){        
        LSB1->setText("保存 "+path);
        imageWidget->image=imageWidget->imgtemp;
        imageWidget->save(path);
        filename=QFileInfo(path).fileName();
        setWindowTitle(filename+" - 海天鹰画图");
    }
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, labelFont->font(), this, "选择字体");
    if(ok){
       labelFont->setFont(font);
       imageWidget->labelFont->setFont(font);
       ui->actionFont->setText(labelFont->font().family() + "," + QString::number(labelFont->font().pointSize()));
    }
}

void MainWindow::on_action_undo_triggered()
{
    imageWidget->undo();
}

void MainWindow::on_action_redo_triggered()
{
    imageWidget->redo();
}

void MainWindow::on_action_setWallpaper_triggered()
{
    QString str="gsettings set org.gnome.desktop.background picture-uri file://"+path;
    QByteArray ba = str.toLocal8Bit();
    qDebug() << ba.data();
    system(ba.data());
}

void MainWindow::on_action_property_triggered()
{
    //qDebug() << image.width() << "X" << image.height();
    QDialog *dialog=new QDialog(this);
    dialog->setWindowTitle("属性");
    QVBoxLayout *vbox=new QVBoxLayout;
    QHBoxLayout *hbox=new QHBoxLayout;
    QLabel *label=new QLabel("宽度：");
    QSpinBox *spinw=new QSpinBox;
    spinw->setRange(0,10000);
    spinw->setValue(imageWidget->imgtemp.width());
    hbox->addWidget(label,0,Qt::AlignCenter);
    hbox->addWidget(spinw);
    vbox->addLayout(hbox);
    label=new QLabel("高度：");
    QSpinBox *spinh=new QSpinBox;
    spinh->setRange(0,10000);
    spinh->setValue(imageWidget->imgtemp.height());
    hbox=new QHBoxLayout;
    hbox->addWidget(label,0,Qt::AlignCenter);
    hbox->addWidget(spinh);
    vbox->addLayout(hbox);
    QPushButton *btnConfirm=new QPushButton("确定");
    QPushButton *btnCancel=new QPushButton("取消");
    hbox=new QHBoxLayout;
    hbox->addWidget(btnConfirm);
    hbox->addWidget(btnCancel);
    vbox->addLayout(hbox);
    dialog->setLayout(vbox);
    connect(btnConfirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), dialog, SLOT(reject()));
    if(dialog->exec()==QDialog::Accepted){       
        imageWidget->newsize(spinw->value(),spinh->value());
    }
    dialog->close();
}

void MainWindow::on_action_excude_triggered()
{   int wo=imageWidget->imgtemp.width();
    int ho=imageWidget->imgtemp.height();
    QDialog *dialog=new QDialog(this);
    dialog->setWindowTitle("缩放");

    QVBoxLayout *vbox=new QVBoxLayout;
    QHBoxLayout *hbox=new QHBoxLayout;
    QLabel *label=new QLabel("宽度：");
    spinw=new QSpinBox;
    spinw->setRange(1,10000);
    spinw->setValue(wo);
    hbox->addWidget(label,0,Qt::AlignCenter);
    hbox->addWidget(spinw);
    label=new QLabel("px");
    hbox->addWidget(label);
    spinwr=new QSpinBox;
    spinwr->setRange(10,1000);
    spinwr->setValue(100);
    hbox->addWidget(spinwr);
    label=new QLabel("%");
    hbox->addWidget(label);
    vbox->addLayout(hbox);

    hbox=new QHBoxLayout;
    label=new QLabel("高度：");
    hbox->addWidget(label,0,Qt::AlignCenter);
    spinh=new QSpinBox;
    spinh->setRange(1,10000);
    spinh->setValue(ho);
    hbox->addWidget(spinh);
    label=new QLabel("px");
    hbox->addWidget(label);
    spinhr=new QSpinBox;
    spinhr->setRange(10,1000);
    spinhr->setValue(100);
    hbox->addWidget(spinhr);
    label=new QLabel("%");
    hbox->addWidget(label);
    vbox->addLayout(hbox);

    QPushButton *btnConfirm=new QPushButton("确定");
    QPushButton *btnCancel=new QPushButton("取消");
    hbox=new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(btnConfirm);
    hbox->addWidget(btnCancel);
    hbox->addStretch();
    vbox->addLayout(hbox);
    dialog->setLayout(vbox);

    connect(spinw, SIGNAL(valueChanged(int)), this, SLOT(onSpinwChanged(int)));
    connect(spinh, SIGNAL(valueChanged(int)), this, SLOT(onSpinhChanged(int)));
    connect(spinwr, SIGNAL(valueChanged(int)), this, SLOT(onSpinwrChanged(int)));
    connect(spinhr, SIGNAL(valueChanged(int)), this, SLOT(onSpinhrChanged(int)));
    connect(btnConfirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), dialog, SLOT(reject()));
    if(dialog->exec()==QDialog::Accepted){
        imageWidget->scale(spinw->value(),spinh->value());
    }
    dialog->close();
}

void MainWindow::onSpinwChanged(int i)
{
    disconnect(spinwr, SIGNAL(valueChanged(int)), this, SLOT(onSpinwrChanged(int)));
    spinwr->setValue(i*100/imageWidget->image.width());
    connect(spinwr, SIGNAL(valueChanged(int)), this, SLOT(onSpinwrChanged(int)));
}

void MainWindow::onSpinhChanged(int i)
{
    disconnect(spinhr, SIGNAL(valueChanged(int)), this, SLOT(onSpinhrChanged(int)));
    spinhr->setValue(i*100/imageWidget->image.height());
    connect(spinhr, SIGNAL(valueChanged(int)), this, SLOT(onSpinhrChanged(int)));
}

void MainWindow::onSpinwrChanged(int i)
{
    disconnect(spinw, SIGNAL(valueChanged(int)), this, SLOT(onSpinwChanged(int)));
    spinw->setValue(imageWidget->image.width()*i/100);
    connect(spinw, SIGNAL(valueChanged(int)), this, SLOT(onSpinwChanged(int)));
}

void MainWindow::onSpinhrChanged(int i)
{
    disconnect(spinh, SIGNAL(valueChanged(int)), this, SLOT(onSpinhChanged(int)));
    spinh->setValue(imageWidget->image.height()*i/100);
    connect(spinh, SIGNAL(valueChanged(int)), this, SLOT(onSpinhChanged(int)));
}

void MainWindow::on_action_copy_triggered()
{
    if(imageWidget->draw_type==imageWidget->SELECT_DRAW){
        imageWidget->copy();
        LSB1->setText("选区已复制到剪贴板");
    }
}

void MainWindow::on_action_paste_triggered()
{
    imageWidget->paste();
    LSB1->setText("剪贴板已粘贴");
}


void MainWindow::on_actionRotateLeft_triggered()
{
    imageWidget->rotate(-90);
}

void MainWindow::on_actionRotateRight_triggered()
{
    imageWidget->rotate(90);
}

void MainWindow::on_actionMirrorHorizontal_triggered()
{
    imageWidget->mirror(true,false);
}

void MainWindow::on_actionMirrorVertical_triggered()
{
    imageWidget->mirror(false,true);
}

void MainWindow::on_action_help_triggered()
{
    QMessageBox MBHelp(QMessageBox::NoIcon, "帮助", "选区上移\tAlt + ↑\n选区下移\tAlt + ↓\n选区左移\tAlt + ←\n选区右移\tAlt + →\n选区上边上移\tCtrl + ↑\n选区上边下移\tCtrl + ↓\n选区左边左移\tCtrl + ←\n选区左边右移\tCtrl + →\n选区下边上移\tShift + ↑\n选区下边下移\tShift + ↓\n选区右边左移\tShift + ←\n选区右边右移\tShift + →");
    MBHelp.exec();
}

void MainWindow::checkBorderChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        imageWidget->boolBorder=true;
    }
    else if(state == Qt::PartiallyChecked) // "半选"
    {

    }
    else // 未选中 - Qt::Unchecked
    {
        imageWidget->boolBorder=false;
    }
}

void MainWindow::checkFillChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
       imageWidget->boolFill=true;
    }
    else if(state == Qt::PartiallyChecked) // "半选"
    {

    }
    else // 未选中 - Qt::Unchecked
    {
       imageWidget->boolFill=false;
    }
}

void MainWindow::on_action_gray_triggered()
{
    imageWidget->gray();
}

void MainWindow::on_action_invert_triggered()
{
    imageWidget->invert();
}

void MainWindow::on_action_transparent_triggered()
{
    imageWidget->transparent();
}

void MainWindow::on_action_blur_triggered()
{
    imageWidget->blur(spinbox->value());
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    //if(e->mimeData()->hasFormat("text/uri-list")) //只能打开文本文件
        e->acceptProposedAction(); //可以在这个窗口部件上拖放对象
}

void MainWindow::dropEvent(QDropEvent *e) //释放对方时，执行的操作
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return ;

    QString fileName = urls.first().toLocalFile();

    foreach (QUrl u, urls) {
        qDebug() << u.toString();
    }
    qDebug() << urls.size();

    if(fileName.isEmpty())
        return;

    open(fileName);
}

void MainWindow::setPicker(QColor color)
{
    QPalette plt = btnColorBorder->palette();
    plt.setColor(QPalette::ButtonText, color);
    btnColorBorder->setPalette(plt);

    plt = btnColorFill->palette();
    plt.setColor(QPalette::ButtonText, color);
    btnColorFill->setPalette(plt);
}
