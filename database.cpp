#include "database.h"
#include<QSqlError>
#include<mainwindow.h>
#include<QString>
#include<QVariantList>
#include<QDebug>
#include <QDateTime>
#include <QTimer>
#include <QDesktopWidget>
#include<QFileDialog>
database::database(QObject *parent) : QObject(parent){ }
void database::run(){
    SetWindowLayout();
    if(!connectdb("demo")){
        QMessageBox::critical(window,"Error","连接数据库失败，请检查数据库连接！",QMessageBox::Yes);
    }
}
database::~database(){ }
void database::SetWindowLayout(){
    //添加计时器 3-26
    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&database::currentTime);
    timer->start(1000);
    SetMainWindow();
    SetCoreWindow();
}
void database::SetMainWindow(){
    window=new QWidget();
    window->setWindowTitle(tr("BOOK MANAGER"));
    registerButton=new QPushButton(tr("注册"));
    loginUserButton=new QPushButton(tr("用户登录"));
    loginManagerButton=new QPushButton(tr("管理员登录"));
    registerButton->setFixedSize(QSize(60,30));//如果不设置在总窗口变化大小时会跟着变化
    loginUserButton->setFixedSize(QSize(90,30));
    loginManagerButton->setFixedSize(QSize(100,30));

    hboxLayout01=new QHBoxLayout();
    hboxLayout00=new QHBoxLayout();
    hboxLayout02=new QHBoxLayout();
    vboxLayout00=new QVBoxLayout();
    vboxLayout01=new QVBoxLayout();
    spaceLabel00=new QLabel();
    spaceLabel01=new QLabel("\n\n");
    spaceLabel02=new QLabel();
    titleBar=new QWidget();
    searchWindowTab=new QTabWidget();
    logoutButton = NULL;
    currentUser = VISITOR;
    spaceLabel00->setText("您好！登录状态：未登录");
    hboxLayout00->addWidget(spaceLabel00);//左边加了一小条空白 ->改成了登录状态 3-26
    hboxLayout00->addStretch();//加这句更新后界面不会乱掉 3-26
    hboxLayout00->addWidget(loginUserButton);
    hboxLayout00->addWidget(loginManagerButton);
    hboxLayout00->addWidget(registerButton);
    hboxLayout02->addStretch();
    hboxLayout02->addWidget(spaceLabel02);//时间显示 3-26

    vboxLayout00->addWidget(spaceLabel01);
    vboxLayout00->addLayout(hboxLayout00);
    vboxLayout00->addLayout(hboxLayout02);

    hboxLayout01->addLayout(vboxLayout00);

    titleBar->setLayout(hboxLayout01);

    vboxLayout01->addWidget(titleBar);
    vboxLayout01->addWidget(searchWindowTab);
    window->setLayout(vboxLayout01);
    window->setFixedSize(1024*1.2,768*1.2);//设置总窗口的大小
    window->show();

    connect(registerButton,&QPushButton::clicked,this,&database::showRegisterWindow);
    connect(loginManagerButton,&QPushButton::clicked,this,&database::showLoginManagerWindow);
    connect(loginUserButton,&QPushButton::clicked,this,&database::showLoginUserWindow);
}
void database::SetCoreWindow(){
    const int bookTypeNumber=22;
    const int bookSearchNumber=8;
    //书号, 类别, 书名, 出版社, 年份, 作者, 价格（区间）0-2000, 总藏书量（区间）0-50, 库存（区间）0-5000
    const int maxPrice=2000;
//    const int maxBookNumber=50;
//    const int maxStock=5000;
    const int bookMessageNumber=9;
    //书号, 类别, 书名, 出版社, 年份, 作者, 价格, 总藏书量, 库存
//    const int rowsEachPage=10;
    searchBookWindow=new QWidget();
    hboxLayout30=new QHBoxLayout();
    hboxLayout31=new QHBoxLayout();
    hboxLayout32=new QHBoxLayout();
    hboxLayout33=new QHBoxLayout();
    hboxLayout34=new QHBoxLayout();
    hboxLayout35=new QHBoxLayout();
    vboxLayout30=new QVBoxLayout();
    vboxLayout31=new QVBoxLayout();
    //vboxLayout32=new QVBoxLayout();
    gridLayout30=new QGridLayout();
    gridLayout31=new QGridLayout();
    groupBox30=new QGroupBox();
    groupBox31=new QGroupBox();
    groupBox32=new QGroupBox();
    groupBox33=new QGroupBox();
    bookId=new QLineEdit();
    bookName=new QLineEdit();
    yearLow=new QComboBox();//addItem添加东西
    yearHigh=new QComboBox();
    press=new QLineEdit();
    author=new QLineEdit();
    priceLow=new QComboBox();
    priceHigh=new QComboBox();
    bookType=new QComboBox();
    //排序
    firstSequence=new QComboBox();
    secondSequence=new QComboBox();
/*    totalNumLow=new QComboBox();
    totalNumHigh=new QComboBox();
    stockLow=new QComboBox();
    stockHigh=new QComboBox();*/
    //显示书籍信息表格
    bookTable = new QTableWidget();
    //翻页
    lastPageButton = new QPushButton(tr("上一页"));
    nextPageButton = new QPushButton(tr("下一页"));
    searchButton30=new QPushButton(tr("搜索"));
    clearButton30=new QPushButton(tr("清空"));
    sortButton=new QPushButton(tr("排序"));
    borrowBookButton=new QPushButton(tr("借书"));
    //searchButton30->setFixedSize(120,35);
    //clearButton30->setFixedSize(120,35);
    label30=new QLabel[bookTypeNumber];
//    label31=new QLabel[bookMessageNumber];
    label32=new QLabel[bookSearchNumber];
//    label33=new QLabel[rowsEachPage];
    label34=new QLabel[2];
    currentPage = maxPage = 0;
    borrowFlag = false;
    //其他设置
    QString bookMessage30[bookTypeNumber]={"马列毛邓","哲学、宗教","社会科学总论","政治、法律","军事","经济",
                                           "教科文体","语言、文字","文学","艺术","历史、地理","自然科学总论","数理科学",
                                           "天文学、地球科学","生物科学","医药、卫生","农业科学","工业技术","交通运输","航空、航天",
                                           "环境、安全科学","综合性图书"};
    QString bookMessage31[bookSearchNumber]={"书名","作者","年份","--","出版社","类别","价格","--"};
    QString bookMessage32[bookMessageNumber]={"书号","类别","书名","出版社","年份","作者","价格","总藏书量","库存"};
//    QString bookMessage33[rowsEachPage]={"1","2","3","4","5","6","7","8","9","10"};
    yearLow->addItem(" ");
    yearHigh->addItem(" ");
    for(int i=2017;i>=1949;i--){
        yearLow->addItem(QString::number(i));
        yearHigh->addItem(QString::number(i));
    }
    priceLow->addItem(" ");
    priceHigh->addItem(" ");
    for(int i=0;i<=maxPrice;i++){
        priceLow->addItem(QString::number(i));
        priceHigh->addItem(QString::number(i));
    }
    bookType->addItem(" ");
    for(int i=0;i<bookTypeNumber;i++){
        bookType->addItem(bookMessage30[i]);
    }
    firstSequence->addItem(bookMessage32[0]);
    secondSequence->addItem(" ");
    for(int i=1; i<bookMessageNumber;i++){
        firstSequence->addItem(bookMessage32[i]);
        secondSequence->addItem(bookMessage32[i]);
    }
/*    totalNumLow->addItem(" ");
    totalNumHigh->addItem(" ");
    for(int i=0;i<=maxBookNumber;i++){
        totalNumLow->addItem(QString::number(i));
        totalNumHigh->addItem(QString::number(i));
    }
    stockLow->addItem(" ");
    stockHigh->addItem(" ");
    for(int i=0;i<=maxStock;i++){
        stockLow->addItem(QString::number(i));
        stockHigh->addItem(QString::number(i));
    }*/
    //书籍信息显示
    showBookTable(VISITOR);
    //分配关系
/*    vboxLayout30->addStretch();
    for(int i=0;i<bookTypeNumber;i++){
        label30[i].setText(bookMessage30[i]);
        vboxLayout30->addWidget(label30+i);
    }
    vboxLayout30->addStretch();*/

    /*for(int i=0;i<bookMessageNumber;i++){
        label31[i].setText(bookMessage32[i]);
        gridLayout30->addWidget(label31+i,1,i+2);
    }
    for(int i=0;i<rowsEachPage;i++){
        label33[i].setText(bookMessage33[i]);
        gridLayout30->addWidget(label33+i,i+2,1);
    }*/
    gridLayout30->addWidget(bookTable);
    for(int i=0;i<bookSearchNumber;i++){
        label32[i].setText(bookMessage31[i]);
        gridLayout31->addWidget(label32+i,i/4+1,(i%4)*2+1);
    }
    label34[0].setText("首排序:");
    label34[1].setText("次排序:");

//   gridLayout31->addWidget(bookId,1,2);
    gridLayout31->addWidget(bookName,1,2);
    gridLayout31->addWidget(author,1,4);
    gridLayout31->addWidget(yearLow,1,6);
    gridLayout31->addWidget(yearHigh,1,8);
    gridLayout31->addWidget(press,2,2);
    gridLayout31->addWidget(bookType,2,4);
    gridLayout31->addWidget(priceLow,2,6);
    gridLayout31->addWidget(priceHigh,2,8);
/*    gridLayout31->addWidget(totalNumLow,3,2);
    gridLayout31->addWidget(totalNumHigh,3,4);
    gridLayout31->addWidget(stockLow,3,6);
    gridLayout31->addWidget(stockHigh,3,8);*/
    //布局
    hboxLayout35->addStretch();
    hboxLayout35->addWidget(searchButton30);
    hboxLayout35->addWidget(clearButton30);
    hboxLayout35->addWidget(borrowBookButton);
    hboxLayout35->addStretch();
    hboxLayout33->addStretch();
    hboxLayout33->addLayout(hboxLayout35);
    hboxLayout33->addStretch();
    groupBox30->setLayout(hboxLayout33);
    groupBox31->setLayout(gridLayout31);
    groupBox30->setFixedSize(300,120);
    groupBox31->setFixedSize(600,120);

    hboxLayout31->addWidget(groupBox31);
    hboxLayout31->addSpacing(20);
    hboxLayout31->addWidget(groupBox30);

    groupBox32->setLayout(gridLayout30);
    groupBox32->setFixedSize(1000,500);

    vboxLayout31->addLayout(hboxLayout31);
    vboxLayout31->addWidget(groupBox32);


    hboxLayout34->addWidget(label34);
    hboxLayout34->addWidget(firstSequence);
    hboxLayout34->addWidget(label34+1);
    hboxLayout34->addWidget(secondSequence);
    hboxLayout34->addWidget(sortButton);
    hboxLayout34->addStretch();
//    hboxLayout34->addWidget(lastPageButton);
//    hboxLayout34->addWidget(nextPageButton);

    vboxLayout31->addLayout(hboxLayout34);


    hboxLayout30->addStretch();
    hboxLayout30->addLayout(vboxLayout30);
    hboxLayout30->addLayout(vboxLayout31);
    hboxLayout30->addStretch();

    searchBookWindow->setLayout(hboxLayout30);
    searchWindowTab->addTab(searchBookWindow,"图书搜索");

    connect(searchButton30,&QPushButton::clicked,this,&database::searchBook);
    connect(sortButton,&QPushButton::clicked,this,&database::searchBook);
    connect(clearButton30,&QPushButton::clicked,this,&database::clearInput);
    connect(lastPageButton,&QPushButton::clicked,this,&database::lastPage);
    connect(nextPageButton,&QPushButton::clicked,this,&database::nextPage);
    connect(borrowBookButton,&QPushButton::clicked,this,&database::borrowBook);


    managerWindow=new QWidget();
    batchProcessButton50=new QPushButton(tr("打开文件"));
    addOneBookConfirmButton50 =new QPushButton(tr("确认添加"));
    searchUserButton50=new QPushButton(tr("搜索"));
    label50=new QLabel(tr("单册添加书籍"));
    label51=new QLabel[9];
    label52=new QLabel(tr("批量导入书籍"));
    label51[0].setText(tr("书号"));
    label51[1].setText(tr("类型"));
    label51[2].setText(tr("书名"));
    label51[3].setText(tr("出版社"));
    label51[4].setText(tr("时间"));
    label51[5].setText(tr("作者"));
    label51[6].setText(tr("价格"));
    label51[7].setText(tr("总量"));
    label51[8].setText(tr("库存"));
    label53=new QLabel(tr("用户管理"));
    label54=new QLabel(tr("用户名"));
    authorizationButton50=new QPushButton(tr("授权"));
    revokeButton50=new QPushButton(tr("撤回"));
    changeButton50=new QPushButton(tr("对立更改"));
    deleteUserButton50=new QPushButton(tr("删除用户"));
    bookId50=new QLineEdit();
    bookType50=new QComboBox();
    bookName50=new QLineEdit();
    press50=new QLineEdit();
    year50=new QLineEdit();
    author50=new QLineEdit();
    price50=new QLineEdit();
    total50=new QLineEdit();
    stock50=new QLineEdit();
    userName50=new QLineEdit();
    hboxLayout50=new QHBoxLayout();
    hboxLayout51=new QHBoxLayout();
    hboxLayout52=new QHBoxLayout();
    hboxLayout53=new QHBoxLayout();
    hboxLayout54=new QHBoxLayout();
    hboxLayout55=new QHBoxLayout();
    hboxLayout56=new QHBoxLayout();
    hboxLayout57=new QHBoxLayout();
    hboxLayout58=new QHBoxLayout();
    vboxLayout50=new QVBoxLayout();
    vboxLayout51=new QVBoxLayout();
    vboxLayout52=new QVBoxLayout();
    vboxLayout53=new QVBoxLayout();
    gridLayout50=new QGridLayout();
    groupBox50=new QGroupBox();
    groupBox51=new QGroupBox();
    groupBox52=new QGroupBox();
    userTable=new QTableWidget();
    bookType50->addItem(" ");
    for(int i=0;i<bookTypeNumber;i++){
        bookType50->addItem(bookMessage30[i]);
    }
    showUserTable(VISITOR);
    gridLayout50->addWidget(&label51[0],1,1);
    gridLayout50->addWidget(bookId50,1,2);
    gridLayout50->addWidget(&label51[1],1,3);
    gridLayout50->addWidget(bookType50,1,4);
    gridLayout50->addWidget(&label51[2],1,5);
    gridLayout50->addWidget(bookName50,1,6);
    gridLayout50->addWidget(&label51[3],2,1);
    gridLayout50->addWidget(press50,2,2);
    gridLayout50->addWidget(&label51[4],2,3);
    gridLayout50->addWidget(year50,2,4);
    gridLayout50->addWidget(&label51[5],2,5);
    gridLayout50->addWidget(author50,2,6);
    gridLayout50->addWidget(&label51[6],3,1);
    gridLayout50->addWidget(price50,3,2);
    gridLayout50->addWidget(&label51[7],3,3);
    gridLayout50->addWidget(total50,3,4);
    gridLayout50->addWidget(&label51[8],3,5);
    gridLayout50->addWidget(stock50,3,6);

    hboxLayout50->addStretch();
    hboxLayout50->addWidget(label50);
    hboxLayout50->addStretch();
    hboxLayout51->addStretch();
    hboxLayout51->addWidget(addOneBookConfirmButton50);
    hboxLayout51->addStretch();
    hboxLayout52->addStretch();
    hboxLayout52->addWidget(batchProcessButton50);
    hboxLayout52->addStretch();
    hboxLayout54->addStretch();
    hboxLayout54->addWidget(label52);
    hboxLayout54->addStretch();
    hboxLayout55->addStretch();
    hboxLayout55->addWidget(label54);
    hboxLayout55->addWidget(userName50);
    hboxLayout55->addWidget(searchUserButton50);
    hboxLayout55->addStretch();
    hboxLayout56->addStretch();
    hboxLayout56->addWidget(label53);
    hboxLayout56->addStretch();

    hboxLayout58->addStretch();
    hboxLayout58->addWidget(authorizationButton50);
    hboxLayout58->addWidget(revokeButton50);
    hboxLayout58->addWidget(changeButton50);
    hboxLayout58->addWidget(deleteUserButton50);
    hboxLayout58->addStretch();

    hboxLayout57->addStretch();
    hboxLayout57->addWidget(userTable);
    hboxLayout57->addStretch();

    vboxLayout51->addLayout(hboxLayout56);
    vboxLayout51->addLayout(hboxLayout55);
    vboxLayout51->addLayout(hboxLayout57);
    vboxLayout51->addLayout(hboxLayout58);
    groupBox50->setLayout(vboxLayout51);


    vboxLayout52->addLayout(hboxLayout50);
    vboxLayout52->addLayout(gridLayout50);
    vboxLayout52->addLayout(hboxLayout51);
    groupBox51->setLayout(vboxLayout52);

    vboxLayout53->addLayout(hboxLayout54);
    vboxLayout53->addLayout(hboxLayout52);
    groupBox52->setLayout(vboxLayout53);

    vboxLayout50->addStretch();
    vboxLayout50->addWidget(groupBox51);
    vboxLayout50->addWidget(groupBox52);
    vboxLayout50->addWidget(groupBox50);
    vboxLayout50->addStretch();

    hboxLayout53->addStretch();
    hboxLayout53->addLayout(vboxLayout50);
    hboxLayout53->addStretch();
    managerWindow->setLayout(hboxLayout53);
    searchWindowTab->addTab(managerWindow,"管理员界面");

    for(int i=0;i<10;i++){
        item50[i]=new QTableWidgetItem();
    }
    for(int i=0;i<10;i++){
        item70[i]=new QTableWidgetItem();
    }
    connect(batchProcessButton50,&QPushButton::clicked,this,&database::batchProcess);
    connect(addOneBookConfirmButton50,&QPushButton::clicked,this,&database::addOneBook);
    connect(searchUserButton50,&QPushButton::clicked,this,&database::searchUser);
    connect(authorizationButton50,&QPushButton::clicked,this,&database::giveAuthorization);
    connect(revokeButton50,&QPushButton::clicked,this,&database::revokeAuthorization);
    connect(changeButton50,&QPushButton::clicked,this,&database::changeAuthorization);
    connect(deleteUserButton50,&QPushButton::clicked,this,&database::deleteUser);

    //users' Home

    userHomeWindow= new QWidget();
    userBorrowTable=new QTableWidget();
    changePasswordAffirmButton=new QPushButton(tr("确定"));
    returnBookButton=new QPushButton(tr("还书"));
    hboxLayout70=new QHBoxLayout();
    hboxLayout71=new QHBoxLayout();
    hboxLayout72=new QHBoxLayout();
    hboxLayout73=new QHBoxLayout();
    vboxLayout72=new QVBoxLayout();
    vboxLayout71=new QVBoxLayout();
    vboxLayout70=new QVBoxLayout();
    gridLayout70=new QGridLayout();
    groupBox70=new QGroupBox();
    groupBox71=new QGroupBox();
    label70=new QLabel[3];
    label71=new QLabel();
    oldPassword70=new QLineEdit();
    newPassword70=new QLineEdit();
    newPassword71=new QLineEdit();
    oldPassword70->setEchoMode(QLineEdit::Password);
    newPassword70->setEchoMode(QLineEdit::Password);
    newPassword71->setEchoMode(QLineEdit::Password);
    label70[0].setText(tr("原密码"));
    label70[1].setText(tr("新密码"));
    label70[2].setText(tr("再次输入"));
    label71->setText(tr("更改密码"));

    hboxLayout71->addStretch();
    hboxLayout71->addWidget(label71);
    hboxLayout71->addStretch();

    gridLayout70->addWidget(&label70[0],1,1);
    gridLayout70->addWidget(&label70[1],2,1);
    gridLayout70->addWidget(&label70[2],3,1);
    gridLayout70->addWidget(oldPassword70,1,2);
    gridLayout70->addWidget(newPassword70,2,2);
    gridLayout70->addWidget(newPassword71,3,2);

    hboxLayout72->addStretch();
    hboxLayout72->addLayout(gridLayout70);
    hboxLayout72->addSpacing(20);
    hboxLayout72->addWidget(changePasswordAffirmButton);
    hboxLayout72->addStretch();

    hboxLayout73->addStretch();
    hboxLayout73->addWidget(returnBookButton);
    hboxLayout73->addStretch();

    vboxLayout71->addStretch();
    vboxLayout71->addLayout(hboxLayout71);
    vboxLayout71->addLayout(hboxLayout72);
    vboxLayout71->addStretch();
    groupBox70->setLayout(vboxLayout71);

    vboxLayout72->addWidget(userBorrowTable);
    vboxLayout72->addLayout(hboxLayout73);
    groupBox71->setLayout(vboxLayout72);

    vboxLayout70->addStretch();
    vboxLayout70->addWidget(groupBox70);
    vboxLayout70->addWidget(groupBox71);
    vboxLayout70->addStretch();

    hboxLayout70->addStretch();
    hboxLayout70->addLayout(vboxLayout70);
    hboxLayout70->addStretch();

    userHomeWindow->setLayout(hboxLayout70);
    searchWindowTab->addTab(userHomeWindow,"个人中心");
    searchWindowTab->removeTab(0);
    searchWindowTab->removeTab(0);
    searchWindowTab->removeTab(0);
    searchWindowTab->insertTab(0,searchBookWindow,"图书搜索");
    connect(changePasswordAffirmButton,&QPushButton::clicked,this,&database::changePassword);
    connect(returnBookButton,&QPushButton::clicked,this,&database::returnBook);

}

