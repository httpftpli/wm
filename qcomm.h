#ifndef QCOMM_H
#define QCOMM_H

#include <QObject>
#include<QUdpSocket>
#include<QTcpSocket>
#include<QFile>
#include<QByteArray>
#include<QPointer>
#include<protocol.h>
#include<QTimer>

#define ntohl(A)        ((((unsigned long)(A) & 0xff000000) >> 24) | \
(((unsigned long)(A) & 0x00ff0000) >> 8) | \
(((unsigned long)(A) & 0x0000ff00) << 8) | \
(((unsigned long)(A) & 0x000000ff) << 24))
#define  htonl(A) ntohl(A)


class FileTrans;

class QCOMM : public QObject
{
    Q_OBJECT
public:
    explicit QCOMM(QObject *parent = 0);
protected:
    virtual void timerEvent (QTimerEvent * event);
    
signals:
    
private slots:

private:
    void tcpConnect(QHostAddress addr,unsigned short port);
    void sshConnect(QHostAddress addr,unsigned short port);
    void reportpendingfiletras();
    void login();
    bool hasterminalfile();
    bool islicensetimeout();
    bool isnetinvade();
    static bool checkMdHeart(MDHEADER heard);
    qint64 udpsend(const char *data,qint64 size);
    static QByteArray md5(QFile &file);
    QUdpSocket *udpsocket;
    QTcpSocket *tcpsocket;
    int timerid;
    unsigned short nexttcpblocksize;
    unsigned int heartbeatnum;
    unsigned int numberofudppack;
    QPointer<FileTrans> filetrans;


private slots:
    void ontcpsocketconneted();
    void readtcppendingdatagrams();
    void readudppendingdatagrams();
    void ontcpdisconnected();
};


class FileTrans:public QObject
{
    Q_OBJECT
public :
    FileTrans(QTcpSocket *tcp,const QString &filename, unsigned int filelen);
    ~FileTrans();
    bool isAvalid();
    void receivData(unsigned short len);
    static unsigned int readTransPos(QFile &file);

private slots:
    void writeTransPos();
private:
    QTcpSocket *ptcp;
    QDataStream in;
    QDataStream out;
    QFile file;
    unsigned int pos;
    unsigned int filelen;
    unsigned int readTransPos();
    void filetransack(unsigned int pos);
    QTimer timer;      //timer for save  file write pos
};

#endif // QCOMM_H
