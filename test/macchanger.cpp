/*************************************************************************
 * File:	macchanger.cpp
 * Author:	liuyongshuai<liuyongshuai@hotmail.com>
 * Time:	2024-04-18 10:27
 ************************************************************************/
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
#include<linux/ethtool.h>
#include<linux/sockios.h>

#define CARD_NAME(x)     mc_maclist_get_cardname_with_default(x, "unknown")
#define EXIT_OK    0
#define EXIT_ERROR 1

typedef struct {
    unsigned char byte[6];
} mac_t;

typedef enum {
    mac_is_anykind,
    mac_is_wireless,
    mac_is_others
} mac_type_t;

typedef struct {
    char *name;
    unsigned char byte[3];
} card_mac_list_item_t;

typedef struct {
    int sock;
    struct ifreq dev;
} net_info_t;

card_mac_list_item_t *list_others = NULL; /* IEEE OUI */
card_mac_list_item_t *list_wireless = NULL; /* Wireless cards */

int list_others_len = 0;
int list_wireless_len = 0;

mac_t *mc_mac_dup(const mac_t *mac) {
    mac_t *t;
    t = (mac_t *) malloc(sizeof(mac_t));
    memcpy(t, mac, sizeof(mac_t));
    return t;
}


void mc_mac_free(mac_t *mac) {
    free(mac);
}


void mc_mac_into_string(const mac_t *mac, char *s) {
    int i;

    for (i = 0; i < 6; i++) {
        sprintf(&s[i * 3], "%02x%s", mac->byte[i], i < 5 ? ":" : "");
    }
}


void mc_mac_random(mac_t *mac, unsigned char last_n_bytes, char set_bia) {
    switch (last_n_bytes) {
        case 6:
            /* 8th bit: Unicast / Multicast address
             * 7th bit: BIA (burned-in-address) / locally-administered
             */
            mac->byte[0] = (random() % 255) & 0xFC;
            mac->byte[1] = random() % 255;
            mac->byte[2] = random() % 255;
        case 3:
            mac->byte[3] = random() % 255;
            mac->byte[4] = random() % 255;
            mac->byte[5] = random() % 255;
    }

    /* Handle the burned-in-address bit
     */
    if (set_bia) {
        mac->byte[0] &= ~2;
    } else {
        mac->byte[0] |= 2;
    }
}


int mc_mac_equal(const mac_t *mac1, const mac_t *mac2) {
    int i;
    for (i = 0; i < 6; i++) {
        if (mac1->byte[i] != mac2->byte[i]) {
            return 0;
        }
    }
    return 1;
}


int mc_mac_read_string(mac_t *mac, char *string) {
    int nbyte = 5;

    /* Check the format */
    if (strlen(string) != 17) {
        fprintf(stderr, "[ERROR] Incorrect format: MAC length should be 17. %s(%lu)\n", string, strlen(string));
        return -1;
    }

    for (nbyte = 2; nbyte < 16; nbyte += 3) {
        if (string[nbyte] != ':') {
            fprintf(stderr, "[ERROR] Incorrect format: %s\n", string);
            return -1;
        }
    }

    /* Read the values */
    for (nbyte = 0; nbyte < 6; nbyte++) {
        mac->byte[nbyte] = (char) (strtoul(string + nbyte * 3, 0, 16) & 0xFF);
    }

    return 0;
}

char *
mc_maclist_get_cardname_from_list(const mac_t *mac, card_mac_list_item_t *list) {
    int i = 0;

    while (list[i].name) {
        if ((mac->byte[0] == list[i].byte[0]) &&
            (mac->byte[1] == list[i].byte[1]) &&
            (mac->byte[2] == list[i].byte[2])) {
            return list[i].name;
        }
        i++;
    }

    return NULL;
}


char *
mc_maclist_get_cardname(const mac_t *mac) {
    char *name;

    name = mc_maclist_get_cardname_from_list(mac, list_wireless);
    if (name) {
        return name;
    }

    name = mc_maclist_get_cardname_from_list(mac, list_others);
    return name;
}


const char *
mc_maclist_get_cardname_with_default(const mac_t *mac, const char *def) {
    char *name;
    name = mc_maclist_get_cardname(mac);
    return name ? name : def;
}

void
mc_maclist_set_random_vendor_from_list(mac_t *mac, card_mac_list_item_t *list, int list_len) {
    int i, num = list_len;
    /* Choose one randomly */
    num = random() % num;

    /* Copy the vendor MAC range */
    for (i = 0; i < 3; i++) {
        mac->byte[i] = list[num].byte[i];
    }
}


