// SumOfTORCs is a global variable, holds the sum of all total_ORCs 
static u32 calculate_score(struct queue_entry* q) {
  u32 perf_score = 100;
  u64 avg_ORC = SumOfORCs / total_bitmap_entries;
  // Consider q->exec_us and q->bitmap_size
  // ..
  // Consider TORC
  if (q->TORC > avg_ORC * 3) perf_score *= 1.4;
  else if(q->TORC > avg_ORC * 2) perf_score *= 1.2;
  else if(q->TORC > avg_ORC) perf_score *= 1.05;
  else if(q->TORC > avg_ORC * 0.75) perf_score *= 0.7;
  else perf_score *= 0.5;
  // ...
}