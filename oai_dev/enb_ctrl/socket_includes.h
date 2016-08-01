
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>


#define MAX_LISTEN_QUE 5
#define SERV_PORT 2819 //tcp 6777
#define MAX_BUFFER_SIZE 1024

struct enb_data{
		int enb_subframe;
		int enb_offset;
		unsigned char enb_eNB_id;
		uint32_t enb_aarx;
		uint32_t enb_rb;
		int enb_len;
		int32_t enb_signal_energy_nodc;
		unsigned short enb_n0_subband_power_dB;
		uint32_t mcs1_su;    //ulsch_mcs
		uint32_t mcs2_su;   //dlsch_mcs
		uint8_t N_RB_UL;
		short rssi;
};
