// Copyright 2019-2022 The jdh99 Authors. All rights reserved.
// 低功耗模块
// Authors: jdh99 <jdh821@163.com>

#ifndef TZLOWPOWER_H
#define TZLOWPOWER_H

#include "tztype.h"

// 最大支持的注册低功耗的应用数
#define TZLOW_POWER_ITEM_NUM_MAX 20

// 唤醒锁.得到唤醒时间如果返回的是此值,则会保持唤醒,不会进入休眠
#define TZLOW_POWER_WAKE_LOCK 0xFF20200107010323
// 解除唤醒锁.允许低功耗模块控制是否进入休眠
#define TZLOW_POWER_WAKE_UNLOCK 0xFF20200108011219

// TZLowPowerSleepFunc 休眠
// wakeTime是本次休眠需要醒来的时间.此时间是本地绝对时间
typedef void (*TZLowPowerSleepFunc)(uint64_t wakeTime);

// TZLowPowerGetWakeTimeFunc 得到唤醒时间函数类型
// 返回的是下一次需要唤醒的时间.单位:us
// 注意:此时间是本地绝对时间,不是相对与当前时刻的差值
// 如果返回的是唤醒锁,则不会进入休眠
typedef uint64_t (*TZLowPowerGetWakeTimeFunc)(void);

// TZLowPowerLoad 模块载入
// minSleepInterval是休眠最小间隔.单位:us.如果需要休眠的时间小于此间隔,则不会进入休眠
// maxSleepInterval是休眠最大间隔.单位:us.如果需要休眠的时间大于此间隔,则会强制设置为此时间
// sleep是传入的休眠函数.需要休眠时低功耗模块会调用此函数
bool TZLowPowerLoad(int minSleepInterval, int maxSleepInterval, TZLowPowerSleepFunc sleep);

// TZLowPowerRegister 注册成低功耗应用
// 注册成功返回true
bool TZLowPowerRegister(TZLowPowerGetWakeTimeFunc getWakeTime);

// TZLowPowerGetRatio 获取休眠时间占比
// 返回范围值从0-10000.单位:万分之一
int TZLowPowerGetRatio(void);

#endif
