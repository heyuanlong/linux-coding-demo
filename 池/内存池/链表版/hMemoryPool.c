#include "hMemoryPool.h"


#define PRENUMS 100
#define MALLOCFAIL -9

static slot *slotArr = NULL;
static int slotArrCurrentNums = 0;
static int slotArrAllNums = 0;
static initStruct preSizeArr[] = {{20,5000},{30,5000},{40,5000},{50,5000}} ;


//内部使用的函数
int quickSort(slot *R,int s,int t);
int removeRepeat(slot *R,int *nums);
int mallocBlock(slot *R,int nums);
int Search(slot *R,int nums,int size);

int initHMemoryPool(initStruct* arr,int nums)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int preNums = 0;
	int returnRes;

	preNums = sizeof(preSizeArr) / sizeof(initStruct);
	slotArrAllNums = nums + preNums + PRENUMS ;
	slotArr = (slot*)malloc(sizeof(slot) * slotArrAllNums);
	if (slotArr == NULL){
		return MALLOCFAIL;
	}
	memset(slotArr,0,sizeof(slot) * slotArrAllNums);

//用户预分配的
	for ( i = 0; i < nums; ++i)
	{
		slotArr[i].blockSize = arr[i].size;
		slotArr[i].freeNums = arr[i].nums;
		slotArr[i].allNums = arr[i].nums;
		slotArr[i].maxNums = arr[i].nums;
	}
	slotArrCurrentNums = nums;
	
//系统预分配的,如果用户分配了，就不加上去了。
	k = slotArrCurrentNums;
	for ( i = 0; i < preNums; ++i)
	{
		for ( j = 0; j < slotArrCurrentNums; ++j)
		{
			if (preSizeArr[i].size == slotArr[j].blockSize){
				break;				
			}
		}
		if (j == slotArrCurrentNums){
			slotArr[k].blockSize = preSizeArr[i].size;
			slotArr[k].freeNums = preSizeArr[i].nums;
			slotArr[k].allNums = preSizeArr[i].nums;
			slotArr[k].maxNums = preSizeArr[i].nums;
			++k;
		}
	}
	slotArrCurrentNums = k;


//排序----------
	quickSort(slotArr,0, slotArrCurrentNums-1 ); 

//去重以及去掉blockSize<=0的------
	if( (returnRes = removeRepeat(slotArr,&slotArrCurrentNums)) != 0 ){
		return returnRes;
	}

//分配空间
	if( (returnRes = mallocBlock(slotArr,slotArrCurrentNums)) != 0 ){
		return returnRes;
	}
	return 0;
}

void* getHMemory(int size,int *status)
{
	int searchRes;
	void *res = NULL;
	blockHead *temp = NULL;
	int headSize = sizeof(blockHead);
	searchRes = Search(slotArr,slotArrCurrentNums,size);
	if(searchRes == slotArrCurrentNums){
		*status = 2;
		temp =(blockHead *)malloc(headSize + size);
		temp->size = size;
		res = (void*)(temp) + headSize;
	}else{
		if (slotArr[searchRes].free != NULL)
		{		
			*status = 0;
			res = slotArr[searchRes].free;

			temp = (blockHead *)(res - headSize);
			slotArr[searchRes].free = temp->next;
			--slotArr[searchRes].freeNums;
		}else{
			*status = -1;
			res =  NULL;
		}	
	}
	return res;
}

int freeHMemory(void *p)
{
	int res;
	int searchRes;
	int headSize = sizeof(blockHead);
	blockHead *temp = (blockHead *)(p - headSize);
	int size = temp->size;
	
	searchRes = Search(slotArr,slotArrCurrentNums,size);
	if(searchRes == slotArrCurrentNums){
		free(p-headSize);
	}else{
		temp->next = slotArr[searchRes].free;
		slotArr[searchRes].free = p;
		++slotArr[searchRes].freeNums;
	}
}

int setHMemoryNums(int size,int maxNums)
{

}


int printHMemory(int fd)
{
	int i;
	const int bufSize = 512;
	char buf[bufSize];
	for ( i = 0; i < slotArrCurrentNums; ++i)
	{
		sprintf(buf,"blockSize:%10d  freeNums:%10d  allNums:%10d  maxNums:%10d\n",slotArr[i].blockSize,slotArr[i].freeNums,slotArr[i].allNums,slotArr[i].maxNums);
		write(fd,buf,strlen(buf));
	}
	return 0;
}



int quickSort(slot *R,int s,int t)  
{  
    int i=s,j=t;  
    slot tmp;  
    if (s<t)                 //区间内至少存在两个元素的情况  
    {  
        tmp=R[s];           //用区间的第1个元素作为基准  
        while (i!=j)        //从区间两端交替向中间扫描,直至i=j为止  
        {  
            while (j>i && R[j].blockSize  >= tmp.blockSize)   
                j--;        //从右向左扫描,找第1个小于tmp的R[j]  
            R[i]=R[j];      //找到这样的R[j],R[i]和R[j]交换  
            while (i<j && R[i].blockSize <= tmp.blockSize)   
                i++;        //从左向右扫描,找第1个大于tmp的元素R[i]  
            R[j]=R[i];      //找到这样的R[i],R[i]和R[j]交换  
        }  
        R[i]=tmp;  
        quickSort(R,s,i-1); //对左区间递归排序  
        quickSort(R,i+1,t); //对右区间递归排序  
    }  
    return 0;
}  

int removeRepeat(slot *R,int *nums)
{
	int i,pre,next;
	char *temp;
	int tempNums;
	if(*nums < 2){
		return 0;
	}
	temp = (char*)malloc( sizeof(char) * (*nums) );
	if (temp == NULL){
		return MALLOCFAIL;
	}
	tempNums = *nums;
	pre = 0;
	next = 1;

	temp[pre]= '0';//true为要去掉的
	if (R[pre].blockSize <=0 ){
		temp[pre] = '1';
	}
	for (; next < *nums; ++pre, ++next)
	{
		if(R[pre].blockSize == R[next].blockSize || R[next].blockSize <= 0){
			temp[next] = '1';
		}else{
			temp[next] = '0';
		}
	}

	*nums = 0;
	for ( i = 0; i < tempNums; ++i)
	{
		if (temp[i] == '0'){
			R[*nums] = R[i];
			++(*nums);
		}
	}
	return 0;
}

int mallocBlock(slot *R,int nums)
{
	int i,j;
	void *head,*temp;
	blockHead *headTemp;
	int headSize,justBlockSize,allBlockSize;
	headSize = sizeof(blockHead);

	for (i = 0; i < nums; ++i)
	{
		if(R[i].allNums <= 0 ){
			R[i].free = NULL;

		}else{			
			justBlockSize = R[i].blockSize;
			allBlockSize = headSize + justBlockSize;
			head = malloc( allBlockSize * R[i].allNums );
			if(head == NULL){
				return MALLOCFAIL;
			}
			R[i].free = head + headSize;

			temp = head;
			for ( j = 0; j < R[i].allNums; ++j)
			{
				headTemp = (blockHead*)temp;
				headTemp->size = R[i].blockSize;
				if (j == (R[i].allNums - 1) ){					
					headTemp->next = NULL;
				}else{
					headTemp->next = temp + allBlockSize + headSize;
				}
				temp += allBlockSize;
 			}

		}
	}
}

int Search(slot *R,int nums,int size)
{
	int i;
	for ( i = 0; i < nums; ++i)
	{
		if ( size <= R[i].blockSize){
			return i;
		}
	}
	return nums;
}