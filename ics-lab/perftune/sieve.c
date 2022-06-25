#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  assert(n + 1 < N);
  int index=1;
  for (int i = 3; i <= n; i+=2){
    is_prime[index] = true;
    index++;
  }

  for (int i = 3; i <= n/2; i+=2) {
    if(i>10000) break;
    if(is_prime[(i-1)>>1]==true){
      int i2=i<<1;
      //index=(i+i2-1)>>1;
      
      for (int j = i*i; j <= n; j += i2) {
        is_prime[(j-1)>>1] = false;

        //index+=i;
      }
    }
  }

  int *p = primes;
  *p++=2;
  index=1;
  for (int i = 3; i <= n; i+=2){
    if (is_prime[index]) {
      *p++ = i;
    }
    index++;
  }

  *p = 0;
  return primes;
}
