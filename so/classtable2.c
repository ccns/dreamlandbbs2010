/*-------------------------------------------------------*/
/* classtable.c   ( YZU_CSE WindTop BBS )                */
/*-------------------------------------------------------*/
/* author : verit.bbs@bbs.yzu.edu.tw                     */
/* modify : verit.bbs@bbs.yzu.edu.tw			 */
/* target : 功課表                                       */
/* create : 2002/01/18                                   */
/* update : 2004/05/20                                   */
/*-------------------------------------------------------*/

#include "bbs.h"



static CLASS_TABLE2 tmp_table[78];
static char fpath_classtable[80];


int show_classtable(int x, int y, char *msg)
{
	y--;
	move(x + 3, 7 + y*12);
	prints("%-8s", msg);
	return 0;
}

void show_icon_classtable(int x, int y, int mode)
{
	int p;
	p = (x - 1) + (y - 1) * 13;
	y--;
	if (mode == 1)
	{
		move(x + 3, 6 + y*12);outc('[');move(x + 3, 15 + y*12);outc(']');
	}
	else
	{
		move(x + 3, 6 + y*12);outc(' ');move(x + 3, 15 + y*12);outc(' ');
	}

	move(19, 20);prints("                                ");
	move(20, 20);prints("           ");
	move(21, 20);prints("                                ");
	move(20, 55);prints("                    ");

	if (tmp_table[p].valid == 1)
	{
		move(19, 20);
		prints("%-s", tmp_table[p].name);
		move(20, 20);
		prints("%-s", tmp_table[p].teacher);
		move(20, 55);
		prints("%-s", tmp_table[p].room);
		move(21, 20);
		prints("%-s", tmp_table[p].other);
	}
	move(b_lines, 78);
	return ;
}

void help_classtable(void)
{
	outz("\033[1;37;42m 【操作說明】a)新增 e)修改 d)刪除 q)離開 i)匯入 c)清除 \033[1;30mCopyRight By Verit@yzu \033[m");
}

int show_table(void)
{
	int i;
	int x = 1;
	int y = 0;

	vs_head("個人課表", str_site);
	move(x++, y);
	prints("┌─┬─────┬─────┬─────┬─────┬─────┬─────┐");
	move(x++, y);
	prints("│　│  星期一  │  星期二  │  星期三  │  星期四　│　星期五　│　星期六　│");
	move(x++, y);
	prints("├─┼─────┼─────┼─────┼─────┼─────┼─────┤");
	for (i = 1;i < 14;i++)
	{
		move(x++, y);
		prints("│%2d│          │          │          │          │          │          │", i);
	}
	move(x++, y);
	prints("└─┴─────┴─────┴─────┴─────┴─────┴─────┘");
	move(x++, y);
	prints("╭─────────────────────────────────────╮");
	move(x++, y);
	prints("│    【課程名稱】                                                          │");
	move(x++, y);
	prints("│    【授課教師】                        【上課地點】                      │");
	move(x++, y);
	prints("│    【備    註】                                                          │");
	move(x++, y);
	prints("╰─────────────────────────────────────╯");
	help_classtable();
	return 0;
}

int load_table()
{
	int i;
	int fd;
	fd = open(fpath_classtable, O_RDONLY);
	if (fd >= 0)
	{
		read(fd, tmp_table, sizeof(CLASS_TABLE2)*78);
		close(fd);
	}
	else
	{
		memset(tmp_table, 0, sizeof(CLASS_TABLE2)*78);
	}

	for (i = 0;i < 78;i++)
	{
		if (tmp_table[i].valid == 1)
		{
			show_classtable(tmp_table[i].x, tmp_table[i].y, tmp_table[i].condensation);
		}
	}
	return 0;
}

