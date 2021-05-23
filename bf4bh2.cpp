#include "bf4bh2.h"
#include "ui_bf4bh2.h"
#include <QTextCodec>
#include <QUrl>
#include <QDebug>
#include <QProgressBar>
#include <QJsonDocument>

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
}

BF4BH2::~BF4BH2()
{
  delete ui;
}

int BF4BH2::initMenu()
{
  //  imodel->clear();
  QStringList column;
  column << tr( "Имя")
         << tr( "Карта")
         << tr( "Режим")
         << tr( "*/*");
  imodel->setColumnCount( column.size());// Указываем число колонок, ток нафиг?
  imodel->setHorizontalHeaderLabels( column);// задаем верхнюю легенду
  ui->uitvMain->setModel( imodel);
  ui->uitvMain->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch);
  ui->uitvMain->sortByColumn( 3, Qt::DescendingOrder);
  ui->uitvMain->resizeColumnsToContents();
  ui->uitvMain->resizeRowsToContents();
  ui->uitvMain->setSortingEnabled( true);
  //  ui->uitvMain->setSelectionMode( QAbstractItemView::SingleSelection);
  ui->uitvMain->setSelectionBehavior( QAbstractItemView::SelectRows);
  return 1;
}

//=======================================
int BF4BH2::getData( QString URL)
{
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
    statusBar()->showMessage( reply->errorString());
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
      QJsonDocument jsonDocument( QJsonDocument::fromJson( content.toUtf8()));
      QJsonObject jsObj = jsonDocument.object();
      player = jsObj[ "players"].toArray().size();
      if( player < 10) players = "0" + QString::number( player);
      else players = QString::number(player);
      QJsonObject jsServer = jsObj[ "server"].toObject();
      QJsonObject jsSlots = jsServer[ "slots"].toObject().value( "2").toObject();
      imodel->insertRow(i);
      imodel->setItem( i, 0, new  QStandardItem( jsServer[ "name"].toString().simplified()));
      imodel->setItem( i, 1, new  QStandardItem( dictionary(jsServer[ "map"].toString())));
      imodel->setItem( i, 2, new  QStandardItem( dictionary2( dictionary2( QString::number( jsServer[ "preset"].toInt())))));
      imodel->setItem( i, 3, new  QStandardItem( players + "/" + QString::number( jsSlots[ "max"].toInt())));
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
  switch ( error) {
    case 1: return true;
      break;
    case 0:
    default: return false;
  }
}

void BF4BH2::setAction(QString name, QString value, int button)
{
  actionList[name] = value;
  QAction *action   = new QAction(name, this);
  action->setObjectName(name);
  connect(action, &QAction::triggered, this, &BF4BH2::actTriggered);
  ui->uimPreset->addAction(action);
  if(button == 0)
  {
    connect(ui->uib0, &QPushButton::clicked, action, &QAction::triggered);
    ui->uib0->setText(name);
  }
  else if(button == 1)
  {
    connect(ui->uib1, &QPushButton::clicked, action, &QAction::triggered);
    ui->uib1->setText(name);
  }
}

void BF4BH2::actTriggered()
{
  QAction* pAction = qobject_cast<QAction*>(sender());
  Q_ASSERT(pAction);
  emit replyAbort();
  err = getData(actionList[pAction->text()]);
  ErrTyper(err);
}
//======================================
QString BF4BH2::dictionary( QString tech_word)
{
  QStringList maps
  {
    "XP7_Valley",   //0 Долина Драконов 2015
    "MP_Journey",   //1 Дорога Голмуд
    "XP5_Night_01",	//2 Завод: Ночная Смена
    "MP_Abandoned",	//3 Завод 311
    "MP_Flooded",   //4 Зона Затопления
    "MP_Tremors",   //5 Кровавая Заря
    "MP_Resort",    //6 Курорт Хайнань
    "XP6_CMP",      //7 Операция "Бунт"
    "MP_Prison",    //8 Операция "Взаперти"
    "MP_Siege",     //9 Осада Шанхая
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
    case ( 0):	tech_word = "Долина Драконов 2015";     break;
    case ( 1):	tech_word = "Дорога Голмуд";            break;
    case ( 2):	tech_word = "Завод: Ночная Смена";      break;
    case ( 3):	tech_word = "Завод 311";                break;
    case ( 4):	tech_word = "Зона Затопления";          break;
    case ( 5):	tech_word = "Кровавая Заря";            break;
    case ( 6):	tech_word = "Курорт Хайнань";           break;
    case ( 7):	tech_word = "Операция \"Бунт\"";        break;
    case ( 8):	tech_word = "Операция \"Взаперти\"";    break;
    case ( 9):	tech_word = "Осада Шанхая";             break;
    case (10):	tech_word = "Плотина На Меконге";       break;
    case (11):	tech_word = "Чужой Сигнал";             break;
    case (12):	tech_word = "Шторм На Параселах";       break;
    case (13):	tech_word = "Алтайский Хребет";         break;
    case (14):	tech_word = "Гуйлиньский пики";         break;
    case (15):	tech_word = "Перевал Дракона";          break;
    case (16):	tech_word = "Шелковый путь";            break;
    case (17):	tech_word = "Граница Каспии";           break;
    case (18):	tech_word = "Оманский залив";           break;
    case (19):	tech_word = "Операция \"Огненый шторм\"";	break;
    case (20):	tech_word = "Операция \"Метро\"";       break;
    case (21):	tech_word = "Волнорез";                 break;
    case (22):	tech_word = "Затерянные острова";       break;
    case (23):	tech_word = "Операция \"Мортира\"";     break;
    case (24):	tech_word = "Удар по Спратли";          break;
    case (25):	tech_word = "Жемчужный рынок";          break;
    case (26):	tech_word = "Затонувций \"дракон\"";    break;
    case (27):	tech_word = "Пропоганда";               break;
    case (28):	tech_word = "Сад Лумфини";              break;
    case (29):	tech_word = "Ангар 21";                 break;
    case (30):	tech_word = "Карельские гиганты";       break;
    case (31):	tech_word = "Молот";                    break;
    case (32):	tech_word = "Операция \"Белая мгла\"";	break;
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
    case (00): tech_word = "Норм";     break;
    case (01): tech_word = "Реал";     break;
    case (02): tech_word = "Инди";     break;

    case (-1): break;
  }
  return tech_word;
}

//======================================
void BF4BH2::on_uiaAbout_triggered()
{
  QMessageBox::about( this,
                      "О программе",
                      "Версия: " + tr(VER_FILEVERSION_STR) + "\n\n"
                                            "Собрано на коленке.\n\n"
                                            "GitHub: https://github.com/UndeadMorose/BF4BH2/\n\n"
                                            "Программа предоставляется \"КАК ЕСТЬ\" БЕЗ ГАРАНТИИ ЛЮБОГО ВИДА, ВКЛЮЧАЯ ГАРАНТИИ КОНСТРУКЦИИ, ТОВАРНОГО РАЗВИТИЯ И ПРИГОДНОСТИ ДЛЯ ОСОБЫХ ЦЕЛЕЙ.\n\n"
                                            "The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.");
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

