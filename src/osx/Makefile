test: test.c
	gcc -framework Carbon $^ -o $@

test1: test.m
	gcc -framework Cocoa -objective-c $^ -o $@

testgl: testgl.c
	gcc -o $@ $^ -framework OpenGL -framework GLUT

testq: testquartz.c
	gcc -o $@ $^ -framework ApplicationServices


list-frameworks:
	ls /System/Library/Frameworks
