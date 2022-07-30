// Copyright 2019-2022 The jdh99 Authors. All rights reserved.
// 低功耗模块
// Authors: jdh99 <jdh821@163.com>

#include "tzlowpower.h"
#include "tztime.h"
#include "async.h"

static int task(void);

static int gMinSleepInterval = 0;
static int gMaxSleepInterval = 0;
static TZLowPowerSleepFunc gSleep = NULL;
static TZLowPowerGetWakeTimeFunc gGetWakeTimeList[TZLOW_POWER_ITEM_NUM_MAX];
static int gGetWakeTimeListLen = 0;
static uint64_t gSleepTime = 0;

// TZLowPowerLoad 模块载入
// minSleepInterval是休眠最小间隔.单位:us.如果需要休眠的时间小于此间隔,则不会进入休眠
// maxSleepInterval是休眠最大间隔.单位:us.如果需要休眠的时间大于此间隔,则会强制设置为此时间
// sleep是传入的休眠函数.需要休眠时低功耗模块会调用此函数
bool TZLowPowerLoad(int minSleepInterval, int maxSleepInterval, TZLowPowerSleepFunc sleep) {
    gMinSleepInterval = minSleepInterval;
    gMaxSleepInterval = maxSleepInterval;
    gSleep = sleep;

    if (AsyncStart(task, ASYNC_NO_WAIT) == false) {
        return false;
    }
    return true;
}

static int task(void) {
    static struct pt pt = {0};

    PT_BEGIN(&pt);

    static int num = 0;

    if (gSleep == NULL) {
        PT_EXIT(&pt);
    }

    // 休眠后第一次运行不会进入低功耗判断逻辑,防止有应用模块还未执行就进入低功耗
    num++;
    if (num <= 1) {
        PT_EXIT(&pt);
    }

    uint64_t timeNow = TZTimeGet();
    uint64_t wakeTime = TZLOW_POWER_WAKE_UNLOCK;
    uint64_t getWakeTimeValue = 0;
    for (int i = 0; i < gGetWakeTimeListLen; i++) {
        getWakeTimeValue = gGetWakeTimeList[i]();
        if (getWakeTimeValue == TZLOW_POWER_WAKE_LOCK) {
            PT_EXIT(&pt);
        }
        if (getWakeTimeValue == TZLOW_POWER_WAKE_UNLOCK) {
            continue;
        }
        if (getWakeTimeValue < timeNow) {
            continue;
        }
        if (getWakeTimeValue < wakeTime) {
            wakeTime = getWakeTimeValue;
        }
    }
    // 休眠时间太短则不需要进入休眠
    if (wakeTime < timeNow + (uint64_t)gMinSleepInterval) {
        PT_EXIT(&pt);
    }
    // 休眠时间太长则设置为最大休眠时间
    if (wakeTime > timeNow + (uint64_t)gMaxSleepInterval) {
        wakeTime = timeNow + (uint64_t)gMaxSleepInterval;
    }
    // 低功耗应用会根据当前时间是否大于wakeTime来判断是否工作,加1是为了避免出现临界问题
    gSleep(wakeTime + 1);
    gSleepTime += TZTimeGet() - timeNow;
    num = 0;

    PT_END(&pt);
}

// TZLowPowerRegister 注册成低功耗应用
// 注册成功返回true
bool TZLowPowerRegister(TZLowPowerGetWakeTimeFunc getWakeTime) {
    if (getWakeTime == NULL) {
        return false;
    }
    if (gGetWakeTimeListLen >= TZLOW_POWER_ITEM_NUM_MAX) {
        return false;
    }
    gGetWakeTimeList[gGetWakeTimeListLen++] = getWakeTime;
    return true;
}

// TZLowPowerGetRatio 获取休眠时间占比
// 返回范围值从0-10000.单位:万分之一
int TZLowPowerGetRatio(void) {
    return (int)(gSleepTime * 10000 / TZTimeGet());
}