void
mc_maclist_set_random_vendor(mac_t *mac, mac_type_t type) {
    int num;

    num = random() % (list_others_len + list_wireless_len);

    switch (type) {
        case mac_is_anykind:
            if (num < list_others_len) {
                mc_maclist_set_random_vendor_from_list(mac, list_others, list_others_len);
            } else {
                mc_maclist_set_random_vendor_from_list(mac, list_wireless, list_wireless_len);
            }
            break;
        case mac_is_wireless:
            mc_maclist_set_random_vendor_from_list(mac, list_wireless, list_wireless_len);
            break;
        case mac_is_others:
            mc_maclist_set_random_vendor_from_list(mac, list_others, list_others_len);
            break;
    }
}


int
mc_maclist_is_wireless(const mac_t *mac) {
    return (mc_maclist_get_cardname_from_list(mac, list_wireless) != NULL);
}


void
mc_maclist_print_from_list(card_mac_list_item_t *list, const char *keyword) {
    int i = 0;
    while (list[i].name) {
        if (!keyword || (keyword && strstr(list[i].name, keyword))) {
            printf("%04i - %02x:%02x:%02x - %s\n", i, list[i].byte[0], list[i].byte[1], list[i].byte[2], list[i].name);
        }
        i++;
    }
}


void
mc_maclist_print(const char *keyword) {
    printf("Misc MACs:\n"
           "Num    MAC        Vendor\n"
           "---    ---        ------\n");
    mc_maclist_print_from_list(list_others, keyword);

    printf("\n"
           "Wireless MACs:\n"
           "Num    MAC        Vendor\n"
           "---    ---        ------\n");
    mc_maclist_print_from_list(list_wireless, keyword);
}


card_mac_list_item_t *
mc_maclist_read_from_file(const char *fullpath, int *list_len) {
    FILE *f;
    char *line;
    char tmp[512];
    int num = 0;
    card_mac_list_item_t *list;

    if ((f = fopen(fullpath, "r")) == NULL) {
        fprintf(stderr, "[ERROR] Could not read data file: %s\n", fullpath);
        return NULL;
    }

    /* Count lines */
    while ((line = fgets(tmp, 511, f)) != NULL) num++;
    rewind(f);

    /* Get mem */
    list = (card_mac_list_item_t *) malloc(sizeof(card_mac_list_item_t) * (num + 1));

    /* Parse it */
    num = 0;
    while ((line = fgets(tmp, 511, f)) != NULL) {
        list[num].byte[0] = (char) (strtoul(line, NULL, 16) & 0xFF);
        list[num].byte[1] = (char) (strtoul(line + 3, NULL, 16) & 0xFF);
        list[num].byte[2] = (char) (strtoul(line + 6, NULL, 16) & 0xFF);

        line[strlen(line) - 1] = '\0';
        list[num].name = (char *) (strdup(line + 9));

        num++;
    }

    /* End of list */
    list[num].byte[0] = list[num].byte[1] = list[num].byte[2] = 0;
    list[num].name = NULL;

    fclose(f);

    *list_len = num;
    return list;
}


int
mc_maclist_init(void) {
    list_others = mc_maclist_read_from_file(LISTDIR
    "/OUI.list", &list_others_len);
    list_wireless = mc_maclist_read_from_file(LISTDIR
    "/wireless.list", &list_wireless_len);

    return (list_others && list_wireless) ? 0 : -1;
}


static void
free_list(card_mac_list_item_t *list) {
    int i = 0;
    while (list[i].name) {
        free(list[i].name);
        i++;
    }
    free(list);
}


void
mc_maclist_free(void) {
    free_list(list_others);
    free_list(list_wireless);
}

net_info_t *
mc_net_info_new(const char *device) {
    net_info_t *t = (net_info_t *) malloc(sizeof(net_info_t));

    t->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (t->sock < 0) {
        perror("[ERROR] Socket");
        free(t);
        return NULL;
    }

    strncpy(t->dev.ifr_name, device, sizeof(new->dev.ifr_name));
    t->dev.ifr_name[sizeof(t->dev.ifr_name) - 1] = '\0';
    if (ioctl(t->sock, SIOCGIFHWADDR, &t->dev) < 0) {
        perror("[ERROR] Set device name");
        free(t);
        return NULL;
    }

    return t;
}


