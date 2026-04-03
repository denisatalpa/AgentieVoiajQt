#pragma once
#include <QString>


struct UserSession {
public:
	static UserSession& getInstance();

	//stergem constructorul d copiere si operatorul de atribuire ca sa fie imposibil sa se creeze o a doua instanta
	UserSession(const UserSession&) = delete;
	UserSession& operator=(const UserSession&) = delete;

	void login(int _id, const QString& _username, const QString& _tip);
	void logout();

	int id;
	QString username;
	QString tipUser;
	bool esteLogat;

private:
	UserSession(); // constructorul e privat - nimeni din afara nu poate face new usersession
};
