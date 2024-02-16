void sendString(char *str)
{
    while(*str)
    {
        cyBot_sendByte(*str);
        str++;
    }
}
