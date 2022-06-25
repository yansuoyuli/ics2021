#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

static uint8_t* cache=NULL;
static int32_t  asso=0;    
static int32_t  asso_width=0;
static int32_t  lines=0;
static int32_t  lines_width=0,team_width=0,tag_width=0;



uint32_t cache_read(uintptr_t addr) {
  uint8_t in_addr=addr&0x3f& ~0x3;
  uint32_t asso_num=(addr>>6)&((1<<team_width)-1);
  uint32_t tag=((addr>>6)>>team_width)&((1<<tag_width)-1);
  uint32_t begin_index=asso*asso_num*69-69;
  //printf("%d\n",begin_index);
  for(int i=0;i<asso;i++){
    begin_index += 69;
    bool is_valid=cache[begin_index]&1;
    uint32_t *t_addr=(uint32_t *)(cache+begin_index+1);
    uint32_t cache_tag=*t_addr;
    if((cache_tag==tag)&&is_valid){
      uint32_t *read_addr=(uint32_t*)(cache+begin_index+5+in_addr);
      //return *(uint32_t *)((((uintptr_t)read_addr>>2)<<2)+1);
      return *read_addr;
    }
  }
  // for(int i=0;i<asso;i++){
  //   begin_index += 69*i;
  //   bool is_valid=cache[begin_index]&1;
  //   if(!is_valid){
  //     cache[begin_index]=cache[begin_index]|1;
  //     uint32_t *t_addr=(uint32_t *)(cache+begin_index+1);
  //     *t_addr=tag;
  //     mem_read(addr>>6,cache+begin_index+5);
  //     uint32_t *read_addr=(uint32_t*)(cache+begin_index+5+in_addr);
  //     return *(uint32_t *)(((uintptr_t)read_addr>>2)<<2);
  //   }
  // }
  begin_index = asso*asso_num*69;
  //printf("%d\n",begin_index);
  uint32_t ran=rand()%asso;
  begin_index += 69*ran;
  bool is_valid=cache[begin_index]&1;
  bool is_dirty=cache[begin_index]&2;
  if(is_dirty){
    uint32_t *t_ad=(uint32_t *)(cache+begin_index+1);
    uint32_t c_tag=*t_ad;
    mem_write((c_tag<<team_width)|asso_num, cache+begin_index+5);
  }
  cache[begin_index]=1;
  //cache[begin_index]=cache[begin_index]&0xfd;
  uint32_t *t_addr=(uint32_t *)(cache+begin_index+1);
  *t_addr=tag;
  mem_read((tag<<team_width)|asso_num,cache+begin_index+5);
  uint32_t *read_addr=(uint32_t*)(cache+begin_index+5+in_addr);
  // printf("0x%08x\n",*(uint32_t *)(uintptr_t)read_addr);
  // printf("--0x%08x\n",*(uint32_t *)((uintptr_t)read_addr-1));
  // printf("++0x%08x\n",*(uint32_t *)((uintptr_t)read_addr+1));
  //return *(uint32_t *)((((uintptr_t)read_addr>>2)<<2));
  return *read_addr;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint8_t in_addr=addr&0x3f & ~0x3;
  uint32_t asso_num=(addr>>6)&((1<<team_width)-1);
  uint32_t tag=((addr>>6)>>team_width)&((1<<tag_width)-1);
  uint32_t begin_index=asso*asso_num*69-69;
  //printf("%d\n",begin_index);
  for(int i=0;i<asso;i++){
    begin_index += 69;
    bool is_valid=cache[begin_index]&1;
    uint32_t *t_addr=(uint32_t *)(cache+begin_index+1);
    uint32_t cache_tag=*t_addr;
    if((cache_tag==tag)&&is_valid){
      cache[begin_index] = cache[begin_index]|0x2;
      uint32_t write_data=data&wmask;
      uint32_t *write_addr=(uint32_t*)(cache+begin_index+5+in_addr);
      *write_addr = (*write_addr)&(~wmask);
      *write_addr = (*write_addr)|write_data;
      return ;
    }
  }
  // for(int i=0;i<asso;i++){
  //   begin_index += 69*i;
  //   bool is_valid=cache[begin_index]&1;
  //   if(!is_valid){
  //     cache[begin_index]=cache[begin_index]|1;
  //     uint32_t *t_addr=(uint32_t *)(cache+begin_index+1);
  //     *t_addr=tag;
  //     mem_read(addr>>6,cache+begin_index+5);
  //     uint32_t write_data=data&wmask;
  //     uint32_t *write_addr=(uint32_t*)(cache+begin_index+5+in_addr);
  //     *write_addr = *write_addr&(~wmask);
  //     *write_addr = *write_addr|write_data;
  //     return ;
  //   }
  // }
  begin_index = asso*asso_num*69;
  //printf("%d\n",begin_index);
  uint32_t write_data=data&wmask;
  // uint32_t *write_addr=(uint32_t*)addr;
  // *write_addr = *write_addr&(~wmask);
  // *write_addr = *write_addr|write_data;
  uint32_t ran=rand()%asso;
  begin_index += 69*ran;
  bool is_valid=cache[begin_index]&1;
  bool is_dirty=cache[begin_index]&2;
  if(is_dirty){
    uint32_t *t_ad=(uint32_t *)(cache+begin_index+1);
    uint32_t c_tag=*t_ad;
    mem_write((c_tag<<team_width)|asso_num, cache+begin_index+5);
    // cache[begin_index]=cache[begin_index]|1;
    // cache[begin_index]=cache[begin_index]&0xfd;
  }
  
  uint32_t *t_addr=(uint32_t *)(cache+begin_index+1);
  *t_addr=tag;
  mem_read((tag<<team_width)|asso_num,cache+begin_index+5);
  uint32_t *write_addr=(uint32_t*)(cache+begin_index+5+in_addr);
  *write_addr = (*write_addr)&(~wmask);
  *write_addr = (*write_addr)|write_data;
  cache[begin_index]=3;
  //cache[begin_index]=cache[begin_index]|0x2;
  return ;
}

void init_cache(int total_size_width, int associativity_width) {
  int total_data_size=1<<total_size_width;  
  lines=total_data_size>>6;
  lines_width=total_size_width-6;
  asso=1<<associativity_width;
  asso_width=associativity_width;
  team_width=lines_width-asso_width;
  tag_width=25-6-team_width;
  cache=(uint8_t *)malloc(69*lines);
  for(int i=0;i<69*lines;i++){
    cache[i]=0;
  }
}

void display_statistic(void) {
}