void database::showBorrowTable(){

    QStringList header;
    //清除表格信息
    userBorrowTable->clear();
    //设置表格为不可选择、不可修改
    userBorrowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userBorrowTable->setSelectionMode(QAbstractItemView::NoSelection);
    userBorrowTable->setFixedSize(725,300);
    userBorrowTable->setRowCount(10);
    userBorrowTable->setColumnCount(5);
    header<<" "<<"书名"<<"作者"<<"借书时间"<<"应还时间";
    userBorrowTable->setHorizontalHeaderLabels(header);
    for(int i=0;i<5;i++){
        if(i == 0) userBorrowTable->setColumnWidth(i,25);
        else if(i==1 || i==2) userBorrowTable->setColumnWidth(i,110);
        else userBorrowTable->setColumnWidth(i,220);
    }

    //查询
    QSqlQuery *borrowRecordQuery;
    borrowRecordQuery=new QSqlQuery();

    //qDebug()<<nameLine40->text();
    if(!borrowRecordQuery->exec("select title, author, borrowDate, returnDate, bookID from books natural join borrow where username = '"+nameLine40->text()+"' and returnDate is null")){
        QMessageBox::critical(window,"Error",borrowRecordQuery->lastError().text());
        return;
    }
    else{
        if(!borrowRecordQuery->next()){
            item71[0][0] = new QTableWidgetItem("暂无记录");
            item71[0][0]->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            userBorrowTable->setItem(0,3,item71[0][0]);
            return;
        }

    }
    //显示书籍信息
    borrowRecordQuery->seek(0);
    for(int i=0;i<10;i++){
        item70[i]=new QTableWidgetItem();
    }
    int count = 0;
    do{
        int i;
        item70[count]->setCheckState(Qt::Unchecked);
        userBorrowTable->setItem(count,0,item70[count]);
        for(i=0; i<3; i++){
            item71[count][i] = new QTableWidgetItem(borrowRecordQuery->value(i).toString());
//            qDebug()<<item71[count][i]->text();
            item71[count][i]->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            userBorrowTable->setItem(count,i+1,item71[count][i]);
        }
        QDateTime returnTime;
        returnTime = QDateTime::fromString(borrowRecordQuery->value(2).toString(),"yyyy-MM-dd hh:mm:ss").addDays(60);
        item71[count][3] = new QTableWidgetItem(returnTime.toString("yyyy-MM-dd hh:mm:ss"));
        item71[count][3]->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        userBorrowTable->setItem(count,4,item71[count][3]);
        item71[count][4] = new QTableWidgetItem(borrowRecordQuery->value(4).toString());
        count++;
        if(count == 10) {
            break;
        }
    }while(borrowRecordQuery->next());
}

