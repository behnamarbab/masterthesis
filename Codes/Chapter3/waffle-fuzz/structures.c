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

struct queue_entry {

  u8* fname;                          /* File name for the test case      */
  u32 len;                            /* Input length                     */

  u8  cal_failed,                     /* Calibration failed?              */
      trim_done,                      /* Trimmed?                         */
      was_fuzzed,                     /* Had any fuzzing done yet?        */
      passed_det,                     /* Deterministic stages passed?     */
      has_new_cov,                    /* Triggers new coverage?           */
      has_new_icnt,                    /* Triggers new max icnt?           */
      var_behavior,                   /* Variable behavior?               */
      favored,                        /* Currently favored?               */
      fs_redundant;                   /* Marked as redundant in the fs?   */

  u32 bitmap_size,                    /* Number of bits set in bitmap     */
      exec_cksum,                     /* Checksum of the execution trace  */
      icnt_cksum;

  u64 exec_us,                        /* Execution time (us)              */
      handicap,                       /* Number of queue cycles behind    */
      depth;                          /* Path depth                       */

  i32 total_icnt;                    /* Total number of instructions     */

  struct queue_entry *next,           /* Next element, if any             */
                     *next_100;       /* 100 elements ahead               */

};

EXP_ST i32 max_total_icnt = 0;
EXP_ST i32 total_icnt_cur = 0;