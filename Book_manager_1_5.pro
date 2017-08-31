#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T13:54:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Book_manager_1_5
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    database.h

FORMS    += mainwindow.ui
#更新日志：
# 3-26 增加时间显示功能；管理员登陆取消键；登录后更新mainwindow，退出函数logout（缺corewindow）。
# 3-29 19:31 修改了初始化的部分。把构造函数里的东西拿出来叫run函数，public。在main函数里先构造对象，再调用run函数，更安全。--clh
# 3-29 20:20 把表的初始化放在了数据库的连接时，具体在connectdb中，这使得只需要demo数据库存在，密码正确就行了。tables如果不存在会自动创建。
# 3-30 16:18 添加了批量导入书籍信息功能，按钮在新table里，叫管理员界面,书籍信息存储在books_batch_Process.txt里，txt在程序文件夹里。--clh
# 3-30 16:19 版本更新：0.6 已上传到BOX
# 3-30 20:46 修复注册窗口bug，均采用禁用关闭按键，选择取消键退出的方式。--yh
# 3-31 17:01 将MessageBox的位置设置在窗口中间, 修复退出登录后注册键失灵以及可能内存泄漏的bug。--yh
# 3-31 17:11 版本更新：0.7 已上传到BOX
# 4-3  19:29 添加图书搜索及显示功能--yh
# 4-3  19:30 版本更新：0.8 已上传到BOX
# 4-4  09:37 添加单册导入功能--clh
# 4-4  09:37 版本更新：0.9 已上传到BOX --clh
# 4-4 10:25 实现了图书搜索排序功能
# 4-4 10:25  版本更新，0.10 已上传到BOX --clh
# 4-4 11:21 添加用户搜索功能 --yh
# 4-4 11:22  版本更新，0.11 已上传到BOX --yh
# 4-4 20:45 add a new function:change the authorization of users, that is, change isLent. --clh
# 4-4 21:02 Edtion update,0.12 already upload to BOX --clh
# 4-4 21:56 即时更新权限--yh
# 4-5 10:40 添加借书功能，新建borrow表，级联删除更新，users和books加primary key--yh
# 4-5 10:45  版本更新，0.13 已上传到BOX --yh
# 4-5 16:57 Add function:manager can delete users--clh
# 4-5 18:58 Add table Home --clh
# 4-5 18:58 Add function:change password --clh
# 4-5 19:04 Edition update:0.14, uploaded to BOX --clh
# 4-5 21:35 个人中心借书显示，选项卡按照当前登录状态显示--yh
# 4-5 21:45 更改密码后自动退出--yh
# 4-5 21:45  版本更新，0.15 已上传到BOX --yh
# 4-6 16:15 添加功能：还书 --wyb
# 4-6 16:15 添加限制：每人最多借十本书. --clh
# 4-6 16:21 修复bug：没有借书权限的用户不能借书 --clh
# 4-6 16:46 版本更新：1.0 --clh 已上传到BOX
# 4-6 23:09 完善书籍类型，addOneBook类型检查，界面美化--yh
# 4-6 23:10 版本更新：1.1, 已上传到BOX--yh
# 4-7 14:28 修复bug：用户注册默认false借书权限--clh
# 4-7 14:28 版本更新：1.2 ，已上传到BOX--clh
# 4-7 18:07 修复bug：借书列表加作者列，查询按照书号搜索--yh
# 4-7 18:22 版本更新：1.3 ，已上传到BOX--yh
# 4-8 10:49 删掉上一页下一页，直接显示50条记录。--clh
# 4-8 10:50 版本更新：1.4 clh
# 4-15 19:30 修复bug：借书不超过十本及最近还书的检查 -- yh
# 4-15 19:30 版本更新：1.5 -- yh
# BUGS：
#   弹窗位置，只能手工调
#   退出登录时提示invalid null parameter？？？
#   批量导入warning
#   还书之后库存需要刷新
#ATTENTION!!!
#BY THE WAY!!!!
#DON'T PUT CONNECT FUNCTIONS DEFINITIONS IN OTHER SLOT FUNCTIONS OR THE FUNCTIONS THAT OTHER FUNCTIONS WILL CALL!!
