#!/bin/bash
for i in {1..150}; do
    curl -s http://localhost:8080
done
wait
