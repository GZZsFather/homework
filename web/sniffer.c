#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <headers.h>
#include <pcap/pcap.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>

#define ETHERNET_IP 0x0800      // Ethernet IP Protocol
#define ETHERNET_ARP 0x0806     // Ethernet ARP Protocol
#define ETHERNET_RARP 0x8035    // Ethernet RARP Protocol
#define ETHERNET_IPv6 0x86DD    // Ethernet IPv6 Protocol

void process_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
void print_ip_header(const u_char *, int);
void print_icmp_packet(const u_char *, int);
void print_tcp_packet(const u_char *, int);
void print_udp_packet(const u_char *, int);
void print_data(const u_char *, int);

FILE *logfile;
struct sockaddr_in source, dest;
int arp=0, rarp=0, icmp=0, igmp=0, ip=0, tcp=0, egp=0, igp=0, udp=0, ipv6=0, esp=0, ospf=0, others=0, total=0, i, j;

int main() {
    pcap_if_t *alldevsp, *dev;
    pcap_t *handle;

    char errbuf[100], *devname, devs[100][100];
    int count = 1, n;

    printf ("Finding available devices ... ");
    if (pcap_findalldevs(&alldevsp, errbuf)) {
        printf("Error finding devices : %s", errbuf);
        exit(EXIT_FAILURE);
    }
    printf("Done\n");

    printf("Available devices:\n");
    for (dev = alldevsp; dev != NULL; dev = dev->next) {
        printf ("%d. %s - %s\n", count, dev->name, dev->description);
        if (dev->name != NULL) {
            strcpy(devs[count], dev->name);
        }
        count++;
    }

    printf ("Select the device. Please enter the number of the device you choose: ");
    scanf("%d", &n);
    devname = devs[n];

    printf("Opening device %s ...", devname);
    handle = pcap_open_live(devname, 65536, 1, 0, errbuf);

    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", devname, errbuf);
        exit(EXIT_FAILURE);
    }
    printf("Done\n");

    logfile = fopen("log.txt", "w");
    if (logfile == NULL) {
        printf("Couldn't create file.\n");
        exit(EXIT_FAILURE);
    }

    pcap_loop(handle, -1, process_packet, NULL);

    return 0;
}

void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *Buffer) {
    int size = header->len;

    struct ethhdr *eth = (struct ethhdr *)Buffer;
    ++total;

    if (ntohs(eth->h_proto) == ETHERNET_ARP) {
        ++arp;
        printf("ARP: %d  RARP: %d  ICMP: %d  IGMP: %d  IP: %d  TCP: %d  EGP: %d  IGP: %d  UDP: %d  IPv6: %d  ESP: %d  OSPF: %d  Others: %d  Total: %d\r",
                arp, rarp, icmp, igmp, ip, tcp, egp, igp, udp, ipv6, esp, ospf, others, total);
    } else if (ntohs(eth->h_proto) == ETHERNET_RARP) {
        ++rarp;
        printf("ARP: %d  RARP: %d  ICMP: %d  IGMP: %d  IP: %d  TCP: %d  EGP: %d  IGP: %d  UDP: %d  IPv6: %d  ESP: %d  OSPF: %d  Others: %d  Total: %d\r",
                arp, rarp, icmp, igmp, ip, tcp, egp, igp, udp, ipv6, esp, ospf, others, total);
    } else if (ntohs(eth->h_proto) == ETHERNET_IP) {
        struct iphdr *iph = (struct iphdr *)(Buffer + sizeof(struct ethhdr));

        switch (iph->protocol) {
            case 1:
                ++icmp;
                print_icmp_packet(Buffer, size);
                break;

            case 2:
                ++igmp;
                break;

            case 6:
                ++tcp;
                print_tcp_packet(Buffer, size);
                break;

            case 8:
                ++egp;
                break;

            case 9:
                ++igp;
                break;

            case 17:
                ++udp;
                print_udp_packet(Buffer, size);
                break;

            case 41:
                ++ipv6;
                break;

            case 50:
                ++esp;
                break;

            case 89:
                ++ospf;
                break;
        }

        printf("ARP: %d  RARP: %d  ICMP: %d  IGMP: %d  IP: %d  TCP: %d  EGP: %d  IGP: %d  UDP: %d  IPv6: %d  ESP: %d  OSPF: %d  Others: %d  Total: %d\r",
                arp, rarp, icmp, igmp, ip, tcp, egp, igp, udp, ipv6, esp, ospf, others, total);
    } else {
        ++others;
        printf("ARP: %d  RARP: %d  ICMP: %d  IGMP: %d  IP: %d  TCP: %d  EGP: %d  IGP: %d  UDP: %d  IPv6: %d  ESP: %d  OSPF: %d  Others: %d  Total: %d\r",
                arp, rarp, icmp, igmp, ip, tcp, egp, igp, udp, ipv6, esp, ospf, others, total);

    }
}

