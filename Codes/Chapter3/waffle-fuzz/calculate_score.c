static u32 calculate_score(struct queue_entry* q) {
  u32 perf_score = 100;
  u64 avg_ORC = ORC / total_bitmap_entries;
  // ...
  if (q->ORC > avg_ORC * 3) perf_score *= 1.4;
  else if(q->ORC > avg_ORC * 2) perf_score *= 1.2;
  else if(q->ORC > avg_ORC) perf_score *= 1.05;
  else if(q->ORC > avg_ORC * 0.75) perf_score *= 0.7;
  else perf_score *= 0.5;
  // ...
}