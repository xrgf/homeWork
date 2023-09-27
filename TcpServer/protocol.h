#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdlib.h>
#include <string.h>
typedef unsigned int uint;

struct FileInfo
{
    char caFileName[32];//文件名字
    int iFileType;      //文件类型
};

enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN=0,
    ENUM_MSG_TYPE_LOGIN_REQUEST,//登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,//登录请求
    ENUM_MSG_TYPE_DATA_TOGETHER_REQUEST,//数据同步请求
    ENUM_MSG_TYPE_DATA_TOGETHER_RESPOND,//数据同步恢复
    ENUM_MSG_TYPE_DATA_COMPARE_REQUEST,//数据对比请求
    ENUM_MSG_TYPE_DATA_COMPARE_RESOPND,//数据对比回复
    ENUM_MSG_TYPE_DATA_CONFILCT_REQUEST,//数据冲突请求
    ENUM_MSG_TYPE_DATA_CONFILCT_RESOPND,//数据冲突回复




    ENUM_MSG_TYPE_MAX=0X00ffffff
};
struct PDU
{
    uint uiPDULen;   //总的协议数据单元大小
    uint uiMsgType;  //消息类型（告诉对方数据是干嘛的）
    char caData[64];
    uint uiMsgLen;   //实际消息长度
    int caMsg[];     //实际消息
};
PDU* mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H
