#include "bf4bh2.h"
#include <QTextCodec>
#include <QApplication>
#include <QSettings>
#include <QJsonDocument>
#include <QFile>

QJsonDocument setJson()
{
  QString defaultJson = "{\"presets\": ["
                        "{\"button\": 1, \"name\": \"Нормальный мод\",\"url\": \"https://battlelog.battlefield.com/bf4/ru/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&gameexpansions=-1&slots=16&slots=1&slots=2&slots=4&gameSize=32&gameSize=48&gameSize=64&q=&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gamepresets=1&mapRotation=-1&modeRotation=-1&password=-1&regions=16&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500\"},"
                        "{\"button\": 0, \"name\": \"Реалистично\",\"url\": \"https://battlelog.battlefield.com/bf4/ru/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&gameexpansions=-1&slots=16&slots=1&slots=2&slots=4&gameSize=32&gameSize=48&gameSize=64&q=&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gamepresets=2&mapRotation=-1&modeRotation=-1&password=-1&regions=16&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500\"},"
                        "{\"name\": \"Всё в Европе\",\"url\": \"https://battlelog.battlefield.com/bf4/ru/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&gameexpansions=-1&slots=16&slots=1&slots=2&gameSize=32&gameSize=48&gameSize=64&q=&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&mapRotation=-1&modeRotation=-1&password=-1&regions=16&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500\"},"
                        "{\"name\": \"Реал; 0-10; 32-64\",\"url\": \"https://battlelog.battlefield.com/bf4/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&maps=MP_Tremors&maps=XP7_Valley&maps=MP_Flooded&maps=MP_Journey&maps=MP_Resort&maps=MP_Damage&maps=MP_Naval&maps=MP_TheDish&maps=MP_Siege&maps=MP_Abandoned&maps=XP5_Night_01&gamemodes=1&gamemodes=64&slots=16&slots=1&slots=2&gameSize=32&gameSize=48&gameSize=64&q=&maps=XP1_002&maps=XP1_004&maps=XP1_003&maps=XP1_001&maps=XP0_Caspian&maps=XP0_Oman&maps=XP0_Firestorm&maps=XP0_Metro&maps=XP2_001&maps=XP2_002&maps=XP2_004&maps=XP2_003&maps=XP3_UrbanGdn&maps=XP3_MarketPl&maps=XP3_Prpganda&maps=XP3_WtrFront&maps=XP4_WlkrFtry&maps=XP4_SubBase&maps=XP4_Titan&maps=XP4_Arctic&gamepresets=2&mapRotation=-1&modeRotation=-1&password=-1&regions=16&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500&includeExpansions=0&includeExpansions=1048576&includeExpansions=524288&includeExpansions=2097152&includeExpansions=4194304&includeExpansions=8388608\"},"
                        "{\"name\": \"Норм; 0-10; 32-64\",\"url\": \"https://battlelog.battlefield.com/bf4/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&maps=MP_Tremors&maps=XP7_Valley&maps=MP_Flooded&maps=MP_Journey&maps=MP_Resort&maps=MP_Damage&maps=MP_Naval&maps=MP_TheDish&maps=MP_Siege&maps=MP_Abandoned&maps=XP5_Night_01&gamemodes=1&gamemodes=64&slots=16&slots=1&slots=2&gameSize=32&gameSize=48&gameSize=64&q=&maps=XP1_002&maps=XP1_004&maps=XP1_003&maps=XP1_001&maps=XP0_Caspian&maps=XP0_Oman&maps=XP0_Firestorm&maps=XP0_Metro&maps=XP2_001&maps=XP2_002&maps=XP2_004&maps=XP2_003&maps=XP3_UrbanGdn&maps=XP3_MarketPl&maps=XP3_Prpganda&maps=XP3_WtrFront&maps=XP4_WlkrFtry&maps=XP4_SubBase&maps=XP4_Titan&maps=XP4_Arctic&gamepresets=1&mapRotation=-1&modeRotation=-1&password=-1&regions=16&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500&includeExpansions=0&includeExpansions=1048576&includeExpansions=524288&includeExpansions=2097152&includeExpansions=4194304&includeExpansions=8388608\"},"
                        "{\"name\": \"Всё; 0-10; 32-64\",\"url\": \"https://battlelog.battlefield.com/bf4/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&maps=MP_Tremors&maps=XP7_Valley&maps=MP_Flooded&maps=MP_Journey&maps=MP_Resort&maps=MP_Damage&maps=MP_Naval&maps=MP_TheDish&maps=MP_Siege&maps=MP_Abandoned&maps=XP5_Night_01&gamemodes=1&gamemodes=64&slots=16&slots=1&slots=2&gameSize=32&gameSize=48&gameSize=64&q=&maps=XP1_002&maps=XP1_004&maps=XP1_003&maps=XP1_001&maps=XP0_Caspian&maps=XP0_Oman&maps=XP0_Firestorm&maps=XP0_Metro&maps=XP2_001&maps=XP2_002&maps=XP2_004&maps=XP2_003&maps=XP3_UrbanGdn&maps=XP3_MarketPl&maps=XP3_Prpganda&maps=XP3_WtrFront&maps=XP4_WlkrFtry&maps=XP4_SubBase&maps=XP4_Titan&maps=XP4_Arctic&mapRotation=-1&modeRotation=-1&password=-1&regions=16&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500&includeExpansions=0&includeExpansions=1048576&includeExpansions=524288&includeExpansions=2097152&includeExpansions=4194304&includeExpansions=8388608\"},"
                        "{\"name\": \"Все моды, норм; 0-10; 32-64\",\"url\": \"https://battlelog.battlefield.com/bf4/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&maps=MP_Tremors&maps=XP7_Valley&maps=MP_Flooded&maps=MP_Journey&maps=MP_Resort&maps=MP_Damage&maps=MP_Naval&maps=MP_TheDish&maps=MP_Siege&maps=MP_Abandoned&maps=XP5_Night_01&slots=16&slots=1&slots=2&gameSize=32&gameSize=48&gameSize=64&q=&maps=XP1_002&maps=XP1_004&maps=XP1_003&maps=XP1_001&maps=XP0_Caspian&maps=XP0_Oman&maps=XP0_Firestorm&maps=XP0_Metro&maps=XP2_001&maps=XP2_002&maps=XP2_004&maps=XP2_003&maps=XP3_UrbanGdn&maps=XP3_MarketPl&maps=XP3_Prpganda&maps=XP3_WtrFront&maps=XP4_WlkrFtry&maps=XP4_SubBase&maps=XP4_Titan&maps=XP4_Arctic&gamepresets=1&mapRotation=-1&modeRotation=-1&password=-1&regions=16&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500&includeExpansions=0&includeExpansions=1048576&includeExpansions=524288&includeExpansions=2097152&includeExpansions=4194304&includeExpansions=8388608\"},"
                        "{\"name\": \"Все моды, реал; 0-10; 32-64\",\"url\": \"https://battlelog.battlefield.com/bf4/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&maps=MP_Tremors&maps=XP7_Valley&maps=MP_Flooded&maps=MP_Journey&maps=MP_Resort&maps=MP_Damage&maps=MP_Naval&maps=MP_TheDish&maps=MP_Siege&maps=MP_Abandoned&maps=XP5_Night_01&slots=16&slots=1&slots=2&gameSize=32&gameSize=48&gameSize=64&q=&maps=XP1_002&maps=XP1_004&maps=XP1_003&maps=XP1_001&maps=XP0_Caspian&maps=XP0_Oman&maps=XP0_Firestorm&maps=XP0_Metro&maps=XP2_001&maps=XP2_002&maps=XP2_004&maps=XP2_003&maps=XP3_UrbanGdn&maps=XP3_MarketPl&maps=XP3_Prpganda&maps=XP3_WtrFront&maps=XP4_WlkrFtry&maps=XP4_SubBase&maps=XP4_Titan&maps=XP4_Arctic&gamepresets=2&mapRotation=-1&modeRotation=-1&password=-1&regions=16&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500&includeExpansions=0&includeExpansions=1048576&includeExpansions=524288&includeExpansions=2097152&includeExpansions=4194304&includeExpansions=8388608\"},"
                        "{\"name\": \"Реал; НОЧНОЙ)\",\"url\": \"https://battlelog.battlefield.com/bf4/ru/servers/pc/?filtered=1&expand=1&settings=&useLocation=1&useAdvanced=1&gameexpansions=-1&gamemodes=1&gamemodes=64&q=&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gameexpansions=-1&gamepresets=2&mapRotation=-1&modeRotation=-1&password=-1&osls=-1&vvsa=-1&vffi=-1&vaba=-1&vkca=-1&v3ca=-1&v3sp=-1&vmsp=-1&vrhe=-1&vhud=-1&vmin=-1&vnta=-1&vbdm-min=1&vbdm-max=300&vprt-min=1&vprt-max=300&vshe-min=1&vshe-max=300&vtkk-min=1&vtkk-max=99&vnit-min=30&vnit-max=86400&vtkc-min=1&vtkc-max=99&vvsd-min=0&vvsd-max=500&vgmc-min=0&vgmc-max=500\"}"
                        "]}";
  QJsonDocument jsDoc = QJsonDocument::fromJson(defaultJson.toUtf8());
  QFile jsonFile(QCoreApplication::applicationDirPath() + "/" + CFG_FILENAME);
  jsonFile.open(QFile::WriteOnly);
  jsonFile.write(jsDoc.toJson(QJsonDocument::Indented));
  return jsDoc;
}

void setSettings(BF4BH2 *link)
{
  QJsonDocument jsDoc;
  QFile jsonFile(QCoreApplication::applicationDirPath() + "/" + CFG_FILENAME);
  if (!jsonFile.open(QIODevice::ReadOnly))
  {
    jsDoc = setJson();
  }
  else
  {
    jsDoc = QJsonDocument::fromJson(jsonFile.readAll());
  }
  QJsonObject jsObj;
  jsObj = jsDoc.object();
  foreach ( const QJsonValue & value, jsObj[ "presets"].toArray()) {
    QJsonObject obj = value.toObject();
    if(obj["button"].isNull())
      link->setAction(obj["name"].toString(), obj["url"].toString());
    else
      link->setAction(obj["name"].toString(), obj["url"].toString(), obj["button"].toInt());
  }
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName(VER_COMPANYNAME_STR);
  QCoreApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);
  QCoreApplication::setApplicationName(VER_PRODUCTNAME_STR);
  BF4BH2 w;
//  QSettings settings( QCoreApplication::applicationDirPath() + "/" + QString(VER_FILEDESCRIPTION_STR) + ".conf", QSettings::IniFormat);
  setSettings(&w);

  w.show();
  return a.exec();
}
