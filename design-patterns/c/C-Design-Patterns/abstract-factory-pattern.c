/* Comment Rong Tao @Sylincom Beijing, 2020年 01月 19日 星期日 17:13:49 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2020年 01月 19日 星期日 17:11:44 CST. */
#include <stdio.h>
#include <assert.h>

//抽象工厂模式
//前面我们写过的工厂模式实际上是对产品的抽象。对于不同的用户需求，我们可以给予
//不同的产品，而且这些产品的接口都是一致的。而抽象工厂呢？顾名思义，就是说我们的工
//厂是不一定的。怎么理解呢，举个例子。
//假设有两个水果店都在卖水果，都卖苹果和葡萄。其中一个水果店买白苹果和白葡
//萄，另外一个水果店卖红苹果和红葡萄。所以说，对于水果店而言，尽管都在卖水果，但是
//两个店卖的品种不一样。
//既然水果不一样，那我们先定义水果

typedef struct _Apple
{
	void (*print_apple)();
}Apple;
typedef struct _Grape
{
	void (*print_grape)();
}Grape;

//上面分别对苹果和葡萄进行了抽象，当然它们的具体函数也是不一样的。

void print_white_apple()
{
	printf("white apple!\n");
}
void print_red_apple()
{
	printf("red apple!\n");
}
void print_white_grape()
{
	printf("white grape!\n");
}
void print_red_grape()
{
	printf("red grape!\n");
}

//完成了水果函数的定义。下面就该定义工厂了，和水果一样，我们也需要对工厂进行抽
//象处理

typedef struct _FruitShop
{
	Apple* (*sell_apple)();
	Grape * (*sell_grape)();
}FruitShop;

//所以，对于卖白苹果、白葡萄的水果店就该这样设计了，红苹果、红葡萄的水果店
//亦是如此。

Apple* sell_white_apple()
{
	Apple* pApple = (Apple*) malloc(sizeof(Apple));
	assert(NULL != pApple);
	pApple->print_apple = print_white_apple;
	return pApple;
}
Apple* sell_red_apple()
{
	Apple* pApple = (Apple*) malloc(sizeof(Apple));
	assert(NULL != pApple);
	pApple->print_apple = print_red_apple;
	return pApple;
}

Grape* sell_white_grape()
{
	Grape* pGrape = (Grape*) malloc(sizeof(Grape));
	assert(NULL != pGrape);
	pGrape->print_grape = print_white_grape;
	return pGrape;
}
Grape* sell_red_grape()
{
	Grape* pGrape = (Grape*) malloc(sizeof(Grape));
	assert(NULL != pGrape);
	pGrape->print_grape = print_red_grape;
	return pGrape;
}


//这样，基本的框架就算搭建完成的，以后创建工厂的时候，
enum color {
    WHITE,RED
};
FruitShop* create_fruit_shop(int color)
{
	FruitShop* pFruitShop = (FruitShop*) malloc(sizeof(FruitShop));
	assert(NULL != pFruitShop);
	if(WHITE == color)
	{
		pFruitShop->sell_apple = sell_white_apple;
		pFruitShop->sell_grape = sell_white_grape;
	}
	else
	{
		pFruitShop->sell_apple = sell_red_apple;
		pFruitShop->sell_grape = sell_red_grape;
	}
	return pFruitShop;
}

int main()
{
    FruitShop*fshop =  create_fruit_shop(RED);
    fshop->sell_apple()->print_apple();
    fshop->sell_grape()->print_grape();

}