int add_classtable(int x, int y)
{
	CLASS_TABLE2 classtable;
	int p;

	p = (x - 1) + (y - 1) * 13;

	if (tmp_table[p].valid == 1)
		return 0;
	classtable.y = y;
	classtable.x = x;

	if (vget(b_lines, 0, "課程簡稱 : " , classtable.condensation, sizeof(classtable.condensation), DOECHO))
	{
		vget(b_lines, 0, "課程名稱 : " , classtable.name, sizeof(classtable.name), DOECHO);
		vget(b_lines, 0, "授課教師 : " , classtable.teacher, sizeof(classtable.teacher), DOECHO);
		vget(b_lines, 0, "上課地點 : " , classtable.room, sizeof(classtable.room), DOECHO);
		vget(b_lines, 0, "備    註 : " , classtable.other, sizeof(classtable.other), DOECHO);


		memcpy(&tmp_table[p], &classtable, sizeof(CLASS_TABLE2));
		tmp_table[p].valid = 1;
		show_classtable(x, y, classtable.condensation);
	}
	show_icon_classtable(x, y, 1);
	help_classtable();
	return 0;
}

int del_classtable(int x, int y)
{
	int p;
	p = (x - 1) + (y - 1) * 13;

	if (vans("是否要刪除 ？ [y/N]") != 'y')
	{
		help_classtable();
		return 0;
	}
	else
	{
		memset(&tmp_table[p], 0, sizeof(CLASS_TABLE2));
		show_classtable(x, y, "         ");
	}
	help_classtable();
	return 0;
}

int edit_classtable(int x, int y)
{
	int p;
	CLASS_TABLE2 classtable;

	p = (x - 1) + (y - 1) * 13;

	if (tmp_table[p].valid == 1)
	{
		memcpy(&classtable, &tmp_table[p], sizeof(CLASS_TABLE2));
		if (vget(b_lines, 0, "課程簡稱 : " , classtable.condensation, sizeof(classtable.condensation), GCARRY))
		{
			vget(b_lines, 0, "課程名稱 : " , classtable.name, sizeof(classtable.name), GCARRY);
			vget(b_lines, 0, "授課教師 : " , classtable.teacher, sizeof(classtable.teacher), GCARRY);
			vget(b_lines, 0, "上課地點 : " , classtable.room, sizeof(classtable.room), GCARRY);
			vget(b_lines, 0, "備    註 : " , classtable.other, sizeof(classtable.other), GCARRY);
			memcpy(&tmp_table[p], &classtable, sizeof(CLASS_TABLE2));

			show_classtable(x, y, classtable.condensation);
		}
	}
	show_icon_classtable(x, y, 1);
	help_classtable();
	return 0;
}

#define HTTP_PORT         80
#define SERVER_student    "portal.yzu.edu.tw"
#define CGI_stage1	  "/logincheck.asp"
#define CGI_stage2	  "/"
#define CGI_stage3        "/VC/classLeft.asp"
#define CGI_stage4        "/VC/Login_Student.asp"

