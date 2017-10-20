#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMetaProperty>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currentItem(nullptr)
{
    ui->setupUi(this);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // create new project node
    Project *p = new Project();
    p->setDescription("Main Project");

//  Project *p1 = new Project();
//  p1->setDescription("Second Project");
//  p1->setType("Project2");

    // add node to tree view
    m_model.appendRow(p);
//  m_model.appendRow(p1);

//  qDebug() << "m_model columnCount = " << m_model.columnCount();
//  qDebug() << "m_model rowCount = " << m_model.rowCount();
    ui->treeView->setModel(&m_model);

    ui->treeView->expandAll();

//    variantManager = new QtVariantPropertyManager();
//    variantFactory = new QtVariantEditorFactory(this);
    variantManager = new VariantManager();
    variantFactory = new VariantFactory(this);

    connect(variantManager, &QtVariantPropertyManager::valueChanged, this, &MainWindow::handleValueChanged);

    propertyEditor = new QtTreePropertyBrowser(ui->scrollArea);
    propertyEditor->setHeaderVisible(true);

//  propertyEditor->setResizeMode();
    propertyEditor->setResizeMode(QtTreePropertyBrowser::ResizeMode(0));
//  qDebug () << "resize mode" << propertyEditor->resizeMode();
    propertyEditor->setFactoryForManager(variantManager, variantFactory);
    ui->scrollArea->setWidget(propertyEditor);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    m_model.set_index(index);

    MainWindow::draw_property_browser();
#if 0
//    QStandardItem *selected = m_model.itemFromIndex(index);
    QStandardItem *selected = m_model.itemFromIndex(m_model.get_index());


    QtVariantProperty *property;
    QtBrowserItem *item;

    Node *n = (Node *)selected;
    m_currentItem = n;

    const QMetaObject *meta = n->metaObject() ;
    const QObject *x = n;
    int cnt = meta->propertyCount();
    qDebug() << "number of properties : " << cnt;
    propertyEditor->clear();

    // Property Editor aufbauen
    for ( int i = 1; i < cnt; i++ ) {
        QMetaProperty prop = meta->property(i);

//        qDebug() << "property " << prop.name() << meta->property(i).read(x);
#if 1
        if ( prop.isWritable() ) {

        // only sample. We could separate Set and Enum...
           QVariant v = prop.read(n);
           switch ( v.type() ) {
           case QVariant::String :
                 property = variantManager->addProperty(QVariant::String, prop.name());
                 property->setValue(v.toString());
                 item = propertyEditor->addProperty(property);
                 propertyEditor->setExpanded(item, false);
                 break ;
           case QVariant::UInt :
               property = variantManager->addProperty(QVariant::Int, prop.name());
               property->setAttribute(QLatin1String("minimum"), 0);
               property->setValue(v.toInt());
               item = propertyEditor->addProperty(property);
               propertyEditor->setExpanded(item, false);
                 break ;
           case QVariant::Int :
               property = variantManager->addProperty(QVariant::Int, prop.name());
               property->setAttribute(QLatin1String("minimum"), 0);
               property->setValue(v.toInt());
               item = propertyEditor->addProperty(property);
               propertyEditor->setExpanded(item, false);
                 break ;

           case QVariant::Bool :
               property = variantManager->addProperty(QVariant::Bool, prop.name());
               property->setValue(v.toBool());
               item = propertyEditor->addProperty(property);
               propertyEditor->setExpanded(item, false);
                 break ;

           case QVariant::UserType :
               if (strcmp(v.typeName(),"FileString") == 0)
               {
                   property = variantManager->addProperty(VariantManager::filePathTypeId(), prop.name());
                   property->setValue(v);
                   item = propertyEditor->addProperty(property);
                   propertyEditor->setExpanded(item, false);
                }
               if (strcmp(v.typeName(),"HexString") == 0)
               {
                   property = variantManager->addProperty(QVariant::String, prop.name());
                   property->setValue(v.value<HexString>().hexstring);
                   property->setAttribute("regExp", QRegExp("0x[0-9A-Fa-f]{1,8}"));
                   property->setToolTip("Enter Address als 0x1324");
                   item = propertyEditor->addProperty(property);
                   propertyEditor->setExpanded(item, false);
                }
               if (strcmp(v.typeName(),"FlashSize") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<FlashSize>().memorysize);
                   property->setValue(v.value<FlashSize>().selectedsize);
                   item = propertyEditor->addProperty(property);
                   propertyEditor->setExpanded(item, false);
                }
                break;
           default :
               break;
           }
        }
#endif
    }
