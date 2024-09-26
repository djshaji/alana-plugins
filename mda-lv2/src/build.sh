for i in *cpp ; do echo $i ; c++ $i  -shared -fPIC -o $i.so ; done
