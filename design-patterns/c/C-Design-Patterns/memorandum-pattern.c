/* Comment Rong Tao @Sylincom Beijing, 2020年 01月 19日 星期日 17:13:49 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2020年 01月 19日 星期日 17:11:44 CST. */
//备忘录模式
//备忘录模式的起源来自于撤销的基本操作。有过word 软件操作经验的朋友，应该基本
//上都使用过撤销的功能。举个例子，假设你不小心删除了好几个段落的文字，这时候你应该
//怎么办呢？其实要做的很简单，单击一些【撤销】就可以全部搞定了。撤销按钮给我们提供
//了一次反悔的机会。
//既然是撤销，那么我们在进行某种动作的时候，就应该创建一个相应的撤销操作？这个
//撤销操作的相关定义可以是这样的。

typedef struct _Action
{
	int type;
	struct _Action* next;
	void* pData;
	void (*process)(void* pData);
}Action;

//数据结构中定义了两个部分：撤销的数据、恢复的操作。那么这个撤销函数应该有一个
//创建的函数，还有一个恢复的函数。所以，作为撤销动作的管理者应该包括，
//既然数据在创建和修改的过程中都会有相应的恢复操作，那么要是真正恢复原来的数据
//也就变得非常简单了。

void restore(struct _Organizer* pOrganizer)
{
	Action* pHead;
	assert(NULL != pOrganizer);
	pHead = pOrganizer->pActionHead;
	pHead->process(pHead->pData);
	pOrganizer->pActionHead = pHead->next;
	pOrganizer->number --;
	free(pHead);
	return;
}
