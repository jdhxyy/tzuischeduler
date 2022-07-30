// Copyright 2020-2020 The TZIOT Authors. All rights reserved.
// UI调度器
// Authors: jdh99 <jdh821@163.com>

#include "tzuischeduler.h"
#include "tzlist.h"
#include "tzmalloc.h"

#define TAG "tzuischeduler"
#define MALLOC_SIZE 4096

#pragma pack(1)

typedef struct {
    int page;
    TZEmptyFunc open;
    TZEmptyFunc close;
} tItem;

#pragma pack()

static int gMid = -1;
static intptr_t gList = 0;
static int gCurrentPage = TZ_UI_SCHEDULER_PAGE_INVALID;
static int gPrevPage = TZ_UI_SCHEDULER_PAGE_INVALID;

static TZListNode* getNode(int page);
static TZListNode* createNode(void);

// TZUiSchedulerLoad 模块载入
bool TZUiSchedulerLoad(void) {
    gMid = TZMallocRegister(0, TAG, MALLOC_SIZE);
    if (gMid == -1) {
        return false;
    }
    gList = TZListCreateList(gMid);
    if (gList == 0) {
        return false;
    }
    return true;
}

// TZUiSchedulerRegister 模块注册
bool TZUiSchedulerRegister(int page, TZEmptyFunc open, TZEmptyFunc close) {
    if (gMid == -1 || gList == 0 || page == TZ_UI_SCHEDULER_PAGE_INVALID || open == NULL || close == NULL) {
        return false;
    }

    tItem* item = NULL;
    TZListNode* node = getNode(page);
    if (node == NULL) {
        // 不存在,则新建
        node = createNode();
        if (node == NULL) {
            return false;
        }
    }
    item = (tItem*)node->Data;
    item->page = page;
    item->open = open;
    item->close = close;
    TZListAppend(gList, node);
    return true;
}

static TZListNode* getNode(int page) {
    TZListNode* node = TZListGetHeader(gList);
    tItem* item = NULL;
    for (;;) {
        if (node == NULL) {
            break;
        }
        item = (tItem*)node->Data;
        if (item->page == page) {
            return node;
        }
        node = node->Next;
    }
    return NULL;
}

static TZListNode* createNode(void) {
    TZListNode* node = TZListCreateNode(gList);
    if (node == NULL) {
        return NULL;
    }
    node->Data = TZMalloc(gMid, sizeof(tItem));
    if (node->Data == NULL) {
        TZFree(node);
        return NULL;
    }
    node->Size = sizeof(tItem);
    return node;
}

// TZUiSchedulerSwitch 切换页面
bool TZUiSchedulerSwitch(int page) {
    if (page == TZ_UI_SCHEDULER_PAGE_INVALID || gCurrentPage == page) {
        return false;
    }
    
    tItem* item = NULL;
    TZListNode* node = getNode(page);
    if (node == NULL) {
        return false;
    }

    if (gCurrentPage != TZ_UI_SCHEDULER_PAGE_INVALID) {
        // 当前页面有效,则需关闭当前页面
        TZListNode* currentNode = getNode(gCurrentPage);
        if (currentNode != NULL) {
            item = (tItem*)currentNode->Data;
            item->close();
        }
    }
    // 切换页面
    item = (tItem*)node->Data;
    item->open();
    gPrevPage = gCurrentPage;
    gCurrentPage = page;
    return true;
}

// TZUiSchedulerGetCurrentPage 获取当前页面
// 返回值为TZ_UI_SCHEDULER_PAGE_INVALID表示当前页面无效
int TZUiSchedulerGetCurrentPage(void) {
    return gCurrentPage;
}

// TZUiSchedulerGetPrevPage 获取上一页
int TZUiSchedulerGetPrevPage(void) {
    return gPrevPage;
}

// TZUiSchedulerSwitch 切换上一页面
bool TZUiSchedulerSwitchPrevPage(void) {
    return TZUiSchedulerSwitch(gPrevPage);
}