void database::changePassword(){
    //passwordLine40
//    if(currentUser == VISITOR) {
//        QMessageBox::critical(window,"提示","请先登录再修改！");
//       return;
//   }
//    if(currentUser == MANAGER){
//        QMessageBox::critical(window,"提示","管理员不能修改密码！");
//        return;
//    }
    QString rightPassword;
    QSqlQuery tmpQuery;
    if(!tmpQuery.exec("select password from users where username='" + nameLine40->text() +"'")){
        QMessageBox::critical(userHomeWindow,"Error",tmpQuery.lastError().text());
        return;
    }
    if(!tmpQuery.next()){
        QMessageBox::critical(userHomeWindow,"Error","该用户名不存在",QMessageBox::Yes);
        return;
    }
    rightPassword=tmpQuery.value(0).toString();
    QVariantList nameList70,passwordList70;
    if(oldPassword70->text().compare(rightPassword)!=0){
        QMessageBox::critical(userHomeWindow,"Error","密码错误");
        return;
    }
    if(newPassword70->text().compare(newPassword71->text())!=0){
        QMessageBox::critical(userHomeWindow,"Error","两次密码输入不一致!");
        return;
    }
    tmpQuery.prepare("update users set password = ? where username = ?");
    nameList70<<nameLine40->text();
    passwordList70<<newPassword70->text();
    tmpQuery.addBindValue(passwordList70);
    tmpQuery.addBindValue(nameList70);
    if(!tmpQuery.execBatch()){
        QMessageBox::critical(userHomeWindow,"Error",tmpQuery.lastError().text());
        return;
    }else{
        QMessageBox::about(userHomeWindow,"database","修改成功,请重新登陆");
        oldPassword70->clear();
        newPassword70->clear();
        newPassword71->clear();
        logout();
    }
}
void database::returnBook(){
    //nameLine40
    const int max=10;
    int flag=0;
//    QString name="";
    QSqlQuery tmpQuery;

    QVariantList usernameList,borrowTimeList,returnTimeList,bookIDList;
    tmpQuery.prepare("update borrow set returnDate = ? where borrowDate = ? and username = ? and bookID = ?");
    for(int i=0;i<max;i++){
        if(item70[i]->checkState() == Qt::Checked){
            borrowTimeList<<item71[i][2]->text();
            returnTimeList<<QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            bookIDList<<item71[i][4]->text();
            usernameList<<nameLine40->text();
            flag=1;
        }
    }
    if(!flag){
        QMessageBox::critical(window,"error","您尚未选择任何书籍！");
        return;
    }
    tmpQuery.addBindValue(returnTimeList);
    tmpQuery.addBindValue(borrowTimeList);
    tmpQuery.addBindValue(usernameList);
    tmpQuery.addBindValue(bookIDList);

    if(!tmpQuery.execBatch()){
        QMessageBox::critical(userHomeWindow,"Error",tmpQuery.lastError().text());
        return;
    }
    QMessageBox::about(window,"database","success");
    tmpQuery.prepare("update books set stock = stock +1 where bookID = ?");
    tmpQuery.addBindValue(bookIDList);
    if(!tmpQuery.execBatch()){
        QMessageBox::critical(userHomeWindow,"Error",tmpQuery.lastError().text());
        return;
        //没有回滚过程
    }
    tmpQuery.finish();
//    showUserTable(USER);
    showBorrowTable();
    showBookTable(VISITOR);
    return;
}


