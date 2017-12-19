#include<includes.h>
#include<MDirectoryItem.h>
#include<Login.h>
int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    /*
    QImage *image=new QImage("/home/rubikchen/Desktop/dog.jpg");
    QLabel *label=new QLabel;
    label->resize(100,100);
    label->setPixmap(QPixmap::fromImage(*image));
    label->setScaledContents(true);
    label->show();

    Window win;
    win.show();
    */
    /*
    FileItem *item = new FileItem;
    item->show();
    */
    /*
    FlowLayout * layout= new FlowLayout;
    layout->addWidget(new FileItem);
    layout->addWidget(new FileItem);
    layout->addWidget(new FileItem);
    layout->addWidget(new FileItem);
    layout->addWidget(new FileItem);
    layout->addWidget(new FileItem);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    widget->setMinimumSize(200,500);
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QScrollArea *s = new QScrollArea;
    s->setGeometry(0, 0, 200, 200);
    //垂直滚动条不可见，只能通过鼠标滑动
    s->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    s->setWidget(widget);
    //s->resize(widget->width(),widget->height());
    s->setWidgetResizable(true);
    s->setMinimumSize(430,500);
    s->show();
    //widget->show();
    */
    /*
    QString filename = QFileDialog::getOpenFileName(NULL,"open file",QDir::currentPath());
    printf("hello %s\n",filename.toLatin1().data());
    */

    /*
    QWidget *widget = new QWidget;
    QHBoxLayout * container = new QHBoxLayout;
    Login * log = new Login(widget);
    QPushButton *button = new QPushButton("test");
    container->addWidget(button);
    container->addWidget(log);
    container->setStretch(1,3);
    container->setStretch(2,1);
    widget->setLayout(container);
    widget->setMinimumSize(600,600);
    widget->show();
*/

    Login log;
    log.show();
/*

    QWidget *widget=new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    QIcon icon(":/new/png/fileIcon.png");
    */
    MFileInfo file(":/new/png","fileIcon.png");
    /* load from qbytearray*/
    /*    QImage image(":/new/png/fileIcon.png");
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer,"png",20);
    QIcon icon(":/new/png/fileIcon.png");
    MItem * item = new DirectoryItem(file,ba);


    layout->addWidget(item);
     load from icon
    MItem *items = new MItem(file,icon);
    layout->addWidget(items);
    widget->setMinimumSize(200,200);
    widget->setLayout(layout);
    widget->show();
*/
    //images/web/newimgs/newbanner.jpg
    /*
    QStringList infolist = QDir("/home/rubikchen").entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for(int i=0; i<infolist.size(); i++)
        printf("%s\n", ((QString)infolist.at(i)).toLatin1().data() );

#include<QFileInfoList>
#include<QFileInfo>
    QFileInfoList infoList = QDir("/home/rubikchen/Desktop").entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for(int i=0; i<infoList.size(); i++)
        printf("%s %s %d\n", ((QFileInfo)infoList.at(i)).path().toLatin1().data(),
               ((QFileInfo)infoList.at(i)).baseName().toLocal8Bit().data(),
              ((QFileInfo)infoList.at(i)).isDir() );
*/
    /*
    QString str("/home/rubikchen/Desektop/sql.c");
    int x = str.lastIndexOf('/')+1;
    printf("index:%s %s\n",str.right(str.length()-x).toLatin1().data(),str.left(x).toLatin1().data());

    QLayout* layout = new MHBoxLayout;
    QPushButton * bt0 = new QPushButton("aaa");
    QPushButton * bt1 = new QPushButton("abb");
    layout->addWidget(bt0);
    layout->addWidget(bt1);
    QObject::connect(bt0,SIGNAL(clicked(bool)),layout,SLOT(hideAll()));
    QWidget widget ;
    widget.setLayout(layout);
    widget.show();
*/
    return a.exec();
}
