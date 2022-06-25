#include <stdint.h>
//#include <stdio.h>
//#include <inttypes.h>

uint64_t mod(uint64_t x,uint64_t y){
  /*if(x<y){
    return x;
  }
  else if(x==y){
    return 0;
  }

  uint64_t pre_y=y;
  uint64_t y_copy=y;
  while (1)
  {
    if(y_copy>=x){
      break;
    }
    pre_y=y_copy;
    y_copy=y_copy<<1;
  }
  return mod(x-pre_y,y);

  */

  if(x==0||y==0){
    return 0;
  }
  uint64_t max=0xffffffffffffffff;
  int n=0;
  uint64_t pre_y=y;
  uint64_t y_copy=y;
  while (1)
  {
    if(y_copy>x){
      if(n>0){
        n--;
      }
      else{
        return x;
      }
      break;
    }
    pre_y=y_copy;
    if(!(y_copy>max-y_copy)){
      y_copy=y_copy<<1;
      n++;
    }
    else{
      break;
    }
  }
  //printf("%d!!!",n);
  uint64_t x_copy=x;
  while(n>=-1){
    //int xx=x_copy;
    //printf("%x..",xx);
    if(x_copy<y){
      //int xx=x;
      //int yy=y;
      //int xc=x_copy;
      //printf("%d,%d,%d!!!",xx,yy,xc);
      return x_copy;
    }
    else if(x_copy==y){
      //int xx=x;
      //int yy=y;
      //int xc=0;
      //printf("%d,%d,%d!!!",xx,yy,xc);
      return 0;
    }
    
    if(x_copy>=pre_y){
      x_copy-=pre_y; 
    }
    pre_y=pre_y>>1;
    n--;
  }
  //printf("eq\n");
  //int xx=x;
  //int yy=y;
  //int xc=0;
  //printf("%d,%d,%d!!!",xx,yy,xc);
  return 0;
}


uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  if(m==0||a==0||b==0){
    return 0;
  }
  //return (a * b) % m; // buggy
  //uint64_t bb=b;
  int ai[64];
  uint64_t a_copy=a;
  int index=0;
  ai[0]=0;
  while(a_copy){
    if(a_copy&1){
      ai[index]=1;
    }
    else{
      ai[index]=0;
    }
    a_copy=a_copy>>1;
    index+=1;
  }
  
  uint64_t res=0;
  uint64_t max=0xffffffffffffffff;
  if(b>=m){
    b=mod(b,m);
  }
  if(ai[0]==1){
    res+=b;
  }
  for(int i=1;i<index;i++){
    if(b>max-b){
      b=b-(max-b)+max-m;
    }
    else{
      b=b<<1;
      if(b>=m){
        b-=m;
      }
    }
    if(ai[i]==1){
      if(res>max-b){
        res=res-(max-b)+max-m;
      }
      else{
        res+=b;
        if(res>=m){
          res-=m;
        }
      }
    }
  }
  //int aa=(int)a*bb;
  //int ans=aa%m;
  //printf(",,,,,%d  ",ans);
  return res;
}