void database::deleteUser(){
    const int max=10;
    int flag=0;
    QString name="";
    QSqlQuery tmpQuery;
    for(int i=0;i<max;i++){
        if(item50[i]->checkState() == Qt::Checked){
            if(flag==0){
                name+=" username = ";
            }else{
                name+=" or username = ";
            }
            name+="'"+item51[i][0]->text()+"'";
            flag = 1;
        }
    }
    if(!flag){
        QMessageBox::critical(window,"error","您尚未选择任何用户！");
        return;
    }
    if(!tmpQuery.exec("delete from users where "+name)){
        QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        return;
    }
    QMessageBox::about(window,"database","success");
    tmpQuery.finish();
    showUserTable(USER);
    return;
}
void database::borrowBook(){
    if(currentUser == VISITOR) {
        QMessageBox::critical(window,"提示","请先登录再借书！");
        return;
    }
    if(currentUser == MANAGER){
        QMessageBox::critical(window,"提示","管理员不能借书！");
        return;
    }
    if(!borrowFlag){
        QMessageBox::critical(window,"提示","您尚未选择任何书籍！");
        return;
    }
    const int max=50;
    int flag=0,count=0;
    QString queryText="";
    QSqlQuery tmpQuery;
    QSqlQuery returnDateQuery;
    QString bookID[max];
    QVariantList bookIDList;
    QString earlistTime;
    for(int i=0;i<max;i++){
        if(item30[i]->checkState() == Qt::Checked){
            if(!item31[i][9]->text().compare("unavailable")){
                returnDateQuery.prepare("select min(borrowDate) from borrow where bookID = ?");
                bookIDList<<item31[i][0]->text();
                returnDateQuery.addBindValue(bookIDList);
                if(!returnDateQuery.execBatch()){
                    QMessageBox::critical(window,"Error",returnDateQuery.lastError().text());
                    return;
                }else{
                    returnDateQuery.next();
                    QDateTime earlistReturnTime;
                    if(returnDateQuery.value(0).toString().compare("null")){
                        QMessageBox::critical(window,"提示",item31[i][2]->text() + "已无库存!");
                        return;
                    }
                    earlistReturnTime=QDateTime::fromString(returnDateQuery.value(0).toString(),"yyyy-MM-dd hh:mm:ss").addDays(60);
                    earlistTime=earlistReturnTime.toString("yyyy-MM-dd hh:mm:ss");
                }
                QMessageBox::critical(window,"提示",item31[i][2]->text() + "暂无库存！\n建议您"+ earlistTime +"再借！");
                return;
            }
            if(flag==0){
                queryText += " bookID = ";
            }else{
                queryText += " or bookID = ";
            }
            queryText +="'"+item31[i][0]->text()+"'";
            bookID[count++] = item31[i][0]->text();
            flag = 1;
        }
    }
    if(!flag){
        QMessageBox::critical(window,"Error","您尚未选择任何书籍！");
        return;
    }
    int notreturnBookNumber;
    QVariantList username;
    username<<nameLine40->text();
    tmpQuery.prepare("select count(*) from borrow where username = ? and returnDate is null");
    tmpQuery.addBindValue(username);
    if(!tmpQuery.execBatch()){
        QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        return;
    }else{
        tmpQuery.next();
        notreturnBookNumber=tmpQuery.value(0).toInt();
        if(notreturnBookNumber + count > 10){
            QMessageBox::critical(window,"Error","一个人不能借多于十本书！");
            return;
        }
    }
    tmpQuery.prepare("select isLent from users where username = ?");
    tmpQuery.addBindValue(username);
//    qDebug()<<username;
    if(!tmpQuery.execBatch()){
        QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        return;
    }else{
        tmpQuery.next();
//        qDebug()<<tmpQuery.value(0).toString();
        if(tmpQuery.value(0).toString().compare("false")==0){
            QMessageBox::critical(window,"Error","您没有借书权限，请向管理员申请！");
            return;
        }
    }
    if(!tmpQuery.exec("update books set stock = stock - 1 where "+queryText)){
        QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        return;
    }
//    if(!tmpQuery.exec("update books set total = total - 1 where "+queryText)){
//        QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
//        tmpQuery.exec("update books set stock = stock + 1 where "+queryText);
//        return;
//    }
    QDateTime dateTime = QDateTime::currentDateTime();//获取系统时间
    for(int i=0; i<count; i++){
        if(!tmpQuery.exec("insert into borrow values('" + nameLine40->text() + "','" + bookID[i] + "','"
                          + dateTime.toString("yyyy-MM-dd hh:mm:ss") + "',null)")){
            QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
            tmpQuery.exec("update books set stock = stock + 1 where "+queryText);
            //tmpQuery.exec("update books set total = total where "+queryText);
            tmpQuery.exec("delete from borrow where username = '" + nameLine40->text() + "' and borrowDate = '"
                          + dateTime.toString("yyyy-MM-dd hh:mm:ss") + "'");
            return;
        }
    }

    QMessageBox::about(window,"database","success");
    tmpQuery.finish();
    showBookTable(USER);
    showBorrowTable();
    return;
}

void database::searchUser(){
    //检查输入信息
    if(userName50->text().isEmpty()){
        QMessageBox::critical(window,"Error","用户名不能为空！",QMessageBox::Yes);
        return;
    }
    userQueryText = userName50->text();
    showUserTable(USER);

}

