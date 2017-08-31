#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QPushButton>
#include <mainwindow.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QGroupBox>
#include <QComboBox>
#include <QCloseEvent>
#include<QFileDialog>
/*******************
命名规则：
    1.仿照Qt的命名规则，开头小写，如果是多个单词拼成，则后面的每个单词开头大写
    2.Label，VBox，HBox的命名，先写相应的变量类型，后接两个数，第一个数是某个界面的编号，在变量声明时规定好，
      第二个数是某界面同一种类型的编号，从0,1,2开始。
********************/
class database : public QObject
{
    Q_OBJECT
public:
    database(QObject *parent = 0);
    void run();
    ~database();

signals:

public slots:
    void showLoginManagerWindow();
    void showLoginUserWindow();
    void showRegisterWindow();
    void registerAUser();
    void loginAManager();
    void logout();
    void currentTime();
    void loginAUser();
    void deleteloginManagerWindow();
    void deleteloginUserWindow();
    void deleteRegisterWindow();
    void batchProcess();
    void addOneBook();
    void searchBook();
    void clearInput();
    void nextPage();
    void lastPage();
    void searchUser();
    void giveAuthorization();
    void revokeAuthorization();
    void changeAuthorization();
    void deleteUser();
    void borrowBook();
    void changePassword();
    void returnBook();
private:
    enum type{MANAGER,USER,VISITOR};//用户状态
    QString currentUser;
    //主界面变量，编号0
    QPushButton *registerButton;
    QPushButton *loginUserButton;
    QPushButton *loginManagerButton;
    QPushButton *logoutButton;

    QTimer *timer;
    QTabWidget *searchWindowTab;
    QWidget *window;
    QWidget *titleBar;
    QLabel *spaceLabel00;
    QLabel *spaceLabel01;
    QLabel *spaceLabel02; //时间显示
    QHBoxLayout *hboxLayout00;
    QHBoxLayout *hboxLayout01;
    QHBoxLayout *hboxLayout02;//时间显示
    QVBoxLayout *vboxLayout00;
    QVBoxLayout *vboxLayout01;

    //注册界面需要的，编号1
    QWidget *registerWindow;
    QHBoxLayout *hboxLayout10;//总布局
    QHBoxLayout *hboxLayout11;//上面一条
    QHBoxLayout *hboxLayout12;//下面一条
    QVBoxLayout *vboxLayout10;//中间三部分
    QGridLayout *gridLayout10;//网格布局
    QLabel *label10;//用户名，密码，确认密码
    QLabel *label11;//请输入信息。。。。
    QPushButton *registerAffirmButton;//确认键
    QPushButton *registerQuitButton;//退出键
    QLineEdit *nameLine10;
    QLineEdit *passwordLine10;
    QLineEdit *passwordLine11;

    //登录管理员界面，编号2
    QWidget *loginManagerWindow;
    QLabel *label20;
    QVBoxLayout *vboxLayout20;
    QVBoxLayout *vboxLayout21;
    QHBoxLayout *hboxLayout20;
    QHBoxLayout *hboxLayout21;
    QGridLayout *gridLayout20;
    QLineEdit *nameLine20;
    QLineEdit *passwordLine20;
    QPushButton *loginManagerAffirmButton;
    QPushButton *loginManagerQuitButton;

    //主界面 图书搜索 窗口 编号3
    QWidget *searchBookWindow;
    QHBoxLayout *hboxLayout30;
    QHBoxLayout *hboxLayout31;
    QHBoxLayout *hboxLayout32;
    QHBoxLayout *hboxLayout33;
    QHBoxLayout *hboxLayout34;
    QHBoxLayout *hboxLayout35;

    QVBoxLayout *vboxLayout30;
    QVBoxLayout *vboxLayout31;
    //QVBoxLayout *vboxLayout32;

    QGridLayout *gridLayout30;
    QGridLayout *gridLayout31;

    QPushButton *lastPageButton;
    QPushButton *nextPageButton;
    QPushButton *searchButton30;
    QPushButton *clearButton30;
    QPushButton *borrowBookButton;

    QComboBox *firstSequence;
    QComboBox *secondSequence;
    QPushButton *sortButton;
    int currentPage;
    int maxPage;
    bool borrowFlag;

    QTableWidgetItem *item30[50];
    QTableWidgetItem *item31[50][10];
    QString queryText;

