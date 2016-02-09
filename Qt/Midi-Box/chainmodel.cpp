#include "chainmodel.h"

ChainModel::ChainModel(QObject *parent)
 : QObject(parent)
{

}

ChainModel::ChainModel(QString &color)
    : m_color(color)
{

}


QString ChainModel::setColor(QString color){
    m_color = color;
    emit colorChanged();
}

QString ChainModel::color() const {
    return m_color;
}
