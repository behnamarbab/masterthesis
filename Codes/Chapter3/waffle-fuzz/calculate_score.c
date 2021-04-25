static u32 calculate_score(struct queue_entry* q) {
  u32 perf_score = 100;
  u64 avg_total_icnt = total_icnt / total_bitmap_entries;
  // ...
  if (q->total_icnt > avg_total_icnt * 3) perf_score *= 1.4;
  else if(q->total_icnt > avg_total_icnt * 2) perf_score *= 1.2;
  else if(q->total_icnt > avg_total_icnt) perf_score *= 0.85;
  else if(q->total_icnt > avg_total_icnt * 0.75) perf_score *= 0.7;
  else perf_score *= 0.5;
  // ...
}