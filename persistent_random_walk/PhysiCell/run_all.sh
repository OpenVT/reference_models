for i in $(seq 0 99); do
  echo "----------- run: $i"
  project_multiruns p_motion_no_save_Jan2026.xml 1 0.0 $i
  cat pc_combined_tracks_random_at_origin.csv >> save_Jan2026.csv
done

