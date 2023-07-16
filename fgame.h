#ifndef FGAME_H
#define FGAME_H

#include <QObject>
#include <QQmlEngine>

class FGame : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(bool borders READ borders WRITE setBorders NOTIFY bordersChanged)
    Q_PROPERTY(bool areas READ areas WRITE setAreas NOTIFY areasChanged)
    Q_PROPERTY(bool capitals READ capitals WRITE setCapitals NOTIFY capitalsChanged)

public:
    explicit FGame(QObject *parent = nullptr);

    Q_INVOKABLE void startFlagsGame();


    bool flags() const;
    void setFlags(bool newFlags);

    bool borders() const;
    void setBorders(bool newBorders);

    bool areas() const;
    void setAreas(bool newAreas);

    bool capitals() const;
    void setCapitals(bool newCapitals);

signals:

    // QML Properties
    void flagsChanged();
    void bordersChanged();
    void areasChanged();
    void capitalsChanged();


private:

    // QML Properties
    bool m_flags;
    bool m_borders;
    bool m_areas;
    bool m_capitals;






    int getFlagsCount();


};

#endif // FGAME_H
