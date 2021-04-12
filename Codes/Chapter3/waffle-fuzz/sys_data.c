struct sys_data {
  int MaxInstCount;
};

struct sys_data* mem_data;                /* SHM (wcventure) - behnamarbab  */

void ReadMemStatus(){
  total_icnt_cur = mem_data->MaxInstCount;
  if(total_icnt_cur > max_total_icnt) {
    max_total_icnt = total_icnt_cur;
  }
}