#include "qcomm.h"
#include<QTimerEvent>
#include<QCryptographicHash>
#include<QDebug>
#include<protocol.h>
#include<QDir>
#include<QTextCodec>
#include<stddef.h>
#include<string.h>


QCOMM::QCOMM(QObject *parent) :
    QObject(parent),udpsocket(new QUdpSocket(this)),tcpsocket(new QTcpSocket(this)),heartbeatnum(1)
  ,filetrans(NULL),nexttcpblocksize(0)
{
    udpsocket->connectToHost(UDP_DST_IP,6001,QIODevice::ReadWrite);
    timerid = startTimer(1000);
    tcpsocket->setSocketOption(QAbstractSocket::LowDelayOption,1);
    connect(udpsocket, SIGNAL(readyRead()),SLOT(readudppendingdatagrams()));
    connect(tcpsocket,SIGNAL(connected()),SLOT(ontcpsocketconneted()));
    connect(tcpsocket,SIGNAL(readyRead()),SLOT(readtcppendingdatagrams()));

}

void QCOMM::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==timerid){
        HEARTBEAT heart;
        heart.header.sizeofpack = sizeof heart;
        heart.header.magic[0] ='M';
        heart.header.magic[1] ='D';
        heart.header.seq = heartbeatnum++;
        heart.customerID = 0xaa;
        heart.fileterminal = hasterminalfile();
        heart.licensetimeout = islicensetimeout();
        heart.netinvade = isnetinvade();
        heart.numofpack = numberofudppack;
        heart.header.crc = qChecksum((char *)(&heart+sizeof (heart.header)),sizeof heart-sizeof (heart.header));
        udpsend((char*)&heart,sizeof(heart));
    }
}

void QCOMM::readudppendingdatagrams()
{
    char data[512];
    while(udpsocket->hasPendingDatagrams()){
        unsigned int len = udpsocket->read(data,512);
        if(len<sizeof(MDHEADER))
            continue;
        HEARTBEATRESULT *heartack = (HEARTBEATRESULT *)data;
        if(!checkMdHeart(heartack->header))
            continue;
        unsigned short packsize = heartack->header.sizeofpack;
        if (packsize<len)
            continue;
        //if(heartack->header.crc!=qChecksum((char *)&(heartack->funcode),
        //                                   len-sizeof (heartack->header)))
        //    continue;
        switch (heartack->funcode){
        case UDP_TCPCONNECT:{
            QHostAddress addr(ntohl(heartack->ipaddr));
            tcpConnect(addr,heartack->port);
            break;
        }
        case UDP_SSHCONNECT:{
            unsigned int ipaddr = *(unsigned int *)(heartack->ipaddr);
            sshConnect(QHostAddress("ipaddr"),ipaddr);
            break;
        }
        case UDP_MACHINEINFO:
            break;
        case UDP_HEARTBEATINC:
            break;
        default:
            break;
        }
    }
}

void QCOMM::ontcpdisconnected()
{
    qDebug("tcp disconneted");
    if(!filetrans.isNull()){
        delete filetrans.data();
    }
}


void QCOMM::tcpConnect(QHostAddress addr, unsigned short port)
{
    if(tcpsocket->state()!=QAbstractSocket::ConnectedState){
        tcpsocket->abort();
        qDebug()<<"tcp connnet to:"<<addr.toString();
        tcpsocket->connectToHost(addr,port);
    }
}

void QCOMM::sshConnect(QHostAddress addr, unsigned short port)
{

}

