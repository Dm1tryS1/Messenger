#include "myserver.h"
#include "myclient.h"

bool MyServer::startServer(QHostAddress addr, qint16 port)
{
    if (!listen(addr, port))
    {
        qDebug() << "Server not started at" << addr << ":" << port;
        return false;
    }
    qDebug() << "Server started at" << addr << ":" << port;
    return true;
}

void MyServer::sendToAllUserJoin(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0 << MyClient::comUserJoin << name;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName() != name && _clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void MyServer::sendToAllUserLeft(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comUserLeft << name;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName() != name && _clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void MyServer::sendMessage(QString message, QString fromUsername,const QStringList &users)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    if (users.empty())
    {
        out << (quint16)0 << MyClient::comMessageToAll << fromUsername << message;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        for (int i = 0; i < _clients.length(); ++i)
            if (_clients.at(i)->getAutched())
                _clients.at(i)->_sok->write(block);
    }
    else
    {
        QByteArray blockToSender;
        out << (quint16)0 << MyClient::comMessageToUsers << fromUsername << message;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        QDataStream outToSender(&blockToSender, QIODevice::WriteOnly);
        outToSender << (quint16)0 << MyClient::comMessageToUsers << users.join(",") << message;
        outToSender.device()->seek(0);
        outToSender << (quint16)(blockToSender.size() - sizeof(quint16));
        for (int j = 0; j < _clients.length(); ++j)
            if (users.contains(_clients.at(j)->getName()))
                _clients.at(j)->_sok->write(block);
            else if (_clients.at(j)->getName() == fromUsername)
                _clients.at(j)->_sok->write(blockToSender);
    }
}

void MyServer::sendServerMessage(QString message,  const QStringList &users)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    if (users.empty())
    {
        out << (quint16)0 << MyClient::comPublicServerMessage << message;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        for (int i = 0; i < _clients.length(); ++i)
            if (_clients.at(i)->getAutched())
                _clients.at(i)->_sok->write(block);
    }
    else
    {
        out << (quint16)0 << MyClient::comPrivateServerMessage << message;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        for (int j = 0; j < _clients.length(); ++j)
            if (users.contains(_clients.at(j)->getName()))
                _clients.at(j)->_sok->write(block);
    }
}

QStringList MyServer::getUsersOnline()
{
    QStringList l;
    foreach (MyClient * c, _clients)
        if (c->getAutched())
            l << c->getName();
    return l;
}

bool MyServer::isNameValid(QString name)
{
    QRegExp r("[A-Za-z0-9_]+");
    if (name.length() > 20 || name.length() < 5)
        return false;
    else if (r.exactMatch(name))
         {
            for (int i = 0; i < _clients.length(); ++i)
                if (_clients.at(i)->getName() == name)
                    return false;
         }
         else
            return false;
    return true;
}

void MyServer::incomingConnection(qintptr handle)
{
    MyClient *client = new MyClient(handle, this);
    _clients.append(client);
    qDebug() << "Client created";
}

void MyServer::onAddUser(QString name)
{
    emit addUserToGui(name);
}

void MyServer::onRemoveUser(QString name, MyClient *client)
{
    emit removeUserFromGui(name);
    _clients.removeAt(_clients.indexOf(client));
}
