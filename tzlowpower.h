// Copyright 2019-2022 The jdh99 Authors. All rights reserved.
// �͹���ģ��
// Authors: jdh99 <jdh821@163.com>

#ifndef TZLOWPOWER_H
#define TZLOWPOWER_H

#include "tztype.h"

// ���֧�ֵ�ע��͹��ĵ�Ӧ����
#define TZLOW_POWER_ITEM_NUM_MAX 20

// ������.�õ�����ʱ��������ص��Ǵ�ֵ,��ᱣ�ֻ���,�����������
#define TZLOW_POWER_WAKE_LOCK 0xFF20200107010323
// ���������.����͹���ģ������Ƿ��������
#define TZLOW_POWER_WAKE_UNLOCK 0xFF20200108011219

// TZLowPowerSleepFunc ����
// wakeTime�Ǳ���������Ҫ������ʱ��.��ʱ���Ǳ��ؾ���ʱ��
typedef void (*TZLowPowerSleepFunc)(uint64_t wakeTime);

// TZLowPowerGetWakeTimeFunc �õ�����ʱ�亯������
// ���ص�����һ����Ҫ���ѵ�ʱ��.��λ:us
// ע��:��ʱ���Ǳ��ؾ���ʱ��,��������뵱ǰʱ�̵Ĳ�ֵ
// ������ص��ǻ�����,�򲻻��������
typedef uint64_t (*TZLowPowerGetWakeTimeFunc)(void);

// TZLowPowerLoad ģ������
// minSleepInterval��������С���.��λ:us.�����Ҫ���ߵ�ʱ��С�ڴ˼��,�򲻻��������
// maxSleepInterval�����������.��λ:us.�����Ҫ���ߵ�ʱ����ڴ˼��,���ǿ������Ϊ��ʱ��
// sleep�Ǵ�������ߺ���.��Ҫ����ʱ�͹���ģ�����ô˺���
bool TZLowPowerLoad(int minSleepInterval, int maxSleepInterval, TZLowPowerSleepFunc sleep);

// TZLowPowerRegister ע��ɵ͹���Ӧ��
// ע��ɹ�����true
bool TZLowPowerRegister(TZLowPowerGetWakeTimeFunc getWakeTime);

// TZLowPowerGetRatio ��ȡ����ʱ��ռ��
// ���ط�Χֵ��0-10000.��λ:���֮һ
int TZLowPowerGetRatio(void);

#endif
