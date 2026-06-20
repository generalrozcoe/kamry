#include "rcan.hpp"
void rCan::socketCreate(){
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) { //
    perror("Socket creation failed");
    // Handle error		
}};

void rCan::networkBind(){
		strcpy(ifr.ifr_name, connection.c_str()); //
ioctl(s, SIOCGIFINDEX, &ifr);
	struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
    }
};
	
rCan::rCan(std::string connect){
	connection = connect;
	this->socketCreate();
	this->networkBind();
	fcntl(s, F_SETFL, O_NONBLOCK);
};
	
bool rCan::canRead(uint32_t& out_msg_id, uint8_t* dest_bytes) {
    struct can_frame socket_frame; //
    
    // Non-blocking or blocking read on the SocketCAN file descriptor
    int bytes_read = read(this->s, &socket_frame, sizeof(struct can_frame)); //
    if (bytes_read < sizeof(struct can_frame)) {
        return false; // No complete network packet ready right now
    }

    // Pass the message ID out to the execution engine loop context
    out_msg_id = socket_frame.can_id;

    // 🛠️ SKIP THE MIDDLEMAN: 
    // Copy the raw 8 payload data bytes straight to the memory target layout address!
    std::memcpy(dest_bytes, socket_frame.data, 8); 
    
    return true;
}
void rCan::closeSocket(){
	close(s);
};


