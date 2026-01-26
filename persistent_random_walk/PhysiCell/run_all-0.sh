for i in $(seq 0 99); do
  echo "----------- run: $i"
  project_multiruns p_motion_no_save.xml 1 0.5 $i
  cat pc_combined_tracks.csv >> save.csv
done

