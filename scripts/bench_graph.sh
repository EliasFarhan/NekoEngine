#!/bin/bash
filename="$1"
filter=""
json_filename="$(basename "$filename").json"
if [[ ${filename} == *.json ]]
  then  echo "Plot Graph $filename"; python3 "$(dirname "$0")/bench_graph.py" "$filename"; exit 0
fi
if [[ $# -eq 2 ]]
  then echo "Setting filters..."; filter="$2"; json_filename="$(basename "$filename")_${filter}.json"
fi
echo "Benchmarking..."
if [ "$filter" == "" ]
  then "./$filename" --benchmark_out_format=json --benchmark_out="$json_filename"
else
  "./$filename" --benchmark_out_format=json --benchmark_filter="$filter" --benchmark_out="$json_filename"
fi


python3 "$(dirname "$0")/bench_graph.py" "$json_filename"