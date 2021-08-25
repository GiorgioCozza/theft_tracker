
/*
********************************************************************************************
+                                                                                          +
+    Author :                  Giorgio Cozza                                               +
+    File :                    theft_tracker_mote.h                                        +
+    Date :                    07/07/21                                                    +  
+    Version :                 1.0                                                         +
+    Desc :                    Header file for car antitheft system                        +
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


#ifndef THEFT_TRACKER_MOTE_H
#define THEFT_TRACKER_MOTE_H

#define MAX_UINT32_T			4294967295

#define HEADER 					"TT_MSG"
#define HEADER_SIZE  			sizeof(HEADER)

#define VEHICLE_ID_NUMBER       "ZAR94000007043651"
#define VIN_SIZE                sizeof(VEHICLE_ID_NUMBER)

#define SESSION_TTL				20			//[sec]
#define SEND_INTERVAL			1 			//[sec]
#define MAX_ALERT_RETX			10
#define MAX_HOP_NUM				20

#define ACCEPTED_ALERT_INTERVAL 60          //[sec]
#define ACCEPTED_ALERT_DISTANCE 200			//[meter]

#define CC2420_RSSI_OFFSET		-45			//[dBm]

#define TRUE 					1
#define FALSE					0

#include "contiki.h"
#include "lib/list.h"



enum {

	/* TheftTracker Status */
	ANTITHEFT_MODE			= 0,
	THEFT_SHOUTING_MODE,	
	SLEEP_MODE,			
};


enum {

	/* TheftTracker MSG types */
	ALERT_MSG_TYPE = 0,
	TIMESYNC_MSG_TYPE,


};

enum {

	/* Distance interval */
	RANGE_4 = 2,
	RANGE_8 = 6,
	RANGE_12 = 10,
	RANGE_16 = 14,
	RANGE_20 = 18,
	RANGE_24 = 22,
	RANGE_28 = 26,
	RANGE_32 = 30,
	RANGE_36 = 34,
	RANGE_40 = 38,
	RANGE_44 = 42,
	RANGE_48 = 46,
	RANGE_50 = 50,
} ;


struct tt_hdr_msg {

	unsigned short node_id;
	char	 type;
};


struct tt_alert_msg {

	struct tt_hdr_msg hdr;		            // message header
	unsigned short first_node_id;
	unsigned long event_clock_time;
	unsigned long orig_clock_time;
	uint16_t hop_count;
	uint32_t last_distance;
	unsigned short hop_list[MAX_HOP_NUM];
	char vin[VIN_SIZE];				                //ISO 3779 Vehicle Identification Number

};
 

struct tt_alert_session {

	unsigned short first_node_id;
	unsigned long event_clock_time;
	unsigned long orig_clock_time;
	unsigned int n_alert_tx;
	uint16_t hop_count;
	uint32_t last_distance;
	unsigned short hop_list[MAX_HOP_NUM];
	short is_expired;
	char vin[VIN_SIZE];				                //ISO 3779 Vehicle Identification Number

};




static void abc_recv(struct abc_conn *abc_c);



#endif