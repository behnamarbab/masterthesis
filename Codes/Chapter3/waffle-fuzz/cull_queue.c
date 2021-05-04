static void cull_queue(void) {
  struct queue_entry* q;
  u32 i;

  if (dumb_mode || !score_changed) return;

  score_changed = 0;

  queued_favored  = 0;
  pending_favored = 0;

  q = queue;

  while (q) {
    q->favored = 0;
    q = q->next;
  }

  for (i = 0; i < ICNT_SIZE; i++) {
    if (top_rated_icnt[i]) {
      /* if top rated for any i, will be favored */
      u8 was_favored_already = top_rated_icnt[i]->favored;

      top_rated_icnt[i]->favored = 1;

      /* increments counts only if not also favored for another i */
      if (!was_favored_already){
        queued_favored++;
        if (!top_rated_icnt[i]->was_fuzzed) pending_favored++;
      }
    }
  }

  q = queue;

  while (q) {
    mark_as_redundant(q, !q->favored);
    q = q->next;
  }

}