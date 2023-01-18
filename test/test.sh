#!/usr/bin/bash

func_do() {
   echo $(python3 ./test_server.py TestClient$1)
}

for ((i=0; i<20; i++))
do
  sleep 0.3
  echo executing task for $i
  func_do $i &
done

echo waiting for $( jobs | wc -l ) jobs to complete
wait
