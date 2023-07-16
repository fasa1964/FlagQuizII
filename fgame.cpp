#include "fgame.h"

#include <QLocale>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>



FGame::FGame(QObject *parent)
    : QObject{parent}
{
    if(getFlagsCount() > 0)
        setFlags(true);
    else
        setFlags(false);


    QFile file("data.json");
    if(file.exists()){
        setBorders(true);
        setAreas(true);
        setCapitals(true);
    }else{
        setBorders(false);
        setAreas(false);
        setCapitals(false);
    }
}

void FGame::startFlagsGame()
{

}

bool FGame::flags() const
{
    return m_flags;
}

void FGame::setFlags(bool newFlags)
{
    if (m_flags == newFlags)
        return;
    m_flags = newFlags;
    emit flagsChanged();
}

bool FGame::borders() const
{
    return m_borders;
}

void FGame::setBorders(bool newBorders)
{
    if (m_borders == newBorders)
        return;
    m_borders = newBorders;
    emit bordersChanged();
}

bool FGame::areas() const
{
    return m_areas;
}

void FGame::setAreas(bool newAreas)
{
    if (m_areas == newAreas)
        return;
    m_areas = newAreas;
    emit areasChanged();
}

bool FGame::capitals() const
{
    return m_capitals;
}

void FGame::setCapitals(bool newCapitals)
{
    if (m_capitals == newCapitals)
        return;
    m_capitals = newCapitals;
    emit capitalsChanged();
}

int FGame::getFlagsCount()
{
    QDir dir;
    QStringList list = dir.entryList( QStringList() << "*.svg", QDir::Files);
    return list.count();
}
