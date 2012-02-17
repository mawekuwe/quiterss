#include "filterrulesdialog.h"
#include "newsfiltersdialog.h"

NewsFiltersDialog::NewsFiltersDialog(QWidget *parent) :
  QDialog(parent)
{
  setWindowFlags (windowFlags() & ~Qt::WindowContextHelpButtonHint);
  setWindowTitle(tr("News filters"));
  setMinimumWidth(400);
  setMinimumHeight(250);

  filtersTree = new QTreeWidget();
  filtersTree->setObjectName("filtersTree");
  filtersTree->setColumnCount(2);
  filtersTree->setColumnHidden(0, true);
  filtersTree->header()->setResizeMode(QHeaderView::Stretch);
  filtersTree->header()->setMovable(false);

  QStringList treeItem;
  treeItem << tr("Id") << tr("Name");
  filtersTree->setHeaderLabels(treeItem);

  treeItem.clear();
  treeItem << QString::number(filtersTree->topLevelItemCount())
           << tr("Test filter %1").arg(filtersTree->topLevelItemCount());
  QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(treeItem);
  treeWidgetItem->setCheckState(1, Qt::Checked);
  filtersTree->addTopLevelItem(treeWidgetItem);

  QPushButton *newButton = new QPushButton(tr("New..."), this);
  connect(newButton, SIGNAL(clicked()), this, SLOT(newFilter()));
  editButton = new QPushButton(tr("Edite..."), this);
  editButton->setEnabled(false);
  connect(editButton, SIGNAL(clicked()), this, SLOT(editeFilter()));
  deleteButton = new QPushButton(tr("Delete..."), this);
  deleteButton->setEnabled(false);
  connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteFilter()));

  moveUpButton = new QPushButton(tr("Move up"), this);
  moveUpButton->setEnabled(false);
  connect(moveUpButton, SIGNAL(clicked()), this, SLOT(moveUpFilter()));
  moveDownButton = new QPushButton(tr("Move down"), this);
  moveDownButton->setEnabled(false);
  connect(moveDownButton, SIGNAL(clicked()), this, SLOT(moveDownFilter()));

  QVBoxLayout *buttonsLayout = new QVBoxLayout();
  buttonsLayout->setAlignment(Qt::AlignCenter);
  buttonsLayout->addWidget(newButton);
  buttonsLayout->addWidget(editButton);
  buttonsLayout->addWidget(deleteButton);
  buttonsLayout->addSpacing(20);
  buttonsLayout->addWidget(moveUpButton);
  buttonsLayout->addWidget(moveDownButton);
  buttonsLayout->addStretch();

  QHBoxLayout *layoutH1 = new QHBoxLayout();
  layoutH1->setMargin(0);
  layoutH1->addWidget(filtersTree);
  layoutH1->addLayout(buttonsLayout);

  QPushButton *closeButton = new QPushButton(tr("&Close"), this);
  closeButton->setDefault(true);
  closeButton->setFocus(Qt::OtherFocusReason);
  connect(closeButton, SIGNAL(clicked()), SLOT(close()));
  QHBoxLayout *closeLayout = new QHBoxLayout();
  closeLayout->setAlignment(Qt::AlignRight);
  closeLayout->addWidget(closeButton);

  QVBoxLayout *mainlayout = new QVBoxLayout(this);
  mainlayout->setAlignment(Qt::AlignCenter);
  mainlayout->setMargin(5);
  mainlayout->addLayout(layoutH1);
  mainlayout->addLayout(closeLayout);

  connect(filtersTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
          this, SLOT(slotCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void NewsFiltersDialog::newFilter()
{
  FilterRulesDialog *filterRulesDialog = new FilterRulesDialog(this);
//  filterRulesDialog->restoreGeometry(settings_->value("newsFiltersDlg/geometry").toByteArray());

  int result = filterRulesDialog->exec();
//  settings_->setValue("newsFiltersDlg/geometry", filterRulesDialog->saveGeometry());

  if (result == QDialog::Rejected) {
    delete filterRulesDialog;
    return;
  }

  QStringList treeItem;
  treeItem << QString::number(filtersTree->topLevelItemCount())
           << tr("Test filter %1").arg(filtersTree->topLevelItemCount());
  QTreeWidgetItem *treeWidgetItem = new QTreeWidgetItem(treeItem);
  treeWidgetItem->setCheckState(1, Qt::Checked);
  filtersTree->addTopLevelItem(treeWidgetItem);

  if (((filtersTree->currentIndex().row() != (filtersTree->topLevelItemCount()-1))) &&
      filtersTree->currentIndex().isValid())
    moveDownButton->setEnabled(true);

  delete filterRulesDialog;
}

void NewsFiltersDialog::editeFilter()
{

}

void NewsFiltersDialog::deleteFilter()
{
  QMessageBox msgBox;
  msgBox.setIcon(QMessageBox::Question);
  msgBox.setWindowTitle(tr("Delete filter"));
  msgBox.setText(QString(tr("Are you sure to delete the filter '%1'?")).
                 arg(filtersTree->currentItem()->text(1)));
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);

  if (msgBox.exec() == QMessageBox::No) return;

  filtersTree->takeTopLevelItem(filtersTree->currentIndex().row());

  if (filtersTree->currentIndex().row() == 0)
    moveUpButton->setEnabled(false);
  if (filtersTree->currentIndex().row() == (filtersTree->topLevelItemCount()-1))
    moveDownButton->setEnabled(false);

}

void NewsFiltersDialog::moveUpFilter()
{
  QTreeWidgetItem *treeWidgetItem = filtersTree->takeTopLevelItem(
        filtersTree->currentIndex().row()-1);
  filtersTree->insertTopLevelItem(filtersTree->currentIndex().row()+1,
                                  treeWidgetItem);
  if (filtersTree->currentIndex().row() == 0)
    moveUpButton->setEnabled(false);
  if (filtersTree->currentIndex().row() != (filtersTree->topLevelItemCount()-1))
    moveDownButton->setEnabled(true);
}

void NewsFiltersDialog::moveDownFilter()
{
  QTreeWidgetItem *treeWidgetItem = filtersTree->takeTopLevelItem(
        filtersTree->currentIndex().row()+1);
  filtersTree->insertTopLevelItem(filtersTree->currentIndex().row(),
                                  treeWidgetItem);
  if (filtersTree->currentIndex().row() == (filtersTree->topLevelItemCount()-1))
    moveDownButton->setEnabled(false);
  if (filtersTree->currentIndex().row() != 0)
    moveUpButton->setEnabled(true);
}

void NewsFiltersDialog::slotCurrentItemChanged(QTreeWidgetItem *current,
                                               QTreeWidgetItem *previous)
{
  if (filtersTree->indexOfTopLevelItem(current) == 0)
    moveUpButton->setEnabled(false);
  else moveUpButton->setEnabled(true);

  if (filtersTree->indexOfTopLevelItem(current) == (filtersTree->topLevelItemCount()-1))
    moveDownButton->setEnabled(false);
  else moveDownButton->setEnabled(true);

  if (filtersTree->indexOfTopLevelItem(current) < 0) {
    editButton->setEnabled(false);
    deleteButton->setEnabled(false);
    moveUpButton->setEnabled(false);
    moveDownButton->setEnabled(false);
  } else {
    editButton->setEnabled(true);
    deleteButton->setEnabled(true);
  }
}