void database::showUserTable(type type){
    //用户信息显示
    QStringList header;
    //清除表格信息
    userTable->clear();
    //设置表格为不可选择、不可修改
    userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userTable->setSelectionMode(QAbstractItemView::NoSelection);

    userTable->setRowCount(10);
    userTable->setColumnCount(4);
    userTable->setFixedWidth(500);
    header<<""<<"用户名"<<"密码"<<"权限"/*<<"操作"*/;
    userTable->setHorizontalHeaderLabels(header);
    for(int i=0;i<4;i++){
        if(i==0) userTable->setColumnWidth(i,40);
        else if(i == 2) userTable->setColumnWidth(i,200);
        else userTable->setColumnWidth(i,110);
    }
    if(type == VISITOR) return;

    userQuery=new QSqlQuery();
    if(!userQuery->exec("select * from users where username like '%" + userQueryText + "%'")){
        QMessageBox::critical(window,"Error",userQuery->lastError().text());
    }
    else{
        if(!userQuery->next()){
            QMessageBox::critical(window,"database","无搜索结果");
        }
        else{
            userQuery->previous();
        }
    }
    //用户信息
//    QTableWidgetItem *item50[10];
//    QTableWidgetItem *item51[10][3];
    //删去有bug
    for(int i=0;i<10;i++){
        item50[i]=new QTableWidgetItem();
    }
    int i = 0;
//    item50=new QTableWidgetItem[10];
    while(userQuery->next()){
//        item50[i]=new QTableWidgetItem();
        item50[i]->setCheckState(Qt::Unchecked);
//        item51[i]=new QTableWidgetItem[3];
//        QTableWidgetItem *item[3];
        userTable->setItem(i,0,item50[i]);
        for(int j=0; j<3; j++){
            item51[i][j] = new QTableWidgetItem(userQuery->value(j).toString());
            item51[i][j]->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            userTable->setItem(i,j+1,item51[i][j]);
        }
        i++;
        if(i == 10) {
            break;
        }
    }
}
void database::giveAuthorization(){
    const int max=10;
    int flag=0;
    QString name="";
    QSqlQuery tmpQuery;
    for(int i=0;i<max;i++){
        if(item50[i]->checkState() == Qt::Checked){
            if(flag==0){
                name+=" username = ";
            }else{
                name+=" or username = ";
            }
            name+="'"+item51[i][0]->text()+"'";
            flag = 1;
        }
    }
    if(!flag){
        QMessageBox::critical(window,"error","您尚未选择任何用户！");
        return;
    }
    if(!tmpQuery.exec("update users set isLent =1 where "+name)){
        QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        return;
    }
    QMessageBox::about(window,"database","success");
    tmpQuery.finish();
    showUserTable(USER);
    return;
}
void database::revokeAuthorization(){
    const int max=10;
    int flag=0;
    QString name="";
    QSqlQuery tmpQuery;
    tmpQuery.clear();
    for(int i=0;i<max;i++){
        if(item50[i]->checkState() == Qt::Checked){
            if(flag==0){
                name+=" username = ";
            }else{
                name+=" or username = ";
            }
            name+="'"+item51[i][0]->text()+"'";
            flag = 1;
        }
    }
    if(!flag){
        QMessageBox::critical(window,"error","您尚未选择任何用户！");
        return;
    }
    if(!tmpQuery.exec("update users set isLent = 2 where "+name)){
        QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        return;
    }else{
        QMessageBox::about(window,"database","success");
        showUserTable(USER);
        return;
    }
}
void database::changeAuthorization(){
    const int max=10;
    int flag1=0,flag2=0;
    QString name1="",name2="";
    QSqlQuery tmpQuery;
    for(int i=0;i<max;i++){
        if(item50[i]->checkState() == Qt::Checked){
            if(item51[i][2]->text().compare("true")==0){
                if(flag1==0){
                    name1+=" username = ";
                }else{
                    name1+=" or username = ";
                }
                name1+="'"+item51[i][0]->text()+"'";
                flag1 = 1;
            }else{
                if(flag2==0){
                    name2+=" username = ";
                }else{
                    name2+=" or username = ";
                }
                name2+="'"+item51[i][0]->text()+"'";
                flag2 = 1;
            }

        }
    }
    if(flag1==0&&flag2==0){
        QMessageBox::critical(window,"error","您尚未选择任何用户！");
        return;
    }
    if(flag1){
        if(!tmpQuery.exec("update users set isLent = 2 where "+name1)){
            QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
            return;
        }
    }
    if(flag2){
        if(!tmpQuery.exec("update users set isLent = 1 where "+name2)){
            QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        }
    }
    QMessageBox::about(window,"database","success");
    showUserTable(USER);
}
void database::addOneBook(){
    if(bookId50->text().isEmpty() || bookType50->currentText().isEmpty() || bookName50->text().isEmpty()
            || press50->text().isEmpty() || year50->text().isEmpty() || author50->text().isEmpty()
            || price50->text().isEmpty() || total50->text().isEmpty() || stock50->text().isEmpty()){
        QMessageBox::critical(window,"Error","填写信息不全！","Ok");
        return;
    }
    QSqlQuery tmpQuery;
    QVariantList bookIDList,typeList,titleList,pressList,yearList,authorList,priceList,totalNumList,stockList;
    bookIDList<<bookId50->text();
    typeList<<bookType50->currentText();
    titleList<<bookName50->text();
    pressList<<press50->text();
    yearList<<year50->text();
    authorList<<author50->text();
    priceList<<price50->text();
    totalNumList<<total50->text();
    stockList<<stock50->text();
    tmpQuery.prepare("insert into books values(?,?,?,?,?,?,?,?,?)");
    tmpQuery.addBindValue(bookIDList);
    tmpQuery.addBindValue(typeList);
    tmpQuery.addBindValue(titleList);
    tmpQuery.addBindValue(pressList);
    tmpQuery.addBindValue(yearList);
    tmpQuery.addBindValue(authorList);
    tmpQuery.addBindValue(priceList);
    tmpQuery.addBindValue(totalNumList);
    tmpQuery.addBindValue(stockList);
    if(tmpQuery.execBatch()){
        QMessageBox::about(window,"database","Success");
        bookId50->clear();
        bookType50->setCurrentIndex(0);
        bookName50->clear();
        press50->clear();
        year50->clear();
        author50->clear();
        price50->clear();
        total50->clear();
        stock50->clear();
    }else{
        QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
    }
}

void database::showBookTable(type type){
    //书籍信息显示
    QStringList header;
    //清除表格信息
    bookTable->clear();
    //设置表格为不可选择、不可修改
    bookTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookTable->setSelectionMode(QAbstractItemView::NoSelection);

    bookTable->setRowCount(50);
    bookTable->setColumnCount(11);
    header<<" "<<"书号"<<"类别"<<"书名"<<"出版社"<<"年份"
         <<"作者"<<"价格"<<"总藏书量"<<"库存"<<"状态";
    bookTable->setHorizontalHeaderLabels(header);
    for(int i=0;i<11;i++){
        if(i < 4 && i > 0) bookTable->setColumnWidth(i,120);
        else if(i == 0) bookTable->setColumnWidth(i,25);
        else if(i == 10) bookTable->setColumnWidth(i,100);
        else bookTable->setColumnWidth(i,75);
    }
    if(type == VISITOR){
        borrowFlag = false;
        maxPage = currentPage = 0;
        return;
    }

    //查询
    if(!bookQuery->exec(queryText)){
        QMessageBox::critical(window,"Error",bookQuery->lastError().text());
        return;
    }else{
        if(!bookQuery->next()){
            QMessageBox::critical(window,"database","无搜索结果");
            return;
        }
        else{
            maxPage = bookQuery->size()/50;
            //currentPage = 0;
            borrowFlag = true;
        }
    }
    //显示书籍信息
    bookQuery->seek(currentPage*50);
    for(int i=0;i<50;i++){
        item30[i]=new QTableWidgetItem();
    }
    int count = 0;
    do{
        int i;
        item30[count]->setCheckState(Qt::Unchecked);
        bookTable->setItem(count,0,item30[count]);
        for(i=0; i<9; i++){
            item31[count][i] = new QTableWidgetItem(bookQuery->value(i).toString());
            item31[count][i]->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            bookTable->setItem(count,i+1,item31[count][i]);
        }
        if(bookQuery->value(i-1).toInt() > 0){
            item31[count][i] = new QTableWidgetItem("available");
        }
        else{
            item31[count][i] = new QTableWidgetItem("unavailable");
        }
        item31[count][i]->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        bookTable->setItem(count,i+1,item31[count][i]);
        count++;
        if(count == 50) {
            break;
        }
    }while(bookQuery->next());
}

void database::clearInput(){
    bookName->clear();
    author->clear();
    press->clear();
    bookType->setCurrentIndex(0);
    yearLow->setCurrentIndex(0);
    yearHigh->setCurrentIndex(0);
    priceLow->setCurrentIndex(0);
    priceHigh->setCurrentIndex(0);
    showBookTable(VISITOR);
}

