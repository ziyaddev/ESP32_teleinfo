#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*ft_teleinfo_extract(char *str, char *labl, int data_len)
{
	char	*nrj_meter_data;
	char	*str_buf;

	// Allocate memory space to store label data
	nrj_meter_data = malloc(sizeof(char) * (data_len + 1));
	if (!nrj_meter_data)
		return (0);
	
	str_buf = malloc(sizeof(char) * (strlen(str) + 1));
	if (!str_buf)
		return (0);

	// Search for label in the string
	str_buf = strstr(str, labl);

	strncpy(nrj_meter_data, (str_buf + strlen(labl) + 1), data_len);

	return (nrj_meter_data);
}

// int	main(void)
// {
// 	char	data[] = "ADCO 026548931894 ;\nOPTARIF BASE 0\nISOUSC 60 <\nBASE 285579372 ;\nPTEC TH.. $\nIINST1 016 O\nIINST2 016 P\nIINST3 021 M\nIMAX1 051 6\nIMAX2 056 <\nIMAX3 066 >\nPMAX 37720 9\nPAPP 07369 :\nMOTDETAT 000000 B\n␃␂OT 00 #";
// 	int		i;
// 	char	*str;

// 	typedef struct teleinfo
// 	{
// 		int	total_kwh;
// 		int inst_amp_1;
// 		int inst_amp_2;
// 		int inst_amp_3;
// 		int	amp_max_1;
// 		int amp_max_2;
// 		int amp_max_3;
// 		int max_pow;
// 		int app_pow;
// 	} teleinfo;

// 	teleinfo Teleinfo;

// 	str = malloc(sizeof(char) * 20);
// 	if (!str)
// 		return (0);
// 	i = 0;

// 	Teleinfo.total_kwh = atoi(ft_teleinfo_extract(data, "\nBASE", 9));
// 	Teleinfo.inst_amp_1 = atoi(ft_teleinfo_extract(data, "\nIINST1", 3));
// 	Teleinfo.inst_amp_2 = atoi(ft_teleinfo_extract(data, "\nIINST2", 3));
// 	Teleinfo.inst_amp_3 = atoi(ft_teleinfo_extract(data, "\nIINST3", 3));
// 	Teleinfo.amp_max_1 = atoi(ft_teleinfo_extract(data, "\nIMAX1", 3));
// 	Teleinfo.amp_max_2 = atoi(ft_teleinfo_extract(data, "\nIMAX2", 3));
// 	Teleinfo.amp_max_3 = atoi(ft_teleinfo_extract(data, "\nIMAX3", 3));
// 	Teleinfo.max_pow = atoi(ft_teleinfo_extract(data, "\nPMAX", 5));
// 	Teleinfo.app_pow = atoi(ft_teleinfo_extract(data, "\nPAPP", 5));

// 	printf("str : %s\n", str);

// 	while (i < 20)
// 	{
// 		printf("i %d : %d | %c\n", i, str[i], str[i]);
// 		i++;
// 	}

// 	printf("\nBASE : %d\n", Teleinfo.total_kwh);
// 	printf("Inst Amp 1 : %d\n", Teleinfo.inst_amp_1);
// 	printf("Inst Amp 2 : %d\n", Teleinfo.inst_amp_2);
// 	printf("Inst Amp 3 : %d\n", Teleinfo.inst_amp_3);
// 	printf("Max Amp 1 : %d\n", Teleinfo.amp_max_1);
// 	printf("Max Amp 2 : %d\n", Teleinfo.amp_max_2);
// 	printf("Max Amp 3 : %d\n", Teleinfo.amp_max_3);
// 	printf("Max Power : %d\n", Teleinfo.max_pow);
// 	printf("App Power : %d\n", Teleinfo.app_pow);
// 	return (0);
// }
