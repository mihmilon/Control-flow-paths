#include <stdio.h>
#include <time.h>
#include <string.h>
#include "elevator.h"

ELEVATOR elevators[3];
bool sleep_flag = FALSE;

void Set_Idle_Lift_Levels();
void decision_algo(EXT_INPUT *current_proc_input);

void receive_user_internal_trigger(int i, int dest_level);
void elevator_input_proc_func(int i);

void swap_func(int *num1, int *num2);
void set_service_level_descending(int i);
void set_service_level_ascending(int i);
ELV_STATUS dispatch_elv (int elv_id, int elv_level){
	elevators[elv_id].current_status.level = elv_level;
	if (!start_elv(elv_id)){
		return ELV_SUCCESS;
	}
	else{
		printf("Elevator start error\r\n");
		return ELV_START_ERROR;
	}
}

/******************************************************************************************************/
// to find the closest elevator to the specified level
// the dnu1 & dnu2 parameters specify which all elevators we can take into account while finding the closes elevator
// if an elevator is currently servicing 3 requests, we do not want to consider it which figuring out the closest elevator
int find_close_elevator(int *level, int dnu1, int dnu2){
	int i, close_elevator, diff;
	for (i = 0; i < MAX_LIFTS; i++)	{
		if (dnu1 == i)
			continue;
		if (dnu2 == i)
			continue;
		close_elevator = i;
		diff = elevators[i].current_status.level - level;
		diff = (diff < 0) ? diff*(-1) : diff;
		if (diff < (elevators[close_elevator].current_status.level - level)){
			close_elevator = i;
		}
	}
	return close_elevator;
}
ELV_STATUS dispatch_idle_elv(){
	int hour = 0, min = 0, level1_close_elevator, level2_close_elevator, level3_close_elevator;
	time_t tm;
	struct tm *ltime;
	ELV_STATUS ret;
	time( &tm );
	ltime = localtime( &tm );
	ltime->tm_mon++;
	ltime->tm_year += 1900;
	hour = ltime->tm_hour;
	min = ltime->tm_min;
	printf( "  %d , %d \n\n", hour, min );
	if ((hour >=7) && (hour <= 9)){
		level1_close_elevator = find_close_elevator(5, -1, -1);
		level2_close_elevator = find_close_elevator(16, level1_close_elevator, -1);
		level3_close_elevator = find_close_elevator(23, level1_close_elevator, level2_close_elevator);
		elevators[0].current_status.level = 5;
		elevators[1].current_status.level = 16;
		elevators[2].current_status.level = 26;
	}
	else if ((hour >=17) && (hour <= 20)){
		level1_close_elevator = find_close_elevator(0, -1, -1);
		level2_close_elevator = find_close_elevator(10, level1_close_elevator, -1);
		level3_close_elevator = find_close_elevator(20, level1_close_elevator, level2_close_elevator);
		elevators[level1_close_elevator].current_status.level = 0;
		elevators[level2_close_elevator].current_status.level = 10;
		elevators[level3_close_elevator].current_status.level = 20;
	}
	else{
		level1_close_elevator = find_close_elevator(0, -1, -1);
		level2_close_elevator = find_close_elevator(14, level1_close_elevator, -1);
		level3_close_elevator = find_close_elevator(23, level1_close_elevator, level2_close_elevator);
		elevators[level1_close_elevator].current_status.level = 0;
		elevators[level2_close_elevator].current_status.level = 14;
		elevators[level3_close_elevator].current_status.level = 23;
	}
	if ((ret = dispatch_elv(0, elevators[0].current_status.level)) != ELV_SUCCESS) return ret;
	if ((ret = dispatch_elv(1, elevators[1].current_status.level)) != ELV_SUCCESS)return ret;
	if ((ret = dispatch_elv(2, elevators[2].current_status.level)) != ELV_SUCCESS)return ret;
}
int check_and_set_dnu(int *lift1, int* lift2){
	int i;
	for (i = 0; i < MAX_LIFTS; i++)	{
		if (elevators[i].serivce_cntr >= 3)		{
			if (*lift1 < 0)			{
				*lift1 = i;
			}
			else if (*lift2 < 0)			{
				*lift2 = i;
			}
			else return -1;
		}
	}
	return 0;
}
void swap_func(int *num1, int *num2){
	*num1+= *num2;
	*num2= *num1 - *num2;
	*num1 -= *num2;
}
void set_service_level_descending(int i){
	if((elevators[i].service[0].dir != DIR_IDLE) &&  (elevators[i].service[1].dir != DIR_IDLE) && (elevators[i].service[2].dir != DIR_IDLE)){
	 	if(elevators[i].service[0].level > elevators[i].service[1].level){
			if (elevators[i].service[0].level > elevators[i].service[2].level){
				;
			}
			else{
				swap_func(& elevators[i].service[0].level,& elevators[i].service[2].level);
				swap_func(& elevators[i].service[1].level,& elevators[i].service[2].level);
			}
		}
		else if(elevators[i].service[1].level > elevators[i].service[2].level){
			swap_func(& elevators[i].service[0].level,& elevators[i].service[1].level);
			if(elevators[i].service[1].level > elevators[i].service[2].level){
				;
			}
			else{
				swap_func(& elevators[i].service[1].level,& elevators[i].service[2].level);
			}
		}
		else{
			swap_func(& elevators[i].service[0].level,& elevators[i].service[2].level);
		}
	}
	else if((elevators[i].service[0].dir != DIR_IDLE) &&  (elevators[i].service[1].dir != DIR_IDLE) && (elevators[i].service[2].dir == DIR_IDLE)){
	 	if(elevators[i].service[0].level > elevators[i].service[1].level){
			;
		}
		else{
			swap_func(& elevators[i].service[0].level,& elevators[i].service[1].level);
		}
	}
	else{
			;
	}
}
void set_service_level_ascending(int i){
	if((elevators[i].service[0].dir != DIR_IDLE) &&  (elevators[i].service[1].dir != DIR_IDLE) && (elevators[i].service[2].dir != DIR_IDLE)){
		if(elevators[i].service[0].level < elevators[i].service[1].level){
			if (elevators[i].service[0].level < elevators[i].service[2].level){
				;
			}
			else{
				swap_func(& elevators[i].service[0].level,& elevators[i].service[2].level);
				swap_func(& elevators[i].service[1].level,& elevators[i].service[2].level);
			}
		}
		else if(elevators[i].service[1].level < elevators[i].service[2].level){
			swap_func(& elevators[i].service[0].level,& elevators[i].service[1].level);
			if(elevators[i].service[1].level < elevators[i].service[2].level){
				;
			}
			else{
				swap_func(& elevators[i].service[1].level,& elevators[i].service[2].level);
			}
		}
		else {
			swap_func(& elevators[i].service[0].level,& elevators[i].service[2].level);
		}
	}
	else if((elevators[i].service[0].dir != DIR_IDLE) &&  (elevators[i].service[1].dir != DIR_IDLE) && (elevators[i].service[2].dir == DIR_IDLE)){
	 	if(elevators[i].service[0].level < elevators[i].service[1].level){
			;
		}
		else{
			swap_func(& elevators[i].service[0].level,& elevators[i].service[1].level);
		}
	}
	else{
			;
	}
}
void add_service_request(int elv_id, EXT_INPUT *curr_proc_ip){
	int i;
	for (i = 0; i < MAX_REQUESTS; i++){
		if (elevators[elv_id].service[i].dir != DIR_IDLE)
			continue;
		else{
			elevators[elv_id].service[i].level = curr_proc_ip->level;
			elevators[elv_id].service[i].dir = curr_proc_ip->dir;
			elevators[elv_id].serivce_cntr++;
			if (elevators[i].service[i].dir == DIR_DOWN){
				set_service_level_descending(i);
			}
			else{
				set_service_level_ascending(i);
			}
		}
	}
}
ELV_STATUS dispatch_pending_elv(void){
	ELV_STATUS ret;
	int i;
	for (i = 0; i < MAX_LIFTS; i++)	{
		if (elevators[i].serivce_cntr > 0){
			if ((ret = dispatch_elv(i, elevators[i].current_status.level)) != ELV_SUCCESS) return ret;
		}
	}
}
void decision_algo(EXT_INPUT *current_proc_input){
	int i, dnu1 = -1, dnu2 = -1, close_elv;
	if (!check_and_set_dnu(&dnu1, &dnu2)){
		printf("All elevators busy\n");
		return;
	}
	for (i = 0; i < MAX_LIFTS; i++){
		close_elv = find_close_elevator(&current_proc_input.level, dnu1, dnu2);
		if (elevators[close_elv].service[0].dir == current_proc_input->dir){
			if (current_proc_input->dir == DIR_UP){
				if (elevators[i].current_status.level < current_proc_input->level && elevators[i].service[2].level >= current_proc_input->level){
					add_service_request(elevators[close_elv].id, current_proc_input);
					break;
				}
			}
			else if (current_proc_input->dir == DIR_DOWN){
				if (elevators[close_elv].current_status.level > current_proc_input->level && elevators[i].service[0].level <= current_proc_input->level){
					add_service_request(elevators[close_elv].id, current_proc_input);
					break;
				}
			}
		}
		else if (elevators[close_elv].current_status.dir == DIR_IDLE){
			if ((elevators[close_elv].service[0].dir == DIR_IDLE) ||  (elevators[close_elv].service[0].dir == current_proc_input->dir))	{
				add_service_request(elevators[close_elv].id, current_proc_input);
				break;
			}
		}
	}
}
void elv_init(){
	int i,j;
	for (i = 0; i < MAX_LIFTS; i++){
		memset(&elevators[i], 0x00, sizeof(ELEVATOR));
		elevators[i].id = 0;
		if (elevators[i].current_status.level != 0){
			elevators[i].current_status.level = 0;
		}
		for(j = 0; j < MAX_INTERNAL_INPUTS; j++){
			elevators[i].user_destination[j] = FALSE;
		}
	}
	dispatch_elv(0, 0);
	dispatch_elv(1, 0);
	dispatch_elv(2, 0);
}
int main (void){
	int i;
	int x_time_units = 600;
	elv_init();
	EXT_INPUT current_proc_input;
	while(1){
		if (!sleep_flag){
			dispatch_idle_elv();
		}
		for (i = 0; i < MAX_REQUESTS_PROC; i++)	{
			if (&current_proc_input != NULL){
				decision_algo(&current_proc_input);
			}
			else{
				break;
			}
		}
		pop_from_queue(current_proc_input);
		dispatch_pending_elv();
		sleep_flag = TRUE;
		sleep(x_time_units);
	}
	return 0;
}
void elevator_input_proc_func(int i){
	int j;
	for(j = 0; j < MAX_INTERNAL_INPUTS; j++){
		if(elevators[i].user_destination[j] == TRUE){
		}
	}
}
void event_interrupt(int level, int dir){
	EXT_INPUT in;
	in.level = level;
	in.dir = dir;
	push_to_queue(in);
}
void timer_expiry(){
	sleep_flag = FALSE;
	stopTimer();
}
void elv_done_service (int elv_id){
	int temp_level;
	temp_level = elevators[elv_id].current_status.level;
	memset(&elevators[elv_id], 0x00, sizeof(ELEVATOR));
	elevators[elv_id].current_status.level = temp_level;
	elevators[elv_id].current_status.dir = DIR_IDLE;
}
void receive_user_internal_trigger(int i, int dest_level){
	if ((0 <= dest_level) && (dest_level <= MAX_INTERNAL_INPUTS)){
		elevators[i].user_destination[dest_level] = TRUE;
	}
	else{
	}
}
