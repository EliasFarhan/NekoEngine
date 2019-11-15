sudo perf record -g $1
sudo perf report -g 'graph,0.5,caller'
