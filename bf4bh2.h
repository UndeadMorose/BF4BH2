#ifndef BF4BH2_H
#define BF4BH2_H

#include <QMainWindow>
#include <defines.h>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QStandardItemModel>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class BF4BH2; }
QT_END_NAMESPACE

class BF4BH2 : public QMainWindow
{
    Q_OBJECT

  public:
    BF4BH2(QWidget *parent = nullptr);
    ~BF4BH2();
    void setAction(QString, QString, int = -1);

  private slots:
    int getPlayers();
    void pbHider();

    void actTriggered();
    void on_uiaAbout_triggered();
    void on_uiaQuit_triggered();
    void on_uiaRow_triggered();

  signals:
    void replyfin();
    void replyAbort();

  private:
    Ui::BF4BH2 *ui;
    int err = 0;
    int player;
    QMap<QString, QString> actionList;
    QString players;
    int getData(QString);
    bool ErrTyper(int);
    int initMenu();

    QStandardItemModel *imodel;
    QNetworkAccessManager* manager;
    QNetworkAccessManager* manager2;

    QJsonArray presetArr;
    QJsonObject presetObj;
    QString URL_server = "https://battlelog.battlefield.com/bf4/ru/servers/show/pc/";
    QStringList servers;
    QString dictionary(QString);
    QString dictionary2(QString);
};
#endif // BF4BH2_H
