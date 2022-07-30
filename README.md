<!-- TOC -->

- [TZUiScheduler操作手册](#tzuischeduler操作手册)
  - [文件历史](#文件历史)
  - [概述](#概述)
  - [API](#api)
  - [注册页面](#注册页面)
  - [切换页面](#切换页面)

<!-- /TOC -->

# TZUiScheduler操作手册

## 文件历史
日期|作者|描述
-|-|-
2020-11-01|周鑫|1.新建

## 概述
TZUiScheduler是UI调度器，是多页面切换框架。

## API
```c
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
```

## 注册页面
每个页面都有3个属性：
- page：页面ID。页面ID得保持唯一
- open：打开页面
- close：关闭页面

使用TZUiSchedulerRegister来注册页面。如果注册的页面ID重复，则会用新的open和close函数覆盖旧的。

## 切换页面
使用TZUiSchedulerSwitch来切换页面。切换时会先关闭当前页面，然后再打开新页面。
