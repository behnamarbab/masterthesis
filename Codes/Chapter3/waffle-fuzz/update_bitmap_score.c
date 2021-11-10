static void update_bitmap_score(struct queue_entry* q) {
  u32 i;
  u64 fav_factor = q->exec_us * q->len;

  /* For every byte set in ERU_list[], see if there is a previous winner,
     and how it compares to us. */
  for (i = 0; i < MAP_SIZE; i++){
    if(trace_bits[i][0]){
      if (top_rated[i][0]) {
        if (fav_factor > top_rated[i][0]->exec_us * top_rated[i][0]->len) {
          if(top_rated[i][1]) {
            // Ignore the fav_factor
            if(!var_bytes[i] || top_rated[i][1]->TERU >= q->TERU) {
              continue;
            }
          }
          top_rated[i][1] = q;
          score_changed = 2;
          continue;
        }
        // ...
        top_rated[i][0] = q;
        score_changed = 1;
      }
    }
  }
  return;
}