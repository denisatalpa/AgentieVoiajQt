#pragma once
#include <QString>
#include <QDebug>

// am facut acest fisier pt a demonstra supraincrcarea operatorilor

struct OfertaInfo
{
    int id = 0;
    QString plecare;
    QString destinatie;
    QString dataPlecare;
    QString dataIntoarcere;
    QString transport;

    double pretRegular = 0.0;
    double pretPremium = 0.0;

    int locuriRegular = 0;
    int locuriPremium = 0;

    // operator== doua oferte sunt egale daca au acelasi id din BD
    bool operator==(const OfertaInfo& other) const
    {
        return id == other.id;
    }

    // operator!= opus lui ==
    bool operator!=(const OfertaInfo& other) const
    {
        return !(*this == other);
    }

    // operator< sorteaza ofertele dupa pretul regular crescator
    // necesar pt std sort

    bool operator<(const OfertaInfo& other) const
    {
        return pretRegular < other.pretRegular;
    }

    // operator> : sorteaza descrescator
    bool operator>(const OfertaInfo& other) const
    {
        return other < *this;
    }
};

// operator<< pt QDebug ca sa permita qdebug() << oferta;

inline QDebug operator<<(QDebug dbg, const OfertaInfo& o)
{
    dbg.nospace()
        << "Oferta[id=" << o.id
        << " | " << o.plecare << " -> " << o.destinatie
        << " | " << o.dataPlecare << " - " << o.dataIntoarcere
        << " | " << o.transport
        << " | Regular: " << o.pretRegular << " RON"
        << " | Premium: " << o.pretPremium << " RON"
        << " | Locuri R:" << o.locuriRegular
        << " P:" << o.locuriPremium << "]";
    return dbg.space();
}