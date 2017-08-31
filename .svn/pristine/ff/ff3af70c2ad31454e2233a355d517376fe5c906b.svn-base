/**
 * Ideal Indirection Lab
 * CS 241 - Spring 2016
 */
#include <stdio.h>
#include "mmu.h"
#include "kernel.h"

MMU *MMU_create() {
  MMU *mmu = calloc(1, sizeof(MMU));
  PageTable **base_pts = mmu->base_pts;
  for (size_t i = 0; i < MAX_PROCESS_ID; i++) {
    base_pts[i] = PageTable_create();
  }
  mmu->tlb = TLB_create();
  return mmu;
}

void *MMU_get_physical_address(MMU *mmu, void *virtual_address, size_t pid) {
	//split vpn
  size_t va_size = (size_t)virtual_address;
	size_t vpn1 = (0xfff & (va_size >> 39));
	size_t vpn2 = (0xfff & (va_size >> 27));
	size_t vpn3 = (0xfff & (va_size >> 15));
	size_t offset = (0x7fff & (va_size));
  void* key = (void*)((long)virtual_address & 0x3ffffffff8000);

	void* result = TLB_get_physical_address(&(mmu->tlb), key, pid);
	if(result){
    void* final_result = result + offset;
		return final_result;
	}
	MMU_tlb_miss(mmu, virtual_address, pid);

  printf("vpn1: %lu, vpn2: %lu, vpn3: %lu, offset: %lu\n", vpn1, vpn2, vpn3, offset);
	//base
	PageTable* PT1 = mmu->base_pts[pid];
	if(!PT1){
    //printf("am i missing PT1\n");
		//MMU_raise_page_fault(mmu, virtual_address, pid);
		PT1 = ask_kernel_for_frame();
	}
  //pageset1
	PageTable* PT2 = PageTable_get_entry(PT1, vpn1);
	if(!PT2){
		MMU_raise_page_fault(mmu, virtual_address, pid);
		PT2 = ask_kernel_for_frame();
		PageTable_set_entry(PT1, vpn1, (void*)PT2);
	}
  //pageset2
	PageTable* PT3 = PageTable_get_entry(PT2, vpn2);
	if(!PT3){
		MMU_raise_page_fault(mmu, virtual_address, pid);
		PT3 = ask_kernel_for_frame();
		PageTable_set_entry(PT2, vpn2, (void*)PT3);
	}
  //pageset3
	void* mem = PageTable_get_entry(PT3, vpn3);
	if(!mem){
		MMU_raise_page_fault(mmu, virtual_address, pid);
    mem = ask_kernel_for_frame(); 
		PageTable_set_entry(PT3, vpn3, mem);
	}
  //final result
	void* final_result = mem + offset;
	TLB_add_physical_address(&(mmu->tlb), key, pid, mem);	
  return final_result;
}

void MMU_tlb_miss(MMU *mmu, void *address, size_t pid) {
  mmu->num_tlb_misses++;
  printf("Process [%lu] tried to access [%p] and it couldn't find it in the "
         "TLB so the MMU has to check the PAGE TABLES\n",
         pid, address);
}

void MMU_raise_page_fault(MMU *mmu, void *address, size_t pid) {
  mmu->num_page_faults++;
  printf(
      "Process [%lu] tried to access [%p] and the MMU got an invalid entry\n",
      pid, address);
}

void MMU_free_process_tables(MMU *mmu, size_t pid) {
  PageTable *base_pt = mmu->base_pts[pid];
  Pagetable_delete_tree(base_pt);
}

void MMU_delete(MMU *mmu) {
  for (size_t i = 0; i < MAX_PROCESS_ID; i++) {
    MMU_free_process_tables(mmu, i);
  }
  TLB_delete(mmu->tlb);
  free(mmu);
}
