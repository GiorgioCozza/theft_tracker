
/*
********************************************************************************************
+                                                                                          +
+    Author :                  Giorgio Cozza                                               +
+    File :                    theft_tracker_utils.h                                       +
+    Date :                    07/07/21                                                    +  
+    Version :                 1.0                                                         +
+    Desc :                    Header utils for Theft Tracker mote                         +
+                                                                                          +
********************************************************************************************

	This is a free software released into public domain. Anyone is free to copy,
	modify, publish, use, compile or distribute this software, either in source
	code form or as compiled binary
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.

********************************************************************************************
*/

#ifndef THEFT_TRACKER_UTILS_H
#define THEFT_TRACKER_UTILS_H


#include "include/theft_tracker.h"

extern int get_distance(const uint32_t lst_dist, const int rssi);


extern void init_array(unsigned short * array, unsigned int size);


extern unsigned int array_len(unsigned short * array, unsigned int size);


extern int add_array_item(unsigned short * array, unsigned int size, unsigned short item);


extern int remove_array_item(unsigned short * array, unsigned int size, unsigned int item_idx);


extern int search_array_item(unsigned short * array, unsigned int size, unsigned short item);

#endif