int main(int argc, char** argv) {
  // Initial Setup

  top_rated_icnt = ck_alloc(ICNT_SIZE * sizeof(struct queue_entry *));

  perform_dry_run(use_argv);

  cull_queue();

  show_init_stats();

  while (1) {
    cull_queue();
    if (!queue_cur) {

      queue_cycle++;
      current_entry     = 0;
      cur_skipped_paths = 0;
      // ! About the queue and structures
      queue_cur         = queue;

      prev_queued = queued_paths;
    }
    // ! One here
    skipped_fuzz = fuzz_one(use_argv);

    queue_pre = queue_cur;
	  queue_cur = queue_cur->next;
    current_entry++;
  }

  write_bitmap();
  write_stats_file(0, 0, 0);
  save_auto();

  // Destroy everything

  exit(0);
}