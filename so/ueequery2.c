/*-------------------------------------------------------*/
/* ueequery.c   ( NTU FPG BBS Ver 2.00 )       by 吳紹衍 */
/*-------------------------------------------------------*/
/* target : 聯考榜單查詢                                 */
/* create : 98/08/08                                     */
/* update : 98/08/08                                     */
/*-------------------------------------------------------*/
/*
資料來源
http://www.ncu.edu.tw/~center25/86union/fa01.txt ~ fa04.txt
http://www.ncu.edu.tw/~center25/87union/fa01.txt ~ fa04.txt
http://www.ncu.edu.tw/~center25/88union/fa01.txt ~ fa04.txt

ftp://140.112.2.84/pub/89exam/
http://www.csie.nctu.edu.tw/service/jcee/ 歷年
http://www.cts.com.tw/exam/university/
http://www.ncu.edu.tw/~center25/89union/
http://140.112.3.171/uee88/default.htm
http://info.ntu.edu.tw/88/
八月七日上午九時開放！！
*/
#include "bbs.h"

int
x_ueequery2()
{
	char buf[256], query[30], query2[30], msg[60];
	char fpath[80], now[80], *ptr;
	int  i, count = 0, year, examnum = 0;
	int school = 0, class = 0;
	FILE *fp;

	vs_bar("聯考榜單查詢");
	outs("作者: Shen Chuan-Hsing <statue.bbs@bbs.yzu.edu.tw>\n");

	outs("注意事項\n"
		 "1. 大學及科系兩項至少需填寫一項才能夠查詢.\n"
		 "2. 輸入部分字串即可.\n");
	if (!vget(7, 0, "請輸入聯考年度 (83 ~ 94)[94]：", buf, 3, DOECHO))
		year = 94;
	else
		year = atoi(buf);
	if (year < 83 || year > 94) return 0;

	if (year >= 83 && year <= 86)
		examnum = 6;
	else if (year >= 87 && year <= 90)
		examnum = 7;
	else if (year >= 91)
		examnum = 8;
	else
		return 0;
	sprintf(fpath, "game/%d/fa00.txt", year);

	vget(9, 0, "請輸入查詢大學 (例：元智)：", query, 20, DOECHO);
	vget(11, 0, "請輸入查詢科系 (例：資訊工程)：", query2, 20, DOECHO);

	if (!strlen(query) && !strlen(query2))
		return 0;

	i = 0;
	sprintf(msg, "%s:%s", query, query2);
	logitfile(FN_UEEQUERY_LOG, fpath, msg);

	if ((fp = fopen(fpath, "r")))
	{
		while (fgets(buf, 256, fp))
		{

			/*
			 0001國立臺灣大學              中國文學系                                  1第
			*/
			if (buf[0] == ' ') /* 這樣就是科系的開頭 */
			{
				ptr = strtok(buf + 5, " ");
				strcpy(now, ptr);
				if (strstr(now, query) || !query) // 找到
					school = 1;
				else
					school = 0;
				ptr = strtok(NULL, " ");
				if (strstr(ptr, query2) || !query2) //找到
					class = 1;
				else
					class = 0;
				sprintf(now, "%-24s%-30s", now, ptr);
				continue;
			}

			if (i == 0)
			{
				vs_bar("聯考榜單查詢");
				move(2, 0);
				outs("[7m 准考證    姓  名    錄 取 大 學             錄 取 科 系                        [m");
			}
			move(1, 0);
			clrtoeol();
			prints("查詢《%s》中....", query);
			ptr = strtok(buf, " ");

			if (school && class && strlen(ptr) >= 6)  /* 處理中文字的問題 */
			{
				count++;
				if (count > 20 && (count % 20) == 1)
				{
					int ch;

					move(1, 0);
					clrtoeol();
					outs("[1;33m本頁查詢已滿，換頁查詢請按任意鍵，結束查詢請按 q[m");
					ch = vkey();
					if (ch == 'q' || ch == 'Q')
					{
						fclose(fp);
						return 0;
					}
					i = 0;
					vs_bar("聯考榜單查詢");
					move(2, 0);
					outs("[7m 准考證    姓  名    錄 取 大 學         錄 取 科 系                            [m");
				}
				move(3 + i, 0);
				if (examnum == 6)
					prints(" %-6.6s    %-8s  %s", ptr, ptr + examnum, now);
				else if (examnum == 7)
					prints(" %-7.7s   %-8s  %s", ptr, ptr + examnum, now);
				else if (examnum == 8)
					prints(" %-8.8s  %-8s  %s", ptr, ptr + examnum, now);
				i++;
				move(b_lines, 0);
				clrtoeol();
				prints("[1;33;42m 榜單查詢 [;30;47m 目前共查詢到 [31m%5d [30m資料                                             [m",
					   count);
				refresh();
			}
			while ((ptr = strtok(NULL, "    ")))
			{
				if (school && class && strlen(ptr) >= 6)  /* 處理中文字的問題 */
				{
					count++;
					if (count > 20 && (count % 20) == 1)
					{
						int ch;

						move(1, 0);
						clrtoeol();
						outs("[1;33m本頁查詢已滿，換頁查詢請按任意鍵，結束查詢請按 q[m");
						ch = vkey();
						if (ch == 'q' || ch == 'Q')
						{
							fclose(fp);
							return 0;
						}
						i = 0;
						vs_bar("聯考榜單查詢");
						move(2, 0);
						outs("[7m 准考證    姓 名     錄 取 大 學         錄 取 科 系                            [m");
					}
					move(3 + i, 0);
					if (examnum == 6)
						prints(" %-6.6s    %-8s  %s", ptr, ptr + examnum, now);
					else if (examnum == 7)
						prints(" %-7.7s   %-8s  %s", ptr, ptr + examnum, now);
					else if (examnum == 8)
						prints(" %-8.8s  %-8s  %s", ptr, ptr + examnum, now);
					i++;
					move(b_lines, 0);
					clrtoeol();
					prints("[1;33;42m 榜單查詢 [;30;47m 目前共查詢到 [31m%5d [30m資料                                             [m",
						   count);
					refresh();
				} // end while
			}
		}
		fclose(fp);
	}
	move(1, 0);
	clrtoeol();
	outs("[1;36m查詢結束，按任意鍵離開....[m");
	move(b_lines, 0);
	clrtoeol();
	prints("[1;33;42m 榜單查詢 [;30;47m 目前共查詢到 [31m%5d [30m資料                                             [m",
		   count);
	if (count == 0)
	{
		move(4, 0);
		outs("沒有查詢到任何資料");
	}
	vkey();
	return 0;
}
