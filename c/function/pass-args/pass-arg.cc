
struct test {
	int v1;
	int v2;
	int v3;
	int v4;
};

void func1(struct test t) 
{}
void func2(struct test *t) 
{}
void func3(struct test &t)
{}

int main()
{
	struct test t = {1,2,3,4};

//	int a = 1024;

	func1(t);
	func2(&t);
	func3(t);
}
