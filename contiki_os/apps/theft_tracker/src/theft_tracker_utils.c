/*
********************************************************************************************
+                                                                                          +
+    Author :                  Giorgio Cozza                                               +
+    File :                    theft_tracker_utils.c                                       +
+    Date :                    07/07/21                                                    +  
+    Version :                 1.0                                                         +
+    Desc :                    Utilities for Theft Tracker mote implementation             +
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

#include "include/theft_tracker_utils.h"
#include <stdio.h>
#include <stdlib.h>


extern int get_distance(const uint32_t lst_dist, const int cc2420_rssi){

	int rssi = cc2420_rssi + CC2420_RSSI_OFFSET;

	
	if ( rssi >= -16 ){ return lst_dist + RANGE_4; }
	else if (rssi < -16 && rssi >= -23) { return lst_dist + RANGE_8; }
	else if (rssi < -23 && rssi >= -30) { return lst_dist + RANGE_12; }
	else if (rssi < -30 && rssi >= -37) { return lst_dist + RANGE_16; }
	else if (rssi < -37 && rssi >= -44) { return lst_dist + RANGE_20; }
	else if (rssi < -44 && rssi >= -50) { return lst_dist + RANGE_24; }
	else if (rssi < -50 && rssi >= -57) { return lst_dist + RANGE_28; }
	else if (rssi < -57 && rssi >= -64) { return lst_dist + RANGE_32; }
	else if (rssi < -64 && rssi >= -71) { return lst_dist + RANGE_36; }
	else if (rssi < -71 && rssi >= -78) { return lst_dist + RANGE_40; }
	else if (rssi < -78 && rssi >= -84) { return lst_dist + RANGE_44; }
	else if (rssi < -84 && rssi >= -91) { return lst_dist + RANGE_48; }
	else if (rssi < -91) { return lst_dist + RANGE_50; }
	else { return -1; }
}


extern void init_array(unsigned short * array, unsigned int size){

    int i = 0;
    // 0 value in array corresponds to empty slot
    for (i = 0; i < size; i++){
        array[i] = 0;
    }
}


extern unsigned int array_len(unsigned short * array, unsigned int size){

    unsigned int i = 0;
    // 0 value in array corresponds to empty slot
    for (i = 0; i < size; i++){
        if (array[i] == 0){
            if (i == 0)
                return 0;
            else
                return ++i;
        }
    }
    return ++i;
}


extern int add_array_item(unsigned short *array, unsigned int size, unsigned short item){
    int i = 0;
    // 0 value in array corresponds to empty slot
    for (i = 0; i < size; i++){
        if(array[i] == 0){
            array[i] = item;
            return i;
        }
    }
    return -1;
}


extern int remove_array_item(unsigned short * array, unsigned int size, unsigned int item_idx){

    if (item_idx < size){
        array[item_idx] = 0;
        return 1;
    }
    return 0;
}


extern int search_array_item(unsigned short * array, unsigned int size, unsigned short item){

    int i = 0;
    // 0 value in array corresponds to empty slot
    for (i = 0; i < size; i++){
        if(array[i] == item){
            return i;
        }
    }
    return -1;

}