int
import_classtable(int x, int y)
{
	char sendform[1024], sendform2[1024];
	char cookie[80];
	int sockfd;
	int cc;
	char *xhead;
	char pool[2048], buf[1025];
	FILE *fp;
	char *s;
	char fname[80];
	char User[7], Password[25], ans;
	CLASS_TABLE2 classtable;

	if (!vget(b_lines, 0, "帳號(不加s) : " , User, sizeof(User), DOECHO) ||
		!vget(b_lines, 0, "密碼 : " , Password, sizeof(Password), NOECHO))
		return 0;

	ans = vans("是否覆蓋\原始資料 [y/N]: ");

	/* -------------------------- */
	/*  取得 Cookie		*/
	/* -------------------------- */
	sprintf(sendform, "GET %s HTTP/1.0\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 0\r\n\r\n", CGI_stage2);

	if ((sockfd = dns_open(SERVER_student, HTTP_PORT)) < 0)
	{
		vmsg("無法與伺服器取得連結，查詢失敗\n");
		return 1;
	}

	write(sockfd, sendform, strlen(sendform));
	shutdown(sockfd, 1);

	for (;;)
	{
		xhead = pool;
		cc = read(sockfd, xhead, sizeof(pool));
		if (*xhead != '\0' && cc > 0)
		{
			xhead[cc] = '\0';
			s = strstr(xhead, "Cookie:");
			if (s != NULL)
			{
				s[strlen(s) - strlen(strstr(s, "\n")) + 1] = '\0';
				strcpy(cookie, s);
				break;
			}
		}
		else
		{
			vmsg("MISS");
			return 1;
		}
	}
	close(sockfd);

	/* -------------------------- */
	/*  Login			*/
	/* -------------------------- */
	sprintf(sendform2, "uid=s%s&pwd=%s&ok=確定", User, Password);
	sprintf(sendform, "POST %s HTTP/1.0\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n%s\r\n%s\r\n", CGI_stage1, strlen(sendform2), cookie, sendform2);

	if ((sockfd = dns_open(SERVER_student, HTTP_PORT)) < 0)
	{
		vmsg("無法與伺服器取得連結，查詢失敗\n");
		return 1;
	}

	write(sockfd, sendform, strlen(sendform));
	shutdown(sockfd, 1);

	for (;;)
	{
		xhead = pool;
		cc = read(sockfd, xhead, sizeof(pool));
		if (*xhead != '\0' && cc > 0)
		{
			xhead[cc] = '\0';
		}
		if (cc <= 0)
		{
			break;
		}
	}
	close(sockfd);

	/* -------------------------- */
	/*  進入學習檔案		*/
	/* -------------------------- */
	sprintf(sendform, "GET %s HTTP/1.1\r\n%sHost: portal.yzu.edu.tw\r\nReferer: http://portal.yzu.edu.tw/VC/left_index.asp\r\n\r\n", CGI_stage4, cookie);


	if ((sockfd = dns_open(SERVER_student, HTTP_PORT)) < 0)
	{
		vmsg("無法與伺服器取得連結，查詢失敗\n");
		return 1;
	}

	write(sockfd, sendform, strlen(sendform));
	shutdown(sockfd, 1);
	for (;;)
	{
		xhead = pool;
		cc = read(sockfd, xhead, sizeof(pool));
		if (*xhead != '\0' && cc > 0)
		{
			xhead[cc] = '\0';
		}
		if (cc <= 0)
		{
			break;
		}
	}
	close(sockfd);

	/* -------------------------- */
	/*  取得選課清單		*/
	/* -------------------------- */
	sprintf(sendform, "GET %s HTTP/1.1\r\n%sHost: portal.yzu.edu.tw\r\nReferer: http://portal.yzu.edu.tw/VC/Login_Student.asp\r\n\r\n", CGI_stage3, cookie);
//  sprintf(sendform, "GET %s HTTP/1.1\r\n%sHost: portal.yzu.edu.tw\r\n\r\n", CGI_stage3, cookie);


	if ((sockfd = dns_open(SERVER_student, HTTP_PORT)) < 0)
	{
		vmsg("無法與伺服器取得連結，查詢失敗\n");
		return 1;
	}

	write(sockfd, sendform, strlen(sendform));
	shutdown(sockfd, 1);
	sprintf(fname, "tmp/%s.student", cuser.userid);
	fp = fopen(fname, "w");
	fputs(sendform, fp);
	if (fp)
	{
		for (;;)
		{
			xhead = pool;
			cc = read(sockfd, xhead, sizeof(pool));
			if (*xhead != '\0' && cc > 0)
			{
				xhead[cc] = '\0';
				fputs(xhead, fp);
			}
			if (cc <= 0)
			{
				break;
			}
		}
		fclose(fp);
	}
	close(sockfd);


	/* stage4 */
	fp = fopen(fname, "r");
	memset(&classtable, 0, sizeof(CLASS_TABLE2));
	if (fp)
	{
		while (fgets(buf, 1024, fp) != NULL)
		{
			s = strstr(buf, "課程名稱");
			if (s != NULL)  // 課程名稱
			{
				s += 10;
				s[strlen(s) - strlen(strstr(s, "】"))] = 0;
				strcpy(classtable.name, s);
				strncpy(classtable.condensation, s, sizeof(classtable.condensation) - 1);
				fgets(buf, 1024, fp);
				fgets(buf, 1024, fp);
				s = strstr(buf, "授課教師");
				s += 10;
				s[strlen(s) - strlen(strstr(s, "】"))] = 0;
				strcat(classtable.teacher, s);

				while (fgets(buf, 1024, fp))
				{
					if ((s = strstr(buf, "（RM")))
					{
						int p, x, y, num;
						char *pp;

						*s = 0;
						pp = strstr(s + 2, "）");
						num = atoi(buf);
						y = num / 100;
						x = num % 100;

						p = (x - 1) + (y - 1) * 13;
						if (tmp_table[p].valid == 0 || ans == 'y' || ans == 'Y')
						{
							classtable.x = x;
							classtable.y = y;
							classtable.valid = 1;

							*pp = 0;
							strcpy(classtable.room, s + 2);
							memcpy(&tmp_table[p], &classtable, sizeof(CLASS_TABLE2));
							show_classtable(x, y, classtable.condensation);
						}

						if (strstr(pp + 3 , "pimindex.asp"))
							break;
					}
				}
				memset(&classtable, 0, sizeof(CLASS_TABLE2));
			}
		}
		fclose(fp);
		unlink(fname);
	}
	show_icon_classtable(x, y, 1);
	help_classtable();
	return 0;
}


