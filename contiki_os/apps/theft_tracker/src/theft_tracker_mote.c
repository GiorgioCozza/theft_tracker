
/*
********************************************************************************************
+                                                                                          +
+    Author :                  Giorgio Cozza                                               +
+    File :                    theft_tracker_mote.c                                        +
+    Date :                    07/07/21                                                    +  
+    Version :                 1.0                                                         +
+    Desc :                    Component for car antitheft system                          +
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

#include "contiki.h"
#include "cc2420.h"
#include "net/rime/rime.h"
#include "sys/log.h"
#include "sys/node-id.h"
#include "sys/etimer.h"
#include "sys/stimer.h"
#include "dev/button-sensor.h"

#include "include/theft_tracker.h"
#include "include/theft_tracker_utils.h"

#include <stdio.h>


#define CHANNEL            129

static int tt_status = ANTITHEFT_MODE;

static int radio_free = TRUE;
// interval between messages
static struct etimer send_et;
// duration of alert session
static struct stimer session_st;
// variable to store system time in clock seconds
static unsigned long clock_secs;


static unsigned long event_clock_secs;

// custom events: receive of alert, switch to antitheft mode, the vehicle is stolen
static process_event_t alert_recv_event;
static process_event_t theft_event;

static struct tt_alert_msg new_amsg;

// current alert session
static struct tt_alert_session cur_session;
static unsigned int is_stolen = FALSE;

// broadcast connection and receiver callback
static const struct abc_callbacks abc_cb = {abc_recv};

static struct abc_conn abc_c;


PROCESS(guard,"guard");
PROCESS(shouter,"shouter");

AUTOSTART_PROCESSES(&guard, &shouter);


static void abc_recv(struct abc_conn *abc_c) {

    if(radio_free == TRUE){

        radio_free = FALSE;
        struct tt_hdr_msg *hdr = NULL;
        struct tt_alert_msg *amsg = NULL;

        /* CHECK VALIDITY OF THE PACKET */
        hdr = (struct tt_hdr_msg *) packetbuf_dataptr();


        if (hdr != NULL) {

            if (hdr->type == ALERT_MSG_TYPE) {

                amsg = (struct tt_alert_msg *) packetbuf_dataptr();

                if (is_stolen == FALSE) {

                    /*
                     * Avoid closed-loop messages
                     * Avoid to exceed maximum path length
                     * Accept only updates in session if mote is approaching the stolen vehicle
                     * */
                    if (search_array_item(amsg->hop_list, MAX_HOP_NUM, node_id) < 0 &&
                        array_len(amsg->hop_list, MAX_HOP_NUM) < MAX_HOP_NUM &&
                        cur_session.hop_count > amsg->hop_count || amsg->hop_count == 0) {

                        int distance = get_distance((const uint32_t) amsg->last_distance, (const int) cc2420_last_rssi);
                        event_clock_secs = amsg->event_clock_time;
                        clock_secs = clock_seconds();

                        // If too old alert received or too far from origin do not retransmit
                        if (clock_secs - amsg->event_clock_time < ACCEPTED_ALERT_INTERVAL ||
                        distance >= 0 && distance < ACCEPTED_ALERT_DISTANCE) {

                            #ifdef DEBUG
                            printf("\n Stolen vehicle detected: \n\t- sender_id: %d \n\t- distance (approx): %d [m] \n\t- first send time: %d [sec] \n\t- system time: %d [sec]",
                                   amsg->hdr.node_id, distance,
                                   amsg->event_clock_time,
                                   clock_secs);
                            #endif

                            cur_session.last_distance = distance;
                            cur_session.hop_count = amsg->hop_count + 1;
                            cur_session.first_node_id = amsg->first_node_id;
                            cur_session.event_clock_time = amsg->event_clock_time;
                            cur_session.orig_clock_time = amsg->orig_clock_time;
                            cur_session.n_alert_tx = 0;
                            strncpy(cur_session.vin, (const char *) (amsg->vin), VIN_SIZE);
                            memcpy(cur_session.hop_list, amsg->hop_list, MAX_HOP_NUM);

                            int idx = -1;
                            if(search_array_item(cur_session.hop_list, MAX_HOP_NUM, node_id) < 0){
                                idx = add_array_item(cur_session.hop_list, MAX_HOP_NUM, node_id);
                            }//    cur_session.is_expired = FALSE;

                            #ifdef DEBUG
                            printf("\n Node added to the route list at position %d", idx);
                            #endif

                            new_amsg.hdr.node_id = node_id;
                            new_amsg.hdr.type = ALERT_MSG_TYPE;
                            new_amsg.first_node_id = amsg->first_node_id;
                            new_amsg.hop_count = cur_session.hop_count;
                            new_amsg.event_clock_time = amsg->event_clock_time;
                            new_amsg.orig_clock_time = amsg->orig_clock_time;
                            strncpy(new_amsg.vin, (const char *) (amsg->vin), VIN_SIZE);
                            memcpy(new_amsg.hop_list, cur_session.hop_list, MAX_HOP_NUM);
                            new_amsg.last_distance = distance;


                            stimer_set(&session_st, SESSION_TTL);

                            #ifdef DEBUG
                            printf("\nNew alert session created, session timer set.");
                            #endif

                            process_post(&shouter, alert_recv_event, NULL);


                        }

                    }
                }

            }

        }
    }
    radio_free = TRUE;
}


