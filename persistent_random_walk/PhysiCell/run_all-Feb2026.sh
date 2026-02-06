# Need: mkdir output_Feb2026
# Before running: rm save_Feb2026.csv (or else append)
# After running, prepend header to top; delete possible empty line at end
# for i in $(seq 0 1); do
#for i in $(seq 0 999); do
for i in $(seq 100 999); do
  echo "----------- run: $i"
  project_multiruns-Feb2026 p_motion_no_save_Feb2026.xml 1 0.0 $i
  cat pc_combined_tracks_random_at_origin.csv >> save_Feb2026.csv
done

echo ""
echo "---> save_Feb2026.csv"