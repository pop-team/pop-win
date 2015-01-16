files: myclass.obj main obj.map

obj.map: myclass.obj
	./myclass.obj -listlong > obj.map
	rm -rf *.o *_0.* *_1.* *_2.* filename*.* *.json

myclass.obj main: main_3.cc myclass_3.cc myclass_3.ph
	popcc -object -o myclass.obj myclass_3.cc myclass_3.ph -ljson
	popcc -o main main_3.cc myclass_3.cc myclass_3.ph -ljson

myclass_3.cc myclass_3.ph main_3.cc: main.cc myclass.cc myclass.ph
	./preparse main.cc myclass.cc myclass.ph
	rm -rf *.o *_0.* *_1.* *_2.* filename*.* *.json

clean:
	rm -rf *.map main *.obj *_3.*
	rm -rf *.o *_0.* *_1.* *_2.* filename*.* *.json
