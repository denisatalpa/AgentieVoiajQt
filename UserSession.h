#pragma once
#include <QString>

// struct care tine minte cine e logat
struct UserSession {
	static int id;
	static QString username;
	static QString tipUser;
	static bool esteLogat;

	static void login(int _id, const QString& _username, const QString& _tip) {
		id = _id;
		username = _username;
		tipUser = _tip;
		esteLogat = true;
	}

	static void logout() {
		id = 0;
		username = "";
		tipUser = "";
		esteLogat = false;
	}
};
