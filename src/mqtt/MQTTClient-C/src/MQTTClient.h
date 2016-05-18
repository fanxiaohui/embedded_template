/*******************************************************************************
 * Copyright (c) 2014, 2015 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander/Ian Craggs - initial API and implementation and/or initial documentation
 *    Ian Craggs - documentation and platform specific header
 *******************************************************************************/

#if !defined(__MQTT_CLIENT_C_)
#define __MQTT_CLIENT_C_

#if defined(__cplusplus)
extern "C" {
#endif

#include "MQTTPacket.h"
#include "stdio.h"

#if defined(MQTTCLIENT_PLATFORM_HEADER)
/* The following sequence of macros converts the MQTTCLIENT_PLATFORM_HEADER value
 * into a string constant suitable for use with include.
 */
#define xstr(s) str(s)
#define str(s) #s
#include xstr(MQTTCLIENT_PLATFORM_HEADER)
#endif

#define MAX_PACKET_ID 65535 /* according to the MQTT specification - do not change! */

#if !defined(MAX_MESSAGE_HANDLERS)
#define MAX_MESSAGE_HANDLERS 5 /* redefinable - how many subscriptions do you want? */
#endif

enum QoS { QOS0, QOS1, QOS2 };

#include "timer.h"

/* all failure return codes must be negative */
enum returnCode { BUFFER_OVERFLOW = -2, FAILURE = -1, SUCCESS = 0 };

typedef int (*mqtt_network_read)(void *priv, uint8_t *buf, int size, os_time_t timeout_ms);
typedef int (*mqtt_network_set_recv_buffer)(void *priv, uint8_t buf, uint8_t size);
typedef int (*mqtt_network_write)(void *priv, const uint8_t *dat, int size, os_time_t timeout_ms);
typedef int (*mqtt_network_connect)(void *priv, const char *host, uint16_t port);
typedef void (*mqtt_network_disconnect)(void *priv);

struct mqtt_network {
    void *priv;
    mqtt_network_read read;
    //mqtt_network_set_recv_buffer set_recv_buffer;
    mqtt_network_write write;
    mqtt_network_connect connect;
    mqtt_network_disconnect disconnect;
};

typedef const struct mqtt_network *mqtt_network_t;

typedef struct MQTTMessage {
    enum QoS qos;
    unsigned char retained;
    unsigned char dup;
    unsigned short id;
    void *payload;
    size_t payloadlen;
} MQTTMessage;

typedef struct MessageData {
    MQTTMessage *message;
    MQTTString *topicName;
} MessageData;

typedef void (*messageHandler)(MessageData *);

struct mqtt_client {
    unsigned int next_packetid, command_timeout_ms;
    size_t buf_size;
    size_t readbuf_size;
    unsigned char *buf;
    unsigned char *readbuf;
    unsigned int keepAliveInterval;
    char ping_outstanding;
    int isconnected;

    struct MessageHandlers {
        const char *topicFilter;
        void (*fp) (MessageData *);
    } messageHandlers[MAX_MESSAGE_HANDLERS];      /* Message handlers are indexed by subscription topic */

    void (*defaultMessageHandler) (MessageData *);

    mqtt_network_t ipstack;
    struct timer ping_timer;
#if defined(MQTT_TASK)
    Mutex mutex;
    Thread thread;
#endif
};
typedef struct mqtt_client *mqtt_client_t;

#define DefaultClient {0, 0, 0, 0, NULL, NULL, 0, 0, 0}


/**
 * Create an MQTT client object
 * @param client
 * @param network
 * @param command_timeout_ms
 * @param
 */
void MQTTClientInit(mqtt_client_t client, mqtt_network_t network, unsigned int command_timeout_ms,
                              unsigned char *sendbuf, size_t sendbuf_size, unsigned char *readbuf, size_t readbuf_size);

/** MQTT Connect - send an MQTT connect packet down the network and wait for a Connack
 *  The nework object must be connected to the network endpoint before calling this
 *  @param options - connect options
 *  @return success code
 */
int MQTTConnect(mqtt_client_t client, MQTTPacket_connectData *options);

/** MQTT Publish - send an MQTT publish packet and wait for all acks to complete for all QoSs
 *  @param client - the client object to use
 *  @param topic - the topic to publish to
 *  @param message - the message to send
 *  @return success code
 */
int MQTTPublish(mqtt_client_t client, const char *, MQTTMessage *);

/** MQTT Subscribe - send an MQTT subscribe packet and wait for suback before returning.
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter to subscribe to
 *  @param message - the message to send
 *  @return success code
 */
int MQTTSubscribe(mqtt_client_t client, const char *topicFilter, enum QoS, messageHandler);

/** MQTT Subscribe - send an MQTT unsubscribe packet and wait for unsuback before returning.
 *  @param client - the client object to use
 *  @param topicFilter - the topic filter to unsubscribe from
 *  @return success code
 */
int MQTTUnsubscribe(mqtt_client_t client, const char *topicFilter);

/** MQTT Disconnect - send an MQTT disconnect packet and close the connection
 *  @param client - the client object to use
 *  @return success code
 */
int MQTTDisconnect(mqtt_client_t client);

/** MQTT Yield - MQTT background
 *  @param client - the client object to use
 *  @param time - the time, in milliseconds, to yield for
 *  @return success code
 */
int MQTTYield(mqtt_client_t client, int time);

#if defined(MQTT_TASK)
/** MQTT start background thread for a client.  After this, MQTTYield should not be called.
*  @param client - the client object to use
*  @return success code
*/
int MQTTStartTask(mqtt_client_t client);
#endif

#if defined(__cplusplus)
}
#endif

#endif
