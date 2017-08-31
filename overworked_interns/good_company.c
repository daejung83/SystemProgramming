/**
 * Overworked Interns Lab
 * CS 241 - Spring 2016
 */
#include "company.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *work_interns(void *p) { 
  Company *company = (Company *)p;
  pthread_mutex_t *left_intern, *right_intern;
  left_intern = Company_get_left_intern(company);
  right_intern = Company_get_right_intern(company);
  pthread_mutex_t *left, *right;
  if(left_intern < right_intern){
    left = right_intern;
    right = left_intern;
  }else{
    left = left_intern;
    right = right_intern;
  }
  
  while(running){
      pthread_mutex_lock(left);
      pthread_mutex_lock(right);
      Company_hire_interns(company);
      pthread_mutex_unlock(right);
      pthread_mutex_unlock(left);
      Company_have_board_meeting(company);
      
  }

  return NULL; 
}
