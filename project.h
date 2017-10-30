#ifndef PROJECT_H
#define PROJECT_H

#include "node.h"
#include "M86_Spartan6.h"
#include "JADE_Package.h"
#include "Masterfile.h"
#include "model.h"


class Project : public Node
{
    Q_OBJECT
    // property to identify the node type
//  Q_PROPERTY(QString node_type READ node_type)

    // Properties for this node
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(FileString logichdr READ logichdr WRITE setLogichdr)
    Q_PROPERTY(FileString mbind READ mbind WRITE setMbind)

public:
    explicit Project(QObject *parent = nullptr);
    ~Project();
#if 0
    explicit Project();
    explicit Project(QObject *parent = nullptr, QString projectName = "Project");
#endif

    QString node_type() {return"Project";}

    QString name();
    void setName(QString name);

    FileString logichdr();
    void setLogichdr(FileString filename);

    FileString mbind();
    void setMbind(FileString filename);

//  bool readJson(const QJsonObject *jsonObj);
//  bool writeJson(QJsonObject *jsonObj);

    void node_menue(QMenu *menu);

private slots:
    void new_M86_Spartan6();
    void new_JADE_Package();
    void new_Masterfile();

private:
    QString projectName;
    QString m_name;
    FileString m_logichdr =(QString) "//pc011/tools/utils/logichdr.exe";
    FileString m_mbind =(QString) "//pc011/tools/utils/mbind.exe";

    QAction *pNewSpartan;
    QAction *pNewMasterFile;
    QAction *pNewJadeFile;

};

#endif // PROJECT_H
