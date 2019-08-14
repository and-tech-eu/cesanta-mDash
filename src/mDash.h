// Copyright (c) 2019 Cesanta Software Limited
// All rights reserved

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if ARDUINO
#define __TOSTR(x) #x
#define STR(x) __TOSTR(x)
#ifndef ARDUINO_BOARD
#define ARDUINO_BOARD "?"
#endif
#define MDASH_FRAMEWORK "a-" STR(ARDUINO) "-" ARDUINO_BOARD
#else
#define MDASH_FRAMEWORK "idf"
#endif

#ifndef MDASH_APP_NAME
#define MDASH_APP_NAME __FILE__
#endif

#define MDASH_BUILD_TIME __DATE__ "-" __TIME__

#define mDashStartWithWifi(a, b, c, d)                                    \
  mDashInitWithWifi((a), (b), (c), (d), MDASH_APP_NAME, MDASH_BUILD_TIME, \
                    MDASH_FRAMEWORK)

#define mDashStart(a, b) \
  mDashInit((a), (b), MDASH_APP_NAME, MDASH_BUILD_TIME, MDASH_FRAMEWORK)

#define mDashBegin()                                                          \
  mDashInitWithWifi(NULL, NULL, NULL, NULL, MDASH_APP_NAME, MDASH_BUILD_TIME, \
                    MDASH_FRAMEWORK)

// mDash states
enum { MDASH_NO_IP, MDASH_AP_IP, MDASH_STA_IP, MDASH_CONNECTED };

// mDash housekeeping
void mDashInitWithWifi(const char *wifi_name, const char *wifi_pass,
                       const char *device_id, const char *device_pass,
                       const char *app_name, const char *build_time,
                       const char *framework);
void mDashInit(const char *device_id, const char *device_pass,
               const char *app_name, const char *build_time,
               const char *framework);

int mDashGetState(void);
void mDashOn(void (*fn)(void *), void *);
void mDashSetLogLevel(int logLevel);
void mDashSetServer(const char *, int);
const char *mDashGetDeviceID(void);
unsigned long mDashGetFreeRam(void);
struct mjson_out;
int mjson_printf(struct mjson_out *, const char *, ...);

// Logging API
enum { LL_NONE, LL_CRIT, LL_INFO, LL_DEBUG };
#define MLOG(ll, fmt, ...) mlog(ll, __func__, (fmt), __VA_ARGS__)
void mlog(int ll, const char *fn, const char *fmt, ...);

// Provisioning API
void mDashCLI(unsigned char input_byte);

// MQTT API
int mDashPublish(const char *topic, const char *message_fmt, ...);
void mDashSubscribe(const char *topic, void (*fn)(const char *, const char *));
int mDashShadowUpdate(const char *message_fmt, ...);
void mDashShadowDeltaSubscribe(void (*fn)(const char *, const char *));

// RPC API
void mDashExport(const char *name, void (*cb)(void *, void *), void *cbdata);
void mDashReturnSuccess(void *ctx, const char *json_fmt, ...);
void mDashReturnError(void *ctx, const char *error_message);
const char *mDashGetParams(void *ctx);

// JSON API
int mDashGetNum(const char *json, const char *json_path, double *value);
int mDashGetStr(const char *json, const char *json_path, char *dst, int len);
int mDashGetBase64(const char *json, const char *json_path, char *dst, int len);
int mDashGetBool(const char *json, const char *json_path, int *);

// Configuration API
int mDashConfigGet(const char *name, char *buf, int bufsize);
int mDashConfigSet(const char *name, const char *value);
void mDashCLI(unsigned char input_byte);

#ifdef __cplusplus
}
#endif