void
mc_net_info_free(net_info_t *net) {
    close(net->sock);
    free(net);
}


mac_t *
mc_net_info_get_mac(const net_info_t *net) {
    int i;
    mac_t *t = (mac_t *) malloc(sizeof(mac_t));

    for (i = 0; i < 6; i++) {
        t->byte[i] = net->dev.ifr_hwaddr.sa_data[i] & 0xFF;
    }

    return new;
}


int
mc_net_info_set_mac(net_info_t *net, const mac_t *mac) {
    int i;

    for (i = 0; i < 6; i++) {
        net->dev.ifr_hwaddr.sa_data[i] = mac->byte[i];
    }

    if (ioctl(net->sock, SIOCSIFHWADDR, &net->dev) < 0) {
        perror("[ERROR] Could not change MAC: interface up or insufficient permissions");
        return -1;
    }

    return 0;
}

mac_t *
mc_net_info_get_permanent_mac(const net_info_t *net) {
    int i;
    struct ifreq req;
    struct ethtool_perm_addr *epa;
    mac_t *newmac;

    newmac = (mac_t *) calloc(1, sizeof(mac_t));

    epa = (struct ethtool_perm_addr *) malloc(sizeof(struct ethtool_perm_addr) + IFHWADDRLEN);
    epa->cmd = ETHTOOL_GPERMADDR;
    epa->size = IFHWADDRLEN;

    memcpy(&req, &(net->dev), sizeof(struct ifreq));
    req.ifr_data = (caddr_t) epa;

    if (ioctl(net->sock, SIOCETHTOOL, &req) < 0) {
        perror("[ERROR] Could not read permanent MAC");
    } else {
        for (i = 0; i < 6; i++) {
            newmac->byte[i] = epa->data[i];
        }
    }

    free(epa);
    return newmac;
}

void
print_mac(const char *s, const mac_t *mac) {
    char string[18];
    int is_wireless;

    is_wireless = mc_maclist_is_wireless(mac);
    mc_mac_into_string(mac, string);
    printf("%s%s%s (%s)\n", s,
           string,
           is_wireless ? " [wireless]" : "",
           CARD_NAME(mac));
}


void
random_seed(void) {
    int fd;
    struct timeval tv;
    unsigned int seed;

    if ((fd = open("/dev/hwrng", O_RDONLY)) >= 0 ||
        (fd = open("/dev/random", O_RDONLY)) >= 0 ||
        (fd = open("/dev/urandom", O_RDONLY)) >= 0) {
        read(fd, &seed, sizeof(seed));
        close(fd);
    } else {
        gettimeofday(&tv, NULL);
        seed = (getpid() << 16) ^ tv.tv_sec ^ tv.tv_usec;
    }

    srandom(seed);
}

void
print_help(void) {
    printf("GNU MAC Changer\n"
           "Usage: macchanger [options] device\n\n"
           "  -h,  --help                   Print this help\n"
           "  -V,  --version                Print version and exit\n"
           "  -s,  --show                   Print the MAC address and exit\n"
           "  -e,  --ending                 Don't change the vendor bytes\n"
           "  -a,  --another                Set random vendor MAC of the same kind\n"
           "  -A                            Set random vendor MAC of any kind\n"
           "  -p,  --permanent              Reset to original, permanent hardware MAC\n"
           "  -r,  --random                 Set fully random MAC\n"
           "  -l,  --list[=keyword]         Print known vendors\n"
           "  -b,  --bia                    Pretend to be a burned-in-address\n"
           "  -m,  --mac=XX:XX:XX:XX:XX:XX  Set the MAC XX:XX:XX:XX:XX:XX\n\n"
           "Report bugs to https://github.com/alobbs/macchanger/issues\n");
}

