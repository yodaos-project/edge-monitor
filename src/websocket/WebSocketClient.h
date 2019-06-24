//
// Created by 周飞宇 on 2018/11/26.
//

#ifndef TESTUV_WEBSOCKETCLIENT_H
#define TESTUV_WEBSOCKETCLIENT_H

#include <vector>
#include <uv.h>
#include <list>
#include <functional>
#include "libwebsockets.h"
#include "caps.h"
#include <functional>

using namespace std;


enum class EventCode {
    Connected, DisConnected
};

enum class SendResult {
    SendOut, SendFailed, Removed
};

typedef std::function<void(SendResult sr, void* userData)> SendCallback;

typedef std::function<void(shared_ptr<Caps> &caps)> RecvCallback;

typedef std::function<void(enum EventCode ec)> EventCallback;


typedef struct _sDataToSend{
    vector<unsigned char> data;
    void* userdata;
    SendCallback cb;
} DataToSend;

class WebSocketClient {
private:
    uv_loop_t *uv = nullptr;
    size_t maxBufferSize = 100;
    uv_timer_t timerHandle;
    string address;
    string path;
    int port = 0;
    std::function<void(shared_ptr<Caps> &)> funcRecvCb = nullptr;
    std::function<void(enum EventCode)> funcEventCb = nullptr;
    struct lws_context *context = nullptr;
    struct lws_context_creation_info info;
    void **foreign_loops = nullptr;
    struct lws *web_socket = nullptr;
    // struct lws_protocols protocols[2];
    struct lws_client_connect_info cinfo = {0};
    list<DataToSend> msgList;

    static int callback_ws(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

    static void reconnectTimerCb(uv_timer_t *handle);

    void reconnect();

    bool connect();

public:
    WebSocketClient();

    ~WebSocketClient();

    int32_t init();

    void sendMsg(shared_ptr<Caps> &msg, SendCallback cb = nullptr, void *cbData = nullptr);

    void sendMsg(vector<shared_ptr<Caps>> &msgs, SendCallback cb = nullptr, void *cbData = nullptr);

    void setRecvCallback(const std::function<void(shared_ptr<Caps> &)> &cb);

    void setEventCallback(const std::function<void(enum EventCode)> &cb);

    bool start(const char *address, int port, const char *path);

    void stop();
};


#endif //TESTUV_WEBSOCKETCLIENT_H
