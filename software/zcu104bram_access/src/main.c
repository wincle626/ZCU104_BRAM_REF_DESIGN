/*
 * main.c
 *
 *  Created on: 24 Aug 2021
 *      Author: UDRC
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long int u64;

typedef struct{
	u16 real;
	u16 imag;
} cplx16;

typedef struct{
	u32 real;
	u32 imag;
} cplx32;

typedef struct{
	u64 real;
	u64 imag;
} cplx64;

void access128(){

	unsigned int bram_size = 0x2000;
	off_t bram_pbase = 0xA0000000; // physical base address
	cplx64 *bram128_vptr;
	int fd;

	// Map the BRAM physical address into user space getting a virtual address for it
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {

		bram128_vptr = (cplx64 *) mmap(NULL, bram_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, bram_pbase);

		// Write to the memory that was mapped, use devmem from the command line of Linux to verify it worked
		// it could be read back here also

		bram128_vptr[0].real = 0xDEADBEEFFACEB00A;
		bram128_vptr[0].imag = 0xDEADBEEFFACEB00D;
		bram128_vptr[1].real = 0xDEADBEEFFACEB00B;
		bram128_vptr[1].imag = 0xDEADBEEFFACEB00C;
		bram128_vptr[2].real = 0xDEADBEEFFACEB00C;
		bram128_vptr[2].imag = 0xDEADBEEFFACEB00B;
		bram128_vptr[3].real = 0xDEADBEEFFACEB00D;
		bram128_vptr[3].imag = 0xDEADBEEFFACEB00A;

	}

	printf("Data %d - (0x%lX, 0x%lX) \n", 0, bram128_vptr[0].real, bram128_vptr[0].imag);
	printf("Data %d - (0x%lX, 0x%lX) \n", 1, bram128_vptr[1].real, bram128_vptr[1].imag);
	printf("Data %d - (0x%lX, 0x%lX) \n", 2, bram128_vptr[2].real, bram128_vptr[2].imag);
	printf("Data %d - (0x%lX, 0x%lX) \n", 3, bram128_vptr[3].real, bram128_vptr[3].imag);

	close(fd);

}

void access64(){

	unsigned int bram_size = 0x2000;
	off_t bram_pbase = 0xA0004000; // physical base address
	cplx32 *bram64_vptr;
	int fd;

	// Map the BRAM physical address into user space getting a virtual address for it
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {

		bram64_vptr = (cplx32 *) mmap(NULL, bram_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, bram_pbase);

		// Write to the memory that was mapped, use devmem from the command line of Linux to verify it worked
		// it could be read back here also

		bram64_vptr[0].real = 0xFACEB00A;
		bram64_vptr[0].imag = 0xFACEB00D;
		bram64_vptr[1].real = 0xFACEB00B;
		bram64_vptr[1].imag = 0xFACEB00C;
		bram64_vptr[2].real = 0xFACEB00C;
		bram64_vptr[2].imag = 0xFACEB00B;
		bram64_vptr[3].real = 0xFACEB00D;
		bram64_vptr[3].imag = 0xFACEB00A;

	}

	printf("Data %d - (0x%X, 0x%X) \n", 0, bram64_vptr[0].real, bram64_vptr[0].imag);
	printf("Data %d - (0x%X, 0x%X) \n", 1, bram64_vptr[1].real, bram64_vptr[1].imag);
	printf("Data %d - (0x%X, 0x%X) \n", 2, bram64_vptr[2].real, bram64_vptr[2].imag);
	printf("Data %d - (0x%X, 0x%X) \n", 3, bram64_vptr[3].real, bram64_vptr[3].imag);

	close(fd);

}

void access32(){

	unsigned int bram_size = 0x2000;
	off_t bram_pbase = 0xA0006000; // physical base address
	cplx16 *bram32_vptr;
	int fd;

	// Map the BRAM physical address into user space getting a virtual address for it
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {

		bram32_vptr = (cplx16 *) mmap(NULL, bram_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, bram_pbase);

		// Write to the memory that was mapped, use devmem from the command line of Linux to verify it worked
		// it could be read back here also

		bram32_vptr[0].real = 0xB00A;
		bram32_vptr[0].imag = 0xB00D;
		bram32_vptr[1].real = 0xB00B;
		bram32_vptr[1].imag = 0xB00C;
		bram32_vptr[2].real = 0xB00C;
		bram32_vptr[2].imag = 0xB00B;
		bram32_vptr[3].real = 0xB00D;
		bram32_vptr[3].imag = 0xB00A;

	}

	printf("Data %d - (0x%X, 0x%X) \n", 0, bram32_vptr[0].real, bram32_vptr[0].imag);
	printf("Data %d - (0x%X, 0x%X) \n", 1, bram32_vptr[1].real, bram32_vptr[1].imag);
	printf("Data %d - (0x%X, 0x%X) \n", 2, bram32_vptr[2].real, bram32_vptr[2].imag);
	printf("Data %d - (0x%X, 0x%X) \n", 3, bram32_vptr[3].real, bram32_vptr[3].imag);

	close(fd);

}

int main(int argc, char** argv){

	int page_size = sysconf(_SC_PAGESIZE);
	printf("Page size is %d\n", page_size);

	access128();
	access64();
	access32();
	return 0;
}
