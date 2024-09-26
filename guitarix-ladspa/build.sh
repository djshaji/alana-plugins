for i in *cpp ; do echo $i ; c++ $i -shared -fPIC -O3 -o $i.so ; done
