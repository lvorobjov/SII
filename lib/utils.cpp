/**
 * @Author: Lev Vorobjev
 * @Date:   05.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: utils.cpp
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 11.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 */

#include "utils.h"

#include <stdlib.h>

void stack_push(stack_t* &stack, void* data) {
    stack_t* s;
    s = (stack_t*)malloc(sizeof(stack_t));
    s -> next = stack;
    s -> data = data;
    stack = s;
}

void* stack_pop(stack_t* &stack) {
    if (stack == NULL)
        return NULL;
    void* data = stack -> data;
    stack_t* s = stack -> next;
    free(stack);
    stack = s;
    return data;
}

void queue_enqueue(queue_t* &queue, void* data) {
    if (queue == NULL) {
        queue = (queue_t*)malloc(sizeof(queue_t));
        stack_t* stack = NULL;
        stack_push(stack, data);
        queue -> first = stack;
        queue -> lastptr = &(stack -> next);
    } else {
        stack_push(*(queue -> lastptr), data);
        queue -> lastptr = &((*(queue -> lastptr)) -> next);
    }
}

void* queue_dequeue(queue_t* &queue) {
    if (queue == NULL)
        return NULL;
    void* data = stack_pop(queue -> first);
    if (queue -> first == NULL) {
        free(queue);
        queue = NULL;
    }
    return data;
}

static
list_t* _list_add(list_t* list, void* data) {
    if (list == NULL) {
		list = (list_t*)malloc(sizeof(list_t));
		list -> data = data;
		list -> next = NULL;
	} else {
		while (list->next != NULL)
			list = list -> next;
		list = list -> next = _list_add(NULL, data);
	}
	return list;
}

list_t* list_add(list_t* &list, void* data) {
    list_t* ptr = _list_add(list, data);
    if (list == NULL)
        list = ptr;
    return ptr;
}

static
list_t* _list_append(list_t* list, list_t* othList) {
	if (list == NULL) {
		list = othList;
	} else {
		while (list->next != NULL)
			list = list -> next;
		list = list -> next = othList;
	}
	return list;
}

list_t* list_append(list_t* &list, list_t* othList) {
    list_t* ptr = _list_append(list, othList);
    if (list == NULL)
        list = ptr;
    return ptr;
}

static
void _list_free(list_t* list) {
	if (list -> next != NULL)
		_list_free(list -> next);
	free (list);
}

void list_free(list_t* &list) {
    if (list != NULL)
        _list_free(list);
    list = NULL;
}

static
list_t* _list_reverse(list_t* list, list_t* end) {
    if (list == end || list == NULL) {
        return list;
    } else if (list -> next == end || list -> next == NULL) {
        return list;
    } else {
        list_t* tmpList = _list_reverse(list -> next, end);
        list -> next -> next = list;
        list -> next = end;
        return tmpList;
    }
}

list_t* list_reverse(list_t* &list, list_t* end) {
    list = _list_reverse(list, end);
    return list;
}

#if 0
list_t* list_remove(list_t* &first, list_t* last) {
    if (first == last || first == NULL) {
        return NULL;
    } else {
        list_t* tmpList = first;
        while (first -> next != last) {
            first = first -> end;
        }
        first -> next = NULL;
        first = last;
        return tmpList;
    }
}
#else
list_t* list_remove(list_t* &list, void* data) {
    if (list == NULL) {
        return NULL;
    } else if (list -> data == data) {
        list_t* tmp = list;
        list = list -> next;
        tmp -> next = NULL;
        return tmp;
    } else {
        list_t* tmp = list;
        while (tmp -> next && tmp -> next -> data != data) {
            tmp = tmp -> next;
        }
        return list_remove(tmp -> next, data);
    }
}
#endif
