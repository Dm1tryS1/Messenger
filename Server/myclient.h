#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QTcpSocket>
#include <QMessageBox>

class MyServer;

class MyClient : public QObject
{
    Q_OBJECT
    friend class MyServer;

public:
    static const QString constNameUnknown;
    static const quint8 comAutchReq = 1;
    static const quint8 comUsersOnline = 2;
    static const quint8 comUserJoin = 11;
    static const quint8 comUserLeft = 12;
    static const quint8 comMessageToAll = 21;
    static const quint8 comMessageToUsers = 22;
    static const quint8 comPublicServerMessage = 31;
    static const quint8 comPrivateServerMessage = 32;
    static const quint8 comAutchSuccess = 41;
    static const quint8 comErrNameInvalid = 201;
    static const quint8 comErrNameUsed = 202;

    MyClient(int desc, MyServer *serv, QObject *parent = 0);

    void setName(QString name) {_name = name;}
    QString getName() const {return _name;}
    bool getAutched() const {return _isAutched;}
    void sendCommand(quint8 comm) const;
    void usersOnline() const;

signals:
    void addUserToGui(QString name);
    void removeUser(MyClient *client);
    void messageToGui(QString message, QString from, const QStringList &users);

private slots:
    void onDisconnect();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError) const;

private:
    QTcpSocket *_sok;
    MyServer *_serv;
    quint16 _blockSize;
    QString _name;
    bool _isAutched;

};

#endif // MYCLIENT_H
