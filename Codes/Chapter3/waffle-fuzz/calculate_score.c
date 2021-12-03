// SumOfTERUs is a global variable, holds the sum of all total_ERUs 
static u32 calculate_score(struct queue_entry* q) {
  u32 perf_score = 100;
  u64 avg_ERU = SumOfERUs / total_bitmap_entries;
  // Consider q->exec_us and q->bitmap_size
  // ..
  // Consider TERU
  if (q->TERU > avg_ERU * 3) perf_score *= 1.4;
  else if(q->TERU > avg_ERU * 2) perf_score *= 1.2;
  else if(q->TERU > avg_ERU) perf_score *= 1.05;
  else if(q->TERU > avg_ERU * 0.75) perf_score *= 0.7;
  else perf_score *= 0.5;
  // ...
}