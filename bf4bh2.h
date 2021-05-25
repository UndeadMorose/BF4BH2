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
#include <QTimer>

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
    void timeOut();
    void on_uitvMain_doubleClicked(const QModelIndex &index);

    void on_uiaUpdate_triggered(bool checked);

    void on_action_URL_triggered(bool checked);

    void on_uibSearch_clicked();

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
    QTimer *autoUpdater;
    QJsonArray presetArr;
    QJsonObject presetObj;
    QString URL_server = "https://battlelog.battlefield.com/bf4/ru/servers/show/pc/";
    QString server;
    QStringList servers;
    QString dictionary(QString);
    QString dictionary2(QString);
};
#endif // BF4BH2_H