#endif
}


void MainWindow::on_actionSave_as_triggered()
{

    qDebug() << "Save as";
    Project_FileName = QFileDialog::getSaveFileName(this, tr("Save File"),".",tr("Pack Gen Files (*.pkg)"));
    if (Project_FileName != "")
    {
        QStandardItem *project = m_model.item(0,0);
        m_model.save(Project_FileName,project);
    }
}

void MainWindow::on_actionLoad_triggered()
{
    qDebug() << "Load";
    Project_FileName = QFileDialog::getOpenFileName(this, tr("Open File"),".",tr("Pack Gen Files (*.pkg)"));
    if (Project_FileName != "")
    {
        QStandardItem *project;
        m_model.load(Project_FileName);
        ui->treeView->expandAll();
    }
}

void MainWindow::on_actionNew_triggered()
{
    auto *old = m_model.takeItem(0,0);
    delete old;
    m_model.clear();
    Project *p = new Project();
    p->setDescription("Main Project");
    m_model.appendRow(p);
}

void MainWindow::on_actionSave_triggered()
{
    if (Project_FileName == "")
        Project_FileName = QFileDialog::getSaveFileName(this, tr("Save File"),".",tr("Pack Gen Files (*.pkg)"));
    if (Project_FileName != "")
    {
        QStandardItem *project = m_model.item(0,0);
        m_model.save(Project_FileName,project);
    }
}

// Menue on right click
void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    qWarning() << "on_treeView_customContextMenuRequested";

    QTreeView *tree = ui->treeView;
    QModelIndex index = tree->indexAt(pos);
    QStandardItem *selected = m_model.itemFromIndex(index);

    if (!selected)
        return;

    Node *n = (Node *)selected;
    QMenu *menu=new QMenu(this);
    n->node_menue(menu);
    menu->popup(tree->viewport()->mapToGlobal(pos));
}

void MainWindow::treeMenu()
{
    qDebug()<<"treeMenu";
}

void MainWindow::handleValueChanged(QtProperty *property, const QVariant &val)
{
    qDebug()<<"valueChanged" << property << val;
    if (m_currentItem)
    {
        QVariant v =  m_currentItem->property(property->propertyName().toStdString().c_str());
        if (v.type() == QVariant::UserType)
        {
            if (strcmp(v.typeName(),"FileString") == 0)
            {
                FileString s;
                s.filestring = val.toString();
                QVariant a;
                a.setValue<FileString>(s);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), a);
            }
            if (strcmp(v.typeName(),"HexString") == 0)
            {
                HexString s;
                s.hexstring = val.toString();
                QVariant a;
                a.setValue<HexString>(s);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), a);
            }
            if (strcmp(v.typeName(),"FlashSize") == 0)
            {
                FlashSize f;
                f.selectedsize = val.toInt();
                QVariant a;
                a.setValue<FlashSize>(f);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), a);
            }
            if (strcmp(v.typeName(),"DualBoot") == 0)
            {
                DualBoot d;
                d.dualbootena = val.toInt();
                QVariant a;
                a.setValue<DualBoot>(d);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(),a);
            }
        }
        else
            if (v.type() == QVariant::Bool)
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), val.toBool());
            else
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), val);
    }

    if (m_currentItem->get_need_redraw() == 1)
            MainWindow::draw_property_browser();
}

