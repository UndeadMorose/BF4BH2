#include "bf4bh2.h"
#include "ui_bf4bh2.h"
#include <QTextCodec>
#include <QUrl>
#include <QDebug>
#include <QProgressBar>
#include <QJsonDocument>
#include <QDesktopServices>

BF4BH2::BF4BH2( QWidget *parent)
  : QMainWindow( parent)
  , ui( new Ui::BF4BH2)
{
  ui->setupUi( this);
  imodel = new QStandardItemModel;
  manager = new QNetworkAccessManager( this);
  manager2 = new QNetworkAccessManager( this);
  connect( ui->uipb, &QProgressBar::valueChanged, this, &BF4BH2::pbHider);
  ui->uipb->hide();
  autoUpdater = new QTimer;

  ui->uileSearch->setHidden(1);
  ui->uibSearch->setHidden(1);
  ui->statusbar->setStyleSheet("color: red");
  proxyModel = new QSortFilterProxyModel();
  proxyModel->setSourceModel(imodel);

}

BF4BH2::~BF4BH2()
{
  delete ui;
}

int BF4BH2::initMenu()
{
  QStringList column;
  column  << tr( "Имя")
          << tr( "Карта")
          << tr( "Режим")
          << tr( "*/*")
          << tr("Ссылка");
  //  imodel->setColumnCount( column.size());// Указываем число колонок, ток нафиг?
  imodel->setHorizontalHeaderLabels( column);// задаем верхнюю легенду
//  ui->uitvMain->setModel( imodel);
  ui->uitvMain->setModel(proxyModel);
  ui->uitvMain->horizontalHeader()->setSectionResizeMode( C_NAME, QHeaderView::Stretch);
//  ui->uitvMain->horizontalHeader()->setStretchLastSection(1);
  ui->uitvMain->setSelectionBehavior( QAbstractItemView::SelectRows);
  ui->uitvMain->setColumnHidden(C_LINK, true);
  ui->uitvMain->sortByColumn( C_BOT, Qt::DescendingOrder);
  ui->uitvMain->resizeColumnsToContents();
  ui->uitvMain->resizeRowsToContents();
  ui->uitvMain->setSortingEnabled( true);
  autoUpdater->stop();
  autoUpdater->start(60000);
  ui->uileFilter->setText("");
  return 1;
}

//=======================================
int BF4BH2::getData( QString URL)
{
  ui->uitvMain->setModel(nullptr); //привязка пустой модели на время загрзки данных
  QUrl url( URL);
  QNetworkRequest request( url);
  QNetworkReply* reply = manager->get( request);
  QEventLoop event;
  connect( reply, &QNetworkReply::finished, &event, &QEventLoop::quit);
  connect( this, &BF4BH2::replyAbort, reply, &QNetworkReply::abort);
  event.exec();
  int i=0;
  servers.clear();
  imodel->clear();
  if ( reply->error() == QNetworkReply::NoError)
  {
    QString content = reply->readAll();
    int t1 = content.indexOf( "true,\"servers\":[") + 5;
    content = "{" + content.mid( t1, content.indexOf( ",\"session\"", t1) - t1) + "}";
    QJsonDocument jsonDocument( QJsonDocument::fromJson( content.toUtf8()));
    foreach ( const QJsonValue & value, jsonDocument.object()[ "servers"].toArray()) {
      QJsonObject obj = value.toObject();
      servers << URL_server + obj[ "guid"].toString();
      i = 1;
    }
    ui->uipb->setRange( 0, servers.size());
  }
  else
  {
    statusBar()->showMessage( reply->errorString(), 2000);
    i = 0;
  }
  // разрешаем объекту-ответа "удалится"
  reply->deleteLater();
  if(i)    getPlayers();
  return i;
}

int BF4BH2::getPlayers()
{
  ui->uipb->show();
  ui->uitvMain->setSortingEnabled( false);
  for ( int i=0; i<servers.size(); i++)
  {
    QNetworkReply* reply2 = manager2->get( QNetworkRequest( QUrl( servers[ i])));
    QEventLoop event;
    connect( reply2, &QNetworkReply::finished, &event, &QEventLoop::quit);
    connect( this, &BF4BH2::replyAbort, reply2, &QNetworkReply::abort);
    event.exec();
    if ( reply2->error() == QNetworkReply::NoError)
    {
      QString content = reply2->readAll();
      int t1 = content.indexOf( "serverbrowserwarsaw.show.surface", content.indexOf( "Surface.globalContext")) + 45;
      content = content.mid( t1, content.indexOf( "block_serverbrowserwarsaw_warsawshow", t1) - t1 - 2);
      //==== old
      QJsonDocument jsonDocument( QJsonDocument::fromJson( content.toUtf8()));
      QJsonObject jsObj = jsonDocument.object();
      player = content.count("onlineGame");
      //      player = jsObj[ "players"].toArray().size();
      if( player < 10) players = "0" + QString::number( player);
      else players = QString::number(player);
      QJsonObject jsServer = jsObj[ "server"].toObject();
      QJsonObject jsSlots = jsServer[ "slots"].toObject().value( "2").toObject();

      imodel->insertRow(i);
      imodel->setItem( i, C_NAME, new  QStandardItem( jsServer[ "name"].toString().simplified()));
      imodel->setItem( i, C_MAP, new  QStandardItem( dictionary(jsServer[ "map"].toString())));
      imodel->setItem( i, C_PRESET, new  QStandardItem( dictionary2( QString::number( jsServer[ "preset"].toInt()))));
      imodel->setItem( i, C_BOT, new  QStandardItem( players + "/" + QString::number(jsSlots[ "max"].toInt())));
      imodel->setItem( i, C_LINK, new  QStandardItem(servers[i]));

      ui->uipb->setValue( i+1);
    }
    else
      return 0;
    reply2->deleteLater();
  }
  initMenu();
  return 1;
}

