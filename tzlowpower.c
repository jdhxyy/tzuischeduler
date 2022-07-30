// Copyright 2019-2022 The jdh99 Authors. All rights reserved.
// �͹���ģ��
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

// TZLowPowerLoad ģ������
// minSleepInterval��������С���.��λ:us.�����Ҫ���ߵ�ʱ��С�ڴ˼��,�򲻻��������
// maxSleepInterval�����������.��λ:us.�����Ҫ���ߵ�ʱ����ڴ˼��,���ǿ������Ϊ��ʱ��
// sleep�Ǵ�������ߺ���.��Ҫ����ʱ�͹���ģ�����ô˺���
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

    // ���ߺ��һ�����в������͹����ж��߼�,��ֹ��Ӧ��ģ�黹δִ�оͽ���͹���
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
    // ����ʱ��̫������Ҫ��������
    if (wakeTime < timeNow + (uint64_t)gMinSleepInterval) {
        PT_EXIT(&pt);
    }
    // ����ʱ��̫��������Ϊ�������ʱ��
    if (wakeTime > timeNow + (uint64_t)gMaxSleepInterval) {
        wakeTime = timeNow + (uint64_t)gMaxSleepInterval;
    }
    // �͹���Ӧ�û���ݵ�ǰʱ���Ƿ����wakeTime���ж��Ƿ���,��1��Ϊ�˱�������ٽ�����
    gSleep(wakeTime + 1);
    gSleepTime += TZTimeGet() - timeNow;
    num = 0;

    PT_END(&pt);
}

// TZLowPowerRegister ע��ɵ͹���Ӧ��
// ע��ɹ�����true
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

// TZLowPowerGetRatio ��ȡ����ʱ��ռ��
// ���ط�Χֵ��0-10000.��λ:���֮һ
int TZLowPowerGetRatio(void) {
    return (int)(gSleepTime * 10000 / TZTimeGet());
}
