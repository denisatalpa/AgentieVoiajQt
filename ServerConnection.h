#pragma once
#include <QString>
#include <QStringList>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

class ServerConnection {
public:
    static ServerConnection& getInstance();
    ServerConnection(const ServerConnection&) = delete;
    ServerConnection& operator=(const ServerConnection&) = delete;

    bool conecteaza(const QString& ip, int port);
    void deconecteaza();
    bool esteConectat() const;

    // Trimite o comanda si primeste raspunsul complet
    QStringList trimiteComanda(const QString& comanda);

private:
    ServerConnection();
    ~ServerConnection();
    SOCKET m_sock = INVALID_SOCKET;
    bool   m_conectat = false;
};