void MainWindow::draw_property_browser()
{
    QStandardItem *selected = m_model.itemFromIndex(m_model.get_index());


    QtVariantProperty *property;
    QtBrowserItem *item;

    Node *n = (Node *)selected;
    m_currentItem = n;

    const QMetaObject *meta = n->metaObject() ;
    const QObject *x = n;
    int cnt = meta->propertyCount();
    qDebug() << "number of properties : " << cnt;
    propertyEditor->clear();

    // Property Editor aufbauen
    for ( int i = 1; i < cnt; i++ ) {
        QMetaProperty prop = meta->property(i);

//        qDebug() << "property " << prop.name() << meta->property(i).read(x);
#if 1
        if ( prop.isWritable() ) {

        // only sample. We could separate Set and Enum...
           QVariant v = prop.read(n);
           switch ( v.type() ) {
           case QVariant::String :
                 property = variantManager->addProperty(QVariant::String, prop.name());
                 property->setValue(v.toString());
                 item = propertyEditor->addProperty(property);
                 propertyEditor->setExpanded(item, false);
                 break ;
           case QVariant::UInt :
               property = variantManager->addProperty(QVariant::Int, prop.name());
               property->setAttribute(QLatin1String("minimum"), 0);
               property->setValue(v.toInt());
               item = propertyEditor->addProperty(property);
               propertyEditor->setExpanded(item, false);
                 break ;
           case QVariant::Int :
               property = variantManager->addProperty(QVariant::Int, prop.name());
               property->setAttribute(QLatin1String("minimum"), 0);
               property->setValue(v.toInt());
               item = propertyEditor->addProperty(property);
               propertyEditor->setExpanded(item, false);
                 break ;

           case QVariant::Bool :
               property = variantManager->addProperty(QVariant::Bool, prop.name());
               property->setValue(v.toBool());
               item = propertyEditor->addProperty(property);
               propertyEditor->setExpanded(item, false);
                 break ;

           case QVariant::UserType :
               if (strcmp(v.typeName(),"FileString") == 0)
               {
                   property = variantManager->addProperty(VariantManager::filePathTypeId(), prop.name());
                   property->setValue(v);
                   item = propertyEditor->addProperty(property);
                   propertyEditor->setExpanded(item, false);
                }
               if (strcmp(v.typeName(),"HexString") == 0)
               {
                   property = variantManager->addProperty(QVariant::String, prop.name());
                   property->setValue(v.value<HexString>().hexstring);
                   property->setAttribute("regExp", QRegExp("0x[0-9A-Fa-f]{1,8}"));
                   property->setToolTip("Enter Address als 0x1324");
                   item = propertyEditor->addProperty(property);
                   propertyEditor->setExpanded(item, false);
                }
               if (strcmp(v.typeName(),"FlashSize") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<FlashSize>().memorysize);
                   property->setValue(v.value<FlashSize>().selectedsize);
                   item = propertyEditor->addProperty(property);
                   propertyEditor->setExpanded(item, false);
                }
               if (strcmp(v.typeName(),"DualBoot") == 0)
               {
                   qDebug() << "dualboot";
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<DualBoot>().dualboot);
                   property->setValue(v.value<DualBoot>().dualbootena);
                   item = propertyEditor->addProperty(property);
                   propertyEditor->setExpanded(item, false);
                }
                break;
           default :
               break;
           }
        }
#endif
    }
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);

    QSize treeViewSize;
    QSize propertyViewSize;

    treeViewSize.setHeight(QMainWindow::height()-55);
    treeViewSize.setWidth(QMainWindow::width()/2-8);

    propertyViewSize.setHeight(QMainWindow::height()-55);
    propertyViewSize.setWidth(QMainWindow::width()/2-8);

//  qDebug() << "width = " << QMainWindow::width() << " length = " << QMainWindow::height() << endl;

    ui->treeView->move(3,1);
    ui->treeView->resize(treeViewSize);
    ui->scrollArea->move(treeViewSize.width()+15,1);
    ui->scrollArea->resize(propertyViewSize);
}