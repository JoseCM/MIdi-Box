#include "monitormodel.h"

MonitorModel::MonitorModel()
{

}


int MonitorModel::rowCount(const QModelIndex &parent) const{

    return messageList.size();
}

int MonitorModel::columnCount(const QModelIndex &parent) const{
    return 4;
}


QVariant MonitorModel::data(const QModelIndex &index, int role) const{

    if (!index.isValid())
        return QVariant();

    if (index.row() >= messageList.size() || index.row() < 0)
        return QVariant();

    switch (role){
        case ChannelRole:
            //return activitylist[index.row()];
        case MessageRole:

        case DataRole:

        default:
            return QVariant();
    }
}
