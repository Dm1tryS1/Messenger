#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QDebug>

class MyClient;

class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    bool startServer(QHostAddress addr, qint16 port); // Запуск сервера

    void sendToAllUserJoin(QString name); // Сообщение о том, что присоединился клиент
    void sendToAllUserLeft(QString name); // Сообщение о том, что отсоединилися клиент

    void sendMessage(QString message, QString fromUsername, const QStringList &users); // Отправка сообщения пользователем
    void sendServerMessage(QString message, const QStringList &users); // Отправка сообщение админом сервера

    QStringList getUsersOnline(); // Список пользователей, подключенных к серверу
    bool isNameValid(QString name); // Валидация имени

    void incomingConnection(qintptr handle); // Подключение пользователя

signals:
    void addUserToGui(QString name); // Добавить нового пользователя (интерфейсная часть)
    void removeUserFromGui(QString name); // Убрать пользователя из списка (интерфейсная часть)

public slots:
    void onAddUser(QString name); // Добавить нового пользователя (внутреннее представление)
    void onRemoveUser(QString name, MyClient *client); // Убрать пользователя из списка (внутреннее представление)

private:
    QList<MyClient *> _clients; // Список клиентов на сервере
};

#endif // MYSERVER_H
