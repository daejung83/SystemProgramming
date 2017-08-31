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
  return NULL;
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
