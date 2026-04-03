#include "UserSession.h"


UserSession::UserSession()
	: id(0), username(""), tipUser(""), esteLogat(false)
{
}

UserSession& UserSession::getInstance()
{
	static UserSession instance;
	return instance;
// static aici inseamna ca instance se creeaza o singura data la primul apel si ramane in memorie pana la inchiderea app
}

void UserSession::login(int _id, const QString& _username, const QString& _tip)
{
	id = _id;
	username = _username;
	tipUser = _tip;
	esteLogat = true;
}

void UserSession::logout()
{
	id = 0;
	username = "";
	tipUser = "";
	esteLogat = false;
}
