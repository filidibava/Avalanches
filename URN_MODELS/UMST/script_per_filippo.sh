for seed in {5000..5001}; do
  for n in 10000; do
    ./exe -o $seed -n $n -p 3 -q 6 -s 300000 -l 0.80
  done
done

