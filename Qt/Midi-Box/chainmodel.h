#ifndef CHAINMODEL_H
#define CHAINMODEL_H

#include <QObject>

class ChainModel
{
    Q_OBJECT

    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)

public:
    ChainModel(QObject *parent=0);
    ChainModel(QString &color);
    QString setColor(QString color);
    QString color() const ;

signals:
    void colorChanged();


private:
    QString m_color;


};

#endif // CHAINMODEL_H
