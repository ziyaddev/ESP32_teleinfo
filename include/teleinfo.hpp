#ifndef TELEINFO_H
# define TELEINFO_H

char    *ft_teleinfo_extract(char *str, const char *labl, int data_len, char *nrj_meter_data);

char    *ft_deserializer(char *str);

void    ft_left_shift(char *str, int len);

#endif