void print_ethernet_header(const u_char *Buffer, int Size) {
    struct ethhdr *eth = (struct ethhdr *)Buffer;

    fprintf(logfile, "\nEthernet Header\n");
    fprintf(logfile, "  |-Protocol           : %u\n", (unsigned short)eth->h_proto);
    fprintf(logfile, "  |-Destination Address: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
            eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    fprintf(logfile, "  |-Source Address     : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
            eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
}

void print_ip_header(const u_char *Buffer, int Size) {
    print_ethernet_header(Buffer, Size);

    struct iphdr *iph = (struct iphdr *)(Buffer + sizeof(struct ethhdr));

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    source.sin_addr.s_addr = iph->daddr;

    fprintf(logfile, "\nIP Header\n");
    fprintf(logfile, "  |-IP Version            : %d\n", (unsigned int)iph->version);
    fprintf(logfile, "  |-IP Header Length      : %d Bytes\n", ((unsigned int)(iph->ihl)*4));
    fprintf(logfile, "  |-Type of Service       : %d\n", (unsigned int)iph->tos);
    fprintf(logfile, "  |-IP Total Length       : %d Bytes\n", (unsigned int)iph->tot_len);
    fprintf(logfile, "  |-Identification        : %d\n", ntohs(iph->id));
    //fprintf(logfile, "  |-Reserved ZERO Field   : %d\n", (unsigned int)iph->ip_reserved_zero);
    //fprintf(logfile, "  |-Don't Fragment Field  : %d\n", (unsigned int)iph->ip_dont_fragment);
    //fprintf(logfile, "  |-More Fragment Field   : %d\n", (unsigned int)iph->ip_more_fragment);
    fprintf(logfile, "  |-Time To Live(TTL)     : %d\n", (unsigned int)iph->ttl);
    fprintf(logfile, "  |-Protocol              : %d\n", (unsigned int)iph->protocol);
    fprintf(logfile, "  |-Checksum              : %d\n", ntohs(iph->check));
    fprintf(logfile, "  |-Source IP             : %s\n", inet_ntoa(source.sin_addr));
    fprintf(logfile, "  |-Destination IP        : %s\n", inet_ntoa(dest.sin_addr));
}

void print_icmp_packet(const u_char *Buffer, int Size) {
    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *)(Buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;

    struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen + sizeof(struct ethhdr));

    int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof(icmph);

    fprintf(logfile, "\n\n-------------------- ICMP Packet --------------------\n");

    print_ip_header(Buffer, Size);

    fprintf(logfile, "\nICMP Header\n");
    fprintf(logfile, "  |-Type:     %d", (unsigned int)(icmph->type));

    switch ((unsigned int)icmph->type) {
        case ICMP_ECHOREPLY:
            fprintf(logfile, " (Echo Reply)\n");
            break;

        case 3:
            fprintf(logfile, " (Destination Unreachable)\n");
            break;

        case 4:
            fprintf(logfile, " (Source Quench)\n");
            break;

        case 5:
            fprintf(logfile, " (Redirect)\n");
            break;

        case 8:
            fprintf(logfile, " (Echo Request)\n");
            break;

        case 9:
            fprintf(logfile, " (Router Advertisement)\n");
            break;

        case 10:
            fprintf(logfile, " (Router Solicitation)\n");
            break;

        case 11:
            fprintf(logfile, " (Time Excceded)\n");
            break;

        case 17:
            fprintf(logfile, " (Address Mask Request)\n");
            break;

        case 18:
            fprintf(logfile, " (Address Mask Reply)\n");
            break;
    }

    fprintf(logfile, "  |-Code:     %d\n", (unsigned int)(icmph->code));
    fprintf(logfile, "  |-Checksum: %d\n", ntohs(icmph->checksum));

    fprintf(logfile, "\nIP Header\n");
    print_data(Buffer, iphdrlen);

    fprintf(logfile, "ICMP Header\n");
    print_data(Buffer + iphdrlen, sizeof(icmph));

    fprintf(logfile, "Data Payload\n");
    print_data(Buffer+header_size, (Size - header_size));

    fprintf(logfile, "----------------------------------------\n");
}

void print_tcp_packet(const u_char *Buffer, int Size) {
    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *)(Buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;

    struct tcphdr *tcph = (struct tcphdr *)(Buffer + iphdrlen + sizeof(struct ethhdr));

    int header_size = sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;

    fprintf(logfile, "\n\n-------------------- TCP Packet --------------------\n");

    print_ip_header(Buffer, Size);

    fprintf(logfile, "\nTCP Header\n");
    fprintf(logfile, "  |-Source Port               : %u\n", ntohs(tcph->source));
    fprintf(logfile, "  |-Destination Port          : %u\n", ntohs(tcph->dest));
    fprintf(logfile, "  |-Sequence Number           : %u\n", ntohl(tcph->seq));
    fprintf(logfile, "  |-Acknowledge Number        : %u\n", ntohl(tcph->ack_seq));
    fprintf(logfile, "  |-Header Length(Data Offset): %d BYTES\n", (unsigned int)tcph->doff*4);
    fprintf(logfile, "  |-URGent Flag               : %d\n", (unsigned int)tcph->urg);
    fprintf(logfile, "  |-ACKnowledgement Flag      : %d\n", (unsigned int)tcph->ack);
    fprintf(logfile, "  |-PuSH Flag                 : %d\n", (unsigned int)tcph->psh);
    fprintf(logfile, "  |-ReSeT Flag                : %d\n", (unsigned int)tcph->rst);
    fprintf(logfile, "  |-SYNchronization Flag      : %d\n", (unsigned int)tcph->syn);
    fprintf(logfile, "  |-FINis Flag                : %d\n", (unsigned int)tcph->fin);
    fprintf(logfile, "  |-Window                    : %d\n", (unsigned int)tcph->window);
    fprintf(logfile, "  |-Checksum                  : %d\n", (unsigned int)tcph->check);
    fprintf(logfile, "  |-Urgent Pointer            : %d\n", (unsigned int)tcph->urg_ptr);

    fprintf(logfile, "\n                    DATA Dump                    \n");

    fprintf(logfile, "IP Header\n");
    print_data(Buffer, iphdrlen);

    fprintf(logfile, "TCP Header\n");
    print_data(Buffer+iphdrlen, tcph->doff*4);

    fprintf(logfile, "Data Payload\n");
    print_data(Buffer+header_size, Size-header_size);

    fprintf(logfile, "----------------------------------------\n");
}

void print_udp_packet(const u_char *Buffer, int Size) {
    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *)(Buffer + sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;

    struct udphdr *udph = (struct udphdr *)(Buffer + iphdrlen + sizeof(struct ethhdr));

    int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof(udph);

    fprintf(logfile, "\n\n-------------------- UDP Packet --------------------\n");

    print_ip_header(Buffer, Size);

    fprintf(logfile, "\nUDP Header\n");
    fprintf(logfile, "  |-Source Port     : %d\n", ntohs(udph->source));
    fprintf(logfile, "  |-Destination Port: %d\n", ntohs(udph->dest));
    fprintf(logfile, "  |-UDP Length      : %d\n", ntohs(udph->len));
    fprintf(logfile, "  |-UDP Checksum    : %d\n", ntohs(udph->check));

    fprintf(logfile, "\nIP Header\n");
    print_data(Buffer, iphdrlen);

    fprintf(logfile, "UDP Header\n");
    print_data(Buffer+iphdrlen, sizeof(udph));

    fprintf(logfile, "Data Payload\n");
    print_data(Buffer+header_size, Size-header_size);

    fprintf(logfile, "\n----------------------------------------\n");
}

void print_data(const u_char *Data, int Size) {
    int i, j;
    for (i = 0; i < Size; i++) {
        if (i != 0 && i%16 == 0) {
            fprintf(logfile, "         ");
            for (j = i - 16; j < i; j++) {
                if (Data[j] >= 32 && Data[j] <= 128) {
                    fprintf(logfile, "%c", (unsigned char)Data[j]);
                } else {
                    fprintf(logfile, ".");
                }
            }
            fprintf(logfile, "\n");
        }

        if (i%16 == 0) {
            fprintf(logfile, "   ");
        }
        fprintf(logfile, " %02X", (unsigned int)Data[i]);

        if (i == Size - 1) {
            for (j = 0; j < 15-i%16; j++) {
                fprintf(logfile, "   ");
            }

            fprintf(logfile, "         ");

            for (j = i-i%16; j <= i; j++) {
                if (Data[j] >= 32 && Data[j] <= 128) {
                    fprintf(logfile, "%c", (unsigned int)Data[j]);
                } else {
                    fprintf(logfile, ".");
                }
            }

            fprintf(logfile, "\n");
        }
    }
}
