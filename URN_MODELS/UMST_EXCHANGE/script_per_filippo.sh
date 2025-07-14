for seed in {5000..5001}; do
  for n in 30000 ; do
    ./exe -o $seed -n $n -p 9 -q 10 -s 1000000 -l 0.9
  done
done

