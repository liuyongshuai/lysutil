/*************************************************************************
 * File:	abc1.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2024-04-18 12:28:59
 * Desc:	
 ************************************************************************/
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<string>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/socket.h>
#include<net/if.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/uio.h>
#include<sys/time.h>
#include<getopt.h>
#include<fcntl.h>

#define IFHWADDRLEN 6
#define ETHTOOL_GPERMADDR	0x00000020 /* Get permanent hardware address */
#define SIOCETHTOOL	0x8946		/* Ethtool interface		*/
struct ethtool_perm_addr {
	uint32_t	cmd;
	uint32_t	size;
	uint8_t	data[0];
};

typedef struct {
    unsigned char byte[6];
} mac_t;

typedef struct {
    int sock;
    struct ifreq dev;
} net_info_t;

/**
 * 设置随机数的种子
 */
void random_seed() {
    int fd;
    struct timeval tv;
    unsigned int seed;
    if ((fd = open("/dev/hwrng", O_RDONLY)) >= 0 || (fd = open("/dev/random", O_RDONLY)) >= 0 || (fd = open("/dev/urandom", O_RDONLY)) >= 0) {
        read(fd, &seed, sizeof(seed));
        close(fd);
    } else {
        gettimeofday(&tv, NULL);
        seed = (getpid() << 16) ^ tv.tv_sec ^ tv.tv_usec;
    }
    srandom(seed);
}

/**
 * 生成随机的mac地址
 */
void get_random_mac(mac_t *mac, unsigned char last_n_bytes) {
    switch (last_n_bytes) {
        case 6:
            mac->byte[0] = (random() % 255) & 0xFC;
            mac->byte[1] = random() % 255;
            mac->byte[2] = random() % 255;
        case 3:
            mac->byte[3] = random() % 255;
            mac->byte[4] = random() % 255;
            mac->byte[5] = random() % 255;
    }
    mac->byte[0] &= ~2;
}

/**
 * 永久的网卡地址
 */
void get_permanent_mac(const net_info_t *net, mac_t *mac) {
    int i;
    struct ifreq req;
    struct ethtool_perm_addr *epa;
    epa = (struct ethtool_perm_addr *) malloc(sizeof(struct ethtool_perm_addr) + IFHWADDRLEN);
    epa->cmd = ETHTOOL_GPERMADDR;
    epa->size = IFHWADDRLEN;

    memcpy(&req, &(net->dev), sizeof(struct ifreq));
    req.ifr_data = (caddr_t) epa;

    if (ioctl(net->sock, SIOCETHTOOL, &req) < 0) {
        std::cout << "Could not read permanent MAC" << std::endl;
    } else {
        for (i = 0; i < 6; i++) {
            mac->byte[i] = epa->data[i];
        }
    }
    free(epa);
}

/**
 * 从网络信息里获取在用的mac地址
 */
void get_net_mac(const net_info_t *net, mac_t *mac) {
    int i;
    for (i = 0; i < 6; i++) {
        mac->byte[i] = net->dev.ifr_hwaddr.sa_data[i] & 0xFF;
    }
}

/**
 * 设置网络的mac
 */
void set_net_mac(net_info_t *net, const mac_t *mac) {
    int i;
    for (i = 0; i < 6; i++) {
        net->dev.ifr_hwaddr.sa_data[i] = mac->byte[i];
    }
    if (ioctl(net->sock, SIOCSIFHWADDR, &net->dev) < 0) {
        std::cout << "Could not change MAC: interface up or insufficient permissions" << std::endl;
		perror("failed");
        return;
    }
}

/**
 * mac地址转为字符串
 */
std::string mac_to_string(const mac_t *mac) {
    int i;
    char buf[18];
    for (i = 0; i < 6; i++) {
        sprintf(&buf[i * 3], "%02x%s", mac->byte[i], i < 5 ? ":" : "");
    }
    return buf;
}

/**
 * 设置mac地址
 * param1：必选，网卡名称，如wlp3s0
 * param2：可选，mac地址，为空则随机生成
 */
int main(int argc, char *argv[]) {
    std::string device_name;
    std::string mac_addr;

    if (argc > 1) {
        device_name = argv[1];
    }
    if (argc > 2) {
        mac_addr = argv[2];
    }
    if (device_name.empty()) {
        std::cout << "empty device_name" << std::endl;
    }
    std::cout << "device_name=" << device_name << std::endl;
    if (!mac_addr.empty()) {
        int nbyte = 5;
        if (mac_addr.size() != 17) {
            std::cout << "invalid mac:" << mac_addr << std::endl;
            return 1;
        }
        for (nbyte = 2; nbyte < 16; nbyte += 3) {
            if (mac_addr[nbyte] != ':') {
                std::cout << "invalid mac:" << mac_addr << std::endl;
                return 1;
            }
        }
        std::cout << "mac=" << mac_addr << std::endl;
    }

    //获取网卡的网络信息
    net_info_t *net_info = (net_info_t *) malloc(sizeof(net_info_t));
    net_info->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (net_info->sock < 0) {
        std::cout << "invalid net sock" << std::endl;
        free(net_info);
        return 1;
    }
    strncpy(net_info->dev.ifr_name, device_name.c_str(), sizeof(net_info->dev.ifr_name));
    net_info->dev.ifr_name[sizeof(net_info->dev.ifr_name) - 1] = '\0';
    if (ioctl(net_info->sock, SIOCGIFHWADDR, &net_info->dev) < 0) {
        std::cout << "set device name failed" << std::endl;
        free(net_info);
        return 2;
    }

    //当前在用的地址
    mac_t *cur_mac = (mac_t *) malloc(sizeof(mac_t));
    get_net_mac(net_info, cur_mac);
    std::cout << "cur_mac=" << mac_to_string(cur_mac) << std::endl;

    //永久网卡地址
    mac_t *permanent_mac = (mac_t *) malloc(sizeof(mac_t));
    get_permanent_mac(net_info, permanent_mac);
    std::cout << "permanent_mac=" << mac_to_string(permanent_mac) << std::endl;

    //构造的地址
    mac_t *faked_mac = (mac_t *) malloc(sizeof(mac_t));
    memcpy(faked_mac, permanent_mac, sizeof(mac_t));
    if (!mac_addr.empty()) {
        for (int nbyte = 0; nbyte < 6; nbyte++) {
            faked_mac->byte[nbyte] = (char) (strtoul(mac_addr.c_str() + nbyte * 3, 0, 16) & 0xFF);
        }
    } else {
        get_random_mac(faked_mac, 3);
    }
    std::cout << "faked_mac=" << mac_to_string(faked_mac) << std::endl;

    //开始设置mac
    set_net_mac(net_info, faked_mac);
    return 0;
}
