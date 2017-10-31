#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "fpga.h"
#include "model.h"
#include "project.h"
#include "M86_Spartan6.h"

#include <QMainWindow>

#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "filepathmanager.h"
#include "fileeditfactory.h"
#include "fileedit.h"





namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Model m_model;

    Node *getMCurrent () {return m_currentItem;}

signals:
    void generateFpga();
private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_actionSave_as_triggered();

    void on_actionLoad_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionProperties_triggered();

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void resizeEvent(QResizeEvent * event);

private slots:
    void treeMenu();

    void handleValueChanged(QtProperty *property, const QVariant &val);

    void draw_property_browser();

    void changeProperty(const QString & name, const QVariant a);

    void on_actionGenerateFpga_triggered();

    QString setTipForProperty (const QMetaProperty & prop);
    QRegExp setRegExpForProperty (const QMetaProperty &prop);

private:
    Ui::MainWindow *ui;
    QString Project_FileName = "";
    QString m_winTitle = "Package Generator v0.4";

    QtVariantPropertyManager *variantManager;
    QtVariantEditorFactory *variantFactory;

    QtTreePropertyBrowser *propertyEditor;

    Node *m_currentItem;
    QMenu *pMainMenu;
};

QObject * getObjectWithName (const QObject * pobject,const QString &name);
#endif // MAINWINDOW_H
