#ifndef rcan
#define rcan

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h> // For close()
#include <map>
#include "pid.hpp"
#include <fcntl.h>

class rCan {
private:
	int s;
	std::string connection;
	struct sockaddr_can addr;
struct ifreq ifr;
struct can_frame frame;
	
	public:
		int data[9][8];
uint32_t current_id = 0;
    uint8_t current_raw_bytes[8] = {0};
void socketCreate();
	
	void networkBind();
	rCan(std::string connect);
	void canRead();
	void closeSocket();
	
	
	
	
};


#endif