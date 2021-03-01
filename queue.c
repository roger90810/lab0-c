#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    list_ele_t *p = q->head;
    while (p) {
        list_ele_t *n = p;
        p = p->next;
        free(n->value);
        free(n);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL)
        return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    int n = strlen(s);
    newh->value = malloc((n + 1) * sizeof(char));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, n);
    newh->value[n] = '\0';
    newh->next = q->head;
    if (q->size == 0)
        q->tail = newh;
    q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL)
        return false;
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (newt == NULL)
        return false;
    int n = strlen(s);
    newt->value = malloc((n + 1) * sizeof(char));
    if (newt->value == NULL) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, n);
    newt->value[n] = '\0';
    newt->next = NULL;
    if (q->size == 0)
        q->head = newt;
    else
        q->tail->next = newt;
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->head == NULL)
        return false;

    if (sp != NULL) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    free(q->head->value);
    list_ele_t *t = q->head;
    q->head = q->head->next;
    free(t);
    t = NULL;
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL || q->head == NULL)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL)
        return;
    list_ele_t *n = NULL;
    list_ele_t *t = NULL;
    list_ele_t *p = q->head;
    q->tail = q->head;
    while (p) {
        t = p;
        p = p->next;
        t->next = n;
        n = t;
    }
    q->head = t;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q == NULL || q->size <= 1)
        return;
    q->head = list_split(q->head);
    list_ele_t *t = q->head;
    while (t->next)
        t = t->next;
    q->tail = t;
}

list_ele_t *list_split(list_ele_t *head)
{
    if (head == NULL || head->next == NULL)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = list_split(head);
    list_ele_t *l2 = list_split(fast);

    return list_merge(l1, l2);
}

list_ele_t *list_merge(list_ele_t *l1, list_ele_t *l2)
{
    if (l1 == NULL)
        return l2;
    if (l2 == NULL)
        return l1;

    if (strcmp(l1->value, l2->value) < 0) {
        l1->next = list_merge(l1->next, l2);
        return l1;
    } else {
        l2->next = list_merge(l1, l2->next);
        return l2;
    }
}