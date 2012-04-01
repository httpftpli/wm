#ifndef _MD_PROTOCOL_H_
#define _MD_PROTOCOL_H_


#define UDP_DST_IP    "192.168.1.113"
#define UDP_DST_PORT  6001


// UDP包头定义
#pragma pack(1)
typedef struct {
    unsigned short sizeofpack;	// 包的总大小
    unsigned char  magic[2];	// 唯一标示,强制为"MD"
    unsigned short seq;		    // 心跳序号，16位
    unsigned short crc;		    // crc校验值，16位
} MDHEADER, *PMDHEADER;
#pragma pack()

// 心跳包, 节点-》服务端
#pragma pack(1)
typedef struct {
    MDHEADER	    header;
    unsigned short  numofpack;	     // 接受到来自服务端的数据包个数
    unsigned short  netinvade:1;	 // 网络入侵标志
    unsigned short  fileterminal:1;	 // 是否有未传完的文件
    unsigned short  licensetimeout:1;// 许可是否到期
    unsigned short  bitpading:6;  	 // 其他状态
    unsigned short  customerID;	     // 代码
    unsigned int    machineId;       // 机器代码
    char		    padding[4];      // 其他
} HEARTBEAT, *PHEARTBEAT;
#pragma pack()


//UDP通讯协议 UDP功能码
#define UDP_TCPCONNECT		0x01	// 发起TCP连接
#define UDP_SSHCONNECT		0x02	// 发起SSH连接
#define UDP_MACHINEINFO		0x11	// 查询机器详细信息
#define UDP_HEARTBEATINC    0x12    // 设置心跳间隔
// 心跳包, 服务端-》节点
#pragma pack(1)
typedef struct {
    MDHEADER	header;
    unsigned char funcode;	   //功能码 如上
    unsigned int  ipaddr;   //IP地址
    unsigned short port;       //port
} HEARTBEATRESULT, *PHEARTBEATRESULT;
#pragma pack(0)
//发起TCP连接：header+funcode+ipaddr+port
//发起SSH连接：header+funcode+ipaddr+port
//设置心跳间隔：header+beatinterval(unsigned char)


// TCP通讯协议  TCP功能码
#define TCP_LOGIN           0x01
#define TCP_LOGOUT          0x02
#define TCP_FILETRANSFER    0x03    // 文件传送数据
#define TCP_MACHINEINFO		0x11	// 查询机器详细信息
#define TCP_STARTPLAY		0x21	// 开始播放
#define TCP_STOPPLAY		0x22	// 停止播放
#define TCP_DOWNLOADFILE	0x31	// 下载文件到机器
#define TCP_FILELIST    	0x41	// 获取媒体文件列表



#pragma pack(1)
typedef struct {
    unsigned short sizeofpack;	// 包的总大小
    unsigned short funcode;	// 功能码 如上
} TCPHEARDER, *PTCPHEARDER;
#pragma pack()

#pragma pack(1)
typedef struct {
    TCPHEARDER hearder;
    unsigned short customerID; // 机器ID
    unsigned int   machineID ; //
} LOGIN, *PLOGIN;
#pragma pack()

#pragma pack(1)
typedef struct {
    TCPHEARDER header;
    unsigned char funcode;
    unsigned char customerID; // 机器ID
} LOGOUT, *PLOGOUT;
#pragma pack()


//服务器上传新文件 ：服务端-》节点
//机器报告断点文件 ：节点 -》服务端
#pragma pack(1)
typedef struct {
    TCPHEARDER header;
    unsigned int filesize;   //文件长度
    char		fileName[256]; // 文件名
} FILETRANSFER, *PFILETRANSFER;
#pragma pack()

//服务端-》节点
//文件数据，数据长度 = header.sizeofpack -sizeof(header)
#pragma pack(1)
typedef struct {
    TCPHEARDER header;
    char  data[1024];
} FILEDATA, *PFILEDATA;
#pragma pack()

//节点-》服务端
//机器对服务器文件请求的应答，当机器收到服务器请求文件传送后应答当前传送的位置
//机器每收到一次服务器的数据报告下一个传送的文件偏移位置
#pragma pack(1)
typedef struct {
    TCPHEARDER header;
    unsigned long pos;		// 数据所在位置 0：表示新文件，POS==filesize表示传送完成，-1表示出错
} FILEDATAACK, *PFILEDATAACK;
#pragma pack()

#pragma pack(1)
typedef struct{
  TCPHEARDER header;
} FILELIST;
#pragma pack()

#pragma pack(1)
typedef struct{
  TCPHEARDER header;
  char filenamelist[512];
} FILELISTACK;
#pragma pack()

#pragma pack(1)
typedef struct{
  TCPHEARDER header;
  unsigned char play; //0:播放文件，1：播放脚本
  char filename[128]; //如play==0，filename表示文件名
} PLAY;
#pragma pack()

#pragma pack(1)
typedef struct{
  TCPHEARDER header;
  unsigned char ack; //0:播放开始 1：播放错误
} LAYACK;
#pragma pack()

#pragma pack(1)
typedef struct{
  TCPHEARDER header;
} STOPPLAY;
#pragma pack()

#endif // PROTOCAL_H
