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

QT_BEGIN_NAMESPACE
namespace Ui { class BF4BH2; }
QT_END_NAMESPACE

class BF4BH2 : public QMainWindow
{
    Q_OBJECT

  public:
    BF4BH2(QWidget *parent = nullptr);
    ~BF4BH2();

  private slots:
    void replyFinished();
    void reply2Finished();

    void on_uibReal_clicked();
    void on_uibNormal_clicked();

    void on_uiaReal_triggered();
    void on_uiaNormal_triggered();
    void on_uiaAbout_triggered();
    void on_uiaAll_triggered();
    void on_uiaRealNE_triggered();
    void on_uiaNormNE_triggered();
    void on_uiaAllNE_triggered();
    void on_uiaAMNormNE_triggered();
    void on_uiaAMRealNE_triggered();
    void on_uiaRealNight_triggered();

  signals:
    void reply2fin();
    void replyfin();

  private:
    Ui::BF4BH2 *ui;
    int err = 0;
    int players;
    int getData(QString);
    bool ErrTyper(int);
    int initMenu();
    int getPlayers();
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