void database::searchBook(){
    //检查输入信息
    if(bookName->text().isEmpty() && author->text().isEmpty() && press->text().isEmpty()
            && bookType->currentIndex()==0 && yearLow->currentIndex()==0 && yearHigh->currentIndex()==0
            && priceLow->currentIndex()==0 && priceHigh->currentIndex()==0){
        showBookTable(VISITOR);
        QMessageBox::critical(window,"Error","您尚未填写任何信息！",QMessageBox::Yes);
        return;
    }
    if(yearLow->currentIndex() < yearHigh->currentIndex() || priceLow->currentIndex() > priceHigh->currentIndex()
            || (yearLow->currentIndex()==0 && yearHigh->currentIndex()!=0)|| (yearLow->currentIndex()!=0 && yearHigh->currentIndex()==0)
            || (priceLow->currentIndex()==0 && priceHigh->currentIndex()!=0) || (priceLow->currentIndex()!=0 && priceHigh->currentIndex()==0)){
        showBookTable(VISITOR);
        QMessageBox::critical(window,"Error","请正确选择年份或价格区间！",QMessageBox::Yes);
        return;
    }
    //查询语句
    //QSqlQuery bookQuery;
    bookQuery = new QSqlQuery;
    currentPage = maxPage = 0;
    queryText = "select * from books where ";
    int count = 0;
    if(!bookName->text().isEmpty()){
        if(count != 0) queryText += " and ";
        queryText += "title like '%" + bookName->text() + "%'";
        count++;
    }
    if(!author->text().isEmpty()){
        if(count != 0) queryText += " and ";
        queryText += "author like '%" + author->text() + "%'";
        count++;
    }
    if(!press->text().isEmpty()){
        if(count != 0) queryText += " and ";
        queryText += "press like '%" + press->text() + "%'";
        count++;
    }
    if(bookType->currentIndex() != 0){
        if(count != 0) queryText += " and ";
        queryText += "type = '" + bookType->currentText() + "'";
        count++;
    }
    if(yearLow->currentIndex() != 0){
        if(count != 0) queryText += " and ";
        queryText += "year >= " + yearLow->currentText() + " and year <= " + yearHigh->currentText();
        count++;
    }
    if(priceLow->currentIndex() != 0){
        if(count != 0) queryText += " and ";
        queryText += "price >= " + priceLow->currentText() + " and price <= " + priceHigh->currentText();
        count++;
    }

    queryText+=" order by ";
    if(firstSequence->currentText().compare("书号")==0){
        queryText+="bookID";
    }else if(firstSequence->currentText().compare("类别")==0){
        queryText+="type";
    }else if(firstSequence->currentText().compare("书名")==0){
        queryText+="title";
    }else if(firstSequence->currentText().compare("出版社")==0){
        queryText+="press";
    }else if(firstSequence->currentText().compare("年份")==0){
        queryText+="year";
    }else if(firstSequence->currentText().compare("作者")==0){
        queryText+="author";
    }else if(firstSequence->currentText().compare("价格")==0){
        queryText+="price";
    }else if(firstSequence->currentText().compare("总藏书量")==0){
        queryText+="total";
    }else if(firstSequence->currentText().compare("库存")==0){
        queryText+="stock";
    }
    if(secondSequence->currentIndex()!=0){
        queryText+=",";
        if(secondSequence->currentText().compare("书号")==0){
            queryText+="bookID";
        }else if(secondSequence->currentText().compare("类别")==0){
            queryText+="type";
        }else if(secondSequence->currentText().compare("书名")==0){
            queryText+="title";
        }else if(secondSequence->currentText().compare("出版社")==0){
            queryText+="press";
        }else if(secondSequence->currentText().compare("年份")==0){
            queryText+="year";
        }else if(secondSequence->currentText().compare("作者")==0){
            queryText+="author";
        }else if(secondSequence->currentText().compare("价格")==0){
            queryText+="price";
        }else if(secondSequence->currentText().compare("总藏书量")==0){
            queryText+="total";
        }else if(secondSequence->currentText().compare("库存")==0){
            queryText+="stock";
        }
    }

    showBookTable(USER);
}

void database::nextPage(){
    if(currentPage == maxPage) return;
    else{
        currentPage++;
        showBookTable(USER);
    }
}

void database::lastPage(){
    if(currentPage == 0) return;
    else{
        currentPage--;
        showBookTable(USER);
    }
}

void database::batchProcess(){

    batchProcessWindow60 = new QWidget();
    QFileDialog dlg(batchProcessWindow60,"打开文件");
    dlg.resize(300,200);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);

    //只允许打开.txt后缀的文件
//    dlg.setNameFilter("*text");

    //打开文件对话框
    if(dlg.exec()==QDialog::Accepted ){
        QStringList files = dlg.selectedFiles();
        if(!files.isEmpty()){
            //以只读形式打开选中文件
            QFile file;
            file.setFileName(files.at(0));
            file.open(QIODevice::ReadOnly);
            if(!file.atEnd()){
                //读取文本所有信息，并进行转码处理
                QByteArray buff;
                buff = file.readAll();
                QString fileContent = QString::fromLocal8Bit(buff);
                //对文本进行读取插入处理
                //书号, 类别, 书名, 出版社, 年份, 作者, 价格（区间）, 总藏书量（区间）, 库存（区间）
                QSqlQuery tmpQuery;
                QString str;
                str="";
                QVariantList bookIDList,typeList,titleList,pressList,yearList,authorList,priceList,totalNumList,stockList;
                for(int i=0,k=0;i<fileContent.length();i++){
                    if(fileContent.at(i)=='\n'||i==fileContent.length()-1){
                        stockList<<str;
                        if(str.length()==0)continue;
                        k=0;
                        str="";
                    }else if(fileContent.at(i)==' '){
                        switch(k){
                            case 0:{bookIDList<<str;break;}
                            case 1:{
                                if(str.compare("马列毛邓")!=0&&str.compare("哲学、宗教")!=0&&str.compare("社会科学总论")!=0
                                        &&str.compare("政治、法律")!=0&&str.compare("军事")!=0&&str.compare("经济")!=0
                                        &&str.compare("教科文体")!=0&&str.compare("语言、文字")!=0&&str.compare("文学")!=0&&str.compare("艺术")!=0
                                        &&str.compare("历史、地理")!=0&&str.compare("自然科学总论")!=0&&str.compare("数理科学")!=0&&str.compare("天文学、地球科学")!=0
                                        &&str.compare("生物科学")!=0&&str.compare("医药、卫生")!=0
                                        &&str.compare("农业科学")!=0&&str.compare("工业技术")!=0&&str.compare("交通运输")!=0&&str.compare("航空、航天")!=0
                                        &&str.compare("环境、安全科学")!=0&&str.compare("综合性图书")!=0){
                                    QMessageBox::critical(window,"Error","类型不存在！");
                                    return;
                                }
                                typeList<<str;
                                break;}
                            case 2:{titleList<<str;break;}
                            case 3:{pressList<<str;break;}
                            case 4:{yearList<<str;break;}
                            case 5:{authorList<<str;break;}
                            case 6:{priceList<<str;break;}
                            case 7:{totalNumList<<str;break;}
                        }
                        k++;
                        str="";
                    }else{
                        str=str+fileContent.at(i);
                    }
                }
                tmpQuery.prepare("insert into books values(?,?,?,?,?,?,?,?,?)");
                tmpQuery.addBindValue(bookIDList);
                tmpQuery.addBindValue(typeList);
                tmpQuery.addBindValue(titleList);
                tmpQuery.addBindValue(pressList);
                tmpQuery.addBindValue(yearList);
                tmpQuery.addBindValue(authorList);
                tmpQuery.addBindValue(priceList);
                tmpQuery.addBindValue(totalNumList);
                tmpQuery.addBindValue(stockList);
                if(tmpQuery.execBatch()){
                    QMessageBox::about(window,"database","Success");
                }else{
                    QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
                }
            }
        }
    }
}

