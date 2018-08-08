//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                Open Source C Container Library like STL in C++
//
//               Copyright (C) 2018, Kim Bomm, all rights reserved.
//
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
#pragma once
#if !defined(_OPENCSTL_LIST_H)
#define _OPENCSTL_LIST_H
#include"types.h"
#include"error.h"
#define cstl_list(TYPE)		__cstl_list(sizeof(TYPE),#TYPE)
#define NTAIL(N)	(N==-1?-2:N)
void* __cstl_list(size_t type_size,char* type) {
	size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
	void* ptr = (char*)malloc(header_sz + sizeof(size_t)) + header_sz;
	void** container = &ptr;
	OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_LIST;
	OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
	OPENCSTL_NIDX(container, NIDX_TSIZE) = type_size;
	OPENCSTL_NIDX(container, -4) = (size_t)type;
	OPENCSTL_NIDX(container, -2) = 0;	//tail
	OPENCSTL_NIDX(container, -1) = 0;	//Not reserved
	OPENCSTL_NIDX(container, 0) = 0;	//head
	return ptr;
}
void* __cstl_list_node(size_t type_size) {
	//[node type][prev][next] ¢Ù [data]
	size_t header_sz = sizeof(void*) * 3;
	size_t node_sz = type_size + header_sz;
	void* ptr = (char*)calloc(node_sz, 1) + header_sz;
	void** node = &ptr;
	OPENCSTL_NIDX(node, -3) = OPENCSTL_LIST;
	return ptr;
}
void __cstl_list_push_back_front(void** container, void* value, int ntail, int nhead) {
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	void** tail = (void**)&OPENCSTL_NIDX(container, NTAIL(ntail));	    //-1 , 0
	void** head = (void**)&OPENCSTL_NIDX(container, nhead);   //0  , -1
	void* n = __cstl_list_node(type_size);
	char* type = (char*)OPENCSTL_NIDX(container, -4);
	float valuef = 0.0F;
	if (strcmp(type, "float") == 0) {
		valuef = (float)*(double*)value;
		value = &valuef;
	}
	memcpy(n, value, type_size);
	if (*head == NULL && *tail == NULL) {
		*head = *tail = n;
	} else {
		OPENCSTL_NIDX(tail, -(ntail + 2)) = (size_t)n;
		OPENCSTL_NIDX(&n, -(nhead + 2)) = (size_t)*tail;
		*tail = n;
	}
}
void __cstl_list_pop_back_front(void** container, int ntail, int nhead) {
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	void** tail = (void**)&OPENCSTL_NIDX(container, NTAIL(ntail));	//-1 , 0
	void** head = (void**)&OPENCSTL_NIDX(container, nhead);	//0  , -1
	if (*head == NULL || *tail == NULL) {
		cstl_error("No elements in cstl_list");
	}
	if (*head == *tail) {
		free(&OPENCSTL_NIDX(tail, -3));	//fix
		*head = *tail = 0;
	} else {
		*tail = (void*)OPENCSTL_NIDX(tail, -(nhead + 2));
		void* fb = (void*)OPENCSTL_NIDX(tail, -(ntail + 2));
		free(&OPENCSTL_NIDX(&fb, -3));
		OPENCSTL_NIDX(tail, -(ntail + 2)) = 0;
	}
}
void* __cstl_list_next_prev(void* it, int n) {
	//next(-1), prev(-2)
	return (void*)OPENCSTL_NIDX(&it, n);
}
size_t __cstl_list_size(void** container) {
	size_t sz = 0;
	for (void** it = (void**)OPENCSTL_NIDX(container, 0); it != NULL; it=(void**)__cstl_list_next_prev(it,-1)) {
		sz++;
	}
	return sz;
}

