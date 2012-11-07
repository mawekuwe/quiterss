#ifndef FEEDSTREEMODEL_H
#define FEEDSTREEMODEL_H

#include <qyursqltreeview.h>

class FeedsTreeModel : public QyurSqlTreeModel
{
  Q_OBJECT
public:
  FeedsTreeModel(const QString &tableName,
      const QStringList &captions,
      const QStringList &fieldNames,
      int rootParentId = 0,
      const QString &decoratedField = QString(),
      QObject* parent = 0);
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  QFont font_;
  QString formatDateTime_;
};

#endif  // FEEDSTREEMODEL_H