void database::showRegisterWindow(){

    //malloc
    registerWindow=new QWidget();
    vboxLayout10=new QVBoxLayout();
    hboxLayout11=new QHBoxLayout();
    hboxLayout12=new QHBoxLayout();
    hboxLayout10=new QHBoxLayout();
    gridLayout10=new QGridLayout();
    label10 =new QLabel[3];
    label11=new QLabel(tr("请输入注册信息"));
    registerAffirmButton=new QPushButton(tr("确认"));
    registerQuitButton=new QPushButton(tr("退出"));
    nameLine10 = new QLineEdit();
    passwordLine10 = new QLineEdit();
    passwordLine11 = new QLineEdit();
    passwordLine10->setEchoMode(QLineEdit::Password);
    passwordLine11->setEchoMode(QLineEdit::Password);
    //words
    label10[0].setText(tr("用户名"));
    label10[1].setText(tr("密码"));
    label10[2].setText(tr("确认密码"));
    //禁用按键
    registerButton->setDisabled(true);
    loginUserButton->setDisabled(true);
    loginManagerButton->setDisabled(true);
    //布局
    hboxLayout11->addStretch();
    hboxLayout11->addWidget(label11);
    hboxLayout11->addStretch();

    hboxLayout12->addStretch();
    hboxLayout12->addWidget(registerAffirmButton);
    hboxLayout12->addWidget(registerQuitButton);
    hboxLayout12->addStretch();

    gridLayout10->addWidget(&label10[0],1,1);
    gridLayout10->addWidget(&label10[1],2,1);
    gridLayout10->addWidget(&label10[2],3,1);
    gridLayout10->addWidget(nameLine10,1,2);
    gridLayout10->addWidget(passwordLine10,2,2);
    gridLayout10->addWidget(passwordLine11,3,2);

    vboxLayout10->addLayout(hboxLayout11);
    vboxLayout10->addLayout(gridLayout10);
    vboxLayout10->addLayout(hboxLayout12);

    hboxLayout10->addStretch();
    hboxLayout10->addLayout(vboxLayout10);
    hboxLayout10->addStretch();

    registerWindow->setLayout(hboxLayout10);

    QPoint posOfWidget;
    posOfWidget.setX(window->pos().x()+window->width()/2-100);
    posOfWidget.setY(window->pos().y()+window->height()/2-120);
    registerWindow->move(posOfWidget);//只能手工调 3-27
//    registerWindow->move(window->pos());//位置固定在左上角 3-27
    registerWindow->setWindowFlags(registerWindow->windowFlags() &~ Qt::WindowCloseButtonHint);//禁用关闭按钮
    registerWindow->show();

    //建立确认键和执行语句的联系
    connect(registerAffirmButton,&QPushButton::clicked,this,&database::registerAUser);
    connect(registerQuitButton,&QPushButton::clicked,this,&database::deleteRegisterWindow);
}
void database::showLoginManagerWindow(){
    //登录管理员界面，编号2
    loginManagerWindow= new QWidget();
    label20=new QLabel[2];
    vboxLayout20=new QVBoxLayout();
    vboxLayout21=new QVBoxLayout();
    hboxLayout20=new QHBoxLayout();
    hboxLayout21=new QHBoxLayout();
    gridLayout20=new QGridLayout();
    nameLine20=new QLineEdit();
    passwordLine20=new QLineEdit();
    passwordLine20->setEchoMode(QLineEdit::Password);
    loginManagerAffirmButton=new QPushButton(tr("登录"));
    loginManagerQuitButton=new QPushButton(tr("取消"));
    //禁用按键
    registerButton->setDisabled(true);
    loginUserButton->setDisabled(true);
    loginManagerButton->setDisabled(true);

    label20[0].setText(tr("用户名"));
    label20[1].setText(tr("密码"));

    gridLayout20->addWidget(&label20[0],1,1);
    gridLayout20->addWidget(&label20[1],2,1);
    gridLayout20->addWidget(nameLine20,1,2);
    gridLayout20->addWidget(passwordLine20,2,2);

    hboxLayout20->addWidget(loginManagerAffirmButton);
    hboxLayout20->addWidget(loginManagerQuitButton);

    vboxLayout21->addLayout(gridLayout20);
    vboxLayout21->addLayout(hboxLayout20);

    hboxLayout21->addStretch();
    hboxLayout21->addLayout(vboxLayout21);
    hboxLayout21->addStretch();

    loginManagerWindow->setLayout(hboxLayout21);
    QPoint posOfWidget;
    posOfWidget.setX(window->pos().x()+window->width()/2-102);
    posOfWidget.setY(window->pos().y()+window->height()/2-77);
    loginManagerWindow->move(posOfWidget);   //只能手工调 3-27
//    loginManagerWindow->move(window->pos());//位置固定在窗口左上角 3-27
    loginManagerWindow->setWindowFlags(loginManagerWindow->windowFlags() &~ Qt::WindowCloseButtonHint);//禁用关闭按钮
    loginManagerWindow->show();

    //建立与执行语句的联系
    connect(loginManagerAffirmButton,&QPushButton::clicked,this,&database::loginAManager);
    connect(loginManagerQuitButton,&QPushButton::clicked,this,&database::deleteloginManagerWindow);
    //退出 3-26
}

void database::showLoginUserWindow(){
    //和管理员登录差不多
    loginUserWindow= new QWidget();
    label40=new QLabel[2];
    vboxLayout40=new QVBoxLayout();
    vboxLayout41=new QVBoxLayout();
    hboxLayout40=new QHBoxLayout();
    hboxLayout41=new QHBoxLayout();
    gridLayout40=new QGridLayout();
    nameLine40=new QLineEdit();
    passwordLine40=new QLineEdit();
    passwordLine40->setEchoMode(QLineEdit::Password);
    //禁用按键
    registerButton->setDisabled(true);
    loginUserButton->setDisabled(true);
    loginManagerButton->setDisabled(true);
    loginUserAffirmButton=new QPushButton(tr("登录"));
    loginUserQuitButton=new QPushButton(tr("取消"));
    label40[0].setText(tr("用户名"));
    label40[1].setText(tr("密码"));

    gridLayout40->addWidget(&label40[0],1,1);
    gridLayout40->addWidget(&label40[1],2,1);
    gridLayout40->addWidget(nameLine40,1,2);
    gridLayout40->addWidget(passwordLine40,2,2);

    hboxLayout40->addWidget(loginUserAffirmButton);
    hboxLayout40->addWidget(loginUserQuitButton);

    vboxLayout41->addLayout(gridLayout40);
    vboxLayout41->addLayout(hboxLayout40);

    hboxLayout41->addStretch();
    hboxLayout41->addLayout(vboxLayout41);
    hboxLayout41->addStretch();

    loginUserWindow->setLayout(hboxLayout41);
    QPoint posOfWidget;
    posOfWidget.setX(window->pos().x()+window->width()/2-110);
    posOfWidget.setY(window->pos().y()+window->height()/2-90);
    loginUserWindow->move(posOfWidget);//只能手工调 3-27
//    loginUserWindow->move(window->pos());
    loginUserWindow->setWindowFlags(loginUserWindow->windowFlags() &~ Qt::WindowCloseButtonHint);//禁用关闭按钮
    loginUserWindow->show();

    //建立与执行语句的联系
    connect(loginUserAffirmButton,&QPushButton::clicked,this,&database::loginAUser);
    connect(loginUserQuitButton,&QPushButton::clicked,this,&database::deleteloginUserWindow);
}
void database::loginAUser(){
    if(nameLine40->text().isEmpty()||passwordLine40->text().isEmpty()){
        QMessageBox::critical(loginUserWindow,"Error","信息填写不完整！",QMessageBox::Yes);
        return;
    }else{
        QSqlQuery tmpQuery;
        if(!tmpQuery.exec("select username,password from users where username='" + nameLine40->text() +"'")){
            QMessageBox::critical(loginUserWindow,"Error",tmpQuery.lastError().text());
            return;
        }else{
            //qDebug()<<"select user name query success!";
        }
        if(!tmpQuery.next()){
            QMessageBox::critical(loginUserWindow,"Error","该用户名不存在",QMessageBox::Yes);
            return;
        }else{
            QString rightPassword=tmpQuery.value(1).toString();
            QString userName = tmpQuery.value(0).toString();
            if(rightPassword.compare(passwordLine40->text())){
                QMessageBox::critical(loginUserWindow,"Error","密码错误");
                return;
            }else{
                QMessageBox::about(loginUserWindow,"database","登陆成功");
                showBorrowTable();
                if(currentUser == MANAGER){
                    searchWindowTab->removeTab(0);
                    searchWindowTab->removeTab(0);
                    searchWindowTab->insertTab(0,userHomeWindow,"个人中心");
                    searchWindowTab->insertTab(0,searchBookWindow,"图书搜索");
                }
                else if(currentUser == VISITOR){
                    searchWindowTab->removeTab(0);
                    searchWindowTab->insertTab(0,userHomeWindow,"个人中心");
                    searchWindowTab->insertTab(0,searchBookWindow,"图书搜索");
                }
                loginUserWindow->QWidget::close();
                currentUser = USER;
                database::UpdateMainWindow(USER,userName);
            }
        }
    }
}
void database::loginAManager(){
    if(nameLine20->text().isEmpty()||passwordLine20->text().isEmpty()){
        QMessageBox::critical(loginManagerWindow,"Error","信息填写不完整！",QMessageBox::Yes);
        return;
    }else{
        QSqlQuery tmpQuery;
        if(!tmpQuery.exec("select managername,password from manager where managername='" + nameLine20->text() +"'")){
            QMessageBox::critical(loginManagerWindow,"Error",tmpQuery.lastError().text());
            return;
        }else{
            //qDebug()<<"select manager name query success!";
        }
        if(!tmpQuery.next()){
            QMessageBox::critical(loginManagerWindow,"Error","该用户名不存在",QMessageBox::Yes);
            return;
        }else{
            QString rightPassword=tmpQuery.value(1).toString();
            QString userName = tmpQuery.value(0).toString();//获取用户名
            if(rightPassword.compare(passwordLine20->text())){
                QMessageBox::critical(loginManagerWindow,"Error","密码错误");
                return;
            }else{//登录成功，关闭登录窗口，更新mainwindow，弹出提示框 3-26
                QMessageBox::about(loginManagerWindow,"database","登陆成功");
                loginManagerWindow->QWidget::close();
                if(currentUser == USER){
                    searchWindowTab->removeTab(0);
                    searchWindowTab->removeTab(0);
                    searchWindowTab->insertTab(0,managerWindow,"管理员界面");
                    searchWindowTab->insertTab(0,searchBookWindow,"图书搜索");
                }
                else if(currentUser == VISITOR){
                    searchWindowTab->removeTab(0);
                    searchWindowTab->insertTab(0,managerWindow,"管理员界面");
                    searchWindowTab->insertTab(0,searchBookWindow,"图书搜索");
                }
                currentUser = MANAGER;
                database::UpdateMainWindow(MANAGER,userName);
            }
        }
    }
}

