#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cyclelist.h"

#define KO_MASTER_COUNT      1000
struct ko_master
{
    struct ko_list_head			    list;
    int 							i;
    int 							j;
    int 							k;
};

KO_LIST_HEAD(ko_free_list);
KO_LIST_HEAD(ko_used_list);

int main(int argc, char const *argv[])
{
	
	//预分配 ko_master 链表
 	ko_master                     *rooms, *r;
    size = sizeof(ko_master) * KO_MASTER_COUNT;
    rooms = malloc(size);
    if (!rooms) {
        return -1;
    }
    for (i = 0; i < KO_MASTER_COUNT; ++i) {
        r = rooms + i;
        ko_list_add(&r-list, &ko_free_list);
    }


    //从free中取出，再放入 used链表里。
    struct ko_list_head    *templist;
    ko_master              *g;
	templist = ko_list_del_next(&ko_free_list);
	g = container_of(templist, ko_master, list);
	g->i = 1;
	g->j = 2;
	g->k = 3;
	ko_list_add(&g->list, &ko_used_list);

	templist = ko_list_del_next(&ko_free_list);
	g = container_of(templist, ko_master, list);
	g->i = 4;
	g->j = 5;
	g->k = 6;
	ko_list_add(&g->list, &ko_used_list);


	//遍历used链表。
	struct ko_list_head   *pos;
	ko_list_for_each(pos, &ko_used_list)
	{
		g = container_of(pos, ko_master, list);
		printf("%d,%d,%d\n", g->i, g->j, g->k);
	}


	return 0;
}