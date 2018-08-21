#include <QCoreApplication>
#include <iostream>
#include <QtDebug>
#include <QtSql>
#include <QSqlError>
#include <QDate>
#include <Qstring>
#include <vector>
using namespace std;
void tbplayer(QSqlQuery *query);
void randtbMatch(QSqlQuery *query);
void tbplayermatch(QSqlQuery *query);
void tb_player_rig(QSqlQuery *query);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifdef Q_OS_WIN32
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 866"));
#endif

#ifdef Q_OS_LINUX
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif
       QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
       db.setHostName("127.0.0.1");
       db.setPort(3306);
       db.setDatabaseName("worldoftanks");
       db.setUserName("root");
       db.setPassword("");
       bool ok = db.open();
       QSqlQuery *query;
       query = new QSqlQuery(db);
   //   randtbMatch(query);
      tbplayermatch(query);
   //   tbplayer(query);
   //     tb_player_rig(query);
         qDebug() << db.lastError().text();
    return a.exec();
}


void randtbMatch(QSqlQuery *query)
{
    int Type_of_Match=0;
    int Map;
    QDate Date;
    int time;
    query->prepare("INSERT INTO tbmatch(Id_Match,Type_of_Match,Map,Date,Time) VALUES(:Id_Match,:Type_of_Match,:Map,:Date,:Time)");
    for (int i=0;i<1000000;i++)
    {
        query->bindValue(":Id_Match", i);
        Type_of_Match = rand() %3+1;
        query->bindValue(":Type_of_Match", Type_of_Match);
        Map = rand() %6+1;
        query->bindValue(":Map", Map);
        time = rand() %900+60;
        int Year =2014 + rand() %4;
        int Month = rand() %12+1;
        int day = rand() %30+1;
        Date.setDate(Year,Month,day);
        query->bindValue(":Date", Date.toString("yyyy/MM/dd"));
        query->bindValue(":Time", time);
        query->exec();
    }
    qDebug() << "OK!";
}

void tbplayermatch(QSqlQuery *query)
{
    query->exec("SELECT count(*) FROM tbmatch");
    int count=0;
    while(query->next()) {count=query->value(0).toInt();}
    query->exec("SELECT id_tank FROM tbtanks");
    vector<int> TanksId;
    while(query->next()) {TanksId.push_back(query->value(0).toInt());}
    vector<int> PlayersId;
    query->exec("SELECT Id_Player FROM tbplayer");
    while(query->next()) {PlayersId.push_back(query->value(0).toInt());}
    query->prepare("INSERT INTO tbplayermatch(Id_Match,Id_Tank,Id_Player,Result,PlayerDamage,PlayerUseful,Exp,Credits) VALUES(:Id_Match,:Id_Tank,:Id_Player,:Result,:PlayerDamage,:PlayerUseful,:Exp,:Credits)");
    for(int i = 0; i<25000;i++)
    {
         query->bindValue(":Id_Match", i);
         int Id_tank=0+rand()%TanksId.size();
         query->bindValue(":Id_Tank",  TanksId[Id_tank]);
         int Id_Player=0+rand()%PlayersId.size();
         query->bindValue(":Id_Player",PlayersId[Id_Player]);
         query->bindValue(":Result",0+rand()%3);
         query->bindValue(":PlayerDamage",0+rand()%5000);
         query->bindValue(":PlayerUseful",0+rand()%100);
         query->bindValue(":Exp", 0+rand()%1500);
         query->bindValue(":Credits", 0+rand()%50000);
         query->exec();
         qDebug() << i;
    }
     qDebug() << "OK!";
}

void tbplayer(QSqlQuery *query)
{
   vector<QString> niks {"George","Shadow","bandit","coffee","falcon","fuckyou","Andrew","Buster","Cowboy","Eagles","golf","green","linda","merlin","secret","soccer","steve"};
    query->prepare("INSERT INTO tbplayer(Id_Player,Region,Nick) VALUES(:Id_Player,:Region,:Nick)");
    for (int i=0;i<17;i++)
    {
        query->bindValue(":Id_Player", i);
        query->bindValue(":Region", 1+rand()%4);
        QString str= niks[i];
        query->bindValue(":Nick", str);
        query->exec();
    }
     qDebug() << "tbPlayer OK!";
}

void tb_player_rig(QSqlQuery *query)
{
    vector<int> PlayersId;
    query->exec("SELECT Id_Player FROM tbplayer");
    while(query->next()) {PlayersId.push_back(query->value(0).toInt());}
    vector<int> RigsId;
    query->exec("SELECT Id_Rig FROM tbrigs");
    while(query->next()) {RigsId.push_back(query->value(0).toInt());}
    query->prepare("INSERT INTO tbplayerrig(Id_Rig,Id_Player,Kolvo) VALUES(:Id_Rig,:Id_Player,:Kolvo)");
    for(int i = 0;i<PlayersId.size();i++)
    {
        for (int j=0;j<RigsId.size();j++)
        {
            query->bindValue(":Id_Player", PlayersId[i]);
            query->bindValue(":Id_Rig", RigsId[j]);
            query->bindValue(":Kolvo", 0+rand()%3);
            query->exec();
        }
    }
}
