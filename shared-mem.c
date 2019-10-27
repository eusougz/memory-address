/*
   LABORATÓRIO DE SISTEMAS OPERACIONAIS - CEFET MG
   Professor: Bruno

   Alunos: Guilherme Giacomin e Uriel Braga
*/

// Implemente um programa que mostre, utilizando memória compartilhada, 
// que dois processos podem ter diferentes endereços lógicos apontando para o mesmo endereço físico.


/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */


/* 
   Conversor logical adress to physical address
   font: https://shanetully.com/2014/12/translating-virtual-addresses-to-physcial-addresses-in-user-space/
*/


#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

// The page frame shifted left by PAGE_SHIFT will give us the physcial address of the frame
// Note that this number is architecture dependent. For me on x86_64 with 4096 page sizes,
// it is defined as 12. If you're running something different, check the kernel source
// for what it is defined as.
#define PAGE_SHIFT 12
#define PAGEMAP_LENGTH 8



unsigned long get_page_frame_number_of_address(void *addr) {
   // Open the pagemap file for the current process
   FILE *pagemap = fopen("/proc/self/pagemap", "rb");

   // Seek to the page that the buffer is on
   unsigned long offset = (unsigned long)addr / getpagesize() * PAGEMAP_LENGTH;
   if(fseek(pagemap, (unsigned long)offset, SEEK_SET) != 0) {
      fprintf(stderr, "Failed to seek pagemap to proper location\n");
      exit(1);
   }

   // The page frame number is in bits 0-54 so read the first 7 bytes and clear the 55th bit
   unsigned long page_frame_number = 0;
   fread(&page_frame_number, 1, PAGEMAP_LENGTH-1, pagemap);

   page_frame_number &= 0x7FFFFFFFFFFFFF;

   fclose(pagemap);

   return page_frame_number;
}

int main()
{
	const char *name = "shared_memory";
	const int SIZE = 4096;

	int shm_fd;
	void *ptr;
	int i;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the 
	address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}
	
	printf("Endereco logico: %p\n", ptr);

   get_page_frame_number_of_address(ptr);
	unsigned long page_frame_number = get_page_frame_number_of_address(ptr);
   printf("Page frame: %lu\n", page_frame_number);

   // Find the difference from the address to the page boundary
   unsigned int distance_from_page_boundary = (unsigned long)ptr % getpagesize();

   // Determine how far to seek into memory to find the adress
   uint64_t offset = (page_frame_number << PAGE_SHIFT) + distance_from_page_boundary;

   /* now read from the shared memory region */
	printf("%s\n",(char*)ptr);
	printf("%d\n", *((int*)(ptr+strlen(ptr)+1))  );

   printf("Endereco fisico: %lx\n", offset);


	return 0;
}