void QCOMM::reportpendingfiletras()
{
    /*if(hasterminalfile()){
        QDir dir("./");
        QStringList list = dir.entryList(QStringList()<<"*.TD",QDir::Files);
        QByteArray filename;
        filename.append(list.at(0));
        filename.remove(filename.size()-4,3);  //remove "*.TD"
        FILETRANSFER *filetransfer = (FILETRANSFER *)malloc(sizeof(TCPHEARDER)+4+filename.size());
        for(int i=0;i<filename.size()-1;i++){
            filetransfer->fileName[i] = filename.at(i);
        }
        QFile file(list.at(0));
        if(!file.open(QIODevice::ReadOnly))
            return;
        filetransfer->pos = FileTrans::readTransPos(file);
        filetransfer->filesize = file.size();
        filetransfer->header.funcode = TCP_FILETRANSFER;
        filetransfer->header.sizeofpack = sizeof(TCPHEARDER)+4+filename.size();
        file.close();
        tcpsocket->write((char *)filetransfer,filetransfer->header.sizeofpack);
        free(filetransfer);
    }*/
}

void QCOMM::ontcpsocketconneted()
{
    qDebug("tcpconneted logining");
    login();
    //reportpendingfiletras();
}

void QCOMM::readtcppendingdatagrams()
{
    QDataStream in(tcpsocket);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setVersion(QDataStream::Qt_4_7);
    unsigned short funcode;
    while(1){
        if(nexttcpblocksize==0){
            if(tcpsocket->bytesAvailable()<2)
                break;
            in>>nexttcpblocksize;
        }
        if(tcpsocket->bytesAvailable()<(nexttcpblocksize-2))
            break;
        int tcpblocksize = nexttcpblocksize;
        nexttcpblocksize = 0;

        in>>funcode;
        Q_ASSERT(sizeof(TCPHEARDER)==4);
        unsigned short  datasize= tcpblocksize-sizeof(TCPHEARDER);
        switch(funcode){
        case TCP_DOWNLOADFILE:{
            unsigned int filelen;
            in>>filelen;
            char temp[datasize-4];
            in.readRawData(temp,datasize-4);
            QString filename = QTextCodec::codecForName("GBK")->toUnicode(temp);
            qDebug()<<"be required to receive file \""<<filename<<"\";"<<"filelen:"<<filelen;
            filetrans = QPointer<FileTrans>(new FileTrans(tcpsocket,filename,filelen));
            break;
        }

        case TCP_FILETRANSFER:{
            if(!filetrans.isNull())
                filetrans->receivData(datasize);
            else{
                in.skipRawData(datasize);
                qDebug("windowmanager:invalid file transmition task");
            }
            break;
        }
        case TCP_MACHINEINFO:

            break;
        case TCP_FILELIST:{
            QDir dir("./media");
            QStringList list = dir.entryList(QStringList());
            QByteArray filenames = list.join(":").toUtf8();
            unsigned short lenofack = sizeof(TCPHEARDER)+filenames.size();
            FILELISTACK *ack = (FILELISTACK*)malloc(lenofack);
            ack->header.sizeofpack = sizeof(TCPHEARDER)+filenames.size();
            ack->header.funcode = TCP_FILELIST;
            qMemCopy(ack->filenamelist,filenames.data(),filenames.size());
            tcpsocket->write((char*)ack,lenofack);
            free(ack);
            break;
        }
        case TCP_STARTPLAY:

            break;
        case TCP_STOPPLAY:

            break;
        default:
            break;
        }
    }
}

void QCOMM::login()
{
    LOGIN loginpack;
    loginpack.hearder.funcode = TCP_LOGIN;
    loginpack.hearder.sizeofpack = sizeof(LOGIN);
    loginpack.customerID = 0x55aa;
    loginpack.machineID = 0;
    tcpsocket->write((char *)&loginpack,sizeof loginpack);
}

bool QCOMM::hasterminalfile()
{
    QDir dir("./media");
    QStringList list = dir.entryList(QStringList()<<"*.TD",QDir::Files);
    if(list.isEmpty())
        return FALSE;
    else
        return TRUE;
}

bool QCOMM::islicensetimeout()
{
    return FALSE;
}

bool QCOMM::isnetinvade()
{
    return FALSE;
}

bool QCOMM::checkMdHeart(MDHEADER heard)
{
    if((heard.magic[0]='M')&&(heard.magic[1]='D'))
        return TRUE;
    else
        return FALSE;
}

