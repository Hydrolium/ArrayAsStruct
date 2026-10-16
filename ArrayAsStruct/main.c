#include <stdio.h>
#include <stdlib.h>

char* createStruct(int* structView, int count);
void pad(int* index, int size);
int getOffset(int* structView, int index);
void set(char* myStruct, int* structView, int index, void* value);
void* get(char* myStruct, int* structView, int index);

int main(void) {
	/*
	구현할 구조체
	struct {
		int a;
		float b;
		char c;
	};
	*/

	// char의 크기는 1.

	int a;
	float b;
	char c;

	int structView[] = { sizeof(a), sizeof(b), sizeof(c) };
	int count = sizeof(structView) / sizeof(structView[0]);

	char* myStruct = createStruct(structView, count);

	set(myStruct, structView, 0, &(int){100});
	set(myStruct, structView, 1, &(float){2.5});
	set(myStruct, structView, 2, &(char){'A'});

	a = *(int*)get(myStruct, structView, 0);
	b = *(float*)get(myStruct, structView, 1);
	c = *(char*)get(myStruct, structView, 2);

	printf("%d  %f  %c\n", a, b, c);

	free(myStruct);

	return 0;
}

char* createStruct(int* structView, int count) {
	char* myStruct = NULL;

	int i, size = 0, max = 0, v;

	for (i = 0; i < count; i++)
		if (structView[i] > max) max = structView[i];

	size = getOffset(structView, count - 1) + structView[count - 1];
	pad(&size, max);

	myStruct = (char*)malloc(size);

	if (myStruct == NULL) {
		printf("메모리 부족.");
		exit(1);
	}

	return myStruct;
}

void pad(int* index, int size) {
	while (*index % size) (*index)++; // 시작 위치가 size의 배수가 되도록 패딩 추가
}

int getOffset(int* structView, int index) {
	//int offset = 0;
	//for (int i = 0; i < index; i++) {
	//	while (offset % structView[i]) offset++;
	//	offset += structView[i];
	//}
	//while (offset % structView[index]) offset++;
	//return offset;

	if (index <= 0) return 0;

	int offset = getOffset(structView, index - 1) + structView[index - 1];

	pad(&offset, structView[index]);

	return offset;
}

void set(char* myStruct, int* structView, int index, void* value) {
	int i, size = structView[index];
	char* start = myStruct + getOffset(structView, index);
	char* v = (char*)value;

	for (i = 0; i < size; i++) 
		start[i] = v[i];
}

void* get(char* myStruct, int* structView, int index) {
	return (myStruct + getOffset(structView, index));
}