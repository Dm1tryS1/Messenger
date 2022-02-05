#ifndef DIALOG_H
#define DIALOG_H

#include "myserver.h"
#include <QtGui>
#include <QMessageBox>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    MyServer *_serv;
    void addToLog(QString text, QColor color);

public slots:
    void onAddUserToGui(QString name);
    void onRemoveUserFromGui(QString name);
    void onMessageToGui(QString message, QString from, const QStringList &users);

private slots:
    void on_pbSend_clicked();
    void on_cbToAll_clicked();
    void on_pbStartStop_toggled(bool checked);
};

#endif // DIALOG_H
