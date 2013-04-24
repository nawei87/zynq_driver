#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
  
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define BRAM0_PHYS 0x80400000
#define BRAM1_PHYS 0x80410000

int main(int argc, char **argv) {
    int fd;
    void *map_base0, *map_base1, *ctrl, *cnt, *addr, *conf, *src, *stat, *dst;  //*virt_addr; 
//	unsigned long read_result, writeval;
//	off_t target;
//	int access_type = 'w';
	
//	if(argc < 2) {
//		fprintf(stderr, "\nUsage:\t%s { address } [ type [ data ] ]\n"
//			"\taddress : memory address to act upon\n"
//			"\ttype    : access operation type : [b]yte, [h]alfword, [w]ord\n"
//			"\tdata    : data to be written\n\n",
//			argv[0]);
//		exit(1);
//	}
//	target = strtoul(argv[1], 0, 0);
//
//	if(argc > 2)
//		access_type = tolower(argv[2][0]);
    
    if((fd = open("/dev/vf-driver", O_RDWR | O_SYNC)) == -1) FATAL;
    printf("/dev/vf-driver opened.\n"); 
    fflush(stdout);
    
    /* Map one page of bram 0*/
    map_base0 = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BRAM0_PHYS & ~MAP_MASK);
    if(map_base0 == (void *) -1) FATAL;
    printf("Memory 0 mapped at address %p.\n", map_base0); 
    fflush(stdout);
    
    ctrl= map_base0 + (BRAM0_PHYS & MAP_MASK);
    cnt = map_base0 + (BRAM0_PHYS & MAP_MASK) + 0x4;
    addr= map_base0 + (BRAM0_PHYS & MAP_MASK) + 0x8;
    conf= map_base0 + (BRAM0_PHYS & MAP_MASK) + 0xc;
    src = map_base0 + (BRAM0_PHYS & MAP_MASK) + 0x100; 

    /* Map one page of bram 1*/
    map_base1 = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BRAM1_PHYS & ~MAP_MASK);
    if(map_base1 == (void *) -1) FATAL;
    printf("Memory 1 mapped at address %p.\n", map_base1); 
    fflush(stdout);

    stat= map_base1 + (BRAM0_PHYS & MAP_MASK);
    dst = map_base1 + (BRAM0_PHYS & MAP_MASK) + 0x100;
//    virt_addr = map_base + (VF_PHYS & MAP_MASK);
//    switch(access_type) {
//		case 'b':
//			read_result = *((unsigned char *) virt_addr);
//			break;
//		case 'h':
//			read_result = *((unsigned short *) virt_addr);
//			break;
//		case 'w':
//			read_result = *((unsigned long *) virt_addr);
//			break;
//		default:
//			fprintf(stderr, "Illegal data type '%c'.\n", access_type);
//			exit(2);
//	}
//    printf("Value at address 0x%X (%p): 0x%X\n", target, virt_addr, read_result); 
//    fflush(stdout);
//
//	if(argc > 3) {
//		writeval = strtoul(argv[3], 0, 0);
//		switch(access_type) {
//			case 'b':
//				*((unsigned char *) virt_addr) = writeval;
//				read_result = *((unsigned char *) virt_addr);
//				break;
//			case 'h':
//				*((unsigned short *) virt_addr) = writeval;
//				read_result = *((unsigned short *) virt_addr);
//				break;
//			case 'w':
//				*((unsigned long *) virt_addr) = writeval;
//				read_result = *((unsigned long *) virt_addr);
//				break;
//		}
//		printf("Written 0x%X; readback 0x%X\n", writeval, read_result); 
//		fflush(stdout);
//	}
	
	if(munmap(map_base0, MAP_SIZE) == -1) FATAL;
	if(munmap(map_base1, MAP_SIZE) == -1) FATAL;
    close(fd);
    return 0;
}

