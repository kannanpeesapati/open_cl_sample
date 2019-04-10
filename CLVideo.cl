//selectBackground filter
__kernel void selectBackground(__global unsigned char* rgb)
{
	int num = get_global_id(0);
	
	rgb[3 * num] = 110;
	//rgb[3 * num + 1] = 0;
	//rgb[3 * num + 2] = 0;
}
