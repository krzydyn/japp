test: test.c
	gcc -framework Carbon $^ -o $@

test1: test.m
	gcc -lobjc -framework Foundation -xobjective-c $^ -o $@

testgl: testgl.c
	gcc -o $@ $^ -framework OpenGL -framework GLUT

testq: testquartz.c
	gcc -o $@ $^ -framework ApplicationServices

main: main.c
	gcc -o $@ $^ -lobjc -framework AppKit

.PHONY: list-frameworks
list-frameworks:
	ls /System/Library/Frameworks
