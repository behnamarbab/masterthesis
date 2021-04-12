static void update_bitmap_score(struct queue_entry* q) {
  u32 i;
  long double fav_factor = q->total_icnt * q->len;
  u64 fav_factor_classic = q->exec_us * q->len;

  /* For every byte set in icnt_bits[], see if there is a previous winner,
     and how it compares to us. */
  for (i = 0; i < ICNT_SIZE; i++)
    if (icnt_bits[i]) {
      if (top_rated_icnt[i]) {
         /* Faster-executing or smaller test cases are favored. */
        if (fav_factor > top_rated_icnt[i]->total_icnt * top_rated_icnt[i]->len) continue;
        if (fav_factor_classic > top_rated_icnt[i]->exec_us * top_rated_icnt[i]->len) continue;
      }

      top_rated_icnt[i] = q;
      score_changed = 1;
    }
  return;
}