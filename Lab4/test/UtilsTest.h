// UtilsTest.h
// Testing list_t, stack_t

#include <cxxtest/TestSuite.h>
#include <utils.h>

#if 0
template <typename T>
typedef list_t<T> list_t;

template <typename T>
T list_get_data(list_t<T> list) {
	return (T) list->data;
}
#endif

#define LIST_ITEM(list) \
	*((int*)(list -> data))


class UtilsTestSuite : public CxxTest::TestSuite {
private:
	stack_t* stack = nullptr;
	queue_t* queue = nullptr;
	list_t* list = nullptr;
	int data= 1234, data2 = 5678, data3 = 1012;

public:
	void setUp() {
		list = nullptr;
	}

	void tearDown() {
		list_free(list);
	}

	void testStackPushPop() {
		stack_push(stack, &data);
		TS_ASSERT_DIFFERS(stack, nullptr);
		TS_ASSERT_EQUALS(stack -> data, &data);

		stack_t* s = stack;
		stack_push(stack, &data2);
		TS_ASSERT_EQUALS(stack -> next, s);
		TS_ASSERT_EQUALS(stack -> data, &data2);

		void* ptr = stack_pop(stack);
		TS_ASSERT_EQUALS(stack, s);
		TS_ASSERT_EQUALS(ptr, &data2);

		ptr = stack_pop(stack);
		TS_ASSERT_EQUALS(stack, nullptr);
		TS_ASSERT_EQUALS(ptr, &data);
	}

	void testStackNull() {
		void* ptr = stack_pop(stack);
		TS_ASSERT_EQUALS(ptr, nullptr);
	}

	void testQueue() {
		TS_TRACE("Enqueue one item");
		queue_enqueue(queue, &data);
		TS_ASSERT_DIFFERS(queue, nullptr);
		TS_ASSERT_EQUALS(queue -> first -> data, &data);

		TS_TRACE("Enqueue two more items");
		queue_enqueue(queue, &data2);
		queue_enqueue(queue, &data3);
		TS_ASSERT_EQUALS(queue -> first -> data, &data);

		TS_TRACE("Dequeue first item");
		void* ptr = queue_dequeue(queue);
		TS_ASSERT_EQUALS(ptr, &data);

		TS_TRACE("Dequeue second item");
		ptr = queue_dequeue(queue);
		TS_ASSERT_EQUALS(ptr, &data2);

		TS_TRACE("Dequeue third item");
		ptr = queue_dequeue(queue);
		TS_ASSERT_EQUALS(ptr, &data3);
		TS_ASSERT_EQUALS(queue, nullptr);

		TS_TRACE("Dequeue from empty queue");
		ptr = queue_dequeue(queue);
		TS_ASSERT_EQUALS(ptr, nullptr);
	}

	void testListAdd() {
		TS_TRACE("Adding first list item");
		list_t* ptr = list_add(list, &data);
		TS_ASSERT_DIFFERS(list, nullptr);
		TS_ASSERT_EQUALS(list -> data, &data);
		TS_ASSERT_EQUALS(list -> next, nullptr);
		TS_ASSERT_EQUALS(ptr, list);

		list_t* l = list;
		TS_TRACE("Adding second list item");
		ptr = list_add(list, &data2);
		TS_ASSERT_EQUALS(list, l);
		TS_ASSERT_EQUALS(list -> data, &data);
		TS_ASSERT_DIFFERS(list -> next, nullptr);
		TS_ASSERT_EQUALS(list -> next, ptr);
		TS_ASSERT_EQUALS(ptr -> data, &data2);
		TS_ASSERT_EQUALS(ptr -> next, nullptr);

		list_t* ll = list->next;
		TS_TRACE("Adding third list item");
		ptr = list_add(list, &data3);
		TS_ASSERT_EQUALS(list, l);
		TS_ASSERT_DIFFERS(ll -> next, nullptr)
		TS_ASSERT_EQUALS(ll -> next, ptr);
		TS_ASSERT_EQUALS(ptr -> data, &data3);
		TS_ASSERT_EQUALS(ptr -> next, nullptr);
	}

	void testFreeList() {
		// Empty list
		TS_TRACE("Free empty list");
		list_free(list);
		TS_ASSERT_EQUALS(list, nullptr);

		// One item list
		TS_TRACE("Adding one item");
		list_add(list, &data);
		TS_ASSERT_DIFFERS(list, nullptr);
		TS_TRACE("Free one item list");
		list_free(list);
		TS_ASSERT_EQUALS(list, nullptr);

		// Two item list
		TS_TRACE("Adding two items");
		list_add(list, &data);
		list_add(list, &data2);
		TS_ASSERT_DIFFERS(list, nullptr);
		TS_ASSERT_DIFFERS(list -> next, nullptr);
		TS_TRACE("Free two item list");
		list_free(list);
		TS_ASSERT_EQUALS(list, nullptr);

		// Three item  list
		TS_TRACE("Adding three item");
		list_add(list, &data);
		list_add(list, &data2);
		list_add(list, &data3);
		TS_ASSERT_DIFFERS(list, nullptr);
		TS_ASSERT_DIFFERS(list -> next, nullptr);
		TS_ASSERT_DIFFERS(list -> next -> next, nullptr);

		TS_TRACE("Free three item list");
		list_free(list);
		TS_ASSERT_EQUALS(list, nullptr);
	}

