sudo perf record -g "$@"
sudo perf report -g 'graph,0.5,caller'
