// Copyright 2020-2020 The TZIOT Authors. All rights reserved.
// UI调度器
// Authors: jdh99 <jdh821@163.com>

#ifndef TZUISCHEDULER_H
#define TZUISCHEDULER_H

#include "tztype.h"

// 无效页面
#define TZ_UI_SCHEDULER_PAGE_INVALID -1

// TZUiSchedulerLoad 模块载入
bool TZUiSchedulerLoad(void);

// TZUiSchedulerRegister 模块注册
bool TZUiSchedulerRegister(int page, TZEmptyFunc open, TZEmptyFunc close);

// TZUiSchedulerSwitch 切换页面
bool TZUiSchedulerSwitch(int page);

// TZUiSchedulerGetCurrentPage 获取当前页面
// 返回值为TZ_UI_SCHEDULER_PAGE_INVALID表示当前页面无效
int TZUiSchedulerGetCurrentPage(void);

// TZUiSchedulerGetPrevPage 获取上一页
int TZUiSchedulerGetPrevPage(void);

// TZUiSchedulerSwitch 切换上一页面
bool TZUiSchedulerSwitchPrevPage(void);

#endif
