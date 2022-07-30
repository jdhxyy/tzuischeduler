// Copyright 2020-2020 The TZIOT Authors. All rights reserved.
// UI������
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

// TZUiSchedulerLoad ģ������
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

// TZUiSchedulerRegister ģ��ע��
bool TZUiSchedulerRegister(int page, TZEmptyFunc open, TZEmptyFunc close) {
    if (gMid == -1 || gList == 0 || page == TZ_UI_SCHEDULER_PAGE_INVALID || open == NULL || close == NULL) {
        return false;
    }

    tItem* item = NULL;
    TZListNode* node = getNode(page);
    if (node == NULL) {
        // ������,���½�
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

// TZUiSchedulerSwitch �л�ҳ��
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
        // ��ǰҳ����Ч,����رյ�ǰҳ��
        TZListNode* currentNode = getNode(gCurrentPage);
        if (currentNode != NULL) {
            item = (tItem*)currentNode->Data;
            item->close();
        }
    }
    // �л�ҳ��
    item = (tItem*)node->Data;
    item->open();
    gPrevPage = gCurrentPage;
    gCurrentPage = page;
    return true;
}

// TZUiSchedulerGetCurrentPage ��ȡ��ǰҳ��
// ����ֵΪTZ_UI_SCHEDULER_PAGE_INVALID��ʾ��ǰҳ����Ч
int TZUiSchedulerGetCurrentPage(void) {
    return gCurrentPage;
}

// TZUiSchedulerGetPrevPage ��ȡ��һҳ
int TZUiSchedulerGetPrevPage(void) {
    return gPrevPage;
}

// TZUiSchedulerSwitch �л���һҳ��
bool TZUiSchedulerSwitchPrevPage(void) {
    return TZUiSchedulerSwitch(gPrevPage);
}