void database::registerAUser(){
    if(nameLine10->text().isEmpty()||passwordLine10->text().isEmpty()||passwordLine11->text().isEmpty()){
        QMessageBox::critical(registerWindow, "Error", "您的信息填写不完整!",QMessageBox::Yes);
        return;
    }else if(passwordLine11->text().compare(passwordLine10->text())){
        QMessageBox::critical(registerWindow,"Error","两次输入密码不一致！",QMessageBox::Yes);
        return;
    }
    //进入数据库
    //已经有了这个用户名，报错
    QSqlQuery tmpQuery;
    if(!tmpQuery.exec("select username from users where username='" + nameLine10->text() + "'")){
        QMessageBox::critical(registerWindow,"Error",tmpQuery.lastError().text());
    }else{
        //qDebug()<<"select query success";
    }
    if(tmpQuery.next()){
        QMessageBox::critical(registerWindow, "Error", "该用户名已被注册",QMessageBox::Yes);
        return;
    }
    //向user表中插入用户信息
    tmpQuery.prepare("insert into users values(?,?,'false')");
    QVariantList name,password;
    name<<nameLine10->text();
    password<<passwordLine10->text();
    //qDebug()<<name<<" "<<password;
    tmpQuery.addBindValue(name);
    tmpQuery.addBindValue(password);
    if(tmpQuery.execBatch()){
        QMessageBox::about(registerWindow,"database","注册成功！");
        registerWindow->close();
        //启用按键
        registerButton->setDisabled(false);
        loginUserButton->setDisabled(false);
        loginManagerButton->setDisabled(false);
    }else{
        QMessageBox::critical(registerWindow, "Error", "注册失败",QMessageBox::Yes);
        return;
    }
    //tmpQuery.finish();
}
//关闭管理员登录窗口 3-26
void database::deleteloginManagerWindow(){
    if (!(QMessageBox::information(loginManagerWindow,tr("提示"),tr("您真的要取消登录吗?"),tr("是"),tr("否")))){
        loginManagerWindow->close();
        //启用按键
        registerButton->setDisabled(false);
        loginUserButton->setDisabled(false);
        loginManagerButton->setDisabled(false);
    }
}
void database::deleteloginUserWindow(){
    if (!(QMessageBox::information(loginUserWindow,tr("提示"),tr("您真的要取消登录吗?"),tr("是"),tr("否")))){
        loginUserWindow->close();
        //启用按键
        registerButton->setDisabled(false);
        loginUserButton->setDisabled(false);
        loginManagerButton->setDisabled(false);
    }
}
void database::deleteRegisterWindow(){
    if (!(QMessageBox::information(registerWindow,tr("提示"),tr("您真的要退出吗?"),tr("是"),tr("否")))){
        registerWindow->close();
        //启用按键
        registerButton->setDisabled(false);
        loginUserButton->setDisabled(false);
        loginManagerButton->setDisabled(false);
    }
}
//登录后更新MainWindow 3-26
void database::UpdateMainWindow(type typeOfUser, QString userName){
    //启用按键
    registerButton->setDisabled(false);
    loginUserButton->setDisabled(false);
    loginManagerButton->setDisabled(false);
    if(typeOfUser == MANAGER){
        spaceLabel00->setText("您好, " + userName + "! 登录状态：管理员登录");
        loginManagerButton->setText("切换管理员");
        loginUserButton->setText("用户登录");
        if(!logoutButton){
            logoutButton=new QPushButton(tr("退出"));
            hboxLayout00->addWidget(logoutButton);
        }
    }else if(typeOfUser == USER){
        spaceLabel00->setText("您好, " + userName + "! 登录状态：用户登录");
        loginUserButton->setText("切换用户");
        loginManagerButton->setText("管理员登录");
        if(!logoutButton){
            logoutButton=new QPushButton(tr("退出"));
            hboxLayout00->addWidget(logoutButton);
        }
    }else if(typeOfUser == VISITOR){
        spaceLabel00->setText("您好！登录状态：未登录");
        loginUserButton->setText("用户登录");
        loginManagerButton->setText("管理员登录");
        delete logoutButton;
        logoutButton = NULL;
    }
    connect(logoutButton,&QPushButton::clicked,this,&database::logout);
}
//退出登录 3-26
void database::logout(){
    searchWindowTab->removeTab(0);
    searchWindowTab->removeTab(0);
    searchWindowTab->insertTab(0,searchBookWindow,"图书搜索");
    currentUser = VISITOR;
    UpdateMainWindow(VISITOR, NULL);
}

//时间显示 3-26
void database::currentTime(){
    QDateTime dateTime = QDateTime::currentDateTime();//获取系统时间
    QString currentTime = dateTime.toString("yyyy-MM-dd ddd hh:mm:ss");//转化为字符串
    QString textTime = "当前时间: " + currentTime + " ";
    spaceLabel02->setText(textTime);
}

bool database::connectdb(const QString & dbName){
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("Yh030610");
    db.setDatabaseName(dbName);
    if (!db.open()) {
        QMessageBox::critical(window, QObject::tr("Database Error"),db.lastError().text());
        return false;
    }else{
        QSqlQuery tmpQuery;
        if(!tmpQuery.exec("create table if not exists users (username varchar(20) primary key,password varchar(30),isLent enum('true','false'))")){
            QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        }else{
            //qDebug()<<"create table users success";
        }
        if(!tmpQuery.exec("create table if not exists manager (managername varchar(20),password varchar(30))")){
            QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        }else{
            //qDebug()<<"create table users success";
        }
        //年份、数量是整数类型； 价格是两位小数类型； 其余为字符串类型
        //3
        if(!tmpQuery.exec("create table if not exists books (bookID char(15) primary key,type varchar(10),title varchar(20),press varchar(20),year int,author varchar(20),price numeric(5,2),total int,stock int)")){
            QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        }else{
            //qDebug()<<"create table users success";
        }
        if(!tmpQuery.exec("create table if not exists borrow (username varchar(20),bookID char(15),borrowDate varchar(20),returnDate varchar(20),foreign key(username) references users(username) on delete cascade on update cascade,foreign key(bookID) references books(bookID) on delete cascade on update cascade)")){
            QMessageBox::critical(window,"Error",tmpQuery.lastError().text());
        }else{
            //qDebug()<<"create table users success";
        }
    }
    return true;
}