qint64 QCOMM::udpsend(const char *data, qint64 size)
{
    return udpsocket->write(data,size);
}

QByteArray QCOMM::md5(QFile &file)
{
    unsigned char *buf;
    bool isopen;
    if(!file.exists())
        return QByteArray();
    if(file.size()==0)
        return QByteArray();
    if(file.isOpen())
        isopen = TRUE;
    if(isopen&&(!file.open(QIODevice::ReadOnly)))
        return QByteArray();
    buf = file.map(0,file.size());
    if(isopen)
        file.close();
    if(0==buf)
        return QByteArray();
    QByteArray data = QByteArray::fromRawData((char*)buf,file.size());
    QByteArray md5 =  QCryptographicHash::hash (data, QCryptographicHash::Md5);
    file.unmap(buf);
    return md5;
}


FileTrans::FileTrans(QTcpSocket *tcp,const QString &filename, unsigned int filelen):ptcp(tcp),pos(0)
{
    file.setFileName("media/"+filename);
    if(file.exists())
        file.remove();
    file.setFileName("media/"+filename+".TD");
    bool fileexit;
    if(filelen>40*1024*1024){
        return;
    }
    if(file.exists()&&(file.size()==filelen-4))
        fileexit = TRUE;
    if(!file.open(QIODevice::ReadWrite)){
        return;
    }
    if(!file.resize(filelen+4)){
        file.close();
        return;
    }
    if(fileexit){
        pos = readTransPos();
    }
    in.setDevice(tcp);
    in.setByteOrder(QDataStream::LittleEndian);
    out.setDevice(&file);
    out.setByteOrder(QDataStream::LittleEndian);
    this->filelen = filelen;
    timer.setInterval(20000);
    timer.setSingleShot(FALSE);
    connect(&timer,SIGNAL(timeout()),SLOT(writeTransPos()));
    filetransack(pos);
}

FileTrans::~FileTrans()
{
    if(pos==filelen){
        file.resize(filelen);
        QString filename = file.fileName();
        filename.truncate(filename.size()-3);
        file.rename(filename);
    }else{
        writeTransPos();
    }
}

bool FileTrans::isAvalid()
{
    return file.isOpen();
}

void FileTrans::receivData(unsigned short len)
{
    unsigned char temp;
    unsigned int postemp = pos;
    timer.start();
    for(;pos<len+postemp;pos++){
        in>>temp;
        out<<temp;
    }
    filetransack(filelen);
    if(pos==filelen)  //file transmit finish
        deleteLater();
}


void FileTrans::writeTransPos()
{
    if(pos==0)
        return;
    if(pos==readTransPos())
        return;
    unsigned int postemp = file.pos();
    file.seek(file.size()-4);
    QDataStream str(&file);
    str.setByteOrder(QDataStream::LittleEndian);
    str<<pos;
    file.seek(postemp);
}


unsigned int FileTrans::readTransPos()
{
    unsigned int postemp = file.pos();
    file.seek(file.size()-4);
    char buf[4];
    unsigned int pos = *(unsigned int*)buf;
    file.read(buf,4);
    file.seek(postemp);
    if(pos<file.size())
        return pos;
    else
        return 0;
}

unsigned int FileTrans::readTransPos(QFile &file)
{
    unsigned int postemp = file.pos();  //push file pos
    file.seek(file.size()-4);
    QDataStream str(&file);
    str.setByteOrder(QDataStream::LittleEndian);
    unsigned int pos ;
    str>>pos;
    file.seek(postemp);    //pop file pos
    if(pos<file.size())
        return pos;
    else
        return 0;
}


void FileTrans::filetransack(unsigned int pos)
{
    FILEDATAACK ack;
    ack.header.sizeofpack = sizeof(ack);
    ack.header.funcode = TCP_FILETRANSFER;
    ack.pos = pos;
    ptcp->write((char *)&ack,sizeof ack);
}


