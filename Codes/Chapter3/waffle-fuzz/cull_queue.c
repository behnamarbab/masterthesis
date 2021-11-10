static void cull_queue(void) {

  struct queue_entry* q, * tmpq;
  static u8 temp_v[MAP_SIZE >> 3];
  /* The visited var_bytes */
  static u8 temp_vv[MAP_SIZE];
  u32 i;
  memset(temp_v, 255, MAP_SIZE >> 3);
  memcpy(temp_vv, var_bytes, MAP_SIZE);
  q = queue;

  while (q) {
    q->favored = 0;
    q = q->next;
  }

  for (i = 0; i < MAP_SIZE; i++)
    if (top_rated[i][0] && (temp_v[i >> 3] & (1 << (i & 7)))) {
      u32 j = MAP_SIZE >> 3;
      /* Remove all bits belonging to the current entry from temp_v. */
      while (j--) 
        if (top_rated[i][0]->trace_mini[j])
          temp_v[j] &= ~top_rated[i][0]->trace_mini[j];
      top_rated[i][0]->favored = 1;
    }
    else if(top_rated[i][1] && temp_vv[i]) {
      u32 j = MAP_SIZE;
      /* Remove all bits belonging to the current entry from temp_vv. */
      while (j--)
        if(var_bytes[j] && top_rated[j][1])
          if(top_rated[i][1]->nTERU == top_rated[j][1]->nTERU)
            temp_vv[j] = 0;
      top_rated[i][1]->favored = 2;
    }

  q = queue;
  tmpq = q;

  while (q) {
    if(q->favored==2 && q->nTERU >= tmpq->nTERU) {
      tmpq->favored = 0;
      q->favored = 1;
      tmpq = q;
    }
    mark_as_redundant(q, !q->favored);
    q = q->next;
  }
}