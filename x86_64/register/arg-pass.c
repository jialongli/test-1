
void func1(a,b,c)
int a,b,c;
{}

void func2(a,b,c,d,e,f)
int a,b,c,d,e,f;
{}

void func3(a,b,c,d,e,f,g,h)
int a,b,c,d,e,f,g,h;
{
	int i = a+b+c+d+e+f+g+h;
}

void func0(){}

int main()
{
#if 1
	int a,b,c,d,e,f,g,h;
	a = 1;
	b = 2;
	c = 3;
	d = 4;
	e = 5;
	f = 6;
	g = 7;
	h = 8;

	//printf("%p\n", &a);
	//printf("%p\n", &b);

	func1(a,b,c);
	func2(a,b,c,d,e,f);
	func3(a,b,c,d,e,f,g,h);
#endif
}