void __cstl_list_insert(void** container, void** iter, size_t N, void* value) {
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	void** tail = (void**)&OPENCSTL_NIDX(container, -2);
	void** head = (void**)&OPENCSTL_NIDX(container, 0);
	char* type = (char*)OPENCSTL_NIDX(container, -4);
	float valuef = 0.0F;
	if (strcmp(type, "float") == 0) {
		valuef = (float)*(double*)value;
		value = &valuef;
	}
	void* nhead = __cstl_list_node(type_size); memcpy(nhead, value, type_size);
	void* ntail = nhead;
	for (size_t i = 1; i < N; i++) {
		void* n= __cstl_list_node(type_size); memcpy(n, value, type_size);
		OPENCSTL_NIDX(&n, -2) = (size_t)ntail;	//n->prev=tail
		OPENCSTL_NIDX(&ntail, -1) = (size_t)n;	//tail->next=n;
		ntail = n;
	}
	OPENCSTL_NIDX(&ntail, -1) = (size_t)*iter;			//n->next=iter
	if (*head == NULL && *tail == NULL) {
		*head = nhead;
		*tail = ntail;
	} else if (*iter != NULL) {
		if (OPENCSTL_NIDX(iter, -2) != 0) {
			OPENCSTL_NIDX(&(OPENCSTL_NIDX(iter, -2)), -1) = (size_t)nhead;	//iter->prev->next=n;
		}
		OPENCSTL_NIDX(&nhead, -2) = OPENCSTL_NIDX(iter, -2);	//n->prev=iter->prev
		OPENCSTL_NIDX(iter, -2) = (size_t)ntail;		//iter->prev=n;
		if (*iter == *head) {
			*head = nhead;
		}
	} else {
		OPENCSTL_NIDX(tail, -1) = (size_t)nhead;		//tail->next=n;
		OPENCSTL_NIDX(&nhead, -2) = (size_t)*tail;		//n->prev=tail;
		*tail = ntail;
	}
}
void __cstl_list_erase(void** container, void** iter_begin, void** iter_end) {
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	void** tail = (void**)&OPENCSTL_NIDX(container, -2);
	void** head = (void**)&OPENCSTL_NIDX(container, 0);
	if (*iter_begin == NULL) {
		cstl_error("iter_begin could not be NULL");
	}
	if (*iter_begin == *head) {
		*head = *iter_end;
		if (*head != NULL) {
			OPENCSTL_NIDX(head, -2) = 0;
		}
	} else {
		OPENCSTL_NIDX(&OPENCSTL_NIDX(iter_begin, -2), -1) = (size_t)*iter_end;	//begin->prev->next=end;
		if (*iter_end != NULL) {
			OPENCSTL_NIDX(iter_end, -2) = OPENCSTL_NIDX(iter_begin, -2);			//end->prev=begin->prev
		}
	}
	void* it = *iter_begin;
	while (it != *iter_end) {
		void* tmp = (void*)OPENCSTL_NIDX(&it, -1);
		free(&OPENCSTL_NIDX(&it, -3));
		it = tmp;
	}
	if (*iter_end == NULL) {
		*tail = 0;
	}
}
void* __cstl_list_begin(void** container) {
	return (void*)OPENCSTL_NIDX(container, 0);
}
void* __cstl_list_end_rend(void** container) {
	return NULL;
}
void* __cstl_list_rbegin(void** container) {
	return (void*)OPENCSTL_NIDX(container, -2);
}
void __cstl_list_clear(void** container) {
	void** tail = (void**)&OPENCSTL_NIDX(container, -2);
	void** head = (void**)&OPENCSTL_NIDX(container, 0);

	void* it = *head;
	while (it != NULL) {
		void* tmp = (void*)OPENCSTL_NIDX(&it, -1);
		free(&OPENCSTL_NIDX(&it, -3));
		it = tmp;
	}
	*head = *tail = NULL;
}
void __cstl_list_free(void** container) {
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	__cstl_list_clear(container);
	free((char*)(*container) - header_sz);
	*container = NULL;
}
void* __cstl_list_find(void** container, void** iter_begin,void* value) {
	size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
	size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
	void** tail = (void**)&OPENCSTL_NIDX(container, -2);
	void** head = (void**)&OPENCSTL_NIDX(container, 0);
	char* type = (char*)OPENCSTL_NIDX(container, -4);
	float valuef = 0.0F;
	if (strcmp(type, "float") == 0) {
		valuef = (float)*(double*)value;
		value = &valuef;
	}
	void* it = *iter_begin;
	while (it != NULL) {
		if (memcmp(it, value, type_size) == 0) {
			return it;
		}
		it = (void*)OPENCSTL_NIDX(&it, -1);
	}
	return NULL;
}
#endif