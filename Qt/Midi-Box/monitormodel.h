#ifndef MONITORMODEL_H
#define MONITORMODEL_H

#include <QAbstractTableModel>
#include "MidiMessage.h"



class MonitorModel: public QAbstractTableModel
{
    Q_OBJECT

private:
    QStringList channelList;
    QStringList messageList;
    QStringList data1List;
    QStringList data2List;

public:

    enum MyRoles{
        ChannelRole = Qt::UserRole + 1,
        MessageRole,
        Data1Role,
        Data2Role
    };

    MonitorModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    QHash<int, QByteArray>  roleNames() const;

    bool clearModel();
    void addMessageToMonitor(string ch, string msg, string dt1, string dt2);
};

#endif // MONITORMODEL_H
