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

    if (index.row() >= channelList.size() || index.row() < 0)
        return QVariant();

    if (role == ChannelRole){

        return channelList[index.row()];
    } else if (role == MessageRole) {

        return messageList[index.row()];
    } else if (role == Data1Role) {

        return data1List[index.row()];
    } else if (role == Data1Role) {

        return data2List[index.row()];
    }

    return QVariant();
}

bool MonitorModel::setData(const QModelIndex & index, const QVariant & value, int role){

    if (index.isValid() && role == Qt::EditRole) {

           int row = index.row();

           if (index.column() == 0){

               channelList[index.row()] = value.toString();
           } else if (index.column() == 1){

               channelList[index.row()] = value.toString();
           } else if (index.column() == 2){

               channelList[index.row()] = value.toString();
           } else if (index.column() == 3){

              channelList[index.row()] = value.toString();
           }
           else
               return false;

           emit(dataChanged(index, index));

           return true;
       }

       return false;
}

Qt::ItemFlags MonitorModel::flags(const QModelIndex & index) const{
    if (!index.isValid())
         return Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant MonitorModel::headerData(int section, Qt::Orientation orientation, int role) const{

    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Channel");

            case 1:
                return tr("Message");

            case 2:
                return tr("Data 1");

            case 3:
                return tr("Data 2");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool MonitorModel::insertRows(int row, int count, const QModelIndex & parent){

    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count -1);

    for(int i = 0; i < count; i++){
        channelList.insert(row, "");
        messageList.insert(row, "");
        data1List.insert(row, "");
        data2List.insert(row, "");
    }

    endInsertRows();

    return true;
}

bool MonitorModel::removeRows(int row, int count, const QModelIndex & parent){
    Q_UNUSED(parent);

    if(row+count-1 < 0)
        return false;

    beginRemoveRows(QModelIndex(), row, row + count -1);

    for(int i = 0; i < count; i++){
        channelList.removeAt(row);
        messageList.removeAt(row);
        data1List.removeAt(row);
        data2List.removeAt(row);
    }

    endRemoveRows();

    return true;
}

QHash<int, QByteArray>  MonitorModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[ChannelRole] = "Channel";
    roles[MessageRole] = "Message";
    roles[Data1Role] = "Data 1";
    roles[Data2Role] = "Data 2";
    return roles;
}

bool MonitorModel::clearModel(){
    return removeRows(0, channelList.size(), QModelIndex());
}

void MonitorModel::addMessageToMonitor(string ch, string msg, string dt1, string dt2){
    insertRows(channelList.size(), 1, QModelIndex());
    setData(index(channelList.size() - 1, 0, QModelIndex()), QString::fromStdString(ch));
    setData(index(messageList.size() - 1, 1, QModelIndex()), QString::fromStdString(msg));
    setData(index(data1List.size() - 1, 2, QModelIndex()), QString::fromStdString(dt1));
    setData(index(data2List.size() - 1, 3, QModelIndex()), QString::fromStdString(dt2));
}
