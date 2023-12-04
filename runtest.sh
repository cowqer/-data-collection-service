cd build/
sudo ./hdf5
sleep 5
./pub0
pid=$(pgrep -f "pub0")
sleep 15
kill "$pid"
