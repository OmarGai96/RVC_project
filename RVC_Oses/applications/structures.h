
#ifndef APPLICATIONS_STRUCTURES_H_
#define APPLICATIONS_STRUCTURES_H_


// ************************************** DEFINITIONS **********************************************************

#define EVENT_OBSTACLE_CONTROL_ACTIVATION (1 << 1)
#define EVENT_MOVEMENT_CONTROL_ACTIVATION (1 << 2)
#define EVENT_CHECK_RESOURCES_ACTIVATION  (1 << 3)
#define EVENT_ACOUSTIC_SIGNALS_ACTIVATION  (1 << 4)
#define EVENT_BRUSHES_SPEED_ACTIVATION  (1 << 5)

#define EVENT_OBSTACLE_FOUND1 (1 << 1)
#define EVENT_OBSTACLE_FOUND2 (1 << 2)
#define EVENT_FLAG1 (1 << 1)
#define EVENT_FLAG2 (1 << 2)

#define EVENT_OBSTACLE_FOUND (1 << 1)


// ************************************ STRUCTURES *************************************************************


/* TIMER used to activate periodically the obstacle control task */
struct rt_timer timer_obstacle_control_activation;
/* TIMER used to activate periodically the map management task */
struct rt_timer timer_movement_control_activation;
/* TIMER used to activate periodically the check resources task */
struct rt_timer timer_check_resources_activation;
/* TIMER used to activate periodically the brushes speed task */
struct rt_timer timer_brushes_speed_activation;


/* EVENT used by the timer to activate tasks */
struct rt_event event_tasks_activation;
/* EVENT used to notify that there's an obstacle */
struct rt_event event_obstacle;
/* EVENT used to notify that there's no more resources */
struct rt_event event_resources;

/* Mailbox control block */
struct rt_mailbox mb2_3;
/* Memory pool for mails storage */
char mb_pool[128];

/* Mailbox control block T2 T5*/
struct rt_mailbox mb2_5;
/* Memory pool for mails storage T2 T5*/
char mb_pool2_5[128];

// ************************************ FUNCTIONS **************************************************************


/* function executed by the timer that activates obstacle control periodically when it reaches the timeout */
void timeout_obstacle_control(void* parameter);
/* function executed by the timer that activates movement control periodically when it reaches the timeout */
void timeout_movement_control(void* parameter);
/* function executed by the timer that activates check resources periodically when it reaches the timeout */
void timeout_check_resources(void* parameter);
/* function executed by the timer that activates check resources periodically when it reaches the timeout */
void timeout_acoustic_signals(void* parameter);
/* function executed by the timer that activates brushes speed periodically when it reaches the timeout */
void timeout_brushes_speed(void* parameter);



#endif /* APPLICATIONS_STRUCTURES_H_ */