int main(int argc, char *argv[]) {
    char random = 0;
    char ending = 0;
    char another_any = 0;
    char another_same = 0;
    char permanent = 0;
    char print_list = 0;
    char show = 0;
    char set_bia = 0;
    char *set_mac = NULL;
    char *search_word = NULL;

    struct option long_options[] = {
            /* Options without arguments */
            {"help",        no_argument,       NULL, 'h'},
            {"version",     no_argument,       NULL, 'V'},
            {"random",      no_argument,       NULL, 'r'},
            {"ending",      no_argument,       NULL, 'e'},
            {"endding",     no_argument,       NULL, 'e'}, /* kept for backwards compatibility */
            {"another",     no_argument,       NULL, 'a'},
            {"permanent",   no_argument,       NULL, 'p'},
            {"show",        no_argument,       NULL, 's'},
            {"another_any", no_argument,       NULL, 'A'},
            {"bia",         no_argument,       NULL, 'b'},
            {"list",        optional_argument, NULL, 'l'},
            {"mac",         required_argument, NULL, 'm'},
            {NULL, 0,                          NULL, 0}
    };

    net_info_t *net;
    mac_t *mac;
    mac_t *mac_permanent;
    mac_t *mac_faked;
    char *device_name;
    int val;
    int ret;

    /* Read the parameters */
    while ((val = getopt_long(argc, argv, "VasAbrephlm:", long_options, NULL)) != -1) {
        switch (val) {
            case 'V':
                printf("GNU MAC changer %s\n"
                       "Written by Alvaro Lopez Ortega <alvaro@gnu.org>\n\n"
                       "Copyright (C) 2003,2013 Alvaro Lopez Ortega <alvaro@gnu.org>.\n"
                       "This is free software; see the source for copying conditions.  There is NO\n"
                       "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n",
                       VERSION);
                exit(EXIT_OK);
                break;
            case 'l':
                print_list = 1;
                search_word = optarg;
                break;
            case 'r':
                random = 1;
                break;
            case 'e':
                ending = 1;
                break;
            case 'b':
                set_bia = 1;
                break;
            case 'a':
                another_same = 1;
                break;
            case 's':
                show = 1;
                break;
            case 'A':
                another_any = 1;
                break;
            case 'p':
                permanent = 1;
                break;
            case 'm':
                set_mac = optarg;
                break;
            case 'h':
            case '?':
            default:
                print_help();
                exit(EXIT_OK);
                break;
        }
    }

    /* Read the MAC lists */
    if (mc_maclist_init() < 0) {
        exit(EXIT_ERROR);
    }

    /* Print list? */
    if (print_list) {
        mc_maclist_print(search_word);
        exit(EXIT_OK);
    }

    /* Get device name argument */
    if (optind >= argc) {
        print_usage();
        exit(EXIT_OK);
    }
    device_name = argv[optind];

    /* Seed a random number generator */
    random_seed();

    /* Read the MAC */
    if ((net = mc_net_info_new(device_name)) == NULL) {
        exit(EXIT_ERROR);
    }
    mac = mc_net_info_get_mac(net);
    mac_permanent = mc_net_info_get_permanent_mac(net);

    /* --bia can only be used with --random */
    if (set_bia && !random) {
        fprintf(stderr, "[WARNING] Ignoring --bia option that can only be used with --random\n");
    }

    /* Print the current MAC info */
    print_mac("Current MAC:   ", mac);
    print_mac("Permanent MAC: ", mac_permanent);

    /* Change the MAC */
    mac_faked = mc_mac_dup(mac);

    if (show) {
        exit(EXIT_OK);
    } else if (set_mac) {
        if (mc_mac_read_string(mac_faked, set_mac) < 0) {
            exit(EXIT_ERROR);
        }
    } else if (random) {
        mc_mac_random(mac_faked, 6, set_bia);
    } else if (ending) {
        mc_mac_random(mac_faked, 3, 1);
    } else if (another_same) {
        val = mc_maclist_is_wireless(mac);
        mc_maclist_set_random_vendor(mac_faked, val);
        mc_mac_random(mac_faked, 3, 1);
    } else if (another_any) {
        mc_maclist_set_random_vendor(mac_faked, mac_is_anykind);
        mc_mac_random(mac_faked, 3, 1);
    } else if (permanent) {
        mac_faked = mc_mac_dup(mac_permanent);
    } else {
        exit(EXIT_OK); /* default to show */
    }

    /* Set the new MAC */
    ret = mc_net_info_set_mac(net, mac_faked);
    if (ret == 0) {
        /* Re-read the MAC */
        mc_mac_free(mac_faked);
        mac_faked = mc_net_info_get_mac(net);

        /* Print it */
        print_mac("New MAC:       ", mac_faked);

        /* Is the same MAC? */
        if (mc_mac_equal(mac, mac_faked)) {
            printf("It's the same MAC!!\n");
        }
    }

    /* Memory free */
    mc_mac_free(mac);
    mc_mac_free(mac_faked);
    mc_mac_free(mac_permanent);
    mc_net_info_free(net);
    mc_maclist_free();

    return (ret == 0) ? EXIT_OK : EXIT_ERROR;
}