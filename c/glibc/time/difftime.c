/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:05:33 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:34 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 13日 星期三 08:54:06 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:18 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:27:55 CST. */
/*
 *	double difftime (time t time1, time t time0) [Function]
 *	The difftime function returns the number of seconds of elapsed time between cal
 *	endar time time1 and calendar time time0, as a value of type double. The diﬀerence
 *	ignores leap seconds unless leap second support is enabled.
 *	In the GNU system, you can simply subtract time_t values. But on other systems,
 *	the time_t data type might use some other encoding where subtraction doesn’t work
 *	directly.
 */

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

int main()
{
	printf("%lf\n", difftime(12.888888883333, 12.4421));

	return 0;
}
