/*
   LABORATÓRIO DE SISTEMAS OPERACIONAIS - CEFET MG
   Professor: Bruno

   Alunos: Guilherme Giacomin e Uriel Braga
*/

/* 
   fonts:
      https://shanetully.com/2014/12/translating-virtual-addresses-to-physcial-addresses-in-user-space/
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>

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



int main (){

	int a = 1;

   int retval ;
   retval = fork () ;

   if ( retval < 0 )
   {
      perror ("Erro") ;
      exit (1) ;
   }
   else
      if ( retval > 0 )
			wait (0);
      else
         sleep (5) ;

	get_page_frame_number_of_address(&a);
	unsigned long page_frame_number = get_page_frame_number_of_address(&a);

   // Find the difference from the address to the page boundary
   unsigned int distance_from_page_boundary = (unsigned long)&a % getpagesize();

   // Determine how far to seek into memory to find the adress
   uint64_t offset = (page_frame_number << PAGE_SHIFT) + distance_from_page_boundary;

   printf("\nProcesso %5d\nEndereço logico: %p\nPage frame: 0x%lx\nEndereço Fisico: %lx\n\n", getpid(), &a, page_frame_number, offset);

   exit(0);

}