bool BF4BH2::ErrTyper( int error)
{
  switch ( error){
    case 1: return true;
      break;
    case 0:
    default: return false;
  }
}

void BF4BH2::setAction(QString nameAct, QString value, int button)
{
  actionList[nameAct] = value;
  QAction *action   = new QAction(nameAct, this);
  action->setObjectName(nameAct);
  connect(action, &QAction::triggered, this, &BF4BH2::actTriggered);
  ui->uimPreset->addAction(action);
  if(button == 0)
  {
    connect(ui->uib0, &QPushButton::clicked, action, &QAction::triggered);
    ui->uib0->setText(nameAct);
  }
  else if(button == 1)
  {
    connect(ui->uib1, &QPushButton::clicked, action, &QAction::triggered);
    ui->uib1->setText(nameAct);
  }
}

void BF4BH2::actTriggered()
{
  QAction* pAction = qobject_cast<QAction*>(sender());
  Q_ASSERT(pAction);
  emit replyAbort();
  server = actionList[pAction->text()];
  err = getData(server);
  ErrTyper(err);
}

void BF4BH2::timeOut()
{
  err = getData(server);
  ErrTyper(err);
}
//======================================
QString BF4BH2::dictionary( QString tech_word)
{
  QStringList maps
  {
    "XP7_Valley",   // 0 Долина Драконов 2015
    "MP_Journey",   // 1 Дорога Голмуд
    "XP5_Night_01",	// 2 Завод: Ночная Смена
    "MP_Abandoned",	// 3 Завод 311
    "MP_Flooded",   // 4 Зона Затопления
    "MP_Tremors",   // 5 Кровавая Заря
    "MP_Resort",    // 6 Курорт Хайнань
    "XP6_CMP",      // 7 Операция "Бунт"
    "MP_Prison",    // 8 Операция "Взаперти"
    "MP_Siege",     // 9 Осада Шанхая
    "MP_Damage",    //10 Плотина На Меконге
    "MP_TheDish",   //11 Чужой Сигнал
    "MP_Naval",     //12 Шторм На Параселах
    "XP1_002",      //13 Алтайский Хребет
    "XP1_003",      //14 Гуйлиньский пики
    "XP1_004",      //15 Перевал Дракона
    "XP1_001",      //16 Шелковый путь
    "XP0_Caspian",	//17 Граница Каспии
    "XP0_Oman",     //18 Оманский залив
    "XP0_Firestorm",//19 Операция "Огненый шторм"
    "XP0_Metro",    //20 Операция "Метро"
    "XP2_003",      //21 Волнорез
    "XP2_001",      //22 Затерянные острова
    "XP2_004",      //23 Операция "Мортира"
    "XP2_002",      //24 Удар по Спратли
    "XP3_MarketPl",	//25 Жемчужный рынок
    "XP3_WtrFront",	//26 Затонувций "дракон"
    "XP3_Prpganda",	//27 Пропоганда
    "XP3_UrbanGdn",	//28 Сад Лумфини
    "XP4_Titan",    //29 Ангар 21
    "XP4_WlkrFtry",	//30 Карельские гиганты
    "XP4_SubBase",	//31 Молот
    "XP4_Arctic"    //32 Операция "Белая мгла"
  };

  int dict = maps.indexOf(tech_word);
  switch(dict)
  {
    case ( 0):	tech_word = tr("Долина Драконов 2015");       break;
    case ( 1):	tech_word = tr("Дорога Голмуд");              break;
    case ( 2):	tech_word = tr("Завод: Ночная Смена");        break;
    case ( 3):	tech_word = tr("Завод 311");                  break;
    case ( 4):	tech_word = tr("Зона Затопления");            break;
    case ( 5):	tech_word = tr("Кровавая Заря");              break;
    case ( 6):	tech_word = tr("Курорт Хайнань");             break;
    case ( 7):	tech_word = tr("Операция \"Бунт\"");          break;
    case ( 8):	tech_word = tr("Операция \"Взаперти\"");      break;
    case ( 9):	tech_word = tr("Осада Шанхая");               break;
    case (10):	tech_word = tr("Плотина На Меконге");         break;
    case (11):	tech_word = tr("Чужой Сигнал");               break;
    case (12):	tech_word = tr("Шторм На Параселах");         break;
    case (13):	tech_word = tr("Алтайский Хребет");           break;
    case (14):	tech_word = tr("Гуйлиньский пики");           break;
    case (15):	tech_word = tr("Перевал Дракона");            break;
    case (16):	tech_word = tr("Шелковый путь");              break;
    case (17):	tech_word = tr("Граница Каспии");             break;
    case (18):	tech_word = tr("Оманский залив");             break;
    case (19):	tech_word = tr("Операция \"Огненый шторм\"");	break;
    case (20):	tech_word = tr("Операция \"Метро\"");         break;
    case (21):	tech_word = tr("Волнорез");                   break;
    case (22):	tech_word = tr("Затерянные острова");         break;
    case (23):	tech_word = tr("Операция \"Мортира\"");       break;
    case (24):	tech_word = tr("Удар по Спратли");            break;
    case (25):	tech_word = tr("Жемчужный рынок");            break;
    case (26):	tech_word = tr("Затонувций \"дракон\"");      break;
    case (27):	tech_word = tr("Пропоганда");                 break;
    case (28):	tech_word = tr("Сад Лумфини");                break;
    case (29):	tech_word = tr("Ангар 21");                   break;
    case (30):	tech_word = tr("Карельские гиганты");         break;
    case (31):	tech_word = tr("Молот");                      break;
    case (32):	tech_word = tr("Операция \"Белая мгла\"");    break;
    case (-1):  break;
  }
  return tech_word;
}

