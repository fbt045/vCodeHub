/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/*
 * Simple generic single linked list implementation
 */

#include <stdio.h>
#include "list.h"

/*
 * Cast ptr of DATA object node to ptr of LIST node
 */
list_t *
list_d2l(void *object, size_t offset)
{
	if (object == NULL)
		return NULL;

	return (list_t *)((char *)object + offset);
}

/*
 * Cast ptr of LIST node to ptr of DATA object node
 */
void *
list_l2d(list_t *list)
{
	if (list == NULL)
		return NULL;

	return (void *)((char *)list - list->offset);
}

/*
 * Insert an object after the tail of list
 */
void
list_insert_tail(list_t **head, void *object, size_t offset)
{
	if (object == NULL)
		return;

	list_t *node = list_d2l(object, offset);
	LIST_INIT_NODE(node, offset);

	list_insert_node_tail(head, node);
}

/*
 * Insert an object before the head of list
 */
void
list_insert_head(list_t **head, void *object, size_t offset)
{
	if (object == NULL)
		return;

	list_t *node = list_d2l(object, offset);
	LIST_INIT_NODE(node, offset);

	list_insert_node_head(head, node);
}

/*
 * Insert a node after the tail of list
 */
void
list_insert_node_tail(list_t **head, list_t *node)
{
	list_t **indirect = head;
	while (*indirect != NULL)
		indirect = &(*indirect)->next;
	*indirect = node;
}

/*
 * Insert a node before the head of list
 */
void
list_insert_node_head(list_t **head, list_t *node)
{
	node->next = *head;
	*head = node;
}

/*
 * Insert node1 after node2
 *
 * NOTE: Both *head and node2 can be NULL
 *       a) if node2 is NULL    : list_insert_node_tail(head, node1)
 *       b) else
 *             node1->next = node2->next
 *             node2->next = node1
 */
void
list_insert_node_after(list_t **head, list_t *node1, list_t *node2)
{
	if (head == NULL || node1 == NULL)
		return;

	if (node2 == NULL) {
		list_insert_node_tail(head, node1);
		return;
	}

	node1->next = node2->next;
	node2->next = node1;
}

/*
 * Insert node1 before node2
 *
 * NOTE: Both *head and node2 can be NULL
 *       a) if node2 is NULL    : list_insert_node_tail(head, node1)
 *       b) else
 *              node1->next = node2
 *              if   *head is node2 : (new head) *head = node1
 *              else                : (prev of node2)->next = node1
 */
void
list_insert_node_before(list_t **head, list_t *node1, list_t *node2)
{
	if (head == NULL || node1 == NULL)
		return;

	if (node2 == NULL) {
		list_insert_node_tail(head, node1);
		return;
	}

	/* Get the prev node of node2 */
	list_t *prev = NULL;
	for (list_t *p = *head; p != node2; p = p->next)
		prev = p;

	/* Note *head = node1 if *head == node2 */
	list_t **indirect = (prev == NULL) ? head : &prev->next;
	*indirect = node1;

	node1->next = node2;
}

#ifdef _NOT_GOOD_TASTE
/*
 * Delete a node from list
 */
void
list_delete_node(list_t **head, list_t *node)
{
	if (head == NULL || *head == NULL || node == NULL)
		return;

	/* if (*head) == node, new head is node->next */
	if (*head == node) {
		*head = node->next;
		return;
	}

	/* else walk the list to find the prev node of entry we wanna remove */
	list_t *q = *head;
	for (list_t *p = *head; p != NULL; p = p->next) {
		if (p == node)
			break;
		q = p;
	}

	/* now remove the node by updating its prev node's next */
	q->next = node->next;
}
#else
/*
 * Delete a node from list
 */
void
list_delete_node(list_t **head, list_t *node)
{
	if (head == NULL || *head == NULL || node == NULL)
		return;

	/*
	 * The "indirect" pointer points to the address of the thing
	 * we will update
	 */
	list_t **indirect = head;

	/*
	 * Walk the list, looking for the thing that points to
	 * the entry we want to remove
	 */
	while (*indirect != node)
		indirect = &(*indirect)->next;

	/* .. and just remove it */
	*indirect = node->next;
}
#endif