    QGroupBox *groupBox30;
    QGroupBox *groupBox31;
    QGroupBox *groupBox32;
    QGroupBox *groupBox33;
    QLabel *label30;
    QLabel *label31;
    QLabel *label32;
    QLabel *label33;
    QLabel *label34;
    //登录用户界面，编号4
    QWidget *loginUserWindow;
    QLabel *label40;
    QVBoxLayout *vboxLayout40;
    QVBoxLayout *vboxLayout41;
    QHBoxLayout *hboxLayout40;
    QHBoxLayout *hboxLayout41;
    QGridLayout *gridLayout40;
    QLineEdit *nameLine40;
    QLineEdit *passwordLine40;
    QPushButton *loginUserAffirmButton;
    QPushButton *loginUserQuitButton;
    //核心窗口中的管理员界面,编号5
    QWidget *managerWindow;
    QTableWidget *userTable;
    QPushButton *batchProcessButton50;
    QPushButton *addOneBookConfirmButton50;
    QPushButton *searchUserButton50;
    QPushButton *authorizationButton50;
    QPushButton *revokeButton50;
    QPushButton *changeButton50;
    QPushButton *deleteUserButton50;
    QLabel *label50;//单册添加
    QLabel *label51;
    QLabel *label52;
    QLabel *label53;
    QLabel *label54;
    QLineEdit *bookId50;
    QComboBox *bookType50;
    QLineEdit *bookName50;
    QLineEdit *press50;
    QLineEdit *year50;
    QLineEdit *author50;
    QLineEdit *price50;
    QLineEdit *total50;
    QLineEdit *stock50;
    QLineEdit *userName50;

    QString userQueryText;
    QTableWidgetItem *item50[10];
    QTableWidgetItem *item51[10][3];

    QVBoxLayout *vboxLayout50;
    QVBoxLayout *vboxLayout51;
    QVBoxLayout *vboxLayout52;
    QVBoxLayout *vboxLayout53;
    QHBoxLayout *hboxLayout50;
    QHBoxLayout *hboxLayout51;
    QHBoxLayout *hboxLayout52;
    QHBoxLayout *hboxLayout53;
    QHBoxLayout *hboxLayout54;
    QHBoxLayout *hboxLayout55;
    QHBoxLayout *hboxLayout56;
    QHBoxLayout *hboxLayout57;
    QHBoxLayout *hboxLayout58;
    QGridLayout *gridLayout50;
    QGroupBox *groupBox50;
    QGroupBox *groupBox51;
    QGroupBox *groupBox52;
    //批量导入窗口，编号6
    QWidget *batchProcessWindow60;
    //书号, 类别, 书名, 出版社, 年份, 作者, 价格（区间）, 总藏书量（区间）, 库存（区间）
    QLineEdit *bookId;
    QLineEdit *bookName;
    QLineEdit *press;
    QComboBox *bookType;
    QComboBox *yearLow;
    QComboBox *yearHigh;
    QLineEdit *author;
    QComboBox *priceLow;
    QComboBox *priceHigh;
    QComboBox *totalNumLow;
    QComboBox *totalNumHigh;
    QComboBox *stockLow;
    QComboBox *stockHigh;
    QTableWidget *bookTable;

    //个人中心 编号7
    QWidget *userHomeWindow;
    QTableWidget *userBorrowTable;
    QPushButton *changePasswordAffirmButton;
    QPushButton *returnBookButton;
    QHBoxLayout *hboxLayout70;
    QHBoxLayout *hboxLayout71;
    QHBoxLayout *hboxLayout72;
    QHBoxLayout *hboxLayout73;
    QVBoxLayout *vboxLayout70;
    QVBoxLayout *vboxLayout71;
    QVBoxLayout *vboxLayout72;
    QGridLayout *gridLayout70;
    QGroupBox *groupBox70;
    QGroupBox *groupBox71;
    QLineEdit *oldPassword70;
    QLineEdit *newPassword70;
    QLineEdit *newPassword71;
    QLabel *label70;
    QLabel *label71;
    QTableWidgetItem *item70[10];
    QTableWidgetItem *item71[10][5];



    //登录用户界面
    void SetWindowLayout();
    void SetMainWindow();
    void SetCoreWindow();
    bool connectdb(const QString & dbName);
    void UpdateMainWindow(type typeOfUser, QString userName);
    void showBookTable(type type);
    void showUserTable(type type);
    void showBorrowTable();
    void setManagerWindow();
    void setUserWindow();
    //数据库变量
    QSqlDatabase db;
    QSqlQuery query;
    QSqlQuery *bookQuery;
    QSqlQuery *userQuery;
    //QString str;
};

#endif // DATABASE_H
