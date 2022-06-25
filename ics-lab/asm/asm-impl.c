#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  int64_t ans=b;
  asm (
    "add %[a] , %[t]"
  :[t] "+r" (ans)
  :[a] "r" (a)
  );
 // ans=a+b;
  return ans;
}


int asm_popcnt(uint64_t x) {
  int s = 0;
  
  // for (int i = 0; i < 64; i++) {
  //   if ((x >> i) & 1) s++;
  // }
  
  int64_t i=0;
  asm(
  ".label:\n\t"
  "mov %%rdi,%%rax\n\t"
  "shr %b[i],%%rax\n\t"
  "and $0x1,%%eax\n\t"
  "add %%eax,%[s]\n\t"
  "add $0x1,%[i]\n\t"
  "cmp $0x40,%[i]\n\t"
  "jne .label\n\t"
  "mov %[s],%%eax\n\t"

  :[s] "+r" (s),[i] "+c" (i)
  :
  : "%rax"
  );
  
  
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  // char *s_out=(char*) dest;
  // char *s_in=(char*) src;
  // size_t i=0;
  // for(;i<n;i++){
  //   s_out[i]=s_in[i];
  // }
  
  size_t i=0;
  char dl='0';
  __asm__ __volatile__ (
    ".labela:\n\t"
    "movb (%[s],%[i],1), %[dl]\n\t"
    "mov  %[dl] , (%[d],%[i],1)\n\t"
    "add $0x1,%[i]\n\t"
    "cmp %[i],%[n]\n\t"
    "jne .labela\n\t"
    "mov %[d] , %[i]\n\t"
    :[i] "+r" (i),[dl] "+r" (dl)
    : [s] "r" (src), [d] "r" (dest),[n] "r" (n)
    
  );
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  asm(
    "mov %%rbx, (%[e])\n\t"
    "pop %%rsi\n\t"
    "mov %%rsp,8(%[e])\n\t"
    "push %%rsi\n\t"
    "mov %%rbp,16(%[e])\n\t"
    "mov %%r12,24(%[e])\n\t"
    "mov %%r13,32(%[e])\n\t"
    "mov %%r14,40(%[e])\n\t"
    "mov %%r15,48(%[e])\n\t"
    "mov %%rsi,56(%[e])\n\t"
    :
    :[e] "r" (env)
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm(
    "mov  (%[e]) , %%rbx\n\t"
    "mov 8(%[e]), %%rsp\n\t"
    "mov 16(%[e]), %%rbp\n\t"
    "mov 24(%[e]), %%r12\n\t"
    "mov 32(%[e]), %%r13\n\t"
    "mov 40(%[e]), %%r14\n\t"
    "mov 48(%[e]), %%r15\n\t"
    "mov %[v], %%eax\n\t"
    "jmp *56(%[e])\n\t"
    :
    :[e] "r" (env), [v] "r" (val)
  );

}
