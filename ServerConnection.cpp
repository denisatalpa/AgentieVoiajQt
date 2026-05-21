#include "ServerConnection.h"
#include <WS2tcpip.h>
#include <QDebug>

ServerConnection::ServerConnection() {
    WSADATA w;
    WSAStartup(MAKEWORD(2, 2), &w);
}

ServerConnection::~ServerConnection() {
    deconecteaza();
    WSACleanup();
}

ServerConnection& ServerConnection::getInstance() {
    static ServerConnection instance;
    return instance;
}

bool ServerConnection::conecteaza(const QString& ip, int port) {
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) return false;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.toStdString().c_str(), &server.sin_addr);

    if (connect(m_sock, (struct sockaddr*)&server, sizeof(server))
        == SOCKET_ERROR) {
        qDebug() << "ServerConnection: nu ma pot conecta la server";
        return false;
    }
    m_conectat = true;
    return true;
}

void ServerConnection::deconecteaza() {
    if (m_sock != INVALID_SOCKET) {
        closesocket(m_sock);
        m_sock = INVALID_SOCKET;
    }
    m_conectat = false;
}

bool ServerConnection::esteConectat() const { return m_conectat; }

QStringList ServerConnection::trimiteComanda(const QString& comanda) {
    // Trimitem comanda cu \n la final
    std::string msg = comanda.toStdString() + "\n";
    send(m_sock, msg.c_str(), (int)msg.size(), 0);

    // Primim raspunsul linie cu linie pana la "END" sau o singura linie
    QStringList linii;
    std::string linie;
    char c;
    while (recv(m_sock, &c, 1, 0) > 0) {
        if (c == '\n') {
            QString l = QString::fromStdString(linie);
            if (l == "END") break;
            linii.append(l);
            // Daca e un raspuns de o singura linie (OK|... sau ERR|...)
            // si nu asteptam END, iesim
            if (!comanda.startsWith("GET_"))
                break;
            linie.clear();
        }
        else {
            linie += c;
        }
    }
    return linii;
}