	void testListAppend() {
		list_t *oth1 = nullptr, *oth2 = nullptr, *oth3 = nullptr;
		list_add(oth1, &data);
		list_add(oth2, &data2);

		// nullptr + nullptr
		TS_TRACE("Append empty to empty list");
		list_append(list, oth3);
		TS_ASSERT_EQUALS(list, nullptr);

		// nullptr + oth1
		TS_TRACE("Append one item to empty list");
		list_append(list, oth1);
		TS_ASSERT_EQUALS(list, oth1);

		// oth1 + nullptr
		TS_TRACE("Append empty list to one item list");
		list_append(list, oth3);
		TS_ASSERT_EQUALS(list, oth1);
		TS_ASSERT_EQUALS(list -> next, nullptr);

		// oth1 + oth2
		TS_TRACE("Append one item list to one item list");
		list_append(list, oth2);
		TS_ASSERT_EQUALS(list, oth1);
		TS_ASSERT_EQUALS(list -> next, oth2);

		// oth1,oth2 + nullptr
		TS_TRACE("Append empty list to two item list");
		list_append(list, oth3);
		TS_ASSERT_EQUALS(list, oth1);
		TS_ASSERT_EQUALS(list -> next, oth2);
		TS_ASSERT_EQUALS(list -> next -> next, nullptr);

		// oth1,oth2 + oth3
		TS_TRACE("Adding one element to oth3");
		list_add(oth3, &data3);
		TS_ASSERT_DIFFERS(oth3, nullptr);
		TS_TRACE("Append one item list to two item list");
		list_append(list, oth3);
		TS_ASSERT_EQUALS(list, oth1);
		TS_ASSERT_EQUALS(list -> next, oth2);
		TS_ASSERT_EQUALS(list -> next -> next, oth3);
	}

	void testListReverse() {
		// Empty list
		TS_TRACE("Reverse empty list");
		list_reverse(list, nullptr);
		TS_ASSERT_EQUALS(list, nullptr);

		// One item list
		TS_TRACE("Adding first item");
		list_add(list, &data);
		TS_ASSERT_DIFFERS(list, nullptr);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 1234);
		TS_TRACE("Reverse one item list");
		list_reverse(list, nullptr);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 1234);

		// Two item list
		TS_TRACE("Adding second item");
		list_add(list, &data2);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next), 5678);
		TS_TRACE("Reverse two item list");
		list_reverse(list, nullptr);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 5678);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next), 1234);

		// Three item list
		TS_TRACE("Adding third item");
		list_add(list, &data3);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next -> next), 1012);
		TS_TRACE("Reverse three item list");
		list_reverse(list, nullptr);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 1012);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next), 1234);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next -> next), 5678);
		TS_TRACE("Reverse firse to second items");
		list_reverse(list, list -> next -> next);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 1234);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next), 1012);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next -> next), 5678);
	}

	void testListRemove() {
		// Remove from empty list
		TS_TRACE("Remove from empty list");
		list_t* l = list_remove(list, &data);
		TS_ASSERT_EQUALS(list, nullptr);
		TS_ASSERT_EQUALS(l, nullptr);

		// One item list
		TS_TRACE("Adding one item");
		list_add(list, &data);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 1234);

		// Remove missing item
		TS_TRACE("Remove mising item");
		l = list_remove(list, &data2);
		TS_ASSERT_EQUALS(l, nullptr);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 1234);

		TS_TRACE("Adding two more intems");
		list_add(list, &data2);
		list_add(list, &data3);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next), 5678);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next -> next), 1012);

		// Remove middle item
		TS_TRACE("Remove middle item");
		list_remove(list, &data2);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 1234);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next), 1012);

		// Remove last item
		TS_TRACE("Remove last item");
		list_remove(list, &data3);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 1234);
		TS_ASSERT_EQUALS(list -> next, nullptr);

		TS_TRACE("Adding one item");
		list_add(list, &data2);
		TS_ASSERT_EQUALS(LIST_ITEM(list -> next), 5678);

		// Remove first item
		TS_TRACE("Remove first item");
		list_remove(list, &data);
		TS_ASSERT_EQUALS(LIST_ITEM(list), 5678);
		TS_ASSERT_EQUALS(list -> next, nullptr);

		// Remove single item
		TS_TRACE("Remove single item");
		list_remove(list, &data2);
		TS_ASSERT_EQUALS(list, nullptr);
	}
};
