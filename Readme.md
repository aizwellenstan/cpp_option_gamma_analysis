# How It Works
skip the row with specify data
edit [csv.cpp](./Csv/csv.cpp) line 50

# How To Use
rename the csv file to in.csv
put the csv file in the data folder
will output out.csv under the data folder

```
sh build.sh
sh run.sh
```

#DEBUG

--ggdb -g

```
sudo apt install gdb valgrind
cmake -DCMAKE_BUILD_TYPE=Debug .
```