PROCESS_THREAD(guard, ev, data){

    PROCESS_BEGIN();

    /* Sensor activation */
    SENSORS_ACTIVATE(button_sensor);

    clock_set_seconds(0);

    //key_recv_event = process_alloc_event();
    theft_event = process_alloc_event();

    #ifdef DEBUG
    printf("\nTheft Tracker System activated");
    #endif

    while(1){

        //PROCESS_WAIT_EVENT_UNTIL(ev == key_recv_event);

        if (tt_status == ANTITHEFT_MODE){

            #ifdef DEBUG
            printf("\n Monitoring the status of the vehicle...");
            #endif

            PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

            is_stolen = TRUE;

            process_post(&shouter, theft_event, NULL);


            #ifdef DEBUG
            printf("\n ALERT: Vehicle movement detected at time: %d", event_clock_secs);
            #endif

    }

}

PROCESS_END();

}


PROCESS_THREAD(shouter, ev, data){

    PROCESS_BEGIN();

    //PROCESS_EXITHANDLER (abc_close(&abc_c);)

    alert_recv_event = process_alloc_event();

    cur_session.first_node_id = 0;
    cur_session.n_alert_tx = 0;
    cur_session.last_distance = MAX_UINT32_T;
    cur_session.hop_count = MAX_UINT32_T;
    cur_session.is_expired = FALSE;
    cur_session.event_clock_time = 0;
    cur_session.orig_clock_time = 0;
    init_array(cur_session.hop_list, MAX_HOP_NUM);

    new_amsg.hdr.node_id = 0;
    new_amsg.hdr.type = 0;
    new_amsg.first_node_id = 0;
    new_amsg.hop_count = 0;
    new_amsg.event_clock_time = 0;
    new_amsg.orig_clock_time = 0;
    new_amsg.last_distance = 0;
    init_array(new_amsg.hop_list, MAX_HOP_NUM);

    abc_open(&abc_c, CHANNEL, &abc_cb);


    #ifdef DEBUG
    printf("\nTheft Tracker Shouter activated");
    #endif


    while(1){

        PROCESS_WAIT_EVENT_UNTIL(ev == alert_recv_event || ev == theft_event);



        if (is_stolen == FALSE){

            #ifdef DEBUG
            printf("\nShouting event posted");
            #endif


            while(!stimer_expired(&session_st)){

                etimer_set(&send_et,SEND_INTERVAL * CLOCK_SECOND);

                packetbuf_clear();

                packetbuf_set_datalen(sizeof(struct tt_alert_msg));
                packetbuf_copyfrom(&new_amsg, sizeof(struct tt_alert_msg));
                abc_send(&abc_c);

                cur_session.
                n_alert_tx = cur_session.n_alert_tx + 1;

                PROCESS_WAIT_EVENT_UNTIL (etimer_expired(&send_et));
            }

            //cur_session.is_expired = TRUE;

            cur_session.first_node_id = 0;
            cur_session.n_alert_tx = 0;
            cur_session.event_clock_time = 0;
            cur_session.orig_clock_time = 0;
            cur_session.last_distance = MAX_UINT32_T;
            cur_session.hop_count = MAX_UINT32_T;
            cur_session.is_expired = FALSE;
            init_array(cur_session.hop_list, MAX_HOP_NUM);

            #ifdef DEBUG
            printf("\nAlert session expired: %d messages sent, %d [m] (last distance), %d [hops]", cur_session.n_alert_tx,
                                                                                                    cur_session.last_distance,
                                                                                                    cur_session.hop_count);
            #endif

        } else {

            event_clock_secs = clock_seconds();
            cur_session.last_distance = 0;
            cur_session.first_node_id = node_id;
            cur_session.event_clock_time = event_clock_secs;
            strncpy(cur_session.vin, (const char *)VEHICLE_ID_NUMBER, VIN_SIZE);

            new_amsg.first_node_id = node_id;
            strncpy(new_amsg.vin, (const char *)VEHICLE_ID_NUMBER, VIN_SIZE);
            new_amsg.hdr.node_id = node_id;
            new_amsg.hdr.type = ALERT_MSG_TYPE;
            new_amsg.hop_count = 0;
            new_amsg.event_clock_time = event_clock_secs;
            new_amsg.last_distance = 0;

            while(is_stolen == TRUE){

                etimer_set(&send_et, SEND_INTERVAL *CLOCK_SECOND);

                new_amsg.orig_clock_time = clock_seconds();
                cur_session.orig_clock_time = clock_seconds();

                packetbuf_clear();

                packetbuf_set_datalen(sizeof(struct tt_alert_msg));
                packetbuf_copyfrom(&new_amsg, sizeof(struct tt_alert_msg));
                abc_send(&abc_c);


                cur_session.n_alert_tx = cur_session.n_alert_tx + 1;

                #ifdef DEBUG
                printf("\nNew alert message sent from %d, number of alert sent: %d.", node_id, cur_session.n_alert_tx);
                #endif

                PROCESS_WAIT_EVENT_UNTIL (etimer_expired(&send_et));
            }

        }

    }

    PROCESS_END();

}