//selectBackground filter
__kernel void selectBackground(__global unsigned char* rgb)
{
	int num = get_global_id(0);
	/*
	if (num > (1919*1080)){
	printf("Enter back?filter success,workitem is %d\n",num);
	}
	*/

    //printf("dept[%d] %d\n ",num,temp);
    rgb[3 * num] = 0;
    rgb[3 * num + 1] = 0;
    rgb[3 * num + 2] = 0;
}