static void update_bitmap_score(struct queue_entry* q) {
  u32 i;
  const u32 MULTIPLIER = 1.10;
  u64 fav_factor = q->exec_us * q->len;
  
  find_elite(q);

  /* For every byte set in ORC_list[], see if there is a previous winner,
     and how it compares to us. */
  for (i = 0; i < MAP_SIZE; i++){
    if(trace_bits[i]){
      if (top_rated[i]) {
        /* ORC: check if the path brings something new */
        if(q->elite_index == i) {
          if (fav_factor * MULTIPLIER > top_rated[i]->exec_us * top_rated[i]->len) continue;
          if (top_rated[i].elite_ORC * MULTIPLIER > q->elite_ORC) continue;
        }
        /* Coverage: Faster-executing or smaller test cases are favored. */
        else if (fav_factor > top_rated[i]->exec_us * top_rated[i]->len) continue;
      
        // ...
        top_rated[i] = q;
        score_changed = 1;
      }
    }
  }
  return;
}