QString BF4BH2::dictionary2( QString tech_word)
{
  QStringList maps
  {
    "1",             //00 Нормально
    "2",             //01 Реально
    "8"              //02 Индивидуально
  };
  int dict = maps.indexOf(tech_word);
  switch(dict)
  {
    case (00): tech_word = tr("Норм");     break;
    case (01): tech_word = tr("Реал");     break;
    case (02): tech_word = tr("Инди");     break;

    case (-1): break;
  }
  return tech_word;
}

//======================================
void BF4BH2::on_uiaAbout_triggered()
{
  QString sAbout = "\n\nСобрано на коленке.\n\n"
           "GitHub: https://github.com/UndeadMorose/BF4BH2/\n\n"
           "Программа предоставляется \"КАК ЕСТЬ\" БЕЗ ГАРАНТИИ ЛЮБОГО ВИДА, "
           "ВКЛЮЧАЯ ГАРАНТИИ КОНСТРУКЦИИ, ТОВАРНОГО РАЗВИТИЯ И ПРИГОДНОСТИ ДЛЯ ОСОБЫХ ЦЕЛЕЙ.\n\n"
           "The program is provided AS IS with NO WARRANTY OF ANY KIND, "
           "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.";
  QMessageBox::about( this, "О программе","Версия: " + tr(VER_FILEVERSION_STR) + sAbout);
}

void BF4BH2::on_uiaQuit_triggered()
{
  emit replyAbort();
  close();
}

void BF4BH2::pbHider()
{
  emit replyAbort();
  if( ui->uipb->value() == ui->uipb->maximum())
    ui->uipb->hide();
}

void BF4BH2::on_uiaRow_triggered()
{
  ui->uitvMain->resizeRowsToContents();
}

void BF4BH2::on_uitvMain_doubleClicked(const QModelIndex &index)
{
  QDesktopServices::openUrl(QUrl(proxyModel->data(proxyModel->index(index.row(), C_LINK)).toString()));
}


void BF4BH2::on_uiaUpdate_triggered(bool checked)
{
  if(checked)
  {
    connect(autoUpdater, &QTimer::timeout, this, &BF4BH2::timeOut);
    autoUpdater->start(60000);
  }
  else
  {
    disconnect(autoUpdater, &QTimer::timeout, this, &BF4BH2::timeOut);
    autoUpdater->stop();
  }
}


void BF4BH2::on_action_URL_triggered(bool checked)
{
  ui->uileSearch->setHidden(!checked);
  ui->uibSearch->setHidden(!checked);
}


void BF4BH2::on_uibSearch_clicked()
{
  QString temp = ui->uileSearch->text();

  if(temp.indexOf("https://battlelog.battlefield.com/bf4/") == 0)
  {
    server = temp;
    getData(server);
  }
  else
  {
    statusBar()->showMessage("Incorrect URL", 6000);

  }
}


void BF4BH2::on_uileFilter_textChanged(const QString &arg1)
{

  QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
  QRegExp regExp(arg1,Qt::CaseInsensitive,syntax);
  proxyModel->setFilterKeyColumn(0); //Устанавливаем колонку фильтрации
  proxyModel->setFilterRegExp(regExp);
}

