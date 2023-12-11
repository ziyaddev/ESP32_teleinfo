#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#include "teleinfo.hpp"

char	*ft_teleinfo_extract(char *str, const char *labl, int data_len, char *nrj_meter_data)
{
	char	*str_buf;
	Serial.println("begin of TI extract ft");
	Serial.print("str : ");
	Serial.println(str);

	str_buf = (char *)malloc(sizeof(char) * (strlen(str) + 1));
	// if (!str_buf)
	// 	return (0);
	Serial.println("1st malloc inside TI extract");

	// Search for label in the string
	Serial.print("1. str_buf : ");
	Serial.println(str_buf);
	str_buf = strstr(str, labl);
	Serial.println("strstr inside TI extract");
	Serial.print("2. str_buf : ");
	Serial.println(str_buf);

	// It fails here !
	if (str_buf)
	{
		strncpy(nrj_meter_data, (str_buf + strlen(labl) + 1), data_len);
		Serial.println("strncpy inside TI extract");
	}
	free(str_buf);
	Serial.println("free inside TI extract");
	return (nrj_meter_data);
}

char	*ft_deserializer(char *str)
{
	char	*buf;
	int		i;
	int		j;
	int		len;

	buf = (char *)malloc(sizeof(char) * (5 + 1));
	// if (!buf)
	// 	return (0);
	i = 0;
	j = 0;
	Serial.println("begin of deserializer ft");
	Serial.println("deserializer ft str begin");
	Serial.println(str);

	bzero(buf, 6);
	if (Serial1.available())
	{
		Serial.println("Serial available");
		// Fill in buf string
		Serial.print("strlen(buf) : ");
		Serial.println(strlen(buf));
		while (strlen(buf) < 6)
		{
			buf[i++] = Serial1.read();
			Serial.print(buf[i - 1]);
			Serial.println("\nstrlen buf < 6");
		}
		// Calculate buf length
		len = strlen(buf);
		Serial.print("len : ");
		Serial.println(len);
		// Check if buf string contains : \nADCO
		while (strncmp(buf, "\nADCO", 4))
		{
			Serial.print("buf shift : ");
			Serial.println(buf);
			ft_left_shift(buf, len);
			buf[len - 1] = Serial1.read();

		}
		// Copy buf content to str
		while (*buf)
		{
			Serial.print("Copying content to str : ");
			str[j++] = *buf++;
			Serial.println(str[j]);
		}
		// Search for end of wanted string
		while (!strstr(str, "\nMOTDETAT"))
		{
			Serial.println("MOTDETAT not found");
			str[j++] = Serial1.read();
			Serial.print(str[j]);
		}
	}
	else
	{
		while (str[i])
			str[i++] = '-';
	}
	free(buf);
	Serial.println("deserializer ft str end");
	Serial.println(str);
	return (str);
}

void	ft_left_shift(char *str, int len)
{
	int i;

	i = 1;
	while (i < len)
	{
		str[i - 1] = str[i];
		i++;
	}
}

// int	main(void)
// {
// 	char	data[] = "ADCO 020830035744 ;\nOPTARIF BASE 0\nISOUSC 60 <\nBASE 285579372 ;\nPTEC TH.. $\nIINST1 016 O\nIINST2 016 P\nIINST3 021 M\nIMAX1 051 6\nIMAX2 056 <\nIMAX3 066 >\nPMAX 37720 9\nPAPP 07369 :\nMOTDETAT 000000 B\n␃␂OT 00 #";
// 	char	*incoming_data;

// 	incoming_data = malloc(sizeof(char) * (209 + 1));
// 	if (!incoming_data)
// 		return (0);

// 	incoming_data = ft_deserializer(incoming_data);

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

// 	Teleinfo.total_kwh = atoi(ft_teleinfo_extract(incoming_data, "\nBASE", 9));
// 	Teleinfo.inst_amp_1 = atoi(ft_teleinfo_extract(incoming_data, "\nIINST1", 3));
// 	Teleinfo.inst_amp_2 = atoi(ft_teleinfo_extract(incoming_data, "\nIINST2", 3));
// 	Teleinfo.inst_amp_3 = atoi(ft_teleinfo_extract(incoming_data, "\nIINST3", 3));
// 	Teleinfo.amp_max_1 = atoi(ft_teleinfo_extract(incoming_data, "\nIMAX1", 3));
// 	Teleinfo.amp_max_2 = atoi(ft_teleinfo_extract(incoming_data, "\nIMAX2", 3));
// 	Teleinfo.amp_max_3 = atoi(ft_teleinfo_extract(incoming_data, "\nIMAX3", 3));
// 	Teleinfo.max_pow = atoi(ft_teleinfo_extract(incoming_data, "\nPMAX", 5));
// 	Teleinfo.app_pow = atoi(ft_teleinfo_extract(incoming_data, "\nPAPP", 5));

// 	printf("\nBASE : %d\n", Teleinfo.total_kwh);
// 	printf("Inst Amp 1 : %d\n", Teleinfo.inst_amp_1);
// 	printf("Inst Amp 2 : %d\n", Teleinfo.inst_amp_2);
// 	printf("Inst Amp 3 : %d\n", Teleinfo.inst_amp_3);
// 	printf("Max Amp 1 : %d\n", Teleinfo.amp_max_1);
// 	printf("Max Amp 2 : %d\n", Teleinfo.amp_max_2);
// 	printf("Max Amp 3 : %d\n", Teleinfo.amp_max_3);
// 	printf("Max Power : %d\n", Teleinfo.max_pow);
// 	printf("App Power : %d\n", Teleinfo.app_pow);
// 	printf("sizeof data : %ld\n", sizeof(incoming_data));

//     free(incoming_data);
//     return (0);
// }
