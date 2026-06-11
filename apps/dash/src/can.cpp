#include "can.hpp"
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
	
void rCan::canRead(){
	int nbytes = read(s, &frame, sizeof(struct can_frame));
if (nbytes < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // No data right now, just return/continue to render
        return; 
    } else {
        perror("Read error");
    }
}
//	std::cout << "Received Frame ID: " << std::hex << frame.can_id << " Data: " << (int)frame.data[0] << std::endl;

	for (int i = 0; i <8; i++){
	data[frame.can_id-512][i] = frame.data[i];
	}
	
	

};

	
void rCan::closeSocket(){
	close(s);
};


