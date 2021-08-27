/*
 * main.c
 *
 *  Created on: 26 Aug 2021
 *      Author: UDRC
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>

typedef struct{
	unsigned long int real;
	unsigned long int imag;
}cmplx64;

int main(int argc, char** argv){

	unsigned int bram_size = 0x2000;
	unsigned int addr_phy = 0xA0000000;
	unsigned int data_in32 = 0xEAFB040A;
	unsigned long int data_in64 = 0xEAFB040AEAFB040A;
	cmplx64 data_in128 = {0xEAFB040AEAFB040A, 0xEAFB040AEAFB040A};
	int data_len = 1, data_width = 32;
	int fd, i;

	if(argc< 2){

		printf("**********Usage***********\n"
			   "Input 1 (opt): Physical Address \n"
			   "Input 2 (opt): Data to Write \n"
			   "Input 3 (opt): Data width \n"
			   "**************************\n"
				);
		return 1;

	}else if(argc == 2){

		sscanf(argv[1], "%X", &addr_phy);

	}else if(argc == 3){

		sscanf(argv[1], "%X", &addr_phy);
		sscanf(argv[2], "%d", &data_len);

	}else if(argc == 4){

		sscanf(argv[1], "%X", &addr_phy);
		sscanf(argv[2], "%d", &data_len);
		sscanf(argv[3], "%d", &data_width);

	}else{

		printf("Too many input!\n");
		return 1;

	}
	// Map the BRAM physical address into user space getting a virtual address for it
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {

		// Write to the memory that was mapped, use devmem from the command line of Linux to verify it worked
		// it could be read back here also
		if(data_width==32){
			unsigned int *bram_vptr;
			bram_vptr = (unsigned int *) mmap(NULL, bram_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_phy);
			for(i=0;i<data_len;i++){
				bram_vptr[i] = data_in32;
				data_in32 += 1;
			}
			for(i=0;i<data_len;i++){
				printf("Data %d - 0x%X\n", i, bram_vptr[i]);
			}
		}else if(data_width==64){
			unsigned long int *bram_vptr;
			bram_vptr = (unsigned long int *) mmap(NULL, bram_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_phy);
			for(i=0;i<data_len;i++){
				bram_vptr[i] = data_in64;
				data_in64 += 1;
			}
			for(i=0;i<data_len;i++){
				printf("Data %d - 0x%lX\n", i, bram_vptr[i]);
			}
		}else if(data_width==128){
			cmplx64 *bram_vptr;
			bram_vptr = (cmplx64 *) mmap(NULL, bram_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_phy);
			for(i=0;i<data_len;i++){
				bram_vptr[i].real = data_in128.real;
				bram_vptr[i].imag = data_in128.imag;
				data_in128.real += 1;
				data_in128.imag += 1;
			}
			for(i=0;i<data_len;i++){
				printf("Data %d - {0x%lX, 0x%lX}\n", i, bram_vptr[i].real, bram_vptr[i].imag);
			}
		}

	}

	return 0;

}

