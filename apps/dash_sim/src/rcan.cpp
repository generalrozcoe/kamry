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

    // NEW: Capture the real-time ID and raw bytes array
    this->current_id = frame.can_id;
    for (int i = 0; i < 8; i++) {
        this->current_raw_bytes[i] = frame.data[i];
    }

    // Keep your legacy grid matrix populated for safety/backwards compatibility
    for (int i = 0; i < 8; i++){
        data[frame.can_id-512][i] = frame.data[i];
    }
}
bool rCan::canWrite(uint32_t message_id, const uint8_t* data_bytes) {
    // 🛠️ Linux Kernel native SocketCAN structure definition mapping
    struct can_frame socket_frame;
    
    socket_frame.can_id = message_id;
    socket_frame.can_dlc = 8; // Standard 8-byte payload depth constraint
    
    // Perform a raw bit replication into the SocketCAN structure buffer
    std::memcpy(socket_frame.data, data_bytes, 8);
    
    // Write directly out to the Linux virtual socket file descriptor
    int bytes_sent = write(this->s, &socket_frame, sizeof(struct can_frame)); //
    
    // Return true if the full block size passed successfully
    return (bytes_sent == sizeof(struct can_frame)); //
}
void rCan::closeSocket(){
	close(s);
};