int init_classtable()
{
	show_table();
	load_table();
	return 0;
}

int main_classtable(void)
{
	char c;
	int x = 1, y = 1;
	int fd;

#ifdef	HAVE_CLASSTABLEALERT
	classtable_free();
#endif
	usr_fpath(fpath_classtable, cuser.userid, FN_CLASSTABLE2);

	init_classtable();
	show_icon_classtable(x, y, 1);

	do
	{
		c = vkey();
		switch (c)
		{
		case 'q' :
			break;
		case KEY_DOWN :
			show_icon_classtable(x, y, 0);
			x++;
			if (x == 14) x = 1;
			show_icon_classtable(x, y, 1);
			break;
		case KEY_UP:
			show_icon_classtable(x, y, 0);
			x--;
			if (x == 0) x = 13;
			show_icon_classtable(x, y, 1);
			break;
		case KEY_LEFT:
			show_icon_classtable(x, y, 0);
			y--;
			if (y == 0) y = 6;
			show_icon_classtable(x, y, 1);
			break;
		case KEY_RIGHT :
			show_icon_classtable(x, y, 0);
			y++;
			if (y == 7) y = 1;
			show_icon_classtable(x, y, 1);
			break;
		case 'a' :
			add_classtable(x, y);
			break;
		case 'd' :
			del_classtable(x, y);
			break;
		case 'e' :
			edit_classtable(x, y);
			break;
		case 'i' :
			import_classtable(x, y);
			break;
		case 'c' :
			if (vans("確定刪除嗎？[y/N] ") == 'y')
			{
				memset(tmp_table, 0, sizeof(CLASS_TABLE2)*78);
				show_table();
			}
			break;
		case '\n' :
			if (tmp_table[x+y*13-14].valid == 1)
				edit_classtable(x, y);
			else
				add_classtable(x, y);
			break;
		}
	}
	while (c != 'q');

	fd = open(fpath_classtable, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd >= 0)
	{
		write(fd, tmp_table, sizeof(CLASS_TABLE2)*78);
		close(fd);
	}
#ifdef	HAVE_CLASSTABLEALERT
	classtable_main();
#endif

	vmsg(NULL);
	return 0;
}
