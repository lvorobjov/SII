/**
 * @Author: Lev Vorobjev
 * @Date:   05.04.2018
 * @Email:  lev.vorobjev@rambler.ru
 * @Filename: utils.h
 * @Last modified by:   Lev Vorobjev
 * @Last modified time: 11.04.2018
 * @License: MIT
 * @Copyright: Copyright (c) 2018 Lev Vorobjev
 * This is free software that provided "as-is" and without 
 * any warranty to the extent permitted by applicable law.
 */

#ifndef UTILS_H
#define UTILS_H

typedef struct _stack_t {
    void* data;
    struct _stack_t* next;
} stack_t;

void stack_push(stack_t* &stack, void* data);
void* stack_pop(stack_t* &stack);

typedef struct _queue_t {
    stack_t* first;
    stack_t** lastptr;
} queue_t;

void queue_enqueue(queue_t* &queue, void* data);
void* queue_dequeue(queue_t* &queue);

typedef struct _list_t {
    void* data;
    struct _list_t* next;
} list_t;

/**
 * Добаление элемента в конец списка
 * @param list список, в конец которого добавляется элемент
 * @data данные добавляемого эоемента
 * @return указатель на добавленный элемент
 */
list_t* list_add(list_t* &list, void* data);

/**
 * Соединение двух списков
 * @param list список, в конец которого добавляется другой список
 * @param othList добавляемый список
 * @deprecated указатель на лемент списка, к которому
 *  было присоединено начало другого списка при list != NULL
 * @return указатель на добавленный список
 */
list_t* list_append(list_t* &list, list_t* othList);

/**
 * Удаление всех элементов списка
 * @param list удаляемый список
 */
void list_free(list_t* &list);

/**
 * Инвертирование элементов списка
 * @param list начало инвертируемого фрагмента
 * @param end конец инвертируемого фрагмента (не инвертируемый)
 * @return указатель на начало инвертированного фрагмента
 */
list_t* list_reverse(list_t* &list, list_t* end);

/**
 * Удаление элемента списка с заданным значением
 * @param list список, в котором удаляется элемент
 * @data содержание удаляемого элемента
 * @return удаленный элемент
 */
list_t* list_remove(list_t* &list, void* data);

#endif
