// Copyright 2020-2020 The TZIOT Authors. All rights reserved.
// UI������
// Authors: jdh99 <jdh821@163.com>

#ifndef TZUISCHEDULER_H
#define TZUISCHEDULER_H

#include "tztype.h"

// ��Чҳ��
#define TZ_UI_SCHEDULER_PAGE_INVALID -1

// TZUiSchedulerLoad ģ������
bool TZUiSchedulerLoad(void);

// TZUiSchedulerRegister ģ��ע��
bool TZUiSchedulerRegister(int page, TZEmptyFunc open, TZEmptyFunc close);

// TZUiSchedulerSwitch �л�ҳ��
bool TZUiSchedulerSwitch(int page);

// TZUiSchedulerGetCurrentPage ��ȡ��ǰҳ��
// ����ֵΪTZ_UI_SCHEDULER_PAGE_INVALID��ʾ��ǰҳ����Ч
int TZUiSchedulerGetCurrentPage(void);

// TZUiSchedulerGetPrevPage ��ȡ��һҳ
int TZUiSchedulerGetPrevPage(void);

// TZUiSchedulerSwitch �л���һҳ��
bool TZUiSchedulerSwitchPrevPage(void